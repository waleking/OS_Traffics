#pragma once
#include "MyProcess.h"

class CMySemaphore
{
public:
	CMySemaphore(void);
	CMySemaphore(int input);
public:
	~CMySemaphore(void);
	bool P(std::vector<CMyProcess*>& vReady);//Pԭ��
	bool V(std::vector<CMyProcess*>& vReady);//Vԭ��
	bool V2version(std::vector<CMyProcess*>& vReady,char* info);//Vԭ��,���Ű汾����ȫ��
	void OutPutMutexValue();
	void ReWrite(int Reset);//�����ź���

	int mutex;
	std::vector<CMyProcess*> vBlocked;
};