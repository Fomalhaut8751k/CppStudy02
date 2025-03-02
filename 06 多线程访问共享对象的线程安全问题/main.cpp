#include<iostream>
#include<memory>

#include<thread>

using namespace std;

/*
C++ �ǳ������Ŀ�Դ�����muduo��
���̷߳��ʹ��������̰߳�ȫ����
*/
class A {
public:
	A() { cout << "A()" << endl; }
	~A() { cout << "~A()" << endl; }
	void pdc(const char* str) { cout << str << endl; }
};

#if 0
// ���߳�
void handler01(A* q) {
	std::this_thread::sleep_for(std::chrono::seconds(2));  // ����2s
	q->pdc("HelloWorld");
}
// main�߳�
int main() {
	// ���A�����������̹߳���ģ���Ϊ���ĵ�ַҲ���������߳�
	A* p = new A();
	thread t1(handler01, p);
	delete p;
	t1.join();  // �ȴ����߳�ִ�����

	return 0;
}
#endif
#if 1
// ���߳�
void handler01(weak_ptr<A> q) {
	std::this_thread::sleep_for(std::chrono::seconds(2));  // ����2s

	shared_ptr<A> ps = q.lock();
	if (ps != nullptr) {
		ps->pdc("HelloWorld");
	}
	else {
		cout << "����Դ�ѱ��ͷţ�" << endl;
	}
}

// main�߳�
int main() {
	// ���A�����������̹߳���ģ���Ϊ���ĵ�ַҲ���������߳�
	{
		shared_ptr<A> p(new A());
		thread t1(handler01, weak_ptr<A>(p));
		t1.detach();  // �����߳�
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
	std::this_thread::sleep_for(std::chrono::seconds(5));  // ���߳�˯5s����֤���߳�������return0
	//t1.join();  // �ȴ����߳�ִ�����

	return 0;
}
#endif