#include "StdAfx.h"
#include "MyDD.h"
#include "resource.h"

CMyDD::CMyDD(void)
: m_ScreenWidth(0)
, m_ScreenHeight(0)
, m_point(300,300)
{
}

CMyDD::~CMyDD(void)
{
}

bool CMyDD::Init(HWND hwnd)
{
	m_ScreenWidth=1024;
	m_ScreenHeight=768;
	////设置DIRECTDRAW
	if (FAILED(DirectDrawCreateEx(NULL, (void **)&lpdd, IID_IDirectDraw7, NULL)))
	{
		::OutputDebugString(TEXT("failed to create directdraw!"));
		return(0);
	}

	if (FAILED(lpdd->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN|DDSCL_ALLOWREBOOT)))
	{
		::OutputDebugString(TEXT("failed to set cooperative level!"));
	   return(0);
	}

	if (FAILED(lpdd->SetDisplayMode(m_ScreenWidth, m_ScreenHeight, 16,0,DDSDM_STANDARDVGAMODE)))
	{
		::OutputDebugString(TEXT("failed to set display mode!"));
		return(0);
	}

	//::ZeroMemory(&ddsd,sizeof(ddsd));
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.dwBackBufferCount = 1;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;

	if (FAILED(lpdd->CreateSurface(&ddsd, &lpddsprimary, NULL)))
	{
		::OutputDebugString(TEXT("failed to create surface!"));
		return(0);
	}

	//::ZeroMemory(&ddsd,sizeof(ddsd));
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;

	if (FAILED(lpddsprimary->GetAttachedSurface(&ddsd.ddsCaps, &lpddsback)))
	{
		::OutputDebugString(TEXT("failed to attache surface"));
		return(0);
	}
	LPDIRECTDRAWCLIPPER pMyClipper; 
	//在窗口下. 为了防止 DirectDraw 画到窗口外面去. 需要加一个画框(裁剪板). 可以用 CreateClipper来 创建剪贴板. 
	if(FAILED(lpdd->CreateClipper(0, &pMyClipper, NULL)))
	{
		::OutputDebugStr(TEXT("cannot create the clipper"));
		return(0);
	};

	//创建后,把它套到窗口上去, 所以要知道是那一个窗口( Handle).

	//if(FAILED(pMyClipper->SetHWnd(0, AfxGetMainWnd()->GetSafeHwnd())))
	if(FAILED(pMyClipper->SetHWnd(0, hwnd)))
    {
		::OutputDebugStr(TEXT("cannot get the window"));
		return(0);
	};

	// 把剪贴板加到窗口上去
	if(FAILED(lpddsprimary->SetClipper(pMyClipper)))
	{
		::OutputDebugStr(TEXT("cannot set clipper"));
		return(0);
	};
	return true;
}

bool CMyDD::InitPlayfield(void)
{
	dc1=NULL;
	dc=NULL;
	dc=::CreateCompatibleDC(NULL);
	dc1=::CreateCompatibleDC(NULL);
	m_bitmap.LoadBitmap(IDB_BITMAP_PLAYFIELD);
	::SelectObject(dc,m_bitmap);
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	ddsd.dwWidth=m_ScreenWidth;
	ddsd.dwHeight=m_ScreenHeight;
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH; 
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	/*ddrval=lpdd->CreateSurface(&ddsd,&lpdds_playfield,NULL);
	if(ddrval!=DD_OK)
	{
		::AfxMessageBox(L"failed to create the playfield surface");
		return(0);
	}*/
	if(FAILED(lpdd->CreateSurface(&ddsd,&lpdds_playfield,NULL)))
	{
		::OutputDebugString(TEXT("failed to create the playfield surface"));
		return(0);
	}
	if(FAILED(lpdds_playfield->GetDC(&dc1)))
	{
		::OutputDebugString(TEXT("failed to get dc"));
		return(0);
	};
	::BitBlt(dc1,0,0,m_ScreenWidth,m_ScreenHeight,dc,0,0,SRCCOPY);
	lpdds_playfield->ReleaseDC(dc1);
	return true;
}

