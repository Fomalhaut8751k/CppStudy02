#include<iostream>

using namespace std;

template<typename T>
class CSmartPtr {
public:
	CSmartPtr(T* ptr = nullptr):mptr(ptr) {}
	~CSmartPtr() { delete mptr; }
	// 解引用
	T& operator*() { return *mptr; }
	T* operator->() { return mptr; }
private:
	T* mptr;
};

int main() {
	// 裸指针
	int* p = new int(10); 
	/*
		需要手动的delete释放掉。如果忘记了或者程序发生，就容易导致内存泄漏
	*/

	// 智能指针    保证能做到资源的自动释放
	CSmartPtr<int> ptr1(new int);  // 依然是开辟在堆区上
	/*
		因为不管是函数执行完成，还是函数从中间走掉了，或亦是程序挂了
		都是要出main函数的作用域的，出作用域，就会调用析构函数、
		即利用栈上的对象处作用域自动析构的特征

		智能指针不能开辟在堆区上(语法没问题，但是不智能)
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

	// 只能开辟在堆区？
	/*int a = 20;
	CSmartPtr<int> ptr3 = &a;
	cout << *ptr3 << endl;*/

	return 0;
}