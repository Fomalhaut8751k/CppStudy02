#include<iostream>

using namespace std;

class Test {
public:
	Test(int a = 10) :ma(a) { cout << "Test(int)" << endl; }
	~Test() { cout << "~Test()" << endl; }
	Test(const Test& t) : ma(t.ma) { cout << "Test(const Test&)" << endl; }
	Test& operator =(const Test& t) {
		if (this == &t) {
			return *this;
		}
		cout << "operator=" << endl;
		this->ma = t.ma;
		return *this;
	}
private:
	int ma;
};

#if 0
int main() {
	cout << "---------------------------------------------------------------" << endl;

	Test t1;  // Test(int)
	Test t2(t1);  // Test(const Test&)
	Test t3 = t1;  // Test(const Test&)  看似赋值，但是这是创建过程，还是拷贝
	Test t4 = Test(20);
	/*
		看上去像是：
		Test(int)  匿名对象的构造(显示生成匿名对象)
		Test(const Test&) 拷贝构造
		Test(int)

		实际上进行了优化： Test t4 = Test(20);  => Test t4(20); 所以是
		Test(int)
		用临时对象生成新对象的时候，临时对象就不产生了，直接构造新对象就可以了
	*/
	cout << "---------------------------------------------------------------" << endl;

	t4 = t2;  // operator=，对象t4已经构造完成，因此是赋值
	t4 = Test(30); // Test(int)  显式生成临时对象
				   // operator=
				   // ~Test()
	t4 = (Test)30;  // Test(int), 强制类型转换，找合适的构造函数，显式生成临时对象
					// operator=
					// ~Test()
	t4 = 30;  // Test(int), 隐式生成临时对象
			  // operator=
			  // ~Test()

	cout << "---------------------------------------------------------------" << endl;
	// Test* p = &Test(40);  // 临时对象创建，p存储临时对象的地址，临时对象出了语句后
							 // 就析构，p指向的地址就无效了，是不安全的
	const Test& ref = Test(40);  // Test(int)  出了语句，临时对象不析构
								 // 临时对象没有名字，出了语句就析构，但现在有了别名，
								 // 因此不会析构，因此是安全的
	system("pause");
	return 0;
}
#endif

class Test2 {
public:
	Test2(int a = 10, int b = 20) :ma(a), mb(b) { cout << "Test2(int, int)" << endl; }
	~Test2() { cout << "~Test2()" << endl; }
	Test2(const Test2& t) : ma(t.ma), mb(t.mb) { cout << "Test2(const Test&)" << endl; }
	Test2& operator =(const Test2& t) {
		if (this == &t) {
			return *this;
		}
		cout << "operator=" << endl;
		this->ma = t.ma;
		this->mb = t.mb;
		return *this;
	}
private:
	int ma;
	int mb;
};

#if 1
Test2 t1(10, 10);  // 1. Test2(int, int)

int main() {
	cout << "---------------------------------------------------------------" << endl;
	Test2 t2(20, 20);  // 3. Test2(int, int)
	Test2 t3 = t2;  //4.  Test2(const Test&)
	static Test2 t4 = Test2(30, 30);  // 5. Test2(int, int) 优化
	/*
		静态对象在编译时期开辟内存，但是在运行时期才初始化
	*/
	t2 = Test2(40, 40); // 6. Test2(int, int)
						// 7. opreator=
						// 8. ~Test()
	t2 = (Test2)(50, 50);  // 9. Test2(int, int)
						   // 10. opreator=
						   // 11. ~Test()
	t2 = 60; // 12. Test2(int, int)
			 // 13. opreator=
			 // 14. ~Test()
	Test2* p1 = new Test2(70, 70);  // 15. Test2(int, int)
	Test2* p2 = new Test2[2];  // 16. Test2(int, int)
							   // 17. Test2(int, int)
	// Test2* p3 = &Test2(80, 80);
	const Test2& p4 = Test2(90, 90);  // 18.Test2(int, int)
	delete p1;  // 19. ~Test2()
	delete[] p2;  // 20. ~Test2()
				// 21. ~Test2()
	system("pause");
	return 0;
	// 22. ~Test2()   p4
	// 23. ~Test2()   t3
	// 24. ~Test2()   t2
}
Test2 t5(100, 100);  // 2. Test2(int, int)
// 25. ~Test2()  t4 静态，程序结束才释放
// 26. ~Test2()  t5
// 27. ~Test2()  t1
#endif