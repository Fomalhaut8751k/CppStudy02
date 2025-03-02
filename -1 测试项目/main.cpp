#include<iostream>
#include<memory>
#include<thread>
#include<vector>

#include<windows.h>


using namespace std;

#if 0  // 堆区上构造和析构
class Test {
public:
	Test(int a = 10) :ma(a) { cout << "Test()" << endl; }
	~Test() { cout << "~Test()" << endl; }
	Test(const Test& t) :ma(t.ma) { cout << "Test(const Test&)" << endl; }
	Test(Test&& t) :ma(t.ma) { cout << "Test(Test&&)" << endl; }
	Test& operator=(const Test& t) {
		cout << "Test::operator(const Test&)" << endl;
		ma = t.ma;
	}
private:
	int ma;
};

int main() {
	Test t1;
	Test t2(t1);
	Test t3 = Test();

	cout << "-------------------------------" << endl;
	Test* p = new Test();  
	delete p;

	Test* p1 = (Test*)malloc(sizeof(Test));  // 只是开辟空间
	cout << "-------------------------------" << endl;
	new(p1) Test(t1);
	cout << "-------------------------------" << endl;
	p1->~Test();
	free(p1);

	return 0;
}
#endif 
#if 0  // move和forward
void test(int&& a) noexcept{
	cout << "test(int&&)" << endl;
}

int main() {
	int a = 20;
	// int&& b = a;  // 右值引用右边必须是右值，a是左值
	int&& b = std::move(20);  
	int&& c = std::move(a);

	test(a);
	test(20);
	test(std::move(a));
	test(std::move(20));

	int&& e = std::forward<int>(a);
	test(std::forward<int>(20));

	return 0;
}
#endif
#if 0  // shared_ptr和自己写的在交叉引用问题上
template<typename T>
class RefCnt {
public:
	RefCnt(T* ptr = nullptr) : mptr(ptr), mcount(0) {
		if (mptr != nullptr) {
			mcount = 1;
		}
	}
	void addRef() { mcount++; }
	int delRef() { return --mcount; }

	int returnMcount() const{ return mcount; }
private:
	T* mptr;
	int mcount;

	friend int main();
};

