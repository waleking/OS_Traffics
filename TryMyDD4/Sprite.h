#pragma once

#include "MyDD.h"
#include "atltypes.h"
#include "afxwin.h"

class CSprite
{
public:
	CSprite(void);
	CSprite(int down);
public:
	~CSprite(void);
public:
	CPoint m_point;//当前位置
public:
	LPDIRECTDRAWSURFACE7 m_lpdds;//对应的离屏表面
public:
	virtual int InitLpdds(int IDBITMAP,CMyDD* pMyDD);//离屏表面的初始化
public:
	CRect rect;//绘制的矩形区域
public:
	CBitmap m_bmp;//要装载的图片
};
