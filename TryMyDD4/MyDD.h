#pragma once
#include <dshow.h>
#include <ddraw.h>
#include <dsound.h>
#include <dinput.h>
#include <stdlib.h>
#include <time.h>
#include "afxwin.h"
#include "atltypes.h"

#pragma comment(lib,"ddraw.lib") 
#pragma comment(lib,"Winmm.lib") 
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput.lib")
#pragma comment(lib,"dinput8.lib")

// this builds a 16 bit color value in 5.6.5 format (green dominate mode)
#define _RGB16BIT565(r,g,b) ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11))

#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

class CMyDD
{
public:
int DDraw_Draw_Surface(LPDIRECTDRAWSURFACE7 source, // source surface to draw
                  int x, int y,                 // position to draw at
                  int width, int height,        // size of source surface
                  LPDIRECTDRAWSURFACE7 dest,    // surface to draw the surface on
                  int transparent = 1 );          // transparency flag
LPDIRECTDRAWSURFACE7 DDraw_Create_Surface_nowhite(int width, 
														 int height, 
														 int mem_flags, 
														 int color_key=0 );
int Draw_Text_GDI(char *text, int x,int y,COLORREF color, LPDIRECTDRAWSURFACE7 lpdds);
public:
	CMyDD(void);
public:
	~CMyDD(void);
public:
	CBitmap m_bitmap;
	CBitmap m_bitmap_wumi;
public:
	LPDIRECTDRAW7 lpdd;
public:
	LPDIRECTDRAWSURFACE7 lpddsprimary;
public:
	LPDIRECTDRAWSURFACE7 lpddsback;
public:
	DDSURFACEDESC2 ddsd;
public:
	DDSCAPS2 ddscaps;
public:
	HRESULT ddrval;
public:
	LPDIRECTDRAWSURFACE7 lpdds_playfield;
public:
	LPDIRECTDRAWSURFACE7 lpdds_wumi;
public:
	HDC dc;
public:
	HDC dc1;
public:
	bool Init(HWND hwnd );
public:
	bool InitPlayfield(void);
public:
	bool DrawPlayField(void);
public:
	bool DrawOnSurface(CPoint point,LPDIRECTDRAWSURFACE7 lpddstemp,CRect rect);
public:
	bool DrawFlip(void);
public:
	int m_ScreenWidth;
public:
	int m_ScreenHeight;
public:
	DDCOLORKEY key;
public:
	void colorkey(LPDIRECTDRAWSURFACE7& lpdds,bool white);
public:
	CPoint m_point;
public:
	void AddPointItself(void);
public:
	CRect InitSurface(CBitmap& bitmap,int IDBITMAP,LPDIRECTDRAWSURFACE7& lpddstemp,bool white);
	int DDraw_Fill_Surface(LPDIRECTDRAWSURFACE7 lpdds);//向表面中填充背景色
};
