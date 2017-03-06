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
#undef THIS_FILE////HW:这是干什么的
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
	srand( (unsigned)time( NULL ) );//以时间为种子产生随机数
	m_car_density=0.03;//发车密度，0.11
	flag_car_density=5;//发车密度标记
	m_police_density=0.1;//警车密度
	flag_police_density=1;//警车密度标记
	wsprintfA(vipINFO,"vipInfo");//初始化重要信息
	m_trafficState=1;//初始时东西向行驶，南北向停止
	m_timer=0;//初始时间是0
	flag_timer=8;//初始时红绿灯是8秒一换
	toGenerateID=0;//用于产生进程号
	for(int i=0;i<4;i++)
	{
		flag2[i]=0;//占用十字路口的四个方向的车辆数量
	}
	mutex_road=new CMySemaphore*[4];//道路上的信号量
	mutex_light=new CMySemaphore[4];//四个方向的交通灯的信号量
	mutex_cross=new CMySemaphore[4];//十字路口信号量,初始值为1
	for(int i=0;i<4;i++)
	{
			mutex_road[i]=new CMySemaphore[1500];
			mutex_light[i].ReWrite(0);//初始时刻交通灯的信号量，互斥量0
	}
	//初始化DirectDraw
	myDD.Init(CWnd::m_hWnd);
	//初始化背景图片
	myDD.InitPlayfield();
	//初始化交通灯，0是红灯，1是绿灯
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
	////用于在DDRAW中显示信息的一块背景黑布，黑色要被滤去，最终仅留下白色文字
	pWhite=new CSprite;
	pWhite->InitLpdds(IDB_BLACK,&myDD);//实际上是黑的
	pWhite->m_point=CPoint(0,0);
	////DEBUG//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	for(int i=0;i<0;i++)//4辆西向东的车
	{
		pVehicle=new CVehicle(CVehicle::West2East,toGenerateID++);
		pVehicle->carSprite->InitLpdds(IDB_WEST2EAST_4,&myDD);
		vReady.push_back(pVehicle);
	}

	for(int i=0;i<0;i++)//东向西
	{
		pVehicle=new CVehicle(CVehicle::East2West,toGenerateID++);
		pVehicle->carSprite->InitLpdds(IDB_EAST2WEST_1,&myDD);
		vReady.push_back(pVehicle);
	}
	for(int i=0;i<0;i++)//北向南
	{
		pVehicle=new CVehicle(CVehicle::North2South,toGenerateID++);
		pVehicle->carSprite->InitLpdds(IDB_NORTH2SOUTH_1,&myDD);
		vReady.push_back(pVehicle);
	}
	for(int i=0;i<0;i++)//南向北
	{
		pVehicle=new CVehicle(CVehicle::South2North,toGenerateID++);
		pVehicle->carSprite->InitLpdds(IDB_SOUTH2NORTH_3,&myDD);
		vReady.push_back(pVehicle);
	}
	//(mutex_cross)->ReWrite(3);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	//一起翻转绘图
	DrawFlipTogether();
	//背景黑布的填充
	myDD.DDraw_Fill_Surface(pWhite->m_lpdds);
	//设置时钟
	SetTimer(1, 30, NULL);
	return 0;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	//红绿灯的PV原语
	if(m_timer%(flag_timer*30)==0)
	{
		if(m_trafficState==false)//南北向行，东西向停，唤醒东西向的被阻塞的进程
		{
			(mutex_light)->V(vReady);//V原语
			(mutex_light+1)->V(vReady);//V原语
			if((mutex_light+2)->mutex>=1)//如果红绿灯信号量没有被车辆消费，需要重置为0
				(mutex_light+2)->ReWrite(0);
			if((mutex_light+3)->mutex>=1)//如果红绿灯信号量没有被车辆消费，需要重置为0
				(mutex_light+3)->ReWrite(0);
		}
		if(m_trafficState==true)//东西向行，南北向停，唤醒南北向的被阻塞的进程
		{
			if((mutex_light)->mutex>=1)//如果红绿灯信号量没有被车辆消费，需要重置为0
				(mutex_light)->ReWrite(0);
			if((mutex_light+1)->mutex>=1)//如果红绿灯信号量没有被车辆消费，需要重置为0
				(mutex_light+1)->ReWrite(0);
			(mutex_light+2)->V(vReady);//V原语
			(mutex_light+3)->V(vReady);//V原语
		}
		m_trafficState=!m_trafficState;//红绿灯信号状态翻转
	}
	OutPutResult(CVehicle::East2West);//实时显示结果
	
	m_timer++;//时钟
	//调度
	if(!vReady.empty())
	{
		int size=(int)vReady.size();
		//
		for(int i=0;i<size;i++)
		{
			CVehicle* tempVehicle=(CVehicle*)(*vReady.begin());
			tempVehicle->Proc(mutex_cross,mutex_road,mutex_light,m_trafficState,vReady,flag2[tempVehicle->m_carType],vipINFO);//？？？有待检验
		}
	}
	//随机产生车辆
	GenerateACarWithPolice(m_car_density,m_police_density);
	//绘图
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

	//删除信号量
	delete []mutex_cross;
	//二维指针数组的指针释放
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
	//阻塞的也都需要绘制
	if(m_trafficState)//东西向行走，南北向停止
	{
		myDD.DrawOnSurface(light_e2w[1]->m_point,light_e2w[1]->m_lpdds,light_e2w[1]->rect);
		myDD.DrawOnSurface(light_w2e[1]->m_point,light_w2e[1]->m_lpdds,light_w2e[1]->rect);
		myDD.DrawOnSurface(light_n2s[0]->m_point,light_n2s[0]->m_lpdds,light_n2s[0]->rect);
		myDD.DrawOnSurface(light_s2n[0]->m_point,light_s2n[0]->m_lpdds,light_s2n[0]->rect);
	}
	else//南北向行走，东西向停止
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
	
	//阻塞队列中的图象也绘制
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
	{//判断是否按下 Esc 键
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
				//减小发车密度
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

bool CMainFrame::RandomNum(float p)//p是概率
{
	int maxnum=10000;
	int minnum=1;
	int random=((rand()%(maxnum-minnum+1)+minnum));
	if(random<=maxnum*p)
		return true;
	else
		return false;
}

float CMainFrame::GetRandom()//p是概率
{
	int maxnum=10000;
	int minnum=1;
	int random=((rand()%(maxnum-minnum+1)+minnum));
	return (float)random/(float)maxnum;
}

bool CMainFrame::GenerateACarWithPolice(float p,float q)//有警车
{
	//生成车辆,以一定的概率
	if(RandomNum(p))//生成车辆
	{
		float pDirection=GetRandom();
		float pSurface=GetRandom();
		float pPolice=GetRandom();
		int carsurface=1;//汽车涂装
		if(pSurface<0.3)//原值
			carsurface=1;
		else if(pSurface>=0.3&&pSurface<0.6)//原值
			carsurface=3;
		else
			carsurface=4;
		if(pPolice<q)//生成警车的概率
		{
			if(pDirection<0.25)//从西向东
			{
				{
					pVehicle=new CVehicle(CVehicle::West2East,toGenerateID++,true);//police
					pVehicle->carSprite->InitLpdds(IDB_WEST2EAST_3,&myDD);
				}
			}
			else if(pDirection>=0.25&&pDirection<0.5)//从东向西
			{
				{
					pVehicle=new CVehicle(CVehicle::East2West,toGenerateID++,true);
					pVehicle->carSprite->InitLpdds(IDB_EAST2WEST_3,&myDD);
				}
				
			}
			else if(pDirection>=0.5&&pDirection<0.75)//从南向北
			{
				pVehicle=new CVehicle(CVehicle::South2North,toGenerateID++,true);
				pVehicle->carSprite->InitLpdds(IDB_SOUTH2NORTH_3,&myDD);
			}
			else//从北向南
			{
				pVehicle=new CVehicle(CVehicle::North2South,toGenerateID++,true);
				pVehicle->carSprite->InitLpdds(IDB_NORTH2SOUTH_3,&myDD);
			}
		}
		else//普通车辆
		{
			if(pDirection<=0.25)//从西向东
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
			else if(pDirection>=0.25&&pDirection<0.5)//从东向西
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
			else if(pDirection>=0.5&&pDirection<0.75)//从南向北
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
			else//从北向南
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
	char info17[128];//显示红绿灯控制
	wsprintfA(title, "十字路口交通状况模拟");
	wsprintfA(info, "Mutex of the light(west to east) is %d",(mutex_light)->mutex);
	wsprintfA(info2,"Mutex of the light(east to west) is %d",(mutex_light+1)->mutex);
	wsprintfA(info3,"Mutex of the light(south to north) is %d",(mutex_light+2)->mutex);
	wsprintfA(info4,"Mutex of the light(north to south) is %d",(mutex_light+3)->mutex);
	wsprintfA(info5,"There were %d cars generated",toGenerateID);
	wsprintfA(info6,"Timer is %d",(int)m_timer/30);
	wsprintfA(info7,"发车密度：%d        键盘左键减1，键盘右键加1",flag_car_density);
	wsprintfA(info8,"警车密度：%d        键盘下键减1，键盘上键加1",flag_police_density);
	wsprintfA(info9, "AUTHOR:WALEKING");
	wsprintfA(info10,"from SSE TONGJI");
	wsprintfA(info11,"ESC键退出");
	wsprintfA(info12,"flag2 are %d %d %d %d",flag2[CVehicle::West2East],flag2[CVehicle::East2West],flag2[CVehicle::South2North],flag2[CVehicle::North2South]);
	wsprintfA(info13,"Mutex of the cross(west to east) is %d",(mutex_cross)->mutex);
	wsprintfA(info14,"Mutex of the cross(east to west ) is %d",(mutex_cross+1)->mutex);
	wsprintfA(info15,"Mutex of the cross(south to north) is %d",(mutex_cross+2)->mutex);
	wsprintfA(info16,"Mutex of the cross(north to south) is %d",(mutex_cross+3)->mutex);
	wsprintfA(info17,"红绿灯切换 %d秒一次   连续按空格键改变切换周期",flag_timer);
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
	//myDD.Draw_Text_GDI(info12,600,50,RGB(255,255,255),pWhite->m_lpdds);//显示flag2的信息
	//myDD.Draw_Text_GDI(info13,600,70,RGB(255,255,255),pWhite->m_lpdds);//显示mutex_cross,w2e
	//myDD.Draw_Text_GDI(info14,600,90,RGB(255,255,255),pWhite->m_lpdds);//显示mutex_cross,e2w
	//myDD.Draw_Text_GDI(info15,600,110,RGB(255,255,255),pWhite->m_lpdds);//显示mutex_cross,s2n
	//myDD.Draw_Text_GDI(info16,600,130,RGB(255,255,255),pWhite->m_lpdds);//显示mutex_cross,n2s
	//myDD.Draw_Text_GDI(vipINFO,600,150,RGB(255,255,255),pWhite->m_lpdds);//显示重要信息
	myDD.Draw_Text_GDI(info17,10,130,RGB(255,255,255),pWhite->m_lpdds);//显示时钟
}