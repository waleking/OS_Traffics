// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "_dx4_.h"

#include "MainFrm.h"
#include "MyDD.h"
//#include "NetTransmit.h"
//#include "DataPackage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE////HW:���Ǹ�ʲô��
static char THIS_FILE[] = __FILE__;
#endif

extern C_dx4_App theApp;




// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};



// CMainFrame construction/destruction

CMainFrame::CMainFrame()
:pWhite(NULL),pVehicle(NULL),vReady(NULL)
{
	Create(NULL,TEXT("right"),WS_POPUP);
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}



// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	srand( (unsigned)time( NULL ) );//��ʱ��Ϊ���Ӳ��������
	m_car_density=0.03;//�����ܶȣ�0.11
	flag_car_density=5;//�����ܶȱ��
	m_police_density=0.1;//�����ܶ�
	flag_police_density=1;//�����ܶȱ��
	wsprintfA(vipINFO,"vipInfo");//��ʼ����Ҫ��Ϣ
	m_trafficState=1;//��ʼʱ��������ʻ���ϱ���ֹͣ
	m_timer=0;//��ʼʱ����0
	flag_timer=8;//��ʼʱ���̵���8��һ��
	toGenerateID=0;//���ڲ������̺�
	for(int i=0;i<4;i++)
	{
		flag2[i]=0;//ռ��ʮ��·�ڵ��ĸ�����ĳ�������
	}
	mutex_road=new CMySemaphore*[4];//��·�ϵ��ź���
	mutex_light=new CMySemaphore[4];//�ĸ�����Ľ�ͨ�Ƶ��ź���
	mutex_cross=new CMySemaphore[4];//ʮ��·���ź���,��ʼֵΪ1
	for(int i=0;i<4;i++)
	{
			mutex_road[i]=new CMySemaphore[1500];
			mutex_light[i].ReWrite(0);//��ʼʱ�̽�ͨ�Ƶ��ź�����������0
	}
	//��ʼ��DirectDraw
	myDD.Init(CWnd::m_hWnd);
	//��ʼ������ͼƬ
	myDD.InitPlayfield();
	//��ʼ����ͨ�ƣ�0�Ǻ�ƣ�1���̵�
	{
		light_s2n[0]=new CSprite;
		light_s2n[0]->InitLpdds(IDB_STOP_SN,&myDD);
		light_s2n[0]->m_point=CPoint(511,444);
		light_s2n[1]=new CSprite;
		light_s2n[1]->InitLpdds(IDB_GO_SN,&myDD);
		light_s2n[1]->m_point=CPoint(511,444);

		light_n2s[0]=new CSprite;
		light_n2s[0]->InitLpdds(IDB_STOP_SN,&myDD);
		light_n2s[0]->m_point=CPoint(456,320);
		light_n2s[1]=new CSprite;
		light_n2s[1]->InitLpdds(IDB_GO_SN,&myDD);
		light_n2s[1]->m_point=CPoint(456,320);

		light_e2w[0]=new CSprite;
		light_e2w[0]->InitLpdds(IDB_STOP_EW,&myDD);
		light_e2w[0]->m_point=CPoint(570,328);
		light_e2w[1]=new CSprite;
		light_e2w[1]->InitLpdds(IDB_GO_EW,&myDD);
		light_e2w[1]->m_point=CPoint(570,328);

		light_w2e[0]=new CSprite;
		light_w2e[0]->InitLpdds(IDB_STOP_EW,&myDD);
		light_w2e[0]->m_point=CPoint(448,386);
		light_w2e[1]=new CSprite;
		light_w2e[1]->InitLpdds(IDB_GO_EW,&myDD);
		light_w2e[1]->m_point=CPoint(448,386);
	}
	////������DDRAW����ʾ��Ϣ��һ�鱳���ڲ�����ɫҪ����ȥ�����ս����°�ɫ����
	pWhite=new CSprite;
	pWhite->InitLpdds(IDB_BLACK,&myDD);//ʵ�����Ǻڵ�
	pWhite->m_point=CPoint(0,0);
	////DEBUG//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	for(int i=0;i<0;i++)//4�����򶫵ĳ�
	{
		pVehicle=new CVehicle(CVehicle::West2East,toGenerateID++);
		pVehicle->carSprite->InitLpdds(IDB_WEST2EAST_4,&myDD);
		vReady.push_back(pVehicle);
	}

	for(int i=0;i<0;i++)//������
	{
		pVehicle=new CVehicle(CVehicle::East2West,toGenerateID++);
		pVehicle->carSprite->InitLpdds(IDB_EAST2WEST_1,&myDD);
		vReady.push_back(pVehicle);
	}
	for(int i=0;i<0;i++)//������
	{
		pVehicle=new CVehicle(CVehicle::North2South,toGenerateID++);
		pVehicle->carSprite->InitLpdds(IDB_NORTH2SOUTH_1,&myDD);
		vReady.push_back(pVehicle);
	}
	for(int i=0;i<0;i++)//����
	{
		pVehicle=new CVehicle(CVehicle::South2North,toGenerateID++);
		pVehicle->carSprite->InitLpdds(IDB_SOUTH2NORTH_3,&myDD);
		vReady.push_back(pVehicle);
	}
	//(mutex_cross)->ReWrite(3);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	//һ��ת��ͼ
	DrawFlipTogether();
	//�����ڲ������
	myDD.DDraw_Fill_Surface(pWhite->m_lpdds);
	//����ʱ��
	SetTimer(1, 30, NULL);
	return 0;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	//���̵Ƶ�PVԭ��
	if(m_timer%(flag_timer*30)==0)
	{
		if(m_trafficState==false)//�ϱ����У�������ͣ�����Ѷ�����ı������Ľ���
		{
			(mutex_light)->V(vReady);//Vԭ��
			(mutex_light+1)->V(vReady);//Vԭ��
			if((mutex_light+2)->mutex>=1)//������̵��ź���û�б��������ѣ���Ҫ����Ϊ0
				(mutex_light+2)->ReWrite(0);
			if((mutex_light+3)->mutex>=1)//������̵��ź���û�б��������ѣ���Ҫ����Ϊ0
				(mutex_light+3)->ReWrite(0);
		}
		if(m_trafficState==true)//�������У��ϱ���ͣ�������ϱ���ı������Ľ���
		{
			if((mutex_light)->mutex>=1)//������̵��ź���û�б��������ѣ���Ҫ����Ϊ0
				(mutex_light)->ReWrite(0);
			if((mutex_light+1)->mutex>=1)//������̵��ź���û�б��������ѣ���Ҫ����Ϊ0
				(mutex_light+1)->ReWrite(0);
			(mutex_light+2)->V(vReady);//Vԭ��
			(mutex_light+3)->V(vReady);//Vԭ��
		}
		m_trafficState=!m_trafficState;//���̵��ź�״̬��ת
	}
	OutPutResult(CVehicle::East2West);//ʵʱ��ʾ���
	
	m_timer++;//ʱ��
	//����
	if(!vReady.empty())
	{
		int size=(int)vReady.size();
		//
		for(int i=0;i<size;i++)
		{
			CVehicle* tempVehicle=(CVehicle*)(*vReady.begin());
			tempVehicle->Proc(mutex_cross,mutex_road,mutex_light,m_trafficState,vReady,flag2[tempVehicle->m_carType],vipINFO);//�������д�����
		}
	}
	//�����������
	GenerateACarWithPolice(m_car_density,m_police_density);
	//��ͼ
	DrawFlipTogether();
	myDD.DDraw_Fill_Surface(pWhite->m_lpdds);
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	if(!vReady.empty())
	{
		std::vector<CMyProcess*>::iterator it=this->vReady.begin();
		for(;it!=this->vReady.end();it++)
		{
			CMyProcess* pVehicleTemp=(*it);
			delete pVehicleTemp;
		}
	}
	else
		delete pVehicle;

	//ɾ���ź���
	delete []mutex_cross;
	//��άָ�������ָ���ͷ�
	for(int   i=0;   i<4;   i++)   
		delete  []mutex_road[i];
	delete   []mutex_road;
	delete light_s2n[0];
	delete light_s2n[1];
	delete light_n2s[0];
	delete light_n2s[1];
	delete light_e2w[0];
	delete light_e2w[1];
	delete light_w2e[0];
	delete light_w2e[1];
	delete pWhite;
	delete []mutex_light;
	CFrameWnd::OnClose();
}

