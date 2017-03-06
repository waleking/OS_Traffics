#include "StdAfx.h"
#include "Vehicle.h"

//extern int IDNum;//��ǰ�ܵ��߳�����

CVehicle::CVehicle(void)
{
	m_Priority=1;//Ĭ�����ȼ�Ϊ��ͨ
	m_OwnPosition=0;
	m_speed=1;
	carSprite=new CSprite;
	context=0;
	flag=0;
	index=0;
	m_length=80;//����80����Ĭ��
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
		m_speed=3;//��ͨ����
	else
		m_speed=3;
	carSprite=new CSprite;
	context=0;
	flag=0;
	flag2=0;
	index=0;
	if(police)
		m_Priority=2;//������ȼ�
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
	if(CheckWillCross()&&flag==0)//����������ʮ��·��
	{
		flag=1;
	}
	this->context+=1;
	return true;
};

bool CVehicle::Proc1(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int&flag2,char* info)
{
	if(flag==1)//���������ٽ���
	{
		if((!lightstate)&&(m_Priority==1))//������������ȼ�������ͨ
		{
			if(!(road_light+m_carType)->P(vReady))
			{
				context++;
				flag=2;
				return false;//P(light)
			}
		}
		flag=2;//�����ٽ���
	}
	context++;
	return true;
};

bool CVehicle::Proc2(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int&flag2,char* info)
{
	if(flag==0)
	{
		this->m_OwnPosition+=this->m_speed;
		//������ά��������ϵת������������ϵ
		TranslateCoodinate();
	}
	this->context+=1;
	return true;
}

bool CVehicle::Proc3(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int&flag2,char* info)
{
	if(flag==2)//���ٽ�����
	{
		if(flag2<=0)
		{
			if(!(m_mutex_cross2->P(vReady)))//��ռ��Դ
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
	if(flag==2)//���ٽ�����
	{
		if(flag2<=0)
		{
			if(!(m_mutex_cross3->P(vReady)))//��ռ��Դ
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
		flag2++;//��ʾ�÷�����ռ��ʮ��·�ڵĳ�����1
		m_mutex_cross2->ReWrite(1);//��4����
		m_mutex_cross3->ReWrite(1);//��4����
	}
	context++;
	return true;
}



bool CVehicle::Proc6(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int&flag2,char* info)
{
	if(flag==2)//���ٽ�����
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
		m_mutex_cross1->ReWrite(-1);//ռ���н�������ʹ֮ǰ��ʱ���ܺ�����Ӱ��
		m_OwnPosition+=m_speed;//ǰ��
		//������ά��������ϵת������������ϵ
		TranslateCoodinate();
	}
	//context++;
	return true;
}

bool CVehicle::Proc7(CMySemaphore* W,CMySemaphore** road_mutex,CMySemaphore* road_light,bool lightstate,std::vector<CMyProcess*>& vReady,int&flag2,char* info)
{
	if(flag==2)//���ٽ�����
	{
		if(!CheckCrossRoad())//�ж��Ƿ�����ٽ���
		{
			flag2--;//��ʾ�÷���ռ��ʮ��·�ڵĳ���-1
			if(flag2==0)//ֻ�е�����û�г�ʱ�Ż��ͷų�����Դ
			{
				if((m_mutex_cross4->mutex)>=0)//�ڵ�Ϊ��
				{
					if(m_mutex_cross4->vBlocked.empty()==true)//�ڵ�Ϊ����δ����Ⱦ�����Ի����Լ�����������
					{
						m_mutex_cross4->ReWrite(1);
						m_mutex_cross1->V2version(vReady,info);//��������ٽ��������ͷ���Դ,������ȫ��
						m_mutex_cross2->ReWrite(1);
						m_mutex_cross3->ReWrite(1);
					}
				}
			    else if((m_mutex_cross4->mutex)<0)//�ڵ���Ϊ�գ����Լ����������мӵ��ڵ���ȥ
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
				m_mutex_cross1->ReWrite(1);//��ʾ�ó����Ѿ��ó�
			}
			//if((m_mutex_cross1->mutex)>1)
			//	m_mutex_cross1->ReWrite(1);//�����οյ�V����
			//if((m_mutex_cross2->mutex)>1)
			//	m_mutex_cross2->ReWrite(1);//�����οյ�V����
			//if((m_mutex_cross3->mutex)>1)
			//	m_mutex_cross3->ReWrite(1);//�����οյ�V����
			context++;
			flag=0;//�ص���ͨ��·��
		}
		else//���򱣴��ֳ����´���Ȼ��Proc3ִ��ǰ������
		{
			flag=2;//�����ٽ�����
			context=6;//�´�ִ��Proc5
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
		//�����û�г���,�����������ж�β
		if(m_OwnPosition>1100)
		{
			//�ͷŵ���ռ��ȫ���ź���
			for(int i=m_OwnPosition+m_speed;i<m_OwnPosition+m_length+m_speed;i++)
			{
				if(!(*(road_mutex+m_carType)+i)->V(vReady))
				{
				}
			}
			CMyProcess* temp=*vReady.begin();
			vReady.erase(vReady.begin());//���ܽ�������
			delete temp;				 //��Ҫɾ��ԭ��
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
	//mutex_cross��������ӳ��
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
	//��lightstate��������ӳ��
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

	for(int i=context;i<9;i++)//����ָ����������
	{
		if(!(this->* pfnVehicleProcS[i])(W,road_mutex,road_light,lightstate,vReady,flag2,info))
			return false;//ע������ĵ������ܲ���C2604�Ĵ���
	}
	return true;
};
void CVehicle::TranslateCoodinate(void)//������ά��������ϵת������������ϵ
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
		carSprite->m_point.x=m_OwnPosition-m_length;//�޸ģ�ע��
		carSprite->m_point.y=400;
		break;
	default:
		break;
	}
}

bool CVehicle::CheckCrossRoad(void)//�ж��Ƿ���ʮ��·����
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
		if(m_OwnPosition+m_length-m_length>331&&m_OwnPosition-m_length<445)//�����õ�����ķ�Χ
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

bool CVehicle::CheckWillCross(void)//�ж��Ƿ���ʮ��·����
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

void CVehicle::InitializeLength(void)//������ά��������ϵת������������ϵ
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