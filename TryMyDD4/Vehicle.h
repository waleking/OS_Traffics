#pragma once
#include "myprocess.h"
#include "MySemaphore.h"
#include "Sprite.h"


class CVehicle :
	public CMyProcess
{
//代码区
public:
	enum CarType{West2East,East2West,South2North,North2South};//车辆种类，从西到东，从东到西，从南到北，从北到南
	typedef bool(CVehicle::*pfnVehicleProc)(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int& flag2,char* info);
public:
	CVehicle(void);
	CVehicle(CVehicle::CarType cartype,int toGenerateID,bool police=0);//默认非警车
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
	bool Proc(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int& flag2,char* info);//有序调用Proc0至Proc7
	void TranslateCoodinate(void);//将自身维护的一维坐标系映射回原坐标系
	bool CVehicle::CheckCrossRoad(void);//判断是否在十字路口内
	bool CVehicle::CheckWillCross(void);//判断是否将在十字路口内
	void InitializeLength(void);//初始化车长
private:
	pfnVehicleProc pfnVehicleProcS[9];//函数指针的索引表
//数据区
public:
	CSprite* carSprite;//指向车辆的DDRAW对象
	CarType m_carType;//车辆种类
	int m_length;//车身长度
	int m_OwnPosition;//维护自身的坐标系
	int m_speed;//速度
	int m_Priority;//优先级，默认为1，最高级为2，可以抢红灯
	CMySemaphore* m_mutex_cross1;//反映过十字路口时所需道路信号量的指针1，等待被重新映射
	CMySemaphore* m_mutex_cross2;//反映过十字路口时所需道路信号量的指针2，等待被重新映射
	CMySemaphore* m_mutex_cross3;//反映过十字路口时所需道路信号量的指针3，等待被重新映射
	CMySemaphore* m_mutex_cross4;//反映过十字路口时所需道路信号量的指针4，等待被重新映射
};

