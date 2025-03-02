#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<memory>
#include<functional>

using namespace std;

/* ����ָ���ɾ���� deletor
	����ָ�룺�ܹ���֤��Դ���Ե��ͷ�
	Ĭ�ϵ��ͷŷ�������delete
	���������з�������delete�ͷ�
	1. ���飺delete[]
	2. �ļ���close

*/
#if 0  // Ĭ�ϵ�ɾ��������������ʵ��
template<typename T>
class Deletor {
public:
	void operator()(T* ptr) {
		delete ptr;
	}
};
#endif

template<typename T>
class MyDeletor {  // �����ɾ����
public:
	void operator()(T* ptr) const{
		cout << "call::MyDeletor.operator()" << endl;
		delete[] ptr;
	}
};

template<typename T>
class MyFileDeletor { // �ļ���ɾ����
public:
	void operator()(T* ptr) const {
		cout << "call::MyFileDeletor.operator()" << endl;
		fclose(ptr);
	}
};

int main() {

	//unique_ptr<int, MyDeletor<int>> ptr1(new int[100]);
	//unique_ptr<FILE, MyFileDeletor<FILE>> ptr2(fopen("data.txt", "w"));

	unique_ptr<int, function<void(int*)>> ptr1(new int[100],
		[](int* p)->void { cout << "call::lambda release new int[100]" << endl; delete[] p; }
	);

	unique_ptr<FILE, function<void(FILE*)>> ptr2(fopen("data.txt", "w"),
		[](FILE* p)->void { cout << "call::lambda release data.txt" << endl; fclose(p); }
	);

	return 0;
}
