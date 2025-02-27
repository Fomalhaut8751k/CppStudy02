#include<iostream>

using namespace std;
/*  优化的三个要点
1. 函数参数传递过程中，对象优化按引用传递，不要按值传递
	可以避免实参到形参的拷贝构造和对应的析构 -2；
2. 直接返回临时对象，不要先创建局部对象，再返回它
	返回临时对象，临时对象拷贝构造在main函数栈帧上的新的临时对象的步骤会被
	优化，Test(val)这个临时对象不会产生和析构 -2；
3. 对象的创建一气呵成，不要创建完再赋值操作
	Test t2;              =>               Test t2 = GetObject(t1, 1);  
	t2 = GetObject(t1);
	因为GetObject返回的是一个临时对象，这样编译器又可以优化成一般有参构造
	因此可以减少临时对象的构造和析构，以及operator=  -3；

*/

class Test {
public:
	Test(int a = 10) :ma(a) { cout << "Test(int)" << endl; }
	~Test() { cout << "~Test()" << endl; }
	Test(const Test& t) : ma(t.ma) { cout << "Test(const Test&)" << endl; }
	Test(Test&& t) : ma(t.ma) { cout << "Test(Test&&)" << endl; }
	Test& operator =(const Test& t) {
		if (this == &t) {
			return *this;
		}
		cout << "operator=" << endl;
		this->ma = t.ma;
		return *this;
	}
	int getData() const { return ma; }
private:
	int ma;
};

Test GetObject(Test& t) {
	// 获取成员变量ma
	int val = t.getData();  

	// 根据获取的值创建对象
	cout << "pdcHelloWorld" << endl;
	Test tmp(val);  // 4. Test(int)

	// 返回创建的对象
	return tmp;  // 5. Test(const Test&)  根据返回值拷贝构造到main函数栈帧上的临时对象
				 // 6. ~Test()  tmp的析构
}

Test GetObject(Test& t, int label) {
	//cout << "pdcHelloWorld" << endl;
	int val = t.getData();
	return Test(val);  // 用临时对象拷贝构造一个新对象时，编译器会自动优化
					   // 这里的临时对象Test(val)拷贝构造一个在main函数栈帧
	                   // 上的新的临时对象，因此这里被优化了，直接构造main
	                   // 函数栈帧上的临时对象，由于Test(val)这个临时对象没
	                   // 有产生，因此也不需要析构
}

int main() {
#if 0
	Test t1;  // 1. Test(int)
	Test t2;  // 2. Test(int)
	t2 = GetObject(t1);  // 3. Test(const Test&)  形参传入
					     // 7. ~Test()  形参的析构
						 // 8. operator=
	                     // 9. ~Test()  main栈帧上临时对象的析构
	return 0;  // 10. ~Test()  t2的析构
		       // 11. ~Test()  t1的析构
#endif
#if 1
	Test t1;
	Test t2 = GetObject(t1, 1);

	return 0;
#endif
}

