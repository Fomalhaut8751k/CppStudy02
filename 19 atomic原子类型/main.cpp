#include<iostream>
#include<thread>
#include<atomic>

#include<list>

using namespace std;

/*
	互斥锁是比较重的，如果临界区代码做的事情稍稍复杂，多
	系统理论：CAS来保证++，--操作的原子特性就足够了(无锁操作)
*/

std::atomic_bool isReady = false;
std::atomic_int count_ = 0;

void task() {
	while (!isReady) {
		std::this_thread::yield();  // 线程出让当前的cpu时间片，等待下一次调度
	}
	for (int i = 0; i < 100; i++) {
		count_++;
	}
}

int main() {

	list<std::thread> tlist;
	for (auto i : { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }) {
		tlist.push_back(std::thread(task));
	}

	std::this_thread::sleep_for(std::chrono::seconds(3));
	isReady = true;
	

	for (std::thread& t : tlist) {
		t.join();
	}

	cout << "count: " << count_ << endl;

	return 0;
}