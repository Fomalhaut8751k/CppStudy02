#include<iostream>

using namespace std;

/* 装饰器模式
	装饰器：主要是增加现有类的功能，但是，增加现有类的功能，还有
	另一个方法，就是新增加一个子类。
	为了增强现有类的功能，通过实现子类的方式，重写接口，是可以完成功能扩展的，
	但是代码中有太多子类添加进来的
*/

class Car
{
public:
	Car(int speed = 120) :_cSpeed(speed) {};
	virtual void show() = 0;  // 仪表盘之速度

	int _cSpeed;
};


class BMW : public Car
{
public:
	BMW(int speed = 120) :Car(speed) {};
	void show()
	{
		cout << "宝马_当前时速为: " << _cSpeed << "公里每小时" << endl;
	}
};

class AUDI : public Car
{
public:
	AUDI(int speed = 120) :Car(speed) {};
	void show()
	{
		cout << "奥迪_当前时速为: " << _cSpeed << "公里每小时" << endl;
	}
};

// 装饰器
class CarDecorator01 : public Car
{
public:
	CarDecorator01(Car* p):pCar(p){}
	void show()
	{
		cout << "开启定速巡航，时速为80" << endl;
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
		cout << "开启紧急制动，时速为0" << endl;
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