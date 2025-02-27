#include<iostream>

using namespace std;

class Test {
public:
	Test(int a = 10) :ma(a) { cout << "Test(int)" << endl; }
	~Test() { cout << "~Test()" << endl; }
	Test(const Test& t) : ma(t.ma) { cout << "Test(const Test&)" << endl; }
	Test& operator =(const Test& t) {
		if (this == &t) {
			return *this;
		}
		cout << "operator=" << endl;
		this->ma = t.ma;
		return *this;
	}
	int getData() const { return ma; }
private:
	int ma;
};

Test GetObject(Test t) {
	// ��ȡ��Ա����ma
	int val = t.getData();  

	// ���ݻ�ȡ��ֵ��������
	Test tmp(val);  // 4. Test(int)

	// ���ش����Ķ���
	return tmp;  // 5. Test(const Test&)  ���ݷ���ֵ�������쵽main����ջ֡�ϵ���ʱ����
				 // 6. ~Test()  tmp������
}

int main() {
	Test t1;  // 1. Test(int)
	Test t2;  // 2. Test(int)
	t2 = GetObject(t1);  // 3. Test(const Test&)  �βδ���
					     // 7. ~Test()  �βε�����
						 // 8. operator=
	                     // 9. ~Test()  mainջ֡����ʱ���������
	return 0;  // 10. ~Test()  t2������
		       // 11. ~Test()  t1������
}