template<typename T>
class CSmartPtr {
public:
	CSmartPtr(T* ptr = nullptr) :mptr(ptr) {
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
		:mptr(src.mptr), mpRefCnt(src.mpRefCnt) {  
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

class B;
class A {
public:
	A() { cout << "A()" << endl; }
	~A() { cout << "~A()" << endl; }
	shared_ptr<B> _ptrb;
};
class B {
public:
	B() { cout << "B()" << endl; }
	~B() { cout << "~B()" << endl; }
	shared_ptr<A> _ptra;
};

class B1;
class A1 {
public:
	A1() { cout << "A1()" << endl; }
	~A1() { cout << "~A1()" << endl; }
	CSmartPtr<B1> _ptrb;
};
class B1 {
public:
	B1() { cout << "B1()" << endl; }
	~B1() { cout << "~B1()" << endl; }
	CSmartPtr<A1> _ptra;
};

int main() {
	shared_ptr<A> pa(new A());
	shared_ptr<B> pb(new B());

	cout << "-------------------------------------------" << endl;

	cout << "pa.use_count: " << pa.use_count() << endl;
	cout << "pb.use_count: " << pb.use_count() << endl;

	cout << "pa->_ptrb.use_count: " << pa->_ptrb.use_count() << endl;
	cout << "pb->_ptra.use_count: " << pb->_ptra.use_count() << endl;

	pa->_ptrb = pb;
	pb->_ptra = pa;

	cout << "pa.use_count: " << pa.use_count() << endl;
	cout << "pb.use_count: " << pb.use_count() << endl;

	cout << "pa->_ptrb.use_count: " << pa->_ptrb.use_count() << endl;
	cout << "pb->_ptra.use_count: " << pb->_ptra.use_count() << endl;

	cout << "-------------------------------------------" << endl;
	
	CSmartPtr<A1> ca(new A1());
	CSmartPtr<B1> cb(new B1());

	cout << "ca.use_count: " << ca.mpRefCnt->mcount << endl;
	cout << "cb.use_count: " << cb.mpRefCnt->mcount << endl;

	cout << "ca->_ptrb.use_count: " << ca->_ptrb.mpRefCnt->mcount << endl;
	cout << "cb->_ptra.use_count: " << cb->_ptra.mpRefCnt->mcount << endl;

	cout << "-------------------------------------------" << endl;

	ca->_ptrb = cb;
	cb->_ptra = ca;

	cout << "-------------------------------------------" << endl;

	cout << "ca.use_count: " << ca.mpRefCnt->mcount << endl;
	cout << "cb.use_count: " << cb.mpRefCnt->mcount << endl;

	cout << "ca->_ptrb.use_count: " << ca->_ptrb.mpRefCnt->mcount << endl;
	cout << "cb->_ptra.use_count: " << cb->_ptra.mpRefCnt->mcount << endl;

	return 0;
}
#endif

#if 0  // CMySmarttr正常使用下(不交叉)
template<typename T>
class RefCnt {
public:
	RefCnt(T* ptr = nullptr) : mptr(ptr), mcount(0) {
		if (mptr != nullptr) {
			mcount = 1;
		}
	}
	void addRef() { mcount++; }
	int delRef() { return --mcount; }

	int returnMcount() const { return mcount; }
private:
	T* mptr;
	int mcount;

	friend int main();
};

template<typename T>
class CSmartPtr {
public:
	CSmartPtr(T* ptr = nullptr) :mptr(ptr) {
		cout << "CSmartPtr()" << endl;
		mpRefCnt = new RefCnt<T>(mptr);
	}
	~CSmartPtr() {
		cout << "~CSmartPtr()" << endl;
		if (mpRefCnt->delRef() == 0) {
			
			delete mptr;
			mptr = nullptr;
		}
	}
	// 解引用
	T& operator*() { return *mptr; }
	T* operator->() { return mptr; }

	CSmartPtr(const CSmartPtr<T>& src)
		:mptr(src.mptr), mpRefCnt(src.mpRefCnt) {
		cout << "CSmartPtr(const CSmartPtr<T>& src)" << endl;
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

	void showMpRefCntAddress() const {
		cout << mpRefCnt << endl;
	}

private:
	T* mptr;  // 指向资源的指针
	RefCnt<T>* mpRefCnt;  // 指向该资源引用计数对象的指针

	friend int main();
};

class B1;
class A1 {
public:
	A1() { cout << "A1()" << endl; }
	~A1() { cout << "~A1()" << endl; }
	CSmartPtr<B1> _ptrb;
};
class B1 {
public:
	B1() { cout << "B1()" << endl; }
	~B1() { cout << "~B1()" << endl; }
	CSmartPtr<A1> _ptra;
};

int main() {
	CSmartPtr<A1> ptr;

	CSmartPtr<A1> ptr1(new A1());
	cout << &ptr1 << endl;
	CSmartPtr<A1> ptr2(ptr1);

	ptr = new A1();

	ptr = ptr2;

	cout << &ptr << endl;

	ptr1.showMpRefCntAddress();
	ptr2.showMpRefCntAddress();

	return 0;
}
#endif
#if 0  // 弱智能指针及其提升方法

int main() {
	// 没有有参构造，只有拷贝和operator=
	//weak_ptr<int> ptr1(new int);
	shared_ptr<int> ptr1(new int);
	cout << "ptr1.use_count: " << ptr1.use_count() << endl;
	if (1) {  // weak_ptr不影响引用计数
		weak_ptr<int> ptr2(ptr1);
		cout << "ptr1.use_count: " << ptr1.use_count() << endl;
	}
	else {  // shared_ptr影响引用计数
		shared_ptr<int> ptr2(ptr1);
		cout << "ptr1.use_count: " << ptr1.use_count() << endl;
	}
	weak_ptr<int> ptr3(ptr1);
	cout << "ptr1.use_count: " << ptr1.use_count() << endl;

	// 提升为强智能指针后，引用计数便加一
	shared_ptr<int> ptr4 = ptr3.lock();
	cout << "ptr1.use_count: " << ptr1.use_count() << endl;
	return 0;

	weak_ptr<int> ptr5;
}
#endif

#if 0  // 智能指针不指向堆区
int main() {
	/*int a = 20;
	shared_ptr<int> ptr(&a);*/  // 不行！

	unique_ptr<int> ptr1(new int(20));  // 00E17758
	unique_ptr<int> ptr2(std::move(ptr1));  // 00000000

	return 0;
}
#endif

#if 1  // 多线程（速度太快了把持不住）
// 对象池
class Item {
public:
	Item(int val = 10) :_val(val), _next(nullptr) { //cout << "Item()" << endl; 
	}
	~Item() { // cout << "~Item()" << endl;
	}

	void* operator new(size_t size) {
		if (_itemPool == nullptr) {
			_itemPool = (Item*)new char[POOL_MAX_SIZE * sizeof(Item)];
			//_itemPool = new Item[POOL_MAX_SIZE];
			Item* curItem = _itemPool;
			for (; curItem < _itemPool + POOL_MAX_SIZE - 1; ++curItem) {
				curItem->_next = curItem + 1;
			}
			curItem->_next = nullptr;
		}
		Item* p = _itemPool;
		_itemPool = _itemPool->_next;
		return p;
	}

	void operator delete(void* p) {
		Item* curItem = (Item*)p;
		curItem->_next = _itemPool;
		_itemPool = curItem;
	}

	static Item* _itemPool;
	static const int POOL_MAX_SIZE = 1000;

private:
	int _val;
	Item* _next;

	friend int main();
};

Item* Item::_itemPool = nullptr;

const int MAX_SIZE = 20000000;

void handler01(int* p) {
	vector<const char*> vec = {"+---------", "-+--------", "--+-------", "---+------", "----+-----",
	                           "-----+----", "------+---", "-------+--", "--------+-", "---------+", };
	int cnt = 0;
	while (*p < 100) {
		//cout << *p << ": " << cnt << endl;
		if (*p != cnt) {
			system("cls");
			cout << "加载中: " << vec[*p % 10] << " (" << cnt + 1 << "%)";
			cnt = *p;
		}
		
	}
}

int main() {

	int count = 0;  // main线程和子线程共享的资源
	int cnt = 0;
	// 用子线程实现一个加载动画
	thread t1(handler01,&count);

	Item* _it = new Item();
	for (int i = 0; i < MAX_SIZE; ++i) {
		Item* it = new Item();
		delete it;
		count = (cnt++ + 1) / (MAX_SIZE / 100);
	}
	delete _it;

	t1.join();  // 等待子进程执行完毕，再执行下面的
	system("cls");
	cout << "长度为 " << MAX_SIZE << " 的对象池加载完成！" << endl;
	
	return 0;
}

#endif