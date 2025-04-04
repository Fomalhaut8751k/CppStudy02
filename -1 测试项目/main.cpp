#include<iostream>
#include<memory>
#include<thread>
#include<vector>
#include<functional>
#include<algorithm>

#include<map>

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

#if 0  // 多线程（速度太快了把持不住）
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
#if 0 // insert
int main() {
	vector<int> vec = { 10,4 };
	auto it = vec.end();
	vec.insert(it, 2);
	for_each(vec.begin(), vec.end(), [](int a)->void {cout << a << " "; });
	cout << endl;
}
#endif

#if 0  // bind2nd的实现

template<typename Compare, typename T>
class _mybind2nd {  // 一元函数对象
public:
	_mybind2nd(Compare con, T val)
		:_Comp(con), _Val(val){}
	bool operator()(const T& val) {
		return _Comp(val, _Val);  // 绑定在第二个值
	}
private:
	Compare _Comp;
	T _Val;
};

template<typename Compare, typename T>
_mybind2nd<Compare, T> mybind2nd(Compare comp, T val) {
	return _mybind2nd<Compare, T>(comp, val);
}

int main() {
	
	vector<int> vec = {5003, 3111, 1876, 17, 9, 7};
	auto it = find_if(vec.begin(), vec.end(), mybind2nd(less<int>(), 10));
	if (it != vec.end()) {
		vec.insert(it, 10);
	}
	for_each(vec.begin(), vec.end(), [](int val)->void {cout << val << " "; });

	return 0;
}
#endif

#if 0  // bind1st, bind2nd和bind
int sum(int a, int b, int c) { return a + b + c; }

class Test {
public:
	void pdc(const char* str) const {
		cout << str << endl;
	}

	bool greater(int a, int b) {
		return a > b;
	}

	bool greater_than_50(int a) {
		return bind(&Test::greater, this, placeholders::_1, 50)(a);
	}
};

int main() {
	//auto func1 = bind1st(sum, 10);
	//func1(30, 20);

	//auto func2 = bind2nd(sum, 10);
	//func2(30, 20);

	auto func3 = bind(sum, 10, placeholders::_1, placeholders::_2);
	auto func4 = bind(sum, placeholders::_1, 10, placeholders::_2);
	auto func5 = bind(sum, placeholders::_1, placeholders::_2, 10);

	cout << func3(30, 20) << endl;

	auto func6 = bind(&Test::pdc, Test(), "HelloWorld");

	func6();

	Test t1;
	cout << t1.greater_than_50(70) << endl;

	return 0;
}
#endif

#if 0  // function
void func(const char* str) {
	cout << str << endl;
}

class Test {
public:
	void operator()(const char* str) {
		cout << str << endl;
	}

	void func(const char* str) {
		cout << str << endl;
	}
};

int main() {
	// 函数
	function<void(const char*)> func1 = func;  
	func1("pdcHelloWorld_1");

	// bind绑定器
	function<void(void)> func2 = bind(func, "pdcHelloWorld");  
	func2();

	// lambda表达式
	function<void(const char*)> func3 = [](const char* str)->void {cout << str << endl; };
	func3("pdcHelloWorld_2");

	// 函数对象
	function<void(const char*)> func4 = Test();
	func4("pdcHelloWorld_3");

	// 函数指针
	void(*pfunc)(const char*) = &func;
	function<void(const char*)> func5 = pfunc;  
	func5("pdcHelloWorld_4");

	// 类成员函数
	function<void(Test*, const char*)> func6 = &Test::func;
	Test t1;
	func6(&t1,"pdcHelloWorld_5");

	map<int, function<void(const char*)>> m = {
		{1, func1},
		{2, func3},
		{3, func4},
		{4, func5},
	};
	while (1) {
		int choice = 0; 
		cout << "请输入你的选择: "; cin >> choice;
		if (choice >= 1 && choice <= 4) {
			m[choice]("pdcHelloWorld");
		}
	}

	return 0;
}
#endif

#if 0  // lambda表达式

int main() {
	cout << [](int a, int b)->int {return a + b; }(10, 20) << endl;
	
	vector<int> vec = { 5, 0, 0, 3 };
	for_each(vec.begin(), vec.end(), [](int num)->void {cout << num; }); cout << endl;

	int a = 10, b = 20;
	cout << "a = " << a << "\t" << "b = " << b << endl;
	[&]()->void {
		int tmp = a;
		a = b;
		b = tmp;
	}();
	cout << "a = " << a << "\t" << "b = " << b << endl;

	return 0;
}
#endif

#if 0  // bind1st, function, lambda底层逻辑
int sum(int a, int b) { return a + b; }

// #################### bind1st的底层逻辑 ####################
class mybind1st {
public:
	mybind1st(int(*pfunc)(int, int), int a) :
		_pfunc(pfunc), _a(a) {}

	// bind1st(sum, 10)整体来看是一个一元的函数对象
	int operator()(int a) {
		return _pfunc(_a, a);  // 调用的时候绑定了第一个参数
	}

private:
	int(*_pfunc)(int, int);  // 代表sum
	int _a;  // 代表10
};

// #################### function的底层逻辑 ####################
template<typename Ty>
class myfunction {};

template<typename R, typename A1, typename A2>
class myfunction<R(A1, A2)> {
public:
	using PFUNC = R(*)(A1, A2);
	myfunction(PFUNC pfunc) { // int(int, int)是一个函数类型
		_pfunc = pfunc;
	}
	R operator()(A1 a1, A2 a2) {
		return _pfunc(a1, a2);
	}
private:
	PFUNC _pfunc;
};

