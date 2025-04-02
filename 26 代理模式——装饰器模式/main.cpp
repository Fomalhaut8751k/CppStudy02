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
	Car(int speed) :_cSpeed(speed) {};
	virtual void show() = 0;  // 仪表盘之速度
protected:
	int _cSpeed;
};


class BMW : public Car
{
public:
	BMW(int speed) :Car(speed) {};
	void show()
	{
		cout << "当前时速为: " << _cSpeed << "公里每小时" << endl;
	}
};

int main()
{


	return 0;
}