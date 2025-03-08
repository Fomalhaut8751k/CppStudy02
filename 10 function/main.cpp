#include<iostream>
#include<vector>
#include<string>
#include<map>

#include<functional>
#include<algorithm>

/*
	function: ��������������lambda���ʽ ����ֻ��ʹ����һ�������
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
	// �Ѻ���ת��Ϊ��������

	// ��function����ģ�嶨�崦������ϣ����һ����������ʵ����function
	std::function<void()> func1 = pdc1;  // func1(pdc1); ���캯��
	func1();  // func1.operator();

	/*
		std::function<void(string)>  void�Ƿ���ֵ��string�ǲ����б�
	*/

	std::function<void(std::string)> func2 = pdc2;
	func2("HelloWorld");

	std::function<int(int, int)> func3 = sum;
	std::cout << func3(10, 20) << std::endl;

	std::function<int(int, int)> func4 = [](int a, int b)->int {return a + b; };
	std::cout << func4(100, 200) << std::endl;

	std::function<void(Test*, std::string)> func5 = &Test::hello;  // Ҫȡ��ַ
	Test t;
	func5(&t, "call Test::hello!");

	return 0;
}
#endif
#if 1  // function��ʲô��

void doShowAllBooks() { std::cout << "�鿴�����鼮��Ϣ" << std::endl; }
void doBorrow() { std::cout << "����" << std::endl; }
void doBack() { std::cout << "����" << std::endl; }
void doQueryBooks() { std::cout << "��ѯ�鼮" << std::endl; }
void doLoginOut() { std::cout << "ע��" << std::endl; }


// ################## function�ĵײ�ԭ�� ######################
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
	������������n���βθ�����ͬ�ĺ�����Ҫдn��������function
*/
template<typename Fty>
class myfunction_ {};
// ģ��Ĳ���ʵ����
template<typename R, typename... A1>  // ��... �ɱ�Σ����������ǲ�����
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
		std::cout << "- 1.�鿴�����鼮��Ϣ -" << std::endl;
		std::cout << "- 2.      ����       -" << std::endl;
		std::cout << "- 3.      ����       -" << std::endl;
		std::cout << "- 4.    ��ѯ�鼮     -" << std::endl;
		std::cout << "- 5.      ע��       -" << std::endl;
		std::cout << "----------------------" << std::endl;
		std::cout << "��ѡ��: ";
		std::cin >> choice;


		switch (choice) {   ��δ��벻�ã��޷��պϣ��޷���������ԭ��
		case 1:             ��Ϊ���ſ��������¹��ܾ�Ҫ���ӣ��ĸ���Ҫ
			break;          �˾�Ҫɾ����������ȥ�ġ�
		case 2:
			break;
		case 3:
			break;
		}

		auto it = actionMap.find(choice);
		if (it == actionMap.end()) {
			std::cout << "����������Ч��������ѡ��" << std::endl;
		}
		else {
			it->second();
		}
	}
	*/

	return 0;
}
#endif