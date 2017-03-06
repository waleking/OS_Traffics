// TryMyDD4.h : main header file for the TryMyDD4 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTryMyDD4App:
// See TryMyDD4.cpp for the implementation of this class
//

class CTryMyDD4App : public CWinApp
{
public:
	CTryMyDD4App();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTryMyDD4App theApp;