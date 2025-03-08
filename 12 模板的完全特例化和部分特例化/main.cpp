#include<iostream>

using namespace std;

/*
ģ�����ȫ�������ͷ���ȫ(����)������
ģ���ʵ������
*/

// Ϊʲô��Ҫģ���ʵ��������Ϊ����ĳЩ����ģ����ܲ������߼�
template<typename T>
bool compare(T a, T b) {
	cout << &a << " " << &b << endl;
	return a > b;
}

// ������(��ȫ��)
template<>
bool compare<const char*>(const char* a, const char* b) {
	return strcmp(a, b) > 0;
}


template<typename T>
class Vector {
public:
	Vector() { cout << "call Vector template init" << endl; }
};
// char*����ȫ�������汾�����ȼ����ڲ�������������ԭģ�壩
template<>
class Vector<char*> {
public:
	Vector() { cout << "call Vector char* init" << endl; }
};
// ָ�����͵Ĳ����������汾
template<typename Ty>
class Vector<Ty*> {
public:
	Vector() { cout << "call Vector Ty* init" << endl; }
};
// ����ָ�����͵Ĳ����������汾
template<typename R, typename A1, typename A2>
class Vector<R(*)(A1, A2)> {
public:
	Vector() {
		cout << "call Vector<R(*)(A1, A2)> init" << endl;
	}
};

int sum(int a, int b) { return a + b; }

int main() {
	const char* str1 = "bbb";
	const char* str2 = "aaa";
	// ����ϣ���Ƚϵ����ֵ��򣬵���ʵ������ǡ�������ַ�Ƚϣ�������Ҫ������
	cout << "bbb > aaa ? : " << compare(str1, str2) << endl;

	Vector<int> vec1;
	Vector<char*> vec2;
	Vector<int*> vec3;
	Vector<int(*)(int, int)> vec4; // ����ָ������
	Vector<int(int, int)> vec5;  // ��������

	// ����ָ������
	using PFUNC = int(*)(int, int);
	Vector<PFUNC> vec6;
	PFUNC pfunc = sum;
	cout << pfunc(20, 30) << endl;

	// ��������
	using FUNC = int(int, int);
	Vector<FUNC> vec7;
	FUNC* func = sum;
	cout << func(20, 30) << endl;

	return 0;

}