bool CMyDD::DrawPlayField()
{
	if(FAILED(lpddsback->BltFast( 0 , 0 ,lpdds_playfield, CRect(0,0,m_ScreenWidth,m_ScreenHeight) , DDBLTFAST_WAIT)))
	{
		::OutputDebugString(TEXT("failed to draw playfield\n"));
		return(0);
	};
	return true;
}

bool CMyDD::DrawOnSurface(CPoint point,LPDIRECTDRAWSURFACE7 lpddstemp,CRect rect)
{
	if(rect.bottom-rect.top+point.y>m_ScreenHeight)//超出下边界的处理
	{
		rect.bottom=rect.top-point.y+m_ScreenHeight;
	}
	if(rect.right-rect.left+point.x>m_ScreenWidth)//超出右边界的处理
	{
		rect.right=rect.left-point.x+m_ScreenWidth;
	}
	if(point.x<=0)//超出左边界的处理
	{
		rect.left=rect.left-point.x;
		rect.right=rect.right;
		point.x=0;
	}

	if(point.y<=0)//超出上边界的处理
	{
		rect.top=-point.y;
		point.y=0;
	}
	if((rect.bottom-rect.top)>0&&(rect.right-rect.left)>0)
	{
		if(FAILED(lpddsback->BltFast( point.x , point.y ,lpddstemp, rect,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY)))
		{
			::OutputDebugString(TEXT("failed to draw on temp surface\n"));
			return(0);
		}/**/
	}
	return true;
}



bool CMyDD::DrawFlip(void)
{
	/*if(FAILED(lpddsback->BltFast( 0 , 0 ,lpdds_playfield, CRect(0,0,m_ScreenWidth,m_ScreenHeight) , DDBLTFAST_WAIT)))
	{
		::AfxMessageBox(TEXT("failed to draw on surface3"));
		return(0);
	};*/
	if(FAILED(lpddsprimary->Flip(NULL, DDFLIP_WAIT)))
	{
		::OutputDebugString(TEXT("failed to flip\n"));
		return(0);
	};
	return true;
}

CRect CMyDD::InitSurface(CBitmap& bitmap,int IDBITMAP,LPDIRECTDRAWSURFACE7& lpddstemp,bool white)//过滤白色则white=1
{
	BITMAP bm;
	dc1=NULL;
	dc=NULL;
	dc=::CreateCompatibleDC(NULL);
	dc1=::CreateCompatibleDC(NULL);
	if(bitmap.LoadBitmap(IDBITMAP)==NULL)
	{
		::OutputDebugString(TEXT("nothing"));
		return 0;
	};
	::SelectObject(dc,bitmap);
	bitmap.GetBitmap(&bm);

	CRect rect(0,0,bm.bmWidth,bm.bmHeight);

	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	ddsd.dwWidth=bm.bmWidth;
	ddsd.dwHeight=bm.bmHeight;
	
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	if(FAILED(lpdd->CreateSurface(&ddsd,&lpddstemp,NULL)))
	{
		::OutputDebugString(TEXT("failed to create the Sprite surface"));
		return(0);
	}
	if(FAILED(lpddstemp->GetDC(&dc1)))
	{
		::OutputDebugString(TEXT("failed to get sprite dc"));
		return(0);
	};
	::BitBlt(dc1,0,0,bm.bmWidth,bm.bmHeight,dc,0,0,SRCCOPY);
	lpddstemp->ReleaseDC(dc1);/**/
	colorkey(lpddstemp,white);
	return rect;
}


void CMyDD::colorkey(LPDIRECTDRAWSURFACE7& lpdds,bool white)
{
	if(white==true)
	{
		key.dwColorSpaceHighValue = RGB(255,255,255);//过滤白色
		key.dwColorSpaceLowValue = RGB(255,255,255);
	}
	else if(white==false)
	{
		key.dwColorSpaceHighValue = RGB(0,0,0);//过滤黑色
		key.dwColorSpaceLowValue = RGB(0,0,0);
	}
	lpdds->SetColorKey(DDCKEY_SRCBLT,&key);
}

