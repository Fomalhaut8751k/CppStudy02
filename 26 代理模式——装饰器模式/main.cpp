#include<iostream>

using namespace std;

/* װ����ģʽ
	װ��������Ҫ������������Ĺ��ܣ����ǣ�����������Ĺ��ܣ�����
	��һ������������������һ�����ࡣ
	Ϊ����ǿ������Ĺ��ܣ�ͨ��ʵ������ķ�ʽ����д�ӿڣ��ǿ�����ɹ�����չ�ģ�
	���Ǵ�������̫��������ӽ�����
*/

class Car
{
public:
	Car(int speed) :_cSpeed(speed) {};
	virtual void show() = 0;  // �Ǳ���֮�ٶ�
protected:
	int _cSpeed;
};


class BMW : public Car
{
public:
	BMW(int speed) :Car(speed) {};
	void show()
	{
		cout << "��ǰʱ��Ϊ: " << _cSpeed << "����ÿСʱ" << endl;
	}
};

int main()
{


	return 0;
}