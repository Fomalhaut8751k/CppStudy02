#include<iostream>
#include<typeinfo>
#include<thread>

#include<vector>
#include<string>
#include<functional>
using namespace std;

// muduo源码文件 threadpool.cc, thread.cc    bind functino

// 线程类
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

// 线程池类
class ThreadPool {
public:
	ThreadPool(){}
	~ThreadPool(){
		// 释放Thread对象占用的堆资源
		for (int i = 0; i < _pool.size(); ++i) {
			delete _pool[i];
		}
	}
	// 开启线程池
	void startPool(int size) {
		for (int i = 0; i < size; ++i) {
			// this相当于之前的ThreadPool()， i是绑定的参数
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

	// 把runInThread这个成员方法充当线程函数 thread   pthread_create
	void runInThread(int id) {
		cout << "call runInThread! id: " << id << endl;
	}
};

int main() {
	ThreadPool pool;
	pool.startPool(10);

	return 0;
}