bool CMainFrame::DrawFlipTogether()
{

	myDD.DrawPlayField();
	myDD.DrawOnSurface(pWhite->m_point,pWhite->m_lpdds,pWhite->rect);
	//������Ҳ����Ҫ����
	if(m_trafficState)//���������ߣ��ϱ���ֹͣ
	{
		myDD.DrawOnSurface(light_e2w[1]->m_point,light_e2w[1]->m_lpdds,light_e2w[1]->rect);
		myDD.DrawOnSurface(light_w2e[1]->m_point,light_w2e[1]->m_lpdds,light_w2e[1]->rect);
		myDD.DrawOnSurface(light_n2s[0]->m_point,light_n2s[0]->m_lpdds,light_n2s[0]->rect);
		myDD.DrawOnSurface(light_s2n[0]->m_point,light_s2n[0]->m_lpdds,light_s2n[0]->rect);
	}
	else//�ϱ������ߣ�������ֹͣ
	{
		myDD.DrawOnSurface(light_e2w[0]->m_point,light_e2w[0]->m_lpdds,light_e2w[0]->rect);
		myDD.DrawOnSurface(light_w2e[0]->m_point,light_w2e[0]->m_lpdds,light_w2e[0]->rect);
		myDD.DrawOnSurface(light_n2s[1]->m_point,light_n2s[1]->m_lpdds,light_n2s[1]->rect);
		myDD.DrawOnSurface(light_s2n[1]->m_point,light_s2n[1]->m_lpdds,light_s2n[1]->rect);
	}
	//////////////////////////////////////////////////////////////////////////
	//DEBUG
	for(int i=0;i<4;i++)
	{
		if(!((mutex_cross+i)->vBlocked.empty()))
		{
			std::vector<CMyProcess*>::iterator it=(mutex_cross+i)->vBlocked.begin();
			for(;it<(mutex_cross+i)->vBlocked.end();it++)
			{
				CVehicle* tempVehicle=(CVehicle*)(*it);
				myDD.DrawOnSurface((tempVehicle)->carSprite->m_point,(tempVehicle)->carSprite->m_lpdds,(tempVehicle)->carSprite->rect);
			}
		}
	}
	for(int i=0;i<4;i++)
	{
		if(!((mutex_light+i)->vBlocked.empty()))
		{
			std::vector<CMyProcess*>::iterator it=(mutex_light+i)->vBlocked.begin();
			for(;it<(mutex_light+i)->vBlocked.end();it++)
			{
				CVehicle* tempVehicle=(CVehicle*)(*it);
				myDD.DrawOnSurface((tempVehicle)->carSprite->m_point,(tempVehicle)->carSprite->m_lpdds,(tempVehicle)->carSprite->rect);
			}
		}
	}
	
	//���������е�ͼ��Ҳ����
	for(int i=0;i<4;i++)
		for(int j=0;j<1500;j++)
		{
			if(mutex_road[i][j].mutex<0)
			{
				std::vector<CMyProcess*>::iterator it=mutex_road[i][j].vBlocked.begin();
				for(;it<mutex_road[i][j].vBlocked.end();it++)
				{
					CVehicle* tempVehicle=(CVehicle*)(*it);
					myDD.DrawOnSurface((tempVehicle)->carSprite->m_point,(tempVehicle)->carSprite->m_lpdds,(tempVehicle)->carSprite->rect);
				}
			}
		}

	if(!vReady.empty())
	{
		std::vector<CMyProcess*>::iterator it=this->vReady.begin();
		for(;it!=this->vReady.end();it++)
		{
			CVehicle* tempVehicle=(CVehicle*)(*it);
			myDD.DrawOnSurface((tempVehicle)->carSprite->m_point,(tempVehicle)->carSprite->m_lpdds,(tempVehicle)->carSprite->rect);
		}
	}
	
	
	myDD.DrawFlip();
	return true;
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	if (nChar == VK_ESCAPE )
	{//�ж��Ƿ��� Esc ��
		PostMessage(WM_CLOSE );
		KillTimer(1);
		//delete []info;
	}
	else
	{
		// TODO: Add your message handler code here and/or call default
		switch (nChar) 
		{
		case VK_LEFT:
				//��С�����ܶ�
				{
					if(flag_car_density>0)
					{
						flag_car_density--;
						m_car_density=m_car_density-0.006;
					}
				}
				break;
		case VK_RIGHT:
				{
					if(flag_car_density<20)
					{
						flag_car_density++;
						m_car_density=m_car_density+0.006;
					}
					
				}
				break;
		case VK_UP:
			{
				if(flag_police_density<10)
				{
					flag_police_density++;
					m_police_density=m_police_density+0.1;
				}
			}
			break;
		case VK_DOWN:
			{
				if(flag_police_density>0)
				{
					flag_police_density--;
					m_police_density=m_police_density-0.1;
				}
			}
			break;
		case VK_SPACE:
			{
				/*static bool fTimekiller=false;
				if(fTimekiller==false)
				{
					KillTimer(1);
					fTimekiller=true;
				}
				else
				{
					SetTimer(1,100,NULL);
					fTimekiller=false;
				}*/
				if(flag_timer<12)
				{
					flag_timer++;
				}
				else if(flag_timer>=12)
					flag_timer=1;
			}
			break;
		}
	}
	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

bool CMainFrame::RandomNum(float p)//p�Ǹ���
{
	int maxnum=10000;
	int minnum=1;
	int random=((rand()%(maxnum-minnum+1)+minnum));
	if(random<=maxnum*p)
		return true;
	else
		return false;
}

float CMainFrame::GetRandom()//p�Ǹ���
{
	int maxnum=10000;
	int minnum=1;
	int random=((rand()%(maxnum-minnum+1)+minnum));
	return (float)random/(float)maxnum;
}

bool CMainFrame::GenerateACarWithPolice(float p,float q)//�о���
{
	//���ɳ���,��һ���ĸ���
	if(RandomNum(p))//���ɳ���
	{
		float pDirection=GetRandom();
		float pSurface=GetRandom();
		float pPolice=GetRandom();
		int carsurface=1;//����Ϳװ
		if(pSurface<0.3)//ԭֵ
			carsurface=1;
		else if(pSurface>=0.3&&pSurface<0.6)//ԭֵ
			carsurface=3;
		else
			carsurface=4;
		if(pPolice<q)//���ɾ����ĸ���
		{
			if(pDirection<0.25)//������
			{
				{
					pVehicle=new CVehicle(CVehicle::West2East,toGenerateID++,true);//police
					pVehicle->carSprite->InitLpdds(IDB_WEST2EAST_3,&myDD);
				}
			}
			else if(pDirection>=0.25&&pDirection<0.5)//�Ӷ�����
			{
				{
					pVehicle=new CVehicle(CVehicle::East2West,toGenerateID++,true);
					pVehicle->carSprite->InitLpdds(IDB_EAST2WEST_3,&myDD);
				}
				
			}
			else if(pDirection>=0.5&&pDirection<0.75)//������
			{
				pVehicle=new CVehicle(CVehicle::South2North,toGenerateID++,true);
				pVehicle->carSprite->InitLpdds(IDB_SOUTH2NORTH_3,&myDD);
			}
			else//�ӱ�����
			{
				pVehicle=new CVehicle(CVehicle::North2South,toGenerateID++,true);
				pVehicle->carSprite->InitLpdds(IDB_NORTH2SOUTH_3,&myDD);
			}
		}
		else//��ͨ����
		{
			if(pDirection<=0.25)//������
			{
				{
					{
						pVehicle=new CVehicle(CVehicle::West2East,toGenerateID++);
						switch(carsurface)
						{
						case 1:
							pVehicle->carSprite->InitLpdds(IDB_WEST2EAST_1,&myDD);
							break;
						case 3:
							pVehicle->carSprite->InitLpdds(IDB_WEST2EAST_2,&myDD);
							break;
						case 4:
							pVehicle->carSprite->InitLpdds(IDB_WEST2EAST_4,&myDD);
							break;
						default:
							break;
						}
					}
					
				}
			}
			else if(pDirection>=0.25&&pDirection<0.5)//�Ӷ�����
			{
				pVehicle=new CVehicle(CVehicle::East2West,toGenerateID++);
				switch(carsurface)
				{
				case 1:
					pVehicle->carSprite->InitLpdds(IDB_EAST2WEST_1,&myDD);
					break;
				case 3:
					pVehicle->carSprite->InitLpdds(IDB_EAST2WEST_2,&myDD);
					break;
				case 4:
					pVehicle->carSprite->InitLpdds(IDB_EAST2WEST_4,&myDD);
					break;
				default:
					break;
				}
			}
			else if(pDirection>=0.5&&pDirection<0.75)//������
			{
				pVehicle=new CVehicle(CVehicle::South2North,toGenerateID++);
				switch(carsurface)
				{
				case 1:
					pVehicle->carSprite->InitLpdds(IDB_SOUTH2NORTH_1,&myDD);
					break;
				case 3:
					pVehicle->carSprite->InitLpdds(IDB_SOUTH2NORTH_2,&myDD);
					break;
				case 4:
					pVehicle->carSprite->InitLpdds(IDB_SOUTH2NORTH_4,&myDD);
					break;
				default:
					break;
				}
			}
			else//�ӱ�����
			{
				pVehicle=new CVehicle(CVehicle::North2South,toGenerateID++);
				switch(carsurface)
				{
				case 1:
					pVehicle->carSprite->InitLpdds(IDB_NORTH2SOUTH_1,&myDD);
					break;
				case 3:
					pVehicle->carSprite->InitLpdds(IDB_NORTH2SOUTH_2,&myDD);
					break;
				case 4:
					pVehicle->carSprite->InitLpdds(IDB_NORTH2SOUTH_4,&myDD);
					break;
				default:
					break;
				}
			}
		}
		vReady.push_back(pVehicle);
	}
	return true;
}

void CMainFrame::OutPutResult(CVehicle::CarType direction)
{
	char title[128];
	char info2[128];
	char info3[128];
	char info4[128];
	char info5[128];
	char info6[128];
	char info7[128];
	char info8[128];
	char info9[128];
	char info10[128];
	char info11[128];
	char info12[128];
	char info13[128];
	char info14[128];
	char info15[128];
	char info16[128];
	char info17[128];//��ʾ���̵ƿ���
	wsprintfA(title, "ʮ��·�ڽ�ͨ״��ģ��");
	wsprintfA(info, "Mutex of the light(west to east) is %d",(mutex_light)->mutex);
	wsprintfA(info2,"Mutex of the light(east to west) is %d",(mutex_light+1)->mutex);
	wsprintfA(info3,"Mutex of the light(south to north) is %d",(mutex_light+2)->mutex);
	wsprintfA(info4,"Mutex of the light(north to south) is %d",(mutex_light+3)->mutex);
	wsprintfA(info5,"There were %d cars generated",toGenerateID);
	wsprintfA(info6,"Timer is %d",(int)m_timer/30);
	wsprintfA(info7,"�����ܶȣ�%d        ���������1�������Ҽ���1",flag_car_density);
	wsprintfA(info8,"�����ܶȣ�%d        �����¼���1�������ϼ���1",flag_police_density);
	wsprintfA(info9, "AUTHOR:WALEKING");
	wsprintfA(info10,"from SSE TONGJI");
	wsprintfA(info11,"ESC���˳�");
	wsprintfA(info12,"flag2 are %d %d %d %d",flag2[CVehicle::West2East],flag2[CVehicle::East2West],flag2[CVehicle::South2North],flag2[CVehicle::North2South]);
	wsprintfA(info13,"Mutex of the cross(west to east) is %d",(mutex_cross)->mutex);
	wsprintfA(info14,"Mutex of the cross(east to west ) is %d",(mutex_cross+1)->mutex);
	wsprintfA(info15,"Mutex of the cross(south to north) is %d",(mutex_cross+2)->mutex);
	wsprintfA(info16,"Mutex of the cross(north to south) is %d",(mutex_cross+3)->mutex);
	wsprintfA(info17,"���̵��л� %d��һ��   �������ո���ı��л�����",flag_timer);
	myDD.Draw_Text_GDI(title,10,30,RGB(255,255,255),pWhite->m_lpdds);
	myDD.Draw_Text_GDI(info, 10,50,RGB(255,255,255),pWhite->m_lpdds);
	myDD.Draw_Text_GDI(info2,10,70,RGB(255,255,255),pWhite->m_lpdds);
	myDD.Draw_Text_GDI(info3,10,90,RGB(255,255,255),pWhite->m_lpdds);
	myDD.Draw_Text_GDI(info4,10,110,RGB(255,255,255),pWhite->m_lpdds);
	//myDD.Draw_Text_GDI(info5,10,130,RGB(255,255,255),pWhite->m_lpdds);
	myDD.Draw_Text_GDI(info6,10,150,RGB(255,255,255),pWhite->m_lpdds);
	myDD.Draw_Text_GDI(info7,10,170,RGB(255,255,255),pWhite->m_lpdds);
	myDD.Draw_Text_GDI(info8,10,190,RGB(255,255,255),pWhite->m_lpdds);
	myDD.Draw_Text_GDI(info9,10,230,RGB(255,255,255),pWhite->m_lpdds);
	myDD.Draw_Text_GDI(info10,10,250,RGB(255,255,255),pWhite->m_lpdds);
	myDD.Draw_Text_GDI(info11,180,230,RGB(255,255,255),pWhite->m_lpdds);
	//myDD.Draw_Text_GDI(info12,600,50,RGB(255,255,255),pWhite->m_lpdds);//��ʾflag2����Ϣ
	//myDD.Draw_Text_GDI(info13,600,70,RGB(255,255,255),pWhite->m_lpdds);//��ʾmutex_cross,w2e
	//myDD.Draw_Text_GDI(info14,600,90,RGB(255,255,255),pWhite->m_lpdds);//��ʾmutex_cross,e2w
	//myDD.Draw_Text_GDI(info15,600,110,RGB(255,255,255),pWhite->m_lpdds);//��ʾmutex_cross,s2n
	//myDD.Draw_Text_GDI(info16,600,130,RGB(255,255,255),pWhite->m_lpdds);//��ʾmutex_cross,n2s
	//myDD.Draw_Text_GDI(vipINFO,600,150,RGB(255,255,255),pWhite->m_lpdds);//��ʾ��Ҫ��Ϣ
	myDD.Draw_Text_GDI(info17,10,130,RGB(255,255,255),pWhite->m_lpdds);//��ʾʱ��
}