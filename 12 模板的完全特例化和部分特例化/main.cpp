#include<iostream>

using namespace std;

/*
模板的完全特例化和非完全(部分)特例化
模板的实参推演
*/

// 为什么需要模板的实例化，因为对于某些场景模板可能不符合逻辑
template<typename T>
bool compare(T a, T b) {
	cout << &a << " " << &b << endl;
	return a > b;
}

// 特例化(完全的)
template<>
bool compare<const char*>(const char* a, const char* b) {
	return strcmp(a, b) > 0;
}


template<typename T>
class Vector {
public:
	Vector() { cout << "call Vector template init" << endl; }
};
// char*的完全特例化版本（优先级大于部分特例化大于原模板）
template<>
class Vector<char*> {
public:
	Vector() { cout << "call Vector char* init" << endl; }
};
// 指针类型的部分特例化版本
template<typename Ty>
class Vector<Ty*> {
public:
	Vector() { cout << "call Vector Ty* init" << endl; }
};
// 函数指针类型的部分特例化版本
template<typename R, typename A1, typename A2>
class Vector<R(*)(A1, A2)> {
public:
	Vector() {
		cout << "call Vector<R(*)(A1, A2)> init" << endl;
	}
};

int sum(int a, int b) { return a + b; }

int main() {
	const char* str1 = "bbb";
	const char* str2 = "aaa";
	// 我们希望比较的是字典序，但是实际情况是——按地址比较，所以需要特例化
	cout << "bbb > aaa ? : " << compare(str1, str2) << endl;

	Vector<int> vec1;
	Vector<char*> vec2;
	Vector<int*> vec3;
	Vector<int(*)(int, int)> vec4; // 函数指针类型
	Vector<int(int, int)> vec5;  // 函数类型

	// 函数指针类型
	using PFUNC = int(*)(int, int);
	Vector<PFUNC> vec6;
	PFUNC pfunc = sum;
	cout << pfunc(20, 30) << endl;

	// 函数类型
	using FUNC = int(int, int);
	Vector<FUNC> vec7;
	FUNC* func = sum;
	cout << func(20, 30) << endl;

	return 0;

}