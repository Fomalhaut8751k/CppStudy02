#include<iostream>
#include<typeinfo>

#include<string>
#include<functional>
using namespace std;

/*
	c++ bind���� =�����ؽ������һ����������
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

	// bind�Ǻ���ģ�壬�����Զ�����ģ���������
	auto binder1 = bind(hello, "pdcHelloWorld");
	binder1();
	//cout << typeid(STR_FUNC).name() << endl;

	using STR_FUNC = function<void(void)>;  // ���˾Ͳ���Ҫ��������
	STR_FUNC sf = bind(hello, "pdcHelloWorld");
	sf();

	using SUM_FUNC = function<int(void)>;
	//SUM_FUNC sumf = bind(sum, 10);  ����ֻ��һ����Ҫռλ
	auto sumf = bind(sum, 10, 20);
	cout << sumf() << endl;

	cout << bind(&Test::sum, Test(), 10, 20)() << endl;  // ֱ�ӵ���

	// ����ռλ��
	cout << bind(&Test::sum, Test(), placeholders::_1, 20)(30) << endl;  // ���Ǿ���ҪΪռλ��λ�ô������
	cout << bind(&Test::sum, Test(), 65, placeholders::_1)(30) << endl;
	cout << bind(&Test::sum, Test(), placeholders::_1, placeholders::_2)(30, 6) << endl;
	// ����20������

	function<int(int)> func1 = bind(&Test::sum, Test(), 65, placeholders::_1);

	return 0;
}