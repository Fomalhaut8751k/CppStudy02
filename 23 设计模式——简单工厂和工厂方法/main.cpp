#include<iostream>
#include<string>
#include<memory>
#include<functional>
#include<map>

using namespace std;

/*
	简单工厂 simple factory
	工厂方法 factory method
	抽象工厂 abstract factory

	场景：项目中的类比较多
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
		cout << "获取了一辆宝马汽车：" << _name << endl;
	}

};

class Audi : public Car
{
public:
	Audi(string name) :Car(name) {}
	void show()
	{
		cout << "获取了一辆奥迪汽车：" << _name << endl;
	}
};

#if 0 // 方案一(简单工厂)
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
			cerr << "传入工厂的参数不正确: " << ct << endl;
			break;
		}
		return nullptr;
	}
};

#endif
#if 0 // 方案二
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
	// 当类的数量多了之后，开发者就必须记住这些类名

	SimpleFactory* factory = new SimpleFactory();
	Car* p1 = factory->createCar(1);
	Car* p2 = factory->createCar(2);
	unique_ptr<Car> p3(factory->createCar(1));

	// 把对象的创建封装起来
	p1->show();
	p2->show();
	p3->show();

	delete p1;
	delete p2;

	delete factory;


	return 0;
}
#endif
#if 1  // 工厂方法
class Factory
{
public:
	virtual Car* createCar(string name) = 0;  // 工厂方法
};
// 宝马工厂
class BMWFactory : public Factory
{  
public:
	Car* createCar(string name)
	{
		return new Bmw(name);
	}
};
// 奥迪工厂
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

	// 把对象的创建封装起来
	p1->show();
	p2->show();
	return 0;
}
#endif
