#pragma once
#include <vector>

class CMyProcess
{
public:
	CMyProcess(void);
public:
	~CMyProcess(void);
	//virtual void Proc(){};

	int m_tID;//�߳�ID
protected:
	int context;//�߳������ģ�����ָ����ָ�������±�
	int flag;//flag=0,��ʾ����ͨ��·�ϣ�flag=1,��ʾ���������ٽ�����flag=2��ʾ�����ٽ���
	int flag2;//flag2=0,�������
	int index;//�߳������ģ�������ʱ����
};