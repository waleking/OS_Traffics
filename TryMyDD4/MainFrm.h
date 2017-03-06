// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "mydd.h"
#include "Sprite.h"
#include "Vehicle.h"
#include <String>
#include <sstream>
#include <vector>

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	
	DECLARE_DYNCREATE(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	CMyDD myDD;
public:
	afx_msg void OnClose();
public:
	std::vector<CMyProcess*> vReady;//就绪队列
	//std::vector<CVehicle*> vBlocked;//阻塞队列,由信号量来维护
public:
	bool DrawFlipTogether();
public:
	bool m_trafficState;//设置红绿灯的变量
public:
	CSprite* pMenu;//对话框询问
public:
	CVehicle* pVehicle;//新产生的一辆车
public:
	//CMySemaphore* mutex_cross;//十字路口的信号量
	int flag2[4];//占用十字路口的四个方向的车辆数量，初始值为0
	CMySemaphore* mutex_cross;//十字路口四个方向的信号量,初始值为1
								//mutex_cross[0]w2e,mutex_cross[1]e2w,mutex_cross[2]s2n,mutex_cross[3]n2s
	CMySemaphore** mutex_road;//用6M的空间去换取效率
	CMySemaphore* mutex_light;//四个方向的红绿灯的信号量,mutex_light[0]w2e,mutex_light[1]e2w,mutex_light[2]s2n,mutex_light[3]n2s
public:
	CSprite* pSprite;//小精灵
public:
	int iCircle;//一个周期中用户进程最大的执行次数
public:
	CSprite* pWhite;//用于显示各种信息的一块白布
public:
	char info[128];//用于显示信息的缓冲区
public:
	int toGenerateID;
	int m_timer;//计时器，每OnTimer一次加1，每240次（约8秒）红绿灯转换一次
public:
	bool fOver;//游戏中止标记
public:
	void Restart();
public:
	CSprite* light_s2n[2];//从南向北的交通灯
	CSprite* light_n2s[2];//从北向南的交通灯
	CSprite* light_e2w[2];//从东向西的交通灯
	CSprite* light_w2e[2];//从西向东的交通灯
	char vipINFO[128];//用于输出重要信息
	float m_car_density;//发车密度
	int flag_car_density;//发车密度标记
	float m_police_density;//警车密度
	int flag_police_density;//警车密度标记
	int flag_timer;//时钟控制

public:
	bool CMainFrame::RandomNum(float p);//p是概率
	bool CMainFrame::GenerateACarWithSurface(float p);//带有涂装
	bool CMainFrame::GenerateACarWithPolice(float p,float q);//随机产生车辆，p所有车辆产生概率，q警车产生概率
	float CMainFrame::GetRandom();
	void OutPutResult(CVehicle::CarType direction);
};


