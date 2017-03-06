// TryMyDD4Doc.cpp : implementation of the CTryMyDD4Doc class
//

#include "stdafx.h"
#include "TryMyDD4.h"

#include "TryMyDD4Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTryMyDD4Doc

IMPLEMENT_DYNCREATE(CTryMyDD4Doc, CDocument)

BEGIN_MESSAGE_MAP(CTryMyDD4Doc, CDocument)
END_MESSAGE_MAP()


// CTryMyDD4Doc construction/destruction

CTryMyDD4Doc::CTryMyDD4Doc()
{
	// TODO: add one-time construction code here

}

CTryMyDD4Doc::~CTryMyDD4Doc()
{
}

BOOL CTryMyDD4Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CTryMyDD4Doc serialization

void CTryMyDD4Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CTryMyDD4Doc diagnostics

#ifdef _DEBUG
void CTryMyDD4Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTryMyDD4Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTryMyDD4Doc commands
