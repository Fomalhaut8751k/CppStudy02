#include<iostream>

using namespace std;
/*  �Ż�������Ҫ��
1. �����������ݹ����У������Ż������ô��ݣ���Ҫ��ֵ����
	���Ա���ʵ�ε��βεĿ�������Ͷ�Ӧ������ -2��
2. ֱ�ӷ�����ʱ���󣬲�Ҫ�ȴ����ֲ������ٷ�����
	������ʱ������ʱ���󿽱�������main����ջ֡�ϵ��µ���ʱ����Ĳ���ᱻ
	�Ż���Test(val)�����ʱ���󲻻���������� -2��
3. ����Ĵ���һ���ǳɣ���Ҫ�������ٸ�ֵ����
	Test t2;              =>               Test t2 = GetObject(t1, 1);  
	t2 = GetObject(t1);
	��ΪGetObject���ص���һ����ʱ���������������ֿ����Ż���һ���вι���
	��˿��Լ�����ʱ����Ĺ�����������Լ�operator=  -3��

*/

class Test {
public:
	Test(int a = 10) :ma(a) { cout << "Test(int)" << endl; }
	~Test() { cout << "~Test()" << endl; }
	Test(const Test& t) : ma(t.ma) { cout << "Test(const Test&)" << endl; }
	Test(Test&& t) : ma(t.ma) { cout << "Test(Test&&)" << endl; }
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

Test GetObject(Test& t) {
	// ��ȡ��Ա����ma
	int val = t.getData();  

	// ���ݻ�ȡ��ֵ��������
	cout << "pdcHelloWorld" << endl;
	Test tmp(val);  // 4. Test(int)

	// ���ش����Ķ���
	return tmp;  // 5. Test(const Test&)  ���ݷ���ֵ�������쵽main����ջ֡�ϵ���ʱ����
				 // 6. ~Test()  tmp������
}

Test GetObject(Test& t, int label) {
	//cout << "pdcHelloWorld" << endl;
	int val = t.getData();
	return Test(val);  // ����ʱ���󿽱�����һ���¶���ʱ�����������Զ��Ż�
					   // �������ʱ����Test(val)��������һ����main����ջ֡
	                   // �ϵ��µ���ʱ����������ﱻ�Ż��ˣ�ֱ�ӹ���main
	                   // ����ջ֡�ϵ���ʱ��������Test(val)�����ʱ����û
	                   // �в��������Ҳ����Ҫ����
}

int main() {
#if 0
	Test t1;  // 1. Test(int)
	Test t2;  // 2. Test(int)
	t2 = GetObject(t1);  // 3. Test(const Test&)  �βδ���
					     // 7. ~Test()  �βε�����
						 // 8. operator=
	                     // 9. ~Test()  mainջ֡����ʱ���������
	return 0;  // 10. ~Test()  t2������
		       // 11. ~Test()  t1������
#endif
#if 1
	Test t1;
	Test t2 = GetObject(t1, 1);

	return 0;
#endif
}

