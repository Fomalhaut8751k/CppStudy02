#include<iostream>
#include<memory>

using namespace std;

// 用make_shared代替shared_ptr

int main() {
	// 各有各的优缺点
	shared_ptr<int> ptr1(new int(20));
	shared_ptr<int> ptr2 = make_shared<int>(20);
	auto ptr3 = make_shared<int>(20);

	return 0;
}