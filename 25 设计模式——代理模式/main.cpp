#include<iostream>

using namespace std;

/* �ṹ��ģʽ
* ��Щ���ģʽ��ע��Ͷ������ϣ��̳еĸ������
  ��ϽӿڵĶ�����϶������¹��ܵķ�ʽ��
	1. ����ģʽ
	2. ������ģʽ
	3. װ����ģʽ
*/

/* ����ģʽ
�ͻ�     ����Proxy     �ϰ�
��ĳһ�������ṩһ���������ɴ��������ƶ�ԭ��������á�
��Ϊ���������ṩһ�ִ����Կ����������ķ��ʣ�
*/

class VideoSite  // ������
{
public:
	virtual void freeMovie() = 0;  // ��ѵ�Ӱ
	virtual void vipMovie() = 0;  // vip��Ӱ
	virtual void ticketMovie() = 0;  // ��ȯ�ۿ���Ӱ
};

class FixBugVideoSite: public VideoSite // ί����
{
public:
	virtual void freeMovie()
	{
		cout << "�ۿ���ѵ�Ӱ" << endl;
	}
	virtual void vipMovie()
	{
		cout << "�ۿ�vip��Ӱ" << endl;
	}
	virtual void ticketMovie()
	{
		cout << "��ȯ�ۿ���Ӱ" << endl;
	}
};

class FreeVideoSiteProxy : public VideoSite  // ������
{
	/*
		�ô���������������ǿͻ���������ί�ж������Ȩ�޵�����
	*/
public:
	FreeVideoSiteProxy()
	{
		pVideo = new FixBugVideoSite();
	}
	virtual void freeMovie()
	{
		pVideo->freeMovie();  // ͨ����������freeMovie����ί��������freeMovie
	}
	virtual void vipMovie()
	{
		cout << "��Ŀǰֻ����ͨ�οͣ���Ҫ������VIP" << endl;
	}
	virtual void ticketMovie()
	{
		cout << "��Ŀǰû��ȯ����Ҫ�����Ӱȯ" << endl;
	}
private:
	VideoSite* pVideo;
};

int main()
{
	VideoSite* p1 = new FixBugVideoSite();
	// ��θ������(�Ƿ��ǻ�Ա������ȯ),���Ʋ�ͬ������ò�ͬ����
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