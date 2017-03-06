#include "StdAfx.h"
#include "MySemaphore.h"

CMySemaphore::CMySemaphore(void)
{
	mutex=1;//����Ҫ���ź�����ʼֵΪ1
}

CMySemaphore::CMySemaphore(int input)
{
	mutex=input;//����Ҫ���ź�����ʼֵΪ1
}

CMySemaphore::~CMySemaphore(void)
{
	if(!vBlocked.empty())
	{
		std::vector<CMyProcess*>::iterator it=vBlocked.begin();
		for(;it!=vBlocked.end();it++)
		{
			CMyProcess* pVehicleTemp=(*it);
			delete pVehicleTemp;
		}
		vBlocked.clear();
	}
}

bool CMySemaphore::P(std::vector<CMyProcess*>& vReady)//Pԭ��
{
	mutex=mutex-1;
	if(mutex>=0)
		return true;//����ִ��
	else
	{
		vBlocked.push_back(*vReady.begin());
		vReady.erase(vReady.begin());
		return false;//��������
	}
}

bool CMySemaphore::V(std::vector<CMyProcess*>& vReady)//Vԭ��
{
	mutex+=1;
	OutPutMutexValue();
	if(mutex>0)
		return true;//����ִ��
	else
	{
		if(!vBlocked.empty())
		{
			vReady.push_back(*vBlocked.begin());
			vBlocked.erase(vBlocked.begin());
		}
		return false;//�����������л��ѵ�һ���̣߳�Ȼ�����ִ��
	}
}

bool CMySemaphore::V2version(std::vector<CMyProcess*>& vReady,char* info)//Vԭ��,���Ű汾����ȫ��
{
	mutex+=1;
	OutPutMutexValue();
	if(mutex>0)
		return true;//����ִ��
	else
	{
		if(!vBlocked.empty())
		{
			wsprintfA(info,"Size of this blocked:%d",vBlocked.size());
			int size=vBlocked.size();
			std::vector<CMyProcess*>::iterator it=vBlocked.begin();
			for(;it!=vBlocked.end();it++)
			{
				CMyProcess* tempProcess=*(it);
				wsprintfA(info,"blocked");
				vReady.push_back(tempProcess);
			}
			vBlocked.clear();
		}
		return false;//�����������л��ѵ�һ���̣߳�Ȼ�����ִ��
	}
}

void CMySemaphore::OutPutMutexValue()
{
	//cout<<"mutex="<<mutex<<"\n";
}

void CMySemaphore::ReWrite(int Reset)
{
	mutex=Reset;
}
