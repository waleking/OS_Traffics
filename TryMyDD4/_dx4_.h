// _dx4_.h : main header file for the _dx4_ application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
//#include "NetTransmit.h"

// C_dx4_App:
// See _dx4_.cpp for the implementation of this class
//

class C_dx4_App : public CWinApp
{
public:
	C_dx4_App();
public:
	//CNetTransmit m_net;

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	//CNetTransmit m_netB;
};

extern C_dx4_App theApp;