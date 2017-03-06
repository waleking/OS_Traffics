#pragma once
#include <vector>

class CMyProcess
{
public:
	CMyProcess(void);
public:
	~CMyProcess(void);
	//virtual void Proc(){};

	int m_tID;//线程ID
protected:
	int context;//线程上下文，具体指函数指针数组下标
	int flag;//flag=0,表示在普通道路上；flag=1,表示即将进入临界区；flag=2表示进入临界区
	int flag2;//flag2=0,辅助标记
	int index;//线程上下文，被阻塞时有用
};