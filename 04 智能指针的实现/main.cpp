#include<iostream>
#include<memory>

using namespace std; 
#if 0
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

	CSmartPtr<int> ptr3(new int(20));
	CSmartPtr<int> ptr4(ptr3);

	return 0;
}
#endif
#if 0
// unique_ptr(unique_ptr<T>&& src)
// unique_ptr<T>& operator=(unique_ptr<T>&& src)
template<typename T>
unique_ptr<T> getSmartPtr() {
	unique_ptr<T> ptr(new T());
	return ptr;
}

int main() {
	// CSmartPtr<int> ptr1(new int(20));
	// CSmartPtr<int> ptr2(ptr1);  // 浅拷贝问题

	// 不带引用计数的智能制造
	/*
		auto_ptr
		c++ 新标准：
		scoped_ptr
		unique_ptr
	*/

	// auto_ptr<int> ptr1(new int);
	// auto_ptr<int> ptr2(ptr1);
	// scoped_ptr<int> ptr2(new int);
	
	unique_ptr<int> p1(new int);
	
	unique_ptr<int> p2(std::move(p1));

	// unique_ptr<int> ptr1 = getSmartPtr<int>(); 拷贝构造
	// ptr1 = getSmertPtr<int>();  赋值

	cout << *p1 << endl;
	cout << *p2 << endl;

	return 0;
}
#endif
#if 1
template<typename T>
class RefCnt {
public:
	RefCnt(T* ptr = nullptr): mptr(ptr), mcount(0){
		if (mptr != nullptr) {
			mcount = 1;
		}
	}
	void addRef() { mcount++; } 
	int delRef() { return --mcount; }
private:
	T* mptr;
	int mcount;

	friend int main();
};

template<typename T>
class CSmartPtr {
public:
	CSmartPtr(T* ptr = nullptr) :mptr(ptr){
		mpRefCnt = new RefCnt<T>(mptr);  
	}
	~CSmartPtr() { 
		if (mpRefCnt->delRef() == 0) {
			delete mptr;
			mptr = nullptr;
		}
	}
	// 解引用
	T& operator*() { return *mptr; }
	T* operator->() { return mptr; }

	CSmartPtr(const CSmartPtr<T>& src)
		:mptr(src.mptr), mpRefCnt(src.mpRefCnt){  // mptr(src.mptr)指向同一个资源
		if (mptr != nullptr) {
			mpRefCnt->addRef();
		}
	}

	CSmartPtr<T>& operator=(const CSmartPtr<T>& src) {
		cout << "CSmartPtr<T>::operator=" << endl;
		if (this == &src) {
			return *this;
		}
		if (mpRefCnt->delRef() == 0) {
			delete mptr;
		}
		mptr = src.mptr;
		mpRefCnt = src.mpRefCnt;
		mpRefCnt->addRef();
		return *this;
	}
private:
	T* mptr;  // 指向资源的指针
	RefCnt<T>* mpRefCnt;  // 指向该资源引用计数对象的指针

	friend int main();
};

int main() {
	CSmartPtr<int> ptr1;
	CSmartPtr<int> ptr2(new int);
	cout << "--------------------------------" << endl;
	ptr1 = new int(20);
	cout << "--------------------------------" << endl;

	//shared_ptr<int> ptr3(new int(20));
	//shared_ptr<int> ptr4(ptr3);
	//cout << "ptr3->: " << *ptr3 << endl;
	//cout << "ptr4->: " << *ptr4 << endl;

	//*ptr3 = 40;

	//cout << "ptr3->: " << *ptr3 << endl;
	//cout << "ptr4->: " << *ptr4 << endl;

	//*ptr4 = 5003;

	//cout << "ptr3->: " << *ptr3 << endl;
	//cout << "ptr4->: " << *ptr4 << endl;


	return 0;
}

#endif