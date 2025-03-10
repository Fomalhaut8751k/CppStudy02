#include<iostream>
#include<thread>
#include<mutex>
#include<list>

using namespace std;

/*
	c++ threadģ�⳵վ����������Ʊ�ĳ���
*/
int countIndex = 100;  // ��վ��100�ų�Ʊ������������һ����Ʊ
std::mutex mtx;

// ģ����Ʊ���̺߳���
void sellTicket(int index) {
	
	while (countIndex > 0) {
		{	// �Ӹ��ֲ�������
			lock_guard<std::mutex> lock(mtx);
			if (countIndex > 0) {
				cout << "����: " << index << "������" << countIndex << "��Ʊ" << endl;
				countIndex--;  // ���̰߳�ȫ����
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
		t.join();  // �ȴ����߳̽���
	}

	return 0;
}