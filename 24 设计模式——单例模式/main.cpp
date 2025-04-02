#include<iostream>
#include<mutex>

using namespace std;

/*  ����ģʽ
	һ���಻�ܴ������ٴζ�����Զֻ�ܵõ�������һ�������ʵ��
*/


// ����ʽ����ģʽ����û�л�ȡʵ������ʵ��������Ѿ�������
#if 0
class Singleton
{
public:
	// ��ȡ���Ψһʵ������Ľӿڷ���
	static Singleton* getInstance()
	{
		return &instance;
	}

private:
	static Singleton instance;  // ����һ��Ψһ��ʵ������
	// ���캯��˽�л�
	Singleton()
	{
		
	}
	// ��ֹ��������͸�ֵ
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
};
Singleton Singleton::instance;
#endif


// ����ʽ����ģʽ��Ψһ��ʵ������ֱ����һ�λ�ȡ����ʱ��Ų���
// �Ƿ��̰߳�ȫ��
#if 0
std::mutex mtx;

class Singleton
{
public:
	// ��ȡ���Ψһʵ������Ľӿڷ���
	static Singleton* getInstance()  // �ǲ��ǿ����뺯��(��ûִ�����ܷ��ٴε���)��
	{
		/* ���ĳ���߳̽����δnew����һ���߳̾ͽ����ˣ�������Ҳ��nullptr
		 ��˲���ֱ�����ڶ��̣߳������̰߳�ȫ�ģ������Ҫ����.....*/
		
		if (instance == nullptr)
		{
			// ��+˫���ж�
			lock_guard<std::mutex> guard(mtx);
			if(instance == nullptr)
				instance = new Singleton();
		}

		return instance;
	}

private:
	static Singleton *instance;  // ����һ��Ψһ��ʵ������
	// ���캯��˽�л�
	Singleton()
	{
		
	}
	// ��ֹ��������͸�ֵ
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
};
Singleton* Singleton::instance = nullptr;
#endif

// �ڶ�����������ģʽ������Ҫ������
#if 1
std::mutex mtx;

class Singleton
{
public:
	// ��ȡ���Ψһʵ������Ľӿڷ���
	static Singleton* getInstance()  // �ǲ��ǿ����뺯��(��ûִ�����ܷ��ٴε���)��
	{
		/*
			��̬�ľֲ�����������������׶Σ��ڴ�ʹ��������ݶ���
			���ǳ�ʼ���ǵȵ����е��ô���ʱ�ų�ʼ����
			��̬�ֲ�����ִֻ��һ�γ�ʼ��
			������̬�ֲ������ĳ�ʼ�����ڻ��ָ�����Ѿ��Զ�����̻߳���ָ����
		*/
		
		static Singleton instance; 
		return &instance;
	}

private:
	
	// ���캯��˽�л�
	Singleton()
	{

	}
	// ��ֹ��������͸�ֵ
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