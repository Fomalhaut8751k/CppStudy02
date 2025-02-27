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
private:
	int ma;
};

#if 0
int main() {
	cout << "---------------------------------------------------------------" << endl;

	Test t1;  // Test(int)
	Test t2(t1);  // Test(const Test&)
	Test t3 = t1;  // Test(const Test&)  ���Ƹ�ֵ���������Ǵ������̣����ǿ���
	Test t4 = Test(20);
	/*
		����ȥ���ǣ�
		Test(int)  ��������Ĺ���(��ʾ������������)
		Test(const Test&) ��������
		Test(int)

		ʵ���Ͻ������Ż��� Test t4 = Test(20);  => Test t4(20); ������
		Test(int)
		����ʱ���������¶����ʱ����ʱ����Ͳ������ˣ�ֱ�ӹ����¶���Ϳ�����
	*/
	cout << "---------------------------------------------------------------" << endl;

	t4 = t2;  // operator=������t4�Ѿ�������ɣ�����Ǹ�ֵ
	t4 = Test(30); // Test(int)  ��ʽ������ʱ����
				   // operator=
				   // ~Test()
	t4 = (Test)30;  // Test(int), ǿ������ת�����Һ��ʵĹ��캯������ʽ������ʱ����
					// operator=
					// ~Test()
	t4 = 30;  // Test(int), ��ʽ������ʱ����
			  // operator=
			  // ~Test()

	cout << "---------------------------------------------------------------" << endl;
	// Test* p = &Test(40);  // ��ʱ���󴴽���p�洢��ʱ����ĵ�ַ����ʱ�����������
							 // ��������pָ��ĵ�ַ����Ч�ˣ��ǲ���ȫ��
	const Test& ref = Test(40);  // Test(int)  ������䣬��ʱ��������
								 // ��ʱ����û�����֣������������������������˱�����
								 // ��˲�������������ǰ�ȫ��
	system("pause");
	return 0;
}
#endif

class Test2 {
public:
	Test2(int a = 10, int b = 20) :ma(a), mb(b) { cout << "Test2(int, int)" << endl; }
	~Test2() { cout << "~Test2()" << endl; }
	Test2(const Test2& t) : ma(t.ma), mb(t.mb) { cout << "Test2(const Test&)" << endl; }
	Test2& operator =(const Test2& t) {
		if (this == &t) {
			return *this;
		}
		cout << "operator=" << endl;
		this->ma = t.ma;
		this->mb = t.mb;
		return *this;
	}
private:
	int ma;
	int mb;
};

#if 1
Test2 t1(10, 10);  // 1. Test2(int, int)

int main() {
	cout << "---------------------------------------------------------------" << endl;
	Test2 t2(20, 20);  // 3. Test2(int, int)
	Test2 t3 = t2;  //4.  Test2(const Test&)
	static Test2 t4 = Test2(30, 30);  // 5. Test2(int, int) �Ż�
	/*
		��̬�����ڱ���ʱ�ڿ����ڴ棬����������ʱ�ڲų�ʼ��
	*/
	t2 = Test2(40, 40); // 6. Test2(int, int)
						// 7. opreator=
						// 8. ~Test()
	t2 = (Test2)(50, 50);  // 9. Test2(int, int)
						   // 10. opreator=
						   // 11. ~Test()
	t2 = 60; // 12. Test2(int, int)
			 // 13. opreator=
			 // 14. ~Test()
	Test2* p1 = new Test2(70, 70);  // 15. Test2(int, int)
	Test2* p2 = new Test2[2];  // 16. Test2(int, int)
							   // 17. Test2(int, int)
	// Test2* p3 = &Test2(80, 80);
	const Test2& p4 = Test2(90, 90);  // 18.Test2(int, int)
	delete p1;  // 19. ~Test2()
	delete[] p2;  // 20. ~Test2()
				// 21. ~Test2()
	system("pause");
	return 0;
	// 22. ~Test2()   p4
	// 23. ~Test2()   t3
	// 24. ~Test2()   t2
}
Test2 t5(100, 100);  // 2. Test2(int, int)
// 25. ~Test2()  t4 ��̬������������ͷ�
// 26. ~Test2()  t5
// 27. ~Test2()  t1
#endif