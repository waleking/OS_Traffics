#include "StdAfx.h"
#include "Vehicle.h"

//extern int IDNum;//当前总的线程数量

CVehicle::CVehicle(void)
{
	m_Priority=1;//默认优先级为普通
	m_OwnPosition=0;
	m_speed=1;
	carSprite=new CSprite;
	context=0;
	flag=0;
	index=0;
	m_length=80;//车长80象素默认
	pfnVehicleProcS[0]=&CVehicle::Proc0;
	pfnVehicleProcS[1]=&CVehicle::Proc1;
	pfnVehicleProcS[2]=&CVehicle::Proc2;
	pfnVehicleProcS[3]=&CVehicle::Proc3;
	pfnVehicleProcS[4]=&CVehicle::Proc4;
	pfnVehicleProcS[5]=&CVehicle::Proc5;
	pfnVehicleProcS[6]=&CVehicle::Proc6;
	pfnVehicleProcS[7]=&CVehicle::Proc7;
	pfnVehicleProcS[8]=&CVehicle::Proc8;
}

CVehicle::CVehicle(CVehicle::CarType cartype,int toGenerateID,bool police)
{
	if(!police)
		m_speed=3;//普通车辆
	else
		m_speed=3;
	carSprite=new CSprite;
	context=0;
	flag=0;
	flag2=0;
	index=0;
	if(police)
		m_Priority=2;//最高优先级
	else
		m_Priority=1;
	m_tID=toGenerateID;
	pfnVehicleProcS[0]=&CVehicle::Proc0;
	pfnVehicleProcS[1]=&CVehicle::Proc1;
	pfnVehicleProcS[2]=&CVehicle::Proc2;
	pfnVehicleProcS[3]=&CVehicle::Proc3;
	pfnVehicleProcS[4]=&CVehicle::Proc4;
	pfnVehicleProcS[5]=&CVehicle::Proc5;
	pfnVehicleProcS[6]=&CVehicle::Proc6;
	pfnVehicleProcS[7]=&CVehicle::Proc7;
	pfnVehicleProcS[8]=&CVehicle::Proc8;
	m_carType=cartype;
	switch(m_carType)
	{
	case CVehicle::South2North:
		carSprite->m_point.x=527;
		carSprite->m_point.y=768;
		m_OwnPosition=-carSprite->rect.Height();
		break;
	case CVehicle::North2South:
		carSprite->m_point.x=470;
		carSprite->m_point.y=-80;
		m_OwnPosition=-carSprite->rect.Height();
		break;
	case CVehicle::East2West:
		carSprite->m_point.x=1024;
		carSprite->m_point.y=341;
		m_OwnPosition=-carSprite->rect.Width();
		break;
	case CVehicle::West2East:
		carSprite->m_point.x=-80;
		carSprite->m_point.y=400;
		m_OwnPosition=-carSprite->rect.Width();
		break;
	default:
		break;
	}
}

CVehicle::~CVehicle(void)
{
	delete carSprite;
}

bool CVehicle::Proc0(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int&flag2,char* info)
{
	for(int i=m_OwnPosition+m_length+index;i<m_OwnPosition+m_length+m_speed;i++)
	{
		if(!(*(road_mutex+m_carType)+i)->P(vReady))
		{
			index=i+1;
			return false;
		}
	}
	if(index>=m_OwnPosition+m_speed)
		index=0;
	if(CheckWillCross()&&flag==0)//即将出现在十字路口
	{
		flag=1;
	}
	this->context+=1;
	return true;
};

bool CVehicle::Proc1(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int&flag2,char* info)
{
	if(flag==1)//即将进入临界区
	{
		if((!lightstate)&&(m_Priority==1))//红灯亮并且优先级仅是普通
		{
			if(!(road_light+m_carType)->P(vReady))
			{
				context++;
				flag=2;
				return false;//P(light)
			}
		}
		flag=2;//进入临界区
	}
	context++;
	return true;
};

bool CVehicle::Proc2(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int&flag2,char* info)
{
	if(flag==0)
	{
		this->m_OwnPosition+=this->m_speed;
		//将自身维护的坐标系转化成世界坐标系
		TranslateCoodinate();
	}
	this->context+=1;
	return true;
}

bool CVehicle::Proc3(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int&flag2,char* info)
{
	if(flag==2)//在临界区内
	{
		if(flag2<=0)
		{
			if(!(m_mutex_cross2->P(vReady)))//抢占资源
			{
				context++;
				return false;
			}
		}
		
	}
	context++;
	return true;
};

