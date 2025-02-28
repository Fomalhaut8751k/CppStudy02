#include<iostream>

using namespace std;

#if 0  // �����Ϲ��������
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

	Test* p1 = (Test*)malloc(sizeof(Test));  // ֻ�ǿ��ٿռ�
	cout << "-------------------------------" << endl;
	new(p1) Test(t1);
	cout << "-------------------------------" << endl;
	p1->~Test();
	free(p1);

	return 0;
}
#endif 
#if 1  // move��forward

void test(int&& a) noexcept{
	cout << "test(int&&)" << endl;
}

int main() {
	int a = 20;
	// int&& b = a;  // ��ֵ�����ұ߱�������ֵ��a����ֵ
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