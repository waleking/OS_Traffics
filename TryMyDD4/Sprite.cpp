#include "StdAfx.h"
#include "Sprite.h"
//#include "resource.h"

CSprite::CSprite(void)
: m_point(0)
{
}
CSprite::~CSprite(void)
{
}




int CSprite::InitLpdds(int IDBITMAP,CMyDD* pMyDD)//flag==true,¹ıÂË°×É«
{
	rect=pMyDD->InitSurface(m_bmp,IDBITMAP,this->m_lpdds,false);
	return 0;
}
