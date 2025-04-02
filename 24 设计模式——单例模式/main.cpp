#include<iostream>
#include<mutex>

using namespace std;

/*  单例模式
	一个类不管创建多少次对象，永远只能得到该类型一个对象的实力
*/


// 饿汉式单例模式：还没有获取实例对象，实例对象就已经产生了
#if 0
class Singleton
{
public:
	// 获取类的唯一实例对象的接口方法
	static Singleton* getInstance()
	{
		return &instance;
	}

private:
	static Singleton instance;  // 定义一个唯一的实例对象
	// 构造函数私有化
	Singleton()
	{
		
	}
	// 防止拷贝构造和赋值
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
};
Singleton Singleton::instance;
#endif


// 懒汉式单例模式：唯一的实例对象，直到第一次获取它的时候才产生
// 是否线程安全？
#if 0
std::mutex mtx;

class Singleton
{
public:
	// 获取类的唯一实例对象的接口方法
	static Singleton* getInstance()  // 是不是可重入函数(还没执行完能否被再次调用)？
	{
		/* 如果某个线程进入后还未new，另一个线程就进来了，看到的也是nullptr
		 因此不能直接用在多线程，不是线程安全的，因此需要互斥.....*/
		
		if (instance == nullptr)
		{
			// 锁+双重判断
			lock_guard<std::mutex> guard(mtx);
			if(instance == nullptr)
				instance = new Singleton();
		}

		return instance;
	}

private:
	static Singleton *instance;  // 定义一个唯一的实例对象
	// 构造函数私有化
	Singleton()
	{
		
	}
	// 防止拷贝构造和赋值
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
};
Singleton* Singleton::instance = nullptr;
#endif

// 第二种懒汉单例模式，不需要互斥锁
#if 1
std::mutex mtx;

class Singleton
{
public:
	// 获取类的唯一实例对象的接口方法
	static Singleton* getInstance()  // 是不是可重入函数(还没执行完能否被再次调用)？
	{
		/*
			静态的局部变量，程序的启动阶段，内存就存在于数据端上
			但是初始化是等到运行到该代码时才初始化的
			静态局部变量只执行一次初始化
			函数静态局部变量的初始化，在汇编指令上已经自动添加线程互斥指令了
		*/
		
		static Singleton instance; 
		return &instance;
	}

private:
	
	// 构造函数私有化
	Singleton()
	{

	}
	// 防止拷贝构造和赋值
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
};

#endif


int main()
{
	Singleton* p1 = Singleton::getInstance();
	Singleton* p2 = Singleton::getInstance();
	Singleton* p3 = Singleton::getInstance();

	return 0;
}