#include<iostream>
#include<memory>

using namespace std;

/*
	shared_ptr: ǿ����ָ�룬���Ըı���Դ�����ü���
	weak_ptr: ������ָ�룬����ı���Դ�����ü���
	weak_ptr�۲�shared_ptr��shared_ptr�۲���Դ(�ڴ�)

	������ʲô����
	ǿ����ָ��ѭ������(��������)��ʲô���⣿ʲô�������ô�����
	���new��������Դ�޷��ͷ�
	-----��������ʱ����ǿ����ָ�룬���ö���ĵط���������ָ��
*/
#if 0 // shared_ptr�Ľ�������
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
#if 1  // ���������
	   //    ��������ʱ����ǿ����ָ�룬���ö���ĵط���������ָ��
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