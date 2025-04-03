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
	Car(int speed = 120) :_cSpeed(speed) {};
	virtual void show() = 0;  // �Ǳ���֮�ٶ�

	int _cSpeed;
};


class BMW : public Car
{
public:
	BMW(int speed = 120) :Car(speed) {};
	void show()
	{
		cout << "����_��ǰʱ��Ϊ: " << _cSpeed << "����ÿСʱ" << endl;
	}
};

class AUDI : public Car
{
public:
	AUDI(int speed = 120) :Car(speed) {};
	void show()
	{
		cout << "�µ�_��ǰʱ��Ϊ: " << _cSpeed << "����ÿСʱ" << endl;
	}
};

// װ����
class CarDecorator01 : public Car
{
public:
	CarDecorator01(Car* p):pCar(p){}
	void show()
	{
		cout << "��������Ѳ����ʱ��Ϊ80" << endl;
		pCar->_cSpeed = 80;
		pCar->show();
	}
private:
	Car* pCar;
};

class CarDecorator02 : public Car
{
public:
	CarDecorator02(Car* p) :pCar(p) {}
	void show()
	{
		cout << "���������ƶ���ʱ��Ϊ0" << endl;
		pCar->_cSpeed = 0;
		pCar->show();
	}
private:
	Car* pCar;
};

int main()
{
	Car* bmw_decorator = new CarDecorator01(new BMW());
	bmw_decorator->show();

	bmw_decorator = new CarDecorator02(new BMW());
	bmw_decorator->show();


	return 0;
}