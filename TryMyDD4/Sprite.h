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
	CPoint m_point;//��ǰλ��
public:
	LPDIRECTDRAWSURFACE7 m_lpdds;//��Ӧ����������
public:
	virtual int InitLpdds(int IDBITMAP,CMyDD* pMyDD);//��������ĳ�ʼ��
public:
	CRect rect;//���Ƶľ�������
public:
	CBitmap m_bmp;//Ҫװ�ص�ͼƬ
};
