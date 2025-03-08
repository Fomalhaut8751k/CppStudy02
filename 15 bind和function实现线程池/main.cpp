#include<iostream>
#include<typeinfo>
#include<thread>

#include<vector>
#include<string>
#include<functional>
using namespace std;

// muduoԴ���ļ� threadpool.cc, thread.cc    bind functino

// �߳���
class Thread {
public:
	Thread(function<void()> func) :_func(func) {}
	thread start() {
		thread t(_func); // _func()
		return t;
	}
private:
	function<void()> _func;
};

// �̳߳���
class ThreadPool {
public:
	ThreadPool(){}
	~ThreadPool(){
		// �ͷ�Thread����ռ�õĶ���Դ
		for (int i = 0; i < _pool.size(); ++i) {
			delete _pool[i];
		}
	}
	// �����̳߳�
	void startPool(int size) {
		for (int i = 0; i < size; ++i) {
			// this�൱��֮ǰ��ThreadPool()�� i�ǰ󶨵Ĳ���
			_pool.push_back(
				new Thread(bind(&ThreadPool::runInThread, this, i))
			);  
		}

		for (int i = 0; i < size; ++i) {
			_handler.push_back(_pool[i]->start());
		}

		for (thread& t : _handler) {
			t.join();
		}
	}
private:
	vector<Thread*> _pool;
	vector<thread> _handler;

	// ��runInThread�����Ա�����䵱�̺߳��� thread   pthread_create
	void runInThread(int id) {
		cout << "call runInThread! id: " << id << endl;
	}
};

int main() {
	ThreadPool pool;
	pool.startPool(10);

	return 0;
}