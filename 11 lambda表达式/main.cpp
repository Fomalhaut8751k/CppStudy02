#include<iostream>
#include<functional>
#include<vector>
#include<algorithm>

#include<map>

using namespace std;

/*
	c++11 ��������������� -> lambda���ʽ
	���������ȱ�㣺
	ʹ���ڷ����㷨��������   �Ƚ�����/�Զ������  ���ȼ����У���Ҫ����������
	���������������ط���������

	lambda���﷨��
	[�����ⲿ����](�β��б�)->����ֵ����{��������};

	[�����ⲿ����]��
	[] �������ⲿ����
	[=] �Դ�ֵ�ķ�ʽ�����ⲿ�����б���
	[&] �Դ����õķ�ʽ�����ⲿ�����б���
	[this] �����ⲿ��thisָ��
	[=, &a] �Դ�ֵ�ķ�ʽ�����ⲿ���еı���������a�����Դ����õķ�ʽ����
	[a, b] ��ֵ���ݵķ�ʽ�����ⲿ����a��b
	[a, &b] a��ֵ���ݲ���b�Դ����õķ�ʽ����
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
		// const�����ڲ��޷��޸ĳ�Ա������
		// ��Ҫ����Ա������mutable
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
	int b = 20;  // ʵ��
	/*
	auto func3 = []()->void {  // ע��ab�����β�
	    // ��պ����ֲ�����������lambda�����ã�����λ�ڲ����б���
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
	cout << a << " " << b << endl;  // ��a b����10 20��û�з�������

	// Ҫ�����ã�Ӧ���ǣ�
	// �����ã�������ָ�룬���������ｻ�����ǵ�ַ�����Բ���Ҫmutable
	auto func4 = [&a, &b]()->void {  // ���ü�mutable
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
// lambda��ʲô�ã�
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
};  // һ�������Ҫд��ôһ����

class Data {
public:
	Data(int a = 10, int b= 20):ma(a), mb(b){}
	bool operator>(const Data& a)const {  // һ�������Ҫд���غ���
		return ma > a.ma;
	}
	int ma;
	int mb;
};

int main() {
	// ����ָ���ɾ����
	unique_ptr<int, Deletor> p(new int[100]);
	unique_ptr<int, function<void(int*)>> p1(new int[100], 
		[](int* p)->void { cout << "pdcHelloWorld" << endl; delete[] p; });;

	// ���ȼ�����
	using FUNC = function<bool(Data&, Data&)>;
	priority_queue<Data, vector<Data>,FUNC> maxHeap([](Data& d1, Data& d2)->bool {
		return d1.ma > d2.mb;
		});
	maxHeap.push(Data(10, 20));
	maxHeap.push(Data(20, 50));
	maxHeap.push(Data(60, 20));
}
#endif