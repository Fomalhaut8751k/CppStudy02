#include<iostream>
#include<typeinfo>

#include<string>
#include<functional>
using namespace std;

/*
	c++ bind绑定器 =》返回结果还是一个函数对象
*/

void hello(string str) { cout << str << endl; }
int sum(int a, int b) { return a + b; }

class Test {
public:
	int sum(int a, int b) {
		cout << a << " " << b << endl;
		return a + b;
	}
};

int main() {

	// bind是函数模板，可以自动推演模板参数类型
	auto binder1 = bind(hello, "pdcHelloWorld");
	binder1();
	//cout << typeid(STR_FUNC).name() << endl;

	using STR_FUNC = function<void(void)>;  // 绑定了就不需要传参数了
	STR_FUNC sf = bind(hello, "pdcHelloWorld");
	sf();

	using SUM_FUNC = function<int(void)>;
	//SUM_FUNC sumf = bind(sum, 10);  不能只绑一个？要占位
	auto sumf = bind(sum, 10, 20);
	cout << sumf() << endl;

	cout << bind(&Test::sum, Test(), 10, 20)() << endl;  // 直接调用

	// 参数占位符
	cout << bind(&Test::sum, Test(), placeholders::_1, 20)(30) << endl;  // 这是就需要为占位的位置传入参数
	cout << bind(&Test::sum, Test(), 65, placeholders::_1)(30) << endl;
	cout << bind(&Test::sum, Test(), placeholders::_1, placeholders::_2)(30, 6) << endl;
	// 最多绑定20个参数

	function<int(int)> func1 = bind(&Test::sum, Test(), 65, placeholders::_1);

	return 0;
}