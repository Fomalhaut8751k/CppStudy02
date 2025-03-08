#include<iostream>
#include<vector>
#include<string>
#include<map>

#include<functional>
#include<algorithm>

/*
	function: 绑定器，函数对象，lambda表达式 它们只能使用在一条语句中
*/

#if 0
void pdc1() {
	std::cout << "helloWorld" << std::endl;
}

void pdc2(std::string str) {
	std::cout << str << std::endl;
}

inline int sum(int a, int b) {
	return a + b;
}

class Test {
public:
	void hello(std::string str) {
		std::cout << str << std::endl;
	}
};

int main() {
	// 把函数转化为函数对象？

	// 从function的类模板定义处，看到希望用一个函数类型实例化function
	std::function<void()> func1 = pdc1;  // func1(pdc1); 构造函数
	func1();  // func1.operator();

	/*
		std::function<void(string)>  void是返回值，string是参数列表
	*/

	std::function<void(std::string)> func2 = pdc2;
	func2("HelloWorld");

	std::function<int(int, int)> func3 = sum;
	std::cout << func3(10, 20) << std::endl;

	std::function<int(int, int)> func4 = [](int a, int b)->int {return a + b; };
	std::cout << func4(100, 200) << std::endl;

	std::function<void(Test*, std::string)> func5 = &Test::hello;  // 要取地址
	Test t;
	func5(&t, "call Test::hello!");

	return 0;
}
#endif
#if 1  // function有什么用

void doShowAllBooks() { std::cout << "查看所有书籍信息" << std::endl; }
void doBorrow() { std::cout << "借书" << std::endl; }
void doBack() { std::cout << "还书" << std::endl; }
void doQueryBooks() { std::cout << "查询书籍" << std::endl; }
void doLoginOut() { std::cout << "注销" << std::endl; }


// ################## function的底层原理 ######################
void hello(std::string str) { std::cout << str << std::endl; }
int sum(int a, int b) { return a + b; }

template<typename Fty>
class myfunction {};

template<typename R, typename A1>
class myfunction<R(A1)> {
public:
	using PFUNC = R(*) (A1);
	myfunction(PFUNC pfunc) :_pfunc(pfunc) {}
	R operator()(A1 a) {
		return _pfunc(a);
	}
private:
	PFUNC _pfunc;
};

template<typename Fty>
class myfunction1 {};

template<typename R, typename A1, typename A2>
class myfunction1<R(A1, A2)> {
public:
	using PFUNC = R(*)(A1, A2);
	myfunction1(PFUNC pfunc) :_pfunc(pfunc) {};
	int operator()(A1 a1, A2 a2) {
		return _pfunc(a1, a2);
	}
private:
	PFUNC _pfunc;
};
/*
	但是这样对于n个形参个数不同的函数就要写n个这样的function
*/
template<typename Fty>
class myfunction_ {};
// 模板的部分实例化
template<typename R, typename... A1>  // 加... 可变参，参数个数是不定的
class myfunction_<R(A1...)> {
public:
	using PFUNC = R(*)(A1...);
	myfunction_(PFUNC pfunc) :_pfunc(pfunc) {};
	int operator()(A1... a1) {
		return _pfunc(a1...);
	}
private:
	PFUNC _pfunc;
};

int main() {

	myfunction<void(std::string)> func(hello);
	func("pdcHelloWorld");

	myfunction1<int(int, int)> func1(sum);
	std::cout << func1(10, 20) << std::endl;

	myfunction_<void(std::string)> func2(hello);
	func("pdcHelloWorld");

	myfunction_<int(int, int)> func3(sum);
	std::cout << func1(10, 20) << std::endl;

	/*
	std::map<int, std::function<void()>> actionMap = {
			{1, doShowAllBooks},
			{2, doBorrow},
			{3, doBack},
			{4, doQueryBooks},
			{5, doLoginOut}
	};
	int choice = 0;
	for (;;) {
		std::cout << "----------------------" << std::endl;
		std::cout << "- 1.查看所有书籍信息 -" << std::endl;
		std::cout << "- 2.      借书       -" << std::endl;
		std::cout << "- 3.      还书       -" << std::endl;
		std::cout << "- 4.    查询书籍     -" << std::endl;
		std::cout << "- 5.      注销       -" << std::endl;
		std::cout << "----------------------" << std::endl;
		std::cout << "请选择: ";
		std::cin >> choice;


		switch (choice) {   这段代码不好，无法闭合，无法做到开闭原则
		case 1:             因为随着开发，有新功能就要增加，哪个不要
			break;          了就要删除，改来改去的。
		case 2:
			break;
		case 3:
			break;
		}

		auto it = actionMap.find(choice);
		if (it == actionMap.end()) {
			std::cout << "输入数字无效，请重新选择" << std::endl;
		}
		else {
			it->second();
		}
	}
	*/

	return 0;
}
#endif