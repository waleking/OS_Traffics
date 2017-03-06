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
	std::vector<CMyProcess*> vReady;//��������
	//std::vector<CVehicle*> vBlocked;//��������,���ź�����ά��
public:
	bool DrawFlipTogether();
public:
	bool m_trafficState;//���ú��̵Ƶı���
public:
	CSprite* pMenu;//�Ի���ѯ��
public:
	CVehicle* pVehicle;//�²�����һ����
public:
	//CMySemaphore* mutex_cross;//ʮ��·�ڵ��ź���
	int flag2[4];//ռ��ʮ��·�ڵ��ĸ�����ĳ�����������ʼֵΪ0
	CMySemaphore* mutex_cross;//ʮ��·���ĸ�������ź���,��ʼֵΪ1
								//mutex_cross[0]w2e,mutex_cross[1]e2w,mutex_cross[2]s2n,mutex_cross[3]n2s
	CMySemaphore** mutex_road;//��6M�Ŀռ�ȥ��ȡЧ��
	CMySemaphore* mutex_light;//�ĸ�����ĺ��̵Ƶ��ź���,mutex_light[0]w2e,mutex_light[1]e2w,mutex_light[2]s2n,mutex_light[3]n2s
public:
	CSprite* pSprite;//С����
public:
	int iCircle;//һ���������û���������ִ�д���
public:
	CSprite* pWhite;//������ʾ������Ϣ��һ��ײ�
public:
	char info[128];//������ʾ��Ϣ�Ļ�����
public:
	int toGenerateID;
	int m_timer;//��ʱ����ÿOnTimerһ�μ�1��ÿ240�Σ�Լ8�룩���̵�ת��һ��
public:
	bool fOver;//��Ϸ��ֹ���
public:
	void Restart();
public:
	CSprite* light_s2n[2];//�����򱱵Ľ�ͨ��
	CSprite* light_n2s[2];//�ӱ����ϵĽ�ͨ��
	CSprite* light_e2w[2];//�Ӷ������Ľ�ͨ��
	CSprite* light_w2e[2];//�����򶫵Ľ�ͨ��
	char vipINFO[128];//���������Ҫ��Ϣ
	float m_car_density;//�����ܶ�
	int flag_car_density;//�����ܶȱ��
	float m_police_density;//�����ܶ�
	int flag_police_density;//�����ܶȱ��
	int flag_timer;//ʱ�ӿ���

public:
	bool CMainFrame::RandomNum(float p);//p�Ǹ���
	bool CMainFrame::GenerateACarWithSurface(float p);//����Ϳװ
	bool CMainFrame::GenerateACarWithPolice(float p,float q);//�������������p���г����������ʣ�q������������
	float CMainFrame::GetRandom();
	void OutPutResult(CVehicle::CarType direction);
};


