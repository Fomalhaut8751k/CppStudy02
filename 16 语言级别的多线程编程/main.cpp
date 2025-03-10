#include<iostream>
#include<thread>

using namespace std;

/*
	c++语音级别的多线程编程=》代码可以跨平台 window/linux/mac
	thread线程/mutex互斥锁/condition_variable通信

	互斥锁：lock_quard/unique_lock自动加锁解锁
	atomic 原子类型，基于CAS操作的原子类型，线程安全的
	sleep_for 睡眠
*/

void threadHandle1() {
	// 让  当前线程     睡眠                    2秒
	std::this_thread::sleep_for(std::chrono::seconds(2));
	// 让  当前线程     睡眠                        500毫秒
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	cout << "hello thread1!" << endl;
}

void threadHandle2(int sleep_time, const char* print_str) {
	std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
	cout << "hello thread2!: "<< print_str << endl;
}

int main() {
	// 定义了一个线程对象 传入一个线程函数，新线程开始运行
	std::thread t1(threadHandle1);
	std::thread t2(threadHandle2, 5, "pdcHelloWorld");
	// 主线程等待子线程结束后，主进程继续往下运行
	t1.join();
	t2.detach();  // 设置为分离进程


	cout << "main thread done!" << endl;
	return 0;
}
