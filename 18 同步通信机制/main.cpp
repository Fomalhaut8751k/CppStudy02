#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>

#include<queue>  // c++ STL���������������̰߳�ȫ��

using namespace std;

/* c++���̱߳�̵���������
* 1. �̼߳�Ļ���
	��̬���� =�� �ٽ�������� =�� ��֤ԭ�Ӳ��� =��������mutex ������������ʵ��CAS

* 2. �̼߳��ͬ��ͨ��
	�����ߣ����������⣨�̶߳���һ���������������߳�һ�����������꣩
*/

std::mutex mtx;  // ���廥����
std::condition_variable cv;  // �����������������̼߳��ͨ��

class Queue {
public:
	// ������Ʒ
	void put(int val) {  
		unique_lock<std::mutex> lck(mtx);
		//cout << "����������" << endl;
		//std::this_thread::sleep_for(std::chrono::seconds(3));
		while (!que.empty()) {
			//cout << "�����߼�������" << endl;
			cv.wait(lck);  // ����ȴ�״̬�������ͷţ��ȴ����������ѡ�
		}
		que.push(val);
		cv.notify_all();  // ֪ͨ�����̣߳���������һ����Ʒ�����ǸϽ�����
		// �����̵߳õ���֪ͨ���ͻ�ӵȴ�״̬ =�� ����״̬ =����ȡ���������ܼ���ִ��
		//cout << "������ ����: " << val << "����Ʒ" << endl;
	}
	// ������Ʒ
	int get() {
		unique_lock<std::mutex> lck(mtx);
		//cout << "����������" << endl;
		//std::this_thread::sleep_for(std::chrono::seconds(3));
		while (que.empty()) {
			// �������̷߳���que�ǿյģ�֪ͨ�������߳���������Ʒ
			//cout << "�����߼�������" << endl;
			cv.wait(lck);
		}
		int val = que.front();
		que.pop();
		cv.notify_all();
		//cout << "������ ����: " << val << "����Ʒ" << endl;
		return val;
	}
private:
	queue<int> que;
};

// �������߳�
void producer(Queue* que) { 
	for (int i = 1; i <= 10; ++i) {
		que->put(i);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

// �������߳�
void consumer(Queue* que) {
	for (int i = 1; i <= 10; ++i) {
		que->get();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}


int main() {

	Queue que; // �������̹���Ķ���
	std::thread t2(consumer, &que);
	std::thread t1(producer, &que);
	

	t1.join();
	t2.join();

	return 0;
}