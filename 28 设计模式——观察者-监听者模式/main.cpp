#include<iostream>
#include<unordered_map>
#include<list>

using namespace std;

/* 行为型模式
    主要关注的是对象之间的通信
	观察者-监听者模式(发送-订阅模式)：主要关注的是对象的一对多的关系，也就是多个
	对象都依赖于一个对象，当该对象的状态发生改变时，其他对象都能够接收到相应的通知

*/

// 观察者抽象类
class Observer
{
public:
	virtual void handle(int msgid) = 0;
};

// 第一个观察者实例
class Observer1 : public Observer
{
public:
	void handle(int msgid)
	{
		switch (msgid)
		{
		case 1:
			cout << "Observer1 recv 1 msg1!" << endl;
			break;
		case 2:
			cout << "Observer1 recv 2 msg1!" << endl;
			break;
		default:
			cout << "Observer1 recv unknow msg1!" << endl;
			break;
		}
	}
};

// 第二个观察者实例
class Observer2 : public Observer
{
public:
	void handle(int msgid)
	{
		switch (msgid)
		{
		case 2:
			cout << "Observer2 recv 2 msg1!" << endl;
			break;
		default:
			cout << "Observer2 recv unknow msg1!" << endl;
			break;
		}
	}
};

// 第三个观察者实例
class Observer3 : public Observer
{
public:
	void handle(int msgid)
	{
		switch (msgid)
		{
		case 1:
			cout << "Observer3 recv 1 msg1!" << endl;
			break;
		default:
			cout << "Observer3 recv unknow msg1!" << endl;
			break;
		}
	}
};


class Subject
{
public:
	void addObserver(Observer* obser, int msgid)
	{
		_subMap[msgid].push_back(obser);
	}

	void dispatch(int msgid)
	{
		auto it = _subMap.find(msgid);
		if (it != _subMap.end())
		{
			for (Observer* pObser : it->second)
			{
				pObser->handle(msgid);
			}
		}
	}
private:
	unordered_map<int, list<Observer*>> _subMap;
};

int main()
{
	Subject subject;
	Observer* p1 = new Observer1();
	Observer* p2 = new Observer2();
	Observer* p3 = new Observer3();

	subject.addObserver(p1, 1);
	subject.addObserver(p1, 2);
	subject.addObserver(p2, 1);
	subject.addObserver(p3, 2);

	int msgid = 0;
	for (;;)
	{
		cout << "输入消息id：" << endl;
		cin >> msgid;
		if (msgid == -1)
			break;
		subject.dispatch(msgid);
	}

	return 0;
}