#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>

#include<queue>  // c++ STL所有容器都不是线程安全的

using namespace std;

/* c++多线程编程的两个问题
* 1. 线程间的互斥
	竞态条件 =》 临界区代码段 =》 保证原子操作 =》互斥锁mutex 轻量级的无锁实现CAS

* 2. 线程间的同步通信
	生产者，消费者问题（线程二的一部分事情依赖于线程一的事情先做完）
*/

std::mutex mtx;  // 定义互斥锁
std::condition_variable cv;  // 定义条件变量，做线程间的通信

class Queue {
public:
	// 生产物品
	void put(int val) {  
		unique_lock<std::mutex> lck(mtx);
		//cout << "生产者上锁" << endl;
		//std::this_thread::sleep_for(std::chrono::seconds(3));
		while (!que.empty()) {
			//cout << "生产者即将解锁" << endl;
			cv.wait(lck);  // 进入等待状态，把锁释放，等待消费者消费。
		}
		que.push(val);
		cv.notify_all();  // 通知其他线程，我生产了一个物品，你们赶紧消费
		// 其他线程得到该通知，就会从等待状态 =》 阻塞状态 =》获取互斥锁才能继续执行
		//cout << "生产者 生产: " << val << "号物品" << endl;
	}
	// 消费物品
	int get() {
		unique_lock<std::mutex> lck(mtx);
		//cout << "消费者上锁" << endl;
		//std::this_thread::sleep_for(std::chrono::seconds(3));
		while (que.empty()) {
			// 消费者线程发现que是空的，通知生产者线程先生产物品
			//cout << "消费者即将解锁" << endl;
			cv.wait(lck);
		}
		int val = que.front();
		que.pop();
		cv.notify_all();
		//cout << "消费者 消费: " << val << "号物品" << endl;
		return val;
	}
private:
	queue<int> que;
};

// 生产者线程
void producer(Queue* que) { 
	for (int i = 1; i <= 10; ++i) {
		que->put(i);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

// 消费者线程
void consumer(Queue* que) {
	for (int i = 1; i <= 10; ++i) {
		que->get();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}


int main() {

	Queue que; // 两个进程共享的队列
	std::thread t2(consumer, &que);
	std::thread t1(producer, &que);
	

	t1.join();
	t2.join();

	return 0;
}