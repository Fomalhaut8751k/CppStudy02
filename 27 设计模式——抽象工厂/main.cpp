#include<iostream>

using namespace std;

/*  抽象工厂
	现在考虑产品，一类产品(有关联关系的系列产品)

	有关联关系的东西：如车架 + 车灯 = 车(大概)
	按照工厂方法来做：创建一个车架的工厂，一个车灯的工厂，如果
	这些有关联关系的系列产品数量很大，就要创建很多个工厂。

	——解决方法：抽象工厂(对一组有关联关系的产品簇提供产品对象的统一创建)
*/

// 系列产品一
class Car
{
public:
	Car(string name) : _name(name) {}
	virtual void show() = 0;
protected:
	string _name;
};

// 系列产品二
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
		cout << "获取了一辆宝马汽车：" << _name << endl;
	}

};

class BmwLight : public Light
{
public:
	void show() { cout << "BMW Light!" << endl; }
};


// ##### 抽象工厂 ###################################################################
class AbstractFactory
{
public:
	virtual Car* createCar(string name) = 0;  // 创建汽车(架)
	virtual Light* createCarLight() = 0;  // 创建车灯
};
// ##################################################################################

// 宝马工厂
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
		cout << "获取了一辆奥迪汽车：" << _name << endl;
	}
};

class AudiLight : public Light
{
public:
	void show() { cout << "AUDI Light!" << endl; }
};

int main()
{
	// 创建一个抽象工厂
	AbstractFactory* bmwfty1 = new BMWFactory();
	unique_ptr<AbstractFactory> bmwfty2(new BMWFactory());
	bmwfty1->createCar("ca7")->show();
	bmwfty2->createCarLight()->show();

	return 0;
}