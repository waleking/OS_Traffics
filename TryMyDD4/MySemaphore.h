#pragma once
#include "MyProcess.h"

class CMySemaphore
{
public:
	CMySemaphore(void);
	CMySemaphore(int input);
public:
	~CMySemaphore(void);
	bool P(std::vector<CMyProcess*>& vReady);//P原语
	bool V(std::vector<CMyProcess*>& vReady);//V原语
	bool V2version(std::vector<CMyProcess*>& vReady,char* info);//V原语,二号版本唤醒全部
	void OutPutMutexValue();
	void ReWrite(int Reset);//重置信号量

	int mutex;
	std::vector<CMyProcess*> vBlocked;
};