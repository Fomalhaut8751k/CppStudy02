#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<memory>
#include<functional>

using namespace std;

/* 智能指针的删除器 deletor
	智能指针：能够保证资源绝对的释放
	默认的释放方法都是delete
	但并非所有方法都用delete释放
	1. 数组：delete[]
	2. 文件：close

*/
#if 0  // 默认的删除器：函数对象实现
template<typename T>
class Deletor {
public:
	void operator()(T* ptr) {
		delete ptr;
	}
};
#endif

template<typename T>
class MyDeletor {  // 数组的删除器
public:
	void operator()(T* ptr) const{
		cout << "call::MyDeletor.operator()" << endl;
		delete[] ptr;
	}
};

template<typename T>
class MyFileDeletor { // 文件的删除器
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
