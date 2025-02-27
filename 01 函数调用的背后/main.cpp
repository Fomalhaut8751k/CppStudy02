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
	int getData() const { return ma; }
private:
	int ma;
};

Test GetObject(Test t) {
	// 获取成员变量ma
	int val = t.getData();  

	// 根据获取的值创建对象
	Test tmp(val);  // 4. Test(int)

	// 返回创建的对象
	return tmp;  // 5. Test(const Test&)  根据返回值拷贝构造到main函数栈帧上的临时对象
				 // 6. ~Test()  tmp的析构
}

int main() {
	Test t1;  // 1. Test(int)
	Test t2;  // 2. Test(int)
	t2 = GetObject(t1);  // 3. Test(const Test&)  形参传入
					     // 7. ~Test()  形参的析构
						 // 8. operator=
	                     // 9. ~Test()  main栈帧上临时对象的析构
	return 0;  // 10. ~Test()  t2的析构
		       // 11. ~Test()  t1的析构
}