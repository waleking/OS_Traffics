#include "StdAfx.h"
#include "MySemaphore.h"

CMySemaphore::CMySemaphore(void)
{
	mutex=1;//很重要，信号量初始值为1
}

CMySemaphore::CMySemaphore(int input)
{
	mutex=input;//很重要，信号量初始值为1
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

bool CMySemaphore::P(std::vector<CMyProcess*>& vReady)//P原语
{
	mutex=mutex-1;
	if(mutex>=0)
		return true;//继续执行
	else
	{
		vBlocked.push_back(*vReady.begin());
		vReady.erase(vReady.begin());
		return false;//阻塞发生
	}
}

bool CMySemaphore::V(std::vector<CMyProcess*>& vReady)//V原语
{
	mutex+=1;
	OutPutMutexValue();
	if(mutex>0)
		return true;//继续执行
	else
	{
		if(!vBlocked.empty())
		{
			vReady.push_back(*vBlocked.begin());
			vBlocked.erase(vBlocked.begin());
		}
		return false;//从阻塞队列中唤醒第一个线程，然后继续执行
	}
}

bool CMySemaphore::V2version(std::vector<CMyProcess*>& vReady,char* info)//V原语,二号版本唤醒全部
{
	mutex+=1;
	OutPutMutexValue();
	if(mutex>0)
		return true;//继续执行
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
		return false;//从阻塞队列中唤醒第一个线程，然后继续执行
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
