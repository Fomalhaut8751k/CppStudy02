#include<iostream>
#include<memory>

using namespace std;

/*
	shared_ptr: 强智能指针，可以改变资源的引用计数
	weak_ptr: 弱智能指针，不会改变资源的引用计数
	weak_ptr观察shared_ptr，shared_ptr观察资源(内存)

	这俩有什么区别？
	强智能指针循环引用(交叉引用)是什么问题？什么结果？怎么解决？
	造成new出来的资源无法释放
	-----定义对象的时候用强智能指针，引用对象的地方用弱智能指针
*/
#if 0 // shared_ptr的交叉引用
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

int main() {
	shared_ptr<A> pa(new A());  
	shared_ptr<B> pb(new B());

	pa->_ptrb = pb;
	pb->_ptra = pa;

	cout << pa.use_count() << endl;
	cout << pb.use_count() << endl;

	cout << pa->_ptrb.use_count() << endl;
	cout << pb->_ptra.use_count() << endl;

	return 0;
}
#endif
#if 1  // 解决方法：
	   //    定义对象的时候用强智能指针，引用对象的地方用弱智能指针
class B;
class A {
public:
	A() { cout << "A()" << endl; }
	~A() { cout << "~A()" << endl; }
	void pdc(const char* str) {
		cout << str;
	}
	weak_ptr<B> _ptrb;
};
class B {
public:
	B() { cout << "B()" << endl; }
	~B() { cout << "~B()" << endl; }
	void func() {
		//_ptra->pdc("HelloWorld");
		shared_ptr<A> ps = _ptra.lock();
		if (ps != nullptr) {
			ps->pdc("HelloWorld");
		}
	}
	weak_ptr<A> _ptra;
};

int main() {
	shared_ptr<A> pa(new A());
	shared_ptr<B> pb(new B());

	pa->_ptrb = pb;
	pb->_ptra = pa;

	cout << pa.use_count() << endl;
	cout << pb.use_count() << endl;

	cout << pa->_ptrb.use_count() << endl;
	cout << pb->_ptra.use_count() << endl;

	return 0;
}
#endif