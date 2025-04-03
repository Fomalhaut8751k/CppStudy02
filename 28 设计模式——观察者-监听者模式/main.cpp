#include<iostream>
#include<unordered_map>
#include<list>

using namespace std;

/* ��Ϊ��ģʽ
    ��Ҫ��ע���Ƕ���֮���ͨ��
	�۲���-������ģʽ(����-����ģʽ)����Ҫ��ע���Ƕ����һ�Զ�Ĺ�ϵ��Ҳ���Ƕ��
	����������һ�����󣬵��ö����״̬�����ı�ʱ�����������ܹ����յ���Ӧ��֪ͨ

*/

// �۲��߳�����
class Observer
{
public:
	virtual void handle(int msgid) = 0;
};

// ��һ���۲���ʵ��
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

// �ڶ����۲���ʵ��
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

// �������۲���ʵ��
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
		cout << "������Ϣid��" << endl;
		cin >> msgid;
		if (msgid == -1)
			break;
		subject.dispatch(msgid);
	}

	return 0;
}