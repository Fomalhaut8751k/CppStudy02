#include<iostream>
#include<thread>
#include<atomic>

#include<list>

using namespace std;

/*
	�������ǱȽ��صģ�����ٽ������������������Ը��ӣ���
	ϵͳ���ۣ�CAS����֤++��--������ԭ�����Ծ��㹻��(��������)
*/

std::atomic_bool isReady = false;
std::atomic_int count_ = 0;

void task() {
	while (!isReady) {
		std::this_thread::yield();  // �̳߳��õ�ǰ��cpuʱ��Ƭ���ȴ���һ�ε���
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