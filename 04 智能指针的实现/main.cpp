#include<iostream>
#include<memory>

using namespace std; 
#if 0
template<typename T>
class CSmartPtr {
public:
	CSmartPtr(T* ptr = nullptr):mptr(ptr) {}
	~CSmartPtr() { delete mptr; }
	// ������
	T& operator*() { return *mptr; }
	T* operator->() { return mptr; }
private:
	T* mptr;
};


int main() {
	// ��ָ��
	int* p = new int(10); 
	/*
		��Ҫ�ֶ���delete�ͷŵ�����������˻��߳������������׵����ڴ�й©
	*/

	// ����ָ��    ��֤��������Դ���Զ��ͷ�
	CSmartPtr<int> ptr1(new int);  // ��Ȼ�ǿ����ڶ�����
	/*
		��Ϊ�����Ǻ���ִ����ɣ����Ǻ������м��ߵ��ˣ������ǳ������
		����Ҫ��main������������ģ��������򣬾ͻ��������������
		������ջ�ϵĶ����������Զ�����������

		����ָ�벻�ܿ����ڶ�����(�﷨û���⣬���ǲ�����)
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

	// ֻ�ܿ����ڶ�����
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
	// CSmartPtr<int> ptr2(ptr1);  // ǳ��������

	// �������ü�������������
	/*
		auto_ptr
		c++ �±�׼��
		scoped_ptr
		unique_ptr
	*/

	// auto_ptr<int> ptr1(new int);
	// auto_ptr<int> ptr2(ptr1);
	// scoped_ptr<int> ptr2(new int);
	
	unique_ptr<int> p1(new int);
	
	unique_ptr<int> p2(std::move(p1));

	// unique_ptr<int> ptr1 = getSmartPtr<int>(); ��������
	// ptr1 = getSmertPtr<int>();  ��ֵ

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
	// ������
	T& operator*() { return *mptr; }
	T* operator->() { return mptr; }

	CSmartPtr(const CSmartPtr<T>& src)
		:mptr(src.mptr), mpRefCnt(src.mpRefCnt){  // mptr(src.mptr)ָ��ͬһ����Դ
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
	T* mptr;  // ָ����Դ��ָ��
	RefCnt<T>* mpRefCnt;  // ָ�����Դ���ü��������ָ��

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