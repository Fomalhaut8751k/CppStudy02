#include<iostream>
#include<functional>
#include<vector>
#include<algorithm>

#include<map>

using namespace std;

/*
	c++11 函数对象的升级版 -> lambda表达式
	函数对象的缺点：
	使用在泛型算法参数传递   比较性质/自定义操作  优先级队列，需要定义个类出来
	这个类可能在其他地方都不用了

	lambda的语法：
	[捕获外部变量](形参列表)->返回值类型{操作代码};

	[捕获外部变量]：
	[] 不捕获外部变量
	[=] 以传值的方式捕获外部的所有变量
	[&] 以传引用的方式捕获外部的所有变量
	[this] 捕获外部的this指针
	[=, &a] 以传值的方式捕获外部所有的变量，但是a变量以传引用的方式捕获
	[a, b] 以值传递的方式捕获外部变量a和b
	[a, &b] a以值传递捕获，b以传引用的方式捕获
*/

template<typename T=void>
class TestLambda {
public:
	TestLambda() {}
	void operator()() const{
		cout << "pdcHelloWorld" << endl;
	}
};

template<typename T = void>
class TestLambda2 {
public:
	TestLambda2() {}
	int operator()(T a, T b) const {
		return a + b;
	}
};

template<typename T = void>
class TestLambda3 {
public:
	TestLambda3(int a, int b):ma(a),mb(b){}
	void operator()() const {  
		// const方法内部无法修改成员变量，
		// 需要给成员变量加mutable
		int tmp = ma;
		ma = mb;
		mb = tmp;
	}
	void show() {
		cout << ma << " " << mb << endl;
	}
private:
	mutable int ma;
	mutable int mb;
};

#if 0
int main() {
	auto func1 = []()->void { cout << "pdcHelloWorld" << endl; };
	func1();

	TestLambda<> t1;
	t1();

	auto func2 = [](int a, int b)->int {return a + b; };
	cout << "10 + 20 = " << func2(10, 20) << endl;

	TestLambda2<int> t2;
	cout << "10 + 20 = " << t2(10, 20) << endl;

	int a = 10;
	int b = 20;  // 实参
	/*
	auto func3 = []()->void {  // 注意ab不是形参
	    // 封闭函数局部变量不能在lambda内引用，除非位于捕获列表中
		int tmp = a; 
		a = b;
		b = a;
	}
	*/
	TestLambda3<int> t3(a, b);
	t3.show();
	t3();
	t3.show();

	auto func3 = [a, b]() mutable->void { 
		int tmp = a;
		a = b;
		b = tmp;
	};

	func3();
	cout << a << " " << b << endl;  // 但a b还是10 20，没有发生交换

	// 要加引用，应该是：
	// 传引用，类似与指针，交换函数里交换的是地址，所以不需要mutable
	auto func4 = [&a, &b]()->void {  // 不用加mutable
		int tmp = a;
		a = b;
		b = tmp;
	};
	func4();
	cout << a << " " << b << endl;

	return 0;
}
#endif

#if 0
// lambda有什么用？
int main() {
	vector<int> vec = { 1, 56,2,532,3 };
	sort(vec.begin(), vec.end(), [](int a, int b)->bool {return a > b; });
	for_each(vec.begin(), vec.end(), [](int a)->void {cout << a << " "; }); cout << endl;
	auto it = find_if(vec.begin(), vec.end(), [](int a)->bool {return a < 5; });
	vec.insert(it, 5);
	for_each(vec.begin(), vec.end(), [](int a)->void {cout << a << " "; }); cout << endl;

	return 0;
}
#endif

#if 0
int main() {
	function<int(int, int)> func = [](int a, int b)->int {return a + b; };
	cout << func(10, 20) << endl;

	map<int, function<int(int, int)>> caculateMap = {
		{1, [](int a, int b)->int {return a + b; }},
		{2, [](int a, int b)->int {return a - b; }},
		{3, [](int a, int b)->int {return a * b; }},
		{4, [](int a, int b)->int {return a / b; }},
	};

	cout << caculateMap[1](10, 20) << endl;
	cout << caculateMap[2](10, 20) << endl;
	cout << caculateMap[3](10, 20) << endl;
	cout << caculateMap[4](10, 20) << endl;

	return 0;
}
#endif

#if 1
#include<memory>
#include<queue>

class Deletor {
public:
	void operator()(int* p) {
		cout << "pdcHelloWorld" << endl;
		delete[] p;
	}
};  // 一般情况下要写这么一个类

class Data {
public:
	Data(int a = 10, int b= 20):ma(a), mb(b){}
	bool operator>(const Data& a)const {  // 一般情况下要写重载函数
		return ma > a.ma;
	}
	int ma;
	int mb;
};

int main() {
	// 智能指针的删除器
	unique_ptr<int, Deletor> p(new int[100]);
	unique_ptr<int, function<void(int*)>> p1(new int[100], 
		[](int* p)->void { cout << "pdcHelloWorld" << endl; delete[] p; });;

	// 优先级队列
	using FUNC = function<bool(Data&, Data&)>;
	priority_queue<Data, vector<Data>,FUNC> maxHeap([](Data& d1, Data& d2)->bool {
		return d1.ma > d2.mb;
		});
	maxHeap.push(Data(10, 20));
	maxHeap.push(Data(20, 50));
	maxHeap.push(Data(60, 20));
}
#endif