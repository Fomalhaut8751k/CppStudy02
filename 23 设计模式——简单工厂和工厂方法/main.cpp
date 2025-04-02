#include<iostream>
#include<string>
#include<memory>
#include<functional>
#include<map>

using namespace std;

/*
	�򵥹��� simple factory
	�������� factory method
	���󹤳� abstract factory

	��������Ŀ�е���Ƚ϶�
*/

class Car
{
public:
	Car(string name) : _name(name) {}
	virtual void show() = 0;
protected:
	string _name;
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

class Audi : public Car
{
public:
	Audi(string name) :Car(name) {}
	void show()
	{
		cout << "��ȡ��һ���µ�������" << _name << endl;
	}
};

#if 0 // ����һ(�򵥹���)
enum CarType
{
	BMW, AUDI
};
class SimpleFactory
{
public:
	Car* createCar(CarType ct)
	{
		switch (ct)
		{
		case BMW:
			return new Bmw("X1");
			break;
		case AUDI:
			return new Audi("A6");
			break;
		default:
			cerr << "���빤���Ĳ�������ȷ: " << ct << endl;
			break;
		}
		return nullptr;
	}
};

#endif
#if 0 // ������
class SimpleFactory
{
public:
	Car* createCar(int choice)
	{
		map<int, function<Car*()>> mapCarType = { 
			{1,[]()->Car(*) { return new Bmw("X1"); }}, 
			{2,[]()->Car(*) { return new Audi("A6"); }}
		};
		return mapCarType[choice]();
	}
};
int main()
{
	//Car* p1 = new BMW("X1");
	//Car* p2 = new Audi("A6");
	// �������������֮�󣬿����߾ͱ����ס��Щ����

	SimpleFactory* factory = new SimpleFactory();
	Car* p1 = factory->createCar(1);
	Car* p2 = factory->createCar(2);
	unique_ptr<Car> p3(factory->createCar(1));

	// �Ѷ���Ĵ�����װ����
	p1->show();
	p2->show();
	p3->show();

	delete p1;
	delete p2;

	delete factory;


	return 0;
}
#endif
#if 1  // ��������
class Factory
{
public:
	virtual Car* createCar(string name) = 0;  // ��������
};
// ������
class BMWFactory : public Factory
{  
public:
	Car* createCar(string name)
	{
		return new Bmw(name);
	}
};
// �µϹ���
class AUDIFactory : public Factory
{
public:
	Car* createCar(string name)
	{
		return new Audi(name);
	}
};

int main()
{
	unique_ptr<Factory> bmwfty(new BMWFactory());
	unique_ptr<Factory> audifty(new AUDIFactory());
	unique_ptr<Car> p1(bmwfty->createCar("X1"));
	unique_ptr<Car> p2(audifty->createCar("A6"));

	// �Ѷ���Ĵ�����װ����
	p1->show();
	p2->show();
	return 0;
}
#endif
