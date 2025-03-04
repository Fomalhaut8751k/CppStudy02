#include<iostream>
#include<vector>

#include<algorithm>
#include<functional>

using namespace std;

/*
	c++绑定器：
	1. bind1st: operator()的第一个形参变量绑定成一个确定的值
	2. bind2nd: operator()的第二个形参变量绑定成一个确定的值

	c++11的bind绑定器和function函数对象机制

	lambda表达式 底层依赖函数对象的机制实现的
*/

class Less {
public:
	bool operator()(int a, int b) {
		return a < b;
	}
};

template<typename Container>
void showContainer(Container& con) {
	typename Container::iterator it = con.begin();
	for (; it != con.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;
}

// ################  my_find_if的实现 ##################
template<typename Iterator, typename Compare>
Iterator my_find_if(Iterator first, Iterator last, Compare comp) {
	for (; first != last; ++first) {  // 遍历
		if (comp(*first)) {
			return first;
		}
	}
	return last;
}

// ################ _mybind1st的实现 ###################
template<typename Compare, typename T>
class _my_bind1st {  // 一元函数对象
public:
	_my_bind1st(Compare& con, T val)
		:_Con(con), _Val(val){

	}

	bool operator()(const T& val) {
		return _Con(_Val, val);
	}

private:
	Compare _Con;
	T _Val;
};

// ################ my_bind1st的实现 ###################
template<typename Compare, typename T>
_my_bind1st<Compare,T> my_bind1st(Compare con, const T& val) {
	return _my_bind1st<Compare, T>(con, val);
}

int main() {
	vector<int> vec;
	srand(time(nullptr));
	for (int i = 0; i < 10; ++i) {
		vec.push_back(rand() % 20 + 1);
	}
	sort(vec.begin(), vec.end());
	//for_each(vec.begin(), vec.end(), [](int a)->void {cout << a << " "; });
	showContainer(vec);

	// greater 二元函数对象，需要两个元素
	sort(vec.begin(), vec.end(), greater<int>());
	showContainer(vec);

	// 插入10，就要找第一个小于10的数
	//      这一块必须是一个一元的函数对象    [                       ]
	// greater和less都是二元的，无法使用
	// 通过绑定器bind1st， 把10绑定到第一个参数，即 10 > a  ? 就变成一元的了
	auto it = find_if(vec.begin(), vec.end(), my_bind1st(greater<int>(), 10));
	if (it != vec.end()) { 
		vec.insert(it, 10);
	} // 如果没有比10小的，那么10就是最小的，放最后也可以
	showContainer(vec);


	auto it2 = find_if(vec.begin(), vec.end(), [](int a)->bool {return a < 10; });
	
	

	return 0;
}