bool CVehicle::Proc4(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int&flag2,char* info)
{
	if(flag==2)//在临界区内
	{
		if(flag2<=0)
		{
			if(!(m_mutex_cross3->P(vReady)))//抢占资源
			{
				context++;
				return false;
			}
		}
	}
	context++;
	return true;
};

bool CVehicle::Proc5(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int&flag2,char* info)
{
	if(flag==2)
	{
		flag2++;//表示该方向上占用十字路口的车辆加1
		m_mutex_cross2->ReWrite(1);//对4开放
		m_mutex_cross3->ReWrite(1);//对4开放
	}
	context++;
	return true;
}



bool CVehicle::Proc6(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int&flag2,char* info)
{
	if(flag==2)//在临界区内
	{
		for(int i=m_OwnPosition+index;i<m_OwnPosition+m_speed;i++)
		{
			if(!(*(road_mutex+m_carType)+i)->V(vReady))
			{
			}
		}
		if(!vReady.empty())
		{
			CMyProcess* temp=*vReady.begin();
			vReady.erase(vReady.begin());
			vReady.push_back(temp);
		}/**/
		m_mutex_cross1->ReWrite(-1);//占用行进车道，使之前进时不受横向车辆影响
		m_OwnPosition+=m_speed;//前进
		//将自身维护的坐标系转化成世界坐标系
		TranslateCoodinate();
	}
	//context++;
	return true;
}

bool CVehicle::Proc7(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int&flag2,char* info)
{
	if(flag==2)//在临界区内
	{
		if(!CheckCrossRoad())//判断是否出了临界区
		{
			flag2--;//表示该方向占用十字路口的车辆-1
			if(flag2==0)//只有当后面没有车时才会释放车道资源
			{
				if((m_mutex_cross4->mutex)>=0)//邻道为空
				{
					if(m_mutex_cross4->vBlocked.empty()==true)//邻道为空且未被污染过可以唤醒自己的阻塞队列
					{
						m_mutex_cross4->ReWrite(1);
						m_mutex_cross1->V2version(vReady,info);//如果出了临界区，则释放资源,并唤醒全部
						m_mutex_cross2->ReWrite(1);
						m_mutex_cross3->ReWrite(1);
					}
				}
			    else if((m_mutex_cross4->mutex)<0)//邻道不为空，将自己的阻塞队列加到邻道上去
				{
					if(!m_mutex_cross1->vBlocked.empty())
					{
						std::vector<CMyProcess*>:: iterator it=m_mutex_cross1->vBlocked.begin();
						for(;it!=m_mutex_cross1->vBlocked.end();it++)
						{
							CMyProcess* tempProcess=*(it);
							m_mutex_cross4->vBlocked.push_back(tempProcess);
						}
						m_mutex_cross1->vBlocked.clear();
						m_mutex_cross2->ReWrite(1);
						m_mutex_cross3->ReWrite(1);
					}
				}
				m_mutex_cross1->ReWrite(1);//表示该车道已经让出
			}
			//if((m_mutex_cross1->mutex)>1)
			//	m_mutex_cross1->ReWrite(1);//避免多次空的V操作
			//if((m_mutex_cross2->mutex)>1)
			//	m_mutex_cross2->ReWrite(1);//避免多次空的V操作
			//if((m_mutex_cross3->mutex)>1)
			//	m_mutex_cross3->ReWrite(1);//避免多次空的V操作
			context++;
			flag=0;//回到普通道路上
		}
		else//否则保存现场，下次仍然从Proc3执行前进程序
		{
			flag=2;//仍在临界区内
			context=6;//下次执行Proc5
			return false;
		}
	}
	return true;
}



bool CVehicle::Proc8(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int&flag2,char* info)
{
	if(flag==0)
	{
		if(m_OwnPosition>0)
		{
			for(int i=m_OwnPosition+index;i<m_OwnPosition+m_speed;i++)
			{
				if(!(*(road_mutex+m_carType)+i)->V(vReady))
				{
				}
			}
		}
		//如果还没有出界,则放入就绪队列队尾
		if(m_OwnPosition>1100)
		{
			//释放掉所占的全部信号量
			for(int i=m_OwnPosition+m_speed;i<m_OwnPosition+m_length+m_speed;i++)
			{
				if(!(*(road_mutex+m_carType)+i)->V(vReady))
				{
				}
			}
			CMyProcess* temp=*vReady.begin();
			vReady.erase(vReady.begin());//不能仅仅擦除
			delete temp;				 //还要删除原件
		}
		else if(!vReady.empty())
		{
			CMyProcess* temp=*vReady.begin();
			vReady.erase(vReady.begin());
			vReady.push_back(temp);
		}
		this->context=0;
		return true;
	}
	return true;
}

