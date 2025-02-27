#include<iostream>

using namespace std;

template<typename T>
class CSmartPtr {
public:
	CSmartPtr(T* ptr = nullptr):mptr(ptr) {}
	~CSmartPtr() { delete mptr; }
	// ������
	T& operator*() { return *mptr; }
	T* operator->() { return mptr; }
private:
	T* mptr;
};

int main() {
	// ��ָ��
	int* p = new int(10); 
	/*
		��Ҫ�ֶ���delete�ͷŵ�����������˻��߳������������׵����ڴ�й©
	*/

	// ����ָ��    ��֤��������Դ���Զ��ͷ�
	CSmartPtr<int> ptr1(new int);  // ��Ȼ�ǿ����ڶ�����
	/*
		��Ϊ�����Ǻ���ִ����ɣ����Ǻ������м��ߵ��ˣ������ǳ������
		����Ҫ��main������������ģ��������򣬾ͻ��������������
		������ջ�ϵĶ����������Զ�����������

		����ָ�벻�ܿ����ڶ�����(�﷨û���⣬���ǲ�����)
	*/
	//throw "pdcHelloWorld";
	*ptr1 = 20;
	
	class Test {
	public:
		void print(const char* str) {
			cout << str;
		}
	};

	CSmartPtr<Test> ptr2(new Test);
	ptr2->print("pdcHelloWorld");

	// ֻ�ܿ����ڶ�����
	/*int a = 20;
	CSmartPtr<int> ptr3 = &a;
	cout << *ptr3 << endl;*/

	return 0;
}