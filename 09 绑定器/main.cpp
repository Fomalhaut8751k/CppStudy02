#include<iostream>
#include<vector>

#include<algorithm>
#include<functional>

using namespace std;

/*
	c++������
	1. bind1st: operator()�ĵ�һ���βα����󶨳�һ��ȷ����ֵ
	2. bind2nd: operator()�ĵڶ����βα����󶨳�һ��ȷ����ֵ

	c++11��bind������function�����������

	lambda���ʽ �ײ�������������Ļ���ʵ�ֵ�
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

// ################  my_find_if��ʵ�� ##################
template<typename Iterator, typename Compare>
Iterator my_find_if(Iterator first, Iterator last, Compare comp) {
	for (; first != last; ++first) {  // ����
		if (comp(*first)) {
			return first;
		}
	}
	return last;
}

// ################ _mybind1st��ʵ�� ###################
template<typename Compare, typename T>
class _my_bind1st {  // һԪ��������
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

// ################ my_bind1st��ʵ�� ###################
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

	// greater ��Ԫ����������Ҫ����Ԫ��
	sort(vec.begin(), vec.end(), greater<int>());
	showContainer(vec);

	// ����10����Ҫ�ҵ�һ��С��10����
	//      ��һ�������һ��һԪ�ĺ�������    [                       ]
	// greater��less���Ƕ�Ԫ�ģ��޷�ʹ��
	// ͨ������bind1st�� ��10�󶨵���һ���������� 10 > a  ? �ͱ��һԪ����
	auto it = find_if(vec.begin(), vec.end(), my_bind1st(greater<int>(), 10));
	if (it != vec.end()) { 
		vec.insert(it, 10);
	} // ���û�б�10С�ģ���ô10������С�ģ������Ҳ����
	showContainer(vec);


	auto it2 = find_if(vec.begin(), vec.end(), [](int a)->bool {return a < 10; });
	
	

	return 0;
}