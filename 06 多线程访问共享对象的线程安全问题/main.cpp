#include<iostream>
#include<memory>

#include<thread>

using namespace std;

/*
C++ 非常著名的开源网络库muduo库
多线程访问共享对象的线程安全问题
*/
class A {
public:
	A() { cout << "A()" << endl; }
	~A() { cout << "~A()" << endl; }
	void pdc(const char* str) { cout << str << endl; }
};

#if 0
// 子线程
void handler01(A* q) {
	std::this_thread::sleep_for(std::chrono::seconds(2));  // 休眠2s
	q->pdc("HelloWorld");
}
// main线程
int main() {
	// 这个A对象是两个线程共享的，因为它的地址也给到了子线程
	A* p = new A();
	thread t1(handler01, p);
	delete p;
	t1.join();  // 等待该线程执行完毕

	return 0;
}
#endif
#if 1
// 子线程
void handler01(weak_ptr<A> q) {
	std::this_thread::sleep_for(std::chrono::seconds(2));  // 休眠2s

	shared_ptr<A> ps = q.lock();
	if (ps != nullptr) {
		ps->pdc("HelloWorld");
	}
	else {
		cout << "该资源已被释放！" << endl;
	}
}

// main线程
int main() {
	// 这个A对象是两个线程共享的，因为它的地址也给到了子线程
	{
		shared_ptr<A> p(new A());
		thread t1(handler01, weak_ptr<A>(p));
		t1.detach();  // 分离线程
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
	std::this_thread::sleep_for(std::chrono::seconds(5));  // 主线程睡5s，保证子线程跑完再return0
	//t1.join();  // 等待该线程执行完毕

	return 0;
}
#endif