int main() {
	/* 首先绑定器怎么使用的？
		bind1st(sum, 10)(20);
		bind1st(sum, 10)整体来看是一个一元的函数对象
	*/
	cout << mybind1st(sum, 10)(20) << endl;
	/* 首先function是怎么使用的？
		function<int(int, int)> func = sum;
		func(10, 20);
	*/
	myfunction<int(int, int)> func(sum);
	cout << func(20, 40) << endl;
	/* 首先lambda表达式是怎么使用的？
		[](const char* str)->void {cout << str << endl; }("pdcHelloWorld");
	*/	
}

#endif

#if 0  // 原模板，完全实例化模板，部分实例化模板
int sum(int a, int b) { return a + b; }

// 原模板实现function
template<typename R, typename A1, typename A2>
class myfunction {
public:
	using PFUNC = R(*)(A1, A2);
	myfunction(PFUNC pfunc) :_pfunc(pfunc) {
		cout << "原模板实现function<R, A1, A2>" << endl;
	};  // 构造和析构函数名字不用加<>

	R operator()(A1 a1, A2 a2) {
		return _pfunc(a1, a2);
	}

private:
	PFUNC _pfunc;
};

// 完全实例化模板实现function
/*
	template<>
	class myfunction<int(int, int)>{}
	
	原模板有三个参数R，A1，A2，而int(int,int)是函数类型，只有一个，会报错
*/
template<typename Ty>  // 只设置一个模板参数
class myfunction1{};  // 然后不用实现

template<>
class myfunction1<int(int, int)> {
public:
	using PFUNC = int(*)(int, int);
	myfunction1(int(*pfunc)(int, int)) :_pfunc(pfunc) {
		cout << "完全实例化模板实现function<int(int, int)>" << endl;
		_pfunc = pfunc;
	}
	int operator()(int a, int b) {
		return _pfunc(a, b);
	}
private:
	PFUNC _pfunc;
};

// 部分实例化模板实现function
template<typename Ty>
class myfunction2{};

template<typename R, typename A1, typename A2>
class myfunction2<R(A1, A2)> {
public:
	using PFUNC = R(*)(A1, A2);
	myfunction2(R(*pfunc)(A1, A2)) :_pfunc(pfunc) {
		cout << "部分实例化模板实现function<R(A1, A2)>" << endl;
	}
	R operator()(A1 a1, A2 a2) {
		return _pfunc(a1, a2);
	}
private:
	PFUNC _pfunc;
};
 
int main() {
	cout << "内置的function<R(A1,A2)>" << endl;
	function<int(int, int)> func = sum;
	cout << "20 + 30 = " << func(20, 30) << endl;
	// function的底层逻辑也可以用原模板实现，就是模板参数<>里面不好看

	myfunction<int, int, int> myfunc = sum;
	cout << "20 + 30 = " << myfunc(20, 30) << endl;

	// function<int(int, int)>用完全实例化的函数模板实现，但是返回值和形参类型定死了

	myfunction1<int(int, int)> myfunc1 = sum;
	cout << "20 + 30 = " << myfunc(20, 30) << endl;

	//function<R(A1, A2)>用部分实例化的函数模板实现

	myfunction2<int(int, int)> myfunc2 = sum;
	cout << "20 + 30 = " << myfunc2(20, 30) << endl;


	return 0;
}
#endif

#if 0  // RTTI
class Base {
public:
	virtual void show() {
		cout << "Base::HelloWorld" << endl;
	}
};
class Derive1 : public Base {
public:
	void show() {
		cout << "Derive1::HelloWorld" << endl;
	}
};

class Derive2 : public Base {
public:
	void show() {
		cout << "Derive2::HelloWorld" << endl;
	}

	void supershow() {
		cout << "Derive2::pdcHelloWorld" << endl;
	}
};

void func(Base* pb) {
	//pb->show();
	Derive2* pdb = dynamic_cast<Derive2*>(pb);
	if (pdb != nullptr) {
		pdb->supershow();
	}
	else {
		pb->show();
	}
}

int main() {
	Derive1 d1;
	Derive2 d2;
	func(&d1);
	func(&d2);
}
#endif

#if 0  // 原子类型
#include<atomic>
#include<mutex>

//atomic<int> numCount = 0;

std::mutex mtx;
int numCount = 0;

void addCount1() {
	for (int i = 0; i < 1000; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		unique_lock<std::mutex> lck(mtx);
		numCount++;
		//cout << "线程一加1: " << ++numCount << endl;
		
	}
}

void addCount2() {
	for (int i = 0; i < 20; ++i) {
		std::this_thread::sleep_for(std::chrono::seconds(3));
		cout << "线程二加1: " << ++numCount << endl;
		
	}
}


int main() {

	vector<thread> vec;
	for (int i = 0; i < 100; ++i) {
		vec.push_back(thread(addCount1));
	}

	for (auto& t : vec) {
		t.join();
	}

	cout << "numCount: " << numCount << endl;

	return 0;
}
#endif
#if 1

// 在一支球队中，中场和后卫是有关联关系的
class Midfielder { public: virtual void show() = 0; };
class Iniesta : public Midfielder { public: void show() { cout << "1" << endl; } };

class Defender{ public: virtual void show() = 0; };
class Cannavaro : public Defender { public: void show() { cout << "2" << endl; } };

// 抽象工厂
class AbstractFactroy
{
public:
	virtual Midfielder* createMidfielder() = 0;
	virtual Defender* createDefender() = 0;
};

// 球队工厂
class TeamFactory: public AbstractFactroy
{
public:
	Midfielder* createMidfielder()
	{
		return new Iniesta();
	}
	Defender* createDefender()
	{
		return new Cannavaro();
	}
};

int main()
{
	unique_ptr<AbstractFactroy> teamf(new TeamFactory());
	teamf->createMidfielder()->show();
	teamf->createDefender()->show();
	return 0;
}
#endif