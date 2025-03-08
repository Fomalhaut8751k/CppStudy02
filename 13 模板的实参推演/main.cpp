#include<iostream>

using namespace std;

template<typename T>
void func(T a) {
	cout << typeid(T).name() << endl;
}

template<typename R, typename A1, typename A2> // 对于函数类型，把返回值和形参分开
void func(R(*a)(A1, A2)) {
	cout << "返回值类型: " << typeid(R).name() << endl;
	cout << "形参类型: " << typeid(A1).name() << ", " << typeid(A2).name() << endl;
}

int sum(int a, int b) { return a + b; }

int main() {
	// 不加类型实参<>，靠实参推演
	func(10);
	func("pdcHelloWorld");
	func(sum);

	return 0;
}