#pragma once
#include "myprocess.h"
#include "MySemaphore.h"
#include "Sprite.h"


class CVehicle :
	public CMyProcess
{
//������
public:
	enum CarType{West2East,East2West,South2North,North2South};//�������࣬�����������Ӷ����������ϵ������ӱ�����
	typedef bool(CVehicle::*pfnVehicleProc)(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int& flag2,char* info);
public:
	CVehicle(void);
	CVehicle(CVehicle::CarType cartype,int toGenerateID,bool police=0);//Ĭ�ϷǾ���
public:
	~CVehicle(void);
	bool Proc0(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int& flag2,char* info);
	bool Proc1(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int& flag2,char* info);
	bool Proc2(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int& flag2,char* info);
	bool Proc3(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int& flag2,char* info);
	bool Proc4(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int& flag2,char* info);
	bool Proc5(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int& flag2,char* info);
	bool Proc6(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int& flag2,char* info);
	bool Proc7(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int& flag2,char* info);
	bool Proc8(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int& flag2,char* info);
	bool Proc(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int& flag2,char* info);//�������Proc0��Proc7
	void TranslateCoodinate(void);//������ά����һά����ϵӳ���ԭ����ϵ
	bool CVehicle::CheckCrossRoad(void);//�ж��Ƿ���ʮ��·����
	bool CVehicle::CheckWillCross(void);//�ж��Ƿ���ʮ��·����
	void InitializeLength(void);//��ʼ������
private:
	pfnVehicleProc pfnVehicleProcS[9];//����ָ���������
//������
public:
	CSprite* carSprite;//ָ������DDRAW����
	CarType m_carType;//��������
	int m_length;//������
	int m_OwnPosition;//ά�����������ϵ
	int m_speed;//�ٶ�
	int m_Priority;//���ȼ���Ĭ��Ϊ1����߼�Ϊ2�����������
	CMySemaphore* m_mutex_cross1;//��ӳ��ʮ��·��ʱ�����·�ź�����ָ��1���ȴ�������ӳ��
	CMySemaphore* m_mutex_cross2;//��ӳ��ʮ��·��ʱ�����·�ź�����ָ��2���ȴ�������ӳ��
	CMySemaphore* m_mutex_cross3;//��ӳ��ʮ��·��ʱ�����·�ź�����ָ��3���ȴ�������ӳ��
	CMySemaphore* m_mutex_cross4;//��ӳ��ʮ��·��ʱ�����·�ź�����ָ��4���ȴ�������ӳ��
};

