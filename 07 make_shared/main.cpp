#include<iostream>
#include<memory>

using namespace std;

// ��make_shared����shared_ptr

int main() {
	// ���и�����ȱ��
	shared_ptr<int> ptr1(new int(20));
	shared_ptr<int> ptr2 = make_shared<int>(20);
	auto ptr3 = make_shared<int>(20);

	return 0;
}