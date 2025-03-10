#include<iostream>
#include<thread>

using namespace std;

/*
	c++��������Ķ��̱߳��=��������Կ�ƽ̨ window/linux/mac
	thread�߳�/mutex������/condition_variableͨ��

	��������lock_quard/unique_lock�Զ���������
	atomic ԭ�����ͣ�����CAS������ԭ�����ͣ��̰߳�ȫ��
	sleep_for ˯��
*/

void threadHandle1() {
	// ��  ��ǰ�߳�     ˯��                    2��
	std::this_thread::sleep_for(std::chrono::seconds(2));
	// ��  ��ǰ�߳�     ˯��                        500����
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	cout << "hello thread1!" << endl;
}

void threadHandle2(int sleep_time, const char* print_str) {
	std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
	cout << "hello thread2!: "<< print_str << endl;
}

int main() {
	// ������һ���̶߳��� ����һ���̺߳��������߳̿�ʼ����
	std::thread t1(threadHandle1);
	std::thread t2(threadHandle2, 5, "pdcHelloWorld");
	// ���̵߳ȴ����߳̽����������̼�����������
	t1.join();
	t2.detach();  // ����Ϊ�������


	cout << "main thread done!" << endl;
	return 0;
}