int CMyDD::DDraw_Draw_Surface(LPDIRECTDRAWSURFACE7 source, // source surface to draw
                      int x, int y,                 // position to draw at
                      int width, int height,        // size of source surface
                      LPDIRECTDRAWSURFACE7 dest,    // surface to draw the surface on
                      int transparent)          // transparency flag
{
// draw the surface at the x,y defined by dest, note that we are sending
// the size of the surface, we could query for it, but that takes time
// basically, we are really lacking datastructure as this point, since
// you would create a datastructure that keep important info about the
// surface, so you did't have to query it from directdraw

RECT dest_rect,   // the destination rectangle
     source_rect; // the source rectangle                             

// fill in the destination rect
dest_rect.left   = x;
dest_rect.top    = y;
dest_rect.right  = x+width-1;
dest_rect.bottom = y+height-1;

// fill in the source rect
source_rect.left    = 0;
source_rect.top     = 0;
source_rect.right   = width-1;
source_rect.bottom  = height-1;

// test transparency flag

if (transparent)
   {
   // enable color key blit
   // blt to destination surface
   if (FAILED(dest->Blt(&dest_rect, source,
                     &source_rect,(DDBLT_WAIT | DDBLT_KEYSRC),
                     NULL)))
           return(0);

   } // end if
else
   {
   // perform blit without color key
   // blt to destination surface
   if (FAILED(dest->Blt(&dest_rect, source,
                     &source_rect,(DDBLT_WAIT),
                     NULL)))
           return(0);

   } // end if

// return success
return(1);

} // end DDraw_Draw_Surface

LPDIRECTDRAWSURFACE7 CMyDD::DDraw_Create_Surface_nowhite(int width, int height, int mem_flags, int color_key )
{
// this function creates an offscreen plain surface

DDSURFACEDESC2 ddsd;         // working description
LPDIRECTDRAWSURFACE7 lpdds;  // temporary surface
    
// set to access caps, width, and height
memset(&ddsd,0,sizeof(ddsd));
ddsd.dwSize  = sizeof(ddsd);
ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

// set dimensions of the new bitmap surface
ddsd.dwWidth  =  width;
ddsd.dwHeight =  height;

// set surface to offscreen plain
ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;

// create the surface
if (FAILED(lpdd->CreateSurface(&ddsd,&lpdds,NULL)))
   return(NULL);

// test if user wants a color key
if (color_key >= 0)
   {
   	DDCOLORKEY color_key;
   color_key.dwColorSpaceLowValue  = _RGB16BIT565(255,255,255);
   color_key.dwColorSpaceHighValue = _RGB16BIT565(255,255,255);
   lpdds->SetColorKey(DDCKEY_SRCBLT, &color_key);
   } // end if

// return surface
return(lpdds);
} // end DDraw_Create_Surface



void CMyDD::AddPointItself(void)
{
	m_point.x+=1;
	m_point.y+=1;
}


int CMyDD::Draw_Text_GDI(char *text, int x,int y,COLORREF color, LPDIRECTDRAWSURFACE7 lpdds)
{
	// this function draws the sent text on the sent surface 
	// using color index as the color in the palette

	HDC xdc; // the working dc

	// get the dc from surface
	if (FAILED(lpdds->GetDC(&xdc)))
		return(0);

	// set the colors for the text up
	SetTextColor(xdc,color);

	// set background mode to transparent so black isn't copied
	SetBkMode(xdc, TRANSPARENT);

	// draw the text a
	///////////////////////////////////////////////////////////////////////////////////////////
	TextOutA(xdc,x,y,text,strlen(text));

	// release the dc
	lpdds->ReleaseDC(xdc);

	// return success
	return(1);
} // end Draw_Text_GDI

int CMyDD::DDraw_Fill_Surface(LPDIRECTDRAWSURFACE7 lpdds)
{
	DDBLTFX ddbltfx; // this contains the DDBLTFX structure

	// clear out the structure and set the size field 
	DDRAW_INIT_STRUCT(ddbltfx);

	// set the dwfillcolor field to the desired color
	ddbltfx.dwFillColor = RGB(0,0,0); 

	// ready to blt to surface
	lpdds->Blt(NULL,       // ptr to dest rectangle
		NULL,       // ptr to source surface, NA            
		NULL,       // ptr to source rectangle, NA
		DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
		&ddbltfx);  // ptr to DDBLTFX structure

	// return success
	return(1);
} // end DDraw_Fill_Surface
