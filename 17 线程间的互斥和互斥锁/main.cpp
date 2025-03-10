#include<iostream>
#include<thread>
#include<mutex>
#include<list>

using namespace std;

/*
	c++ thread模拟车站三个窗口卖票的程序
*/
int countIndex = 100;  // 车站有100张车票，由三个窗口一起卖票
std::mutex mtx;

// 模拟卖票的线程函数
void sellTicket(int index) {
	
	while (countIndex > 0) {
		{	// 加个局部作用域
			lock_guard<std::mutex> lock(mtx);
			if (countIndex > 0) {
				cout << "窗口: " << index << "卖出第" << countIndex << "张票" << endl;
				countIndex--;  // 非线程安全操作
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main() {
	list<std::thread> tlist;
	for (int i = 1; i <= 3; ++i) {
		tlist.push_back(std::thread(sellTicket, i));
	}
	for (std::thread &t: tlist) {
		t.join();  // 等待子线程结束
	}

	return 0;
}