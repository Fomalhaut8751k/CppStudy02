#include<iostream>

using namespace std;

/*  ���󹤳�
	���ڿ��ǲ�Ʒ��һ���Ʒ(�й�����ϵ��ϵ�в�Ʒ)

	�й�����ϵ�Ķ������糵�� + ���� = ��(���)
	���չ�����������������һ�����ܵĹ�����һ�����ƵĹ��������
	��Щ�й�����ϵ��ϵ�в�Ʒ�����ܴ󣬾�Ҫ�����ܶ��������

	����������������󹤳�(��һ���й�����ϵ�Ĳ�Ʒ���ṩ��Ʒ�����ͳһ����)
*/

// ϵ�в�Ʒһ
class Car
{
public:
	Car(string name) : _name(name) {}
	virtual void show() = 0;
protected:
	string _name;
};

// ϵ�в�Ʒ��
class Light
{
public:
	virtual void show() = 0;
};


class Bmw : public Car
{
public:
	Bmw(string name) :Car(name) {}
	void show()
	{
		cout << "��ȡ��һ������������" << _name << endl;
	}

};

class BmwLight : public Light
{
public:
	void show() { cout << "BMW Light!" << endl; }
};


// ##### ���󹤳� ###################################################################
class AbstractFactory
{
public:
	virtual Car* createCar(string name) = 0;  // ��������(��)
	virtual Light* createCarLight() = 0;  // ��������
};
// ##################################################################################

// ������
class BMWFactory: public AbstractFactory
{
public:
	virtual Car* createCar(string name)
	{
		return new Bmw(name);
	}
	virtual Light* createCarLight()
	{
		return new BmwLight();
	}
};


class Audi : public Car
{
public:
	Audi(string name) :Car(name) {}
	void show()
	{
		cout << "��ȡ��һ���µ�������" << _name << endl;
	}
};

class AudiLight : public Light
{
public:
	void show() { cout << "AUDI Light!" << endl; }
};

int main()
{
	// ����һ�����󹤳�
	AbstractFactory* bmwfty1 = new BMWFactory();
	unique_ptr<AbstractFactory> bmwfty2(new BMWFactory());
	bmwfty1->createCar("ca7")->show();
	bmwfty2->createCarLight()->show();

	return 0;
}