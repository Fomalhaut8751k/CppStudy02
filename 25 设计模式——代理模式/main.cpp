#include<iostream>

using namespace std;

/* 结构型模式
* 这些设计模式关注类和对象的组合，继承的概念被用来
  组合接口的定义组合对象获得新功能的方式。
	1. 代理模式
	2. 适配器模式
	3. 装饰器模式
*/

/* 代理模式
客户     助理Proxy     老板
给某一个对象提供一个代理，并由代理对象控制对原对象的引用。
（为其他对象提供一种代理以控制这个对象的访问）
*/

class VideoSite  // 抽象类
{
public:
	virtual void freeMovie() = 0;  // 免费电影
	virtual void vipMovie() = 0;  // vip电影
	virtual void ticketMovie() = 0;  // 用券观看电影
};

class FixBugVideoSite: public VideoSite // 委托类
{
public:
	virtual void freeMovie()
	{
		cout << "观看免费电影" << endl;
	}
	virtual void vipMovie()
	{
		cout << "观看vip电影" << endl;
	}
	virtual void ticketMovie()
	{
		cout << "用券观看电影" << endl;
	}
};

class FreeVideoSiteProxy : public VideoSite  // 代理类
{
	/*
		让代理对象来控制我们客户对于我们委托对象访问权限的问题
	*/
public:
	FreeVideoSiteProxy()
	{
		pVideo = new FixBugVideoSite();
	}
	virtual void freeMovie()
	{
		pVideo->freeMovie();  // 通过代理对象的freeMovie访问委托类对象的freeMovie
	}
	virtual void vipMovie()
	{
		cout << "您目前只是普通游客，需要升级成VIP" << endl;
	}
	virtual void ticketMovie()
	{
		cout << "您目前没有券，需要购买电影券" << endl;
	}
private:
	VideoSite* pVideo;
};

int main()
{
	VideoSite* p1 = new FixBugVideoSite();
	// 如何根据身份(是否是会员或者有券),限制不同对象调用不同函数
	p1->freeMovie();
	p1->vipMovie();
	p1->ticketMovie();

	cout << "\n";

	VideoSite* p2 = new FreeVideoSiteProxy();
	p2->freeMovie();
	p2->ticketMovie();
	p2->vipMovie();

	return 0;
}