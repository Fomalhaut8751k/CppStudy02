#include<iostream>

using namespace std;

template<typename T>
void func(T a) {
	cout << typeid(T).name() << endl;
}

template<typename R, typename A1, typename A2> // ���ں������ͣ��ѷ���ֵ���βηֿ�
void func(R(*a)(A1, A2)) {
	cout << "����ֵ����: " << typeid(R).name() << endl;
	cout << "�β�����: " << typeid(A1).name() << ", " << typeid(A2).name() << endl;
}

int sum(int a, int b) { return a + b; }

int main() {
	// ��������ʵ��<>����ʵ������
	func(10);
	func("pdcHelloWorld");
	func(sum);

	return 0;
}