bool CVehicle::Proc(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int& flag2,char* info)
{
	InitializeLength();
	//mutex_cross进行重新映射
	switch(m_carType)
	{
		case CVehicle::West2East:
			{
				m_mutex_cross1=W;//w2e
				m_mutex_cross2=W+3;//n2s
				m_mutex_cross3=W+2;//s2n
				m_mutex_cross4=W+1;//e2w
			}
			break;
		case CVehicle::East2West:
			{
				m_mutex_cross1=W+1;//e2w
				m_mutex_cross2=W+2;//s2n
				m_mutex_cross3=W+3;//n2s
				m_mutex_cross4=W;//w2e
			}
			break;
		case CVehicle::South2North:
			{
				m_mutex_cross1=W+2;//s2n
				m_mutex_cross2=W;//w2e
				m_mutex_cross3=W+1;//e2w
				m_mutex_cross4=W+3;//n2s
			}
			break;
		case CVehicle::North2South:
			{
				m_mutex_cross1=W+3;//n2s
				m_mutex_cross2=W+1;//e2w
				m_mutex_cross3=W;//w2e
				m_mutex_cross4=W+2;//s2n
			}
			break;
		default:
			break;
	}
	//对lightstate进行重新映射
	switch(m_carType)
	{
	case CVehicle::South2North:
		{
			lightstate=!lightstate;
		}
		break;
	case CVehicle::North2South:
		{
			lightstate=!lightstate;
		}
		break;
	case CVehicle::East2West:
		break;
	case CVehicle::West2East:
		break;
	default:
		break;
	}

	for(int i=context;i<9;i++)//函数指针的有序调用
	{
		if(!(this->* pfnVehicleProcS[i])(W,road_mutex,road_light,lightstate,vReady,flag2,info))
			return false;//注意查阅文档，可能产生C2604的错误
	}
	return true;
};
void CVehicle::TranslateCoodinate(void)//将自身维护的坐标系转化成世界坐标系
{
	switch(m_carType)
	{
	case CVehicle::South2North:
		carSprite->m_point.x=527;
		carSprite->m_point.y=768-m_OwnPosition;
		break;
	case CVehicle::North2South:
		carSprite->m_point.x=470;
		carSprite->m_point.y=m_OwnPosition-m_length;
		break;
	case CVehicle::East2West:
		carSprite->m_point.x=1024-m_OwnPosition;
		carSprite->m_point.y=341;
		break;
	case CVehicle::West2East:
		carSprite->m_point.x=m_OwnPosition-m_length;//修改，注意
		carSprite->m_point.y=400;
		break;
	default:
		break;
	}
}

bool CVehicle::CheckCrossRoad(void)//判断是否在十字路口内
{
	switch(m_carType)
	{
	case CVehicle::South2North:
		if(m_OwnPosition>323&&m_OwnPosition<514)//DEBUG
		{
			return true;
		}
		break;
	case CVehicle::North2South:
		if(m_OwnPosition+m_length-m_length>331&&m_OwnPosition-m_length<445)//摸索得到坐标的范围
		{
			return true;
		}
		break;
	case CVehicle::East2West:
		if(m_OwnPosition>450&&m_OwnPosition<658)
		{
			return true;
		}
		break;
	case CVehicle::West2East:
		if(m_OwnPosition+m_length-m_length>455&&m_OwnPosition-m_length<574)
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool CVehicle::CheckWillCross(void)//判断是否将在十字路口内
{
	switch(m_carType)
	{
	case CVehicle::South2North:
		if(m_OwnPosition>323&&m_OwnPosition<=323+m_speed+2)
		{
			return true;
		}
		break;
	case CVehicle::North2South:
		if(m_OwnPosition+m_length-m_length>331&&m_OwnPosition+m_length-m_length<=331+m_speed+2)
		{
			return true;
		}
		break;
	case CVehicle::East2West:
		if(m_OwnPosition>450&&m_OwnPosition<=450+m_speed+2)
		{
			return true;
		}
		break;
	case CVehicle::West2East:
		if(m_OwnPosition+m_length-m_length>455&&m_OwnPosition+m_length-m_length<=455+m_speed+2)
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

void CVehicle::InitializeLength(void)//将自身维护的坐标系转化成世界坐标系
{
	switch(m_carType)
	{
	case CVehicle::South2North:
		m_length=carSprite->rect.Height();
		break;
	case CVehicle::North2South:
		m_length=carSprite->rect.Height();
		break;
	case CVehicle::East2West:
		m_length=carSprite->rect.Width();
		break;
	case CVehicle::West2East:
		m_length=carSprite->rect.Width();
		break;
	default:
		break;
	}
}