// _dx4_Doc.cpp : implementation of the C_dx4_Doc class
//

#include "stdafx.h"
#include "_dx4_.h"

#include "_dx4_Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C_dx4_Doc

IMPLEMENT_DYNCREATE(C_dx4_Doc, CDocument)

BEGIN_MESSAGE_MAP(C_dx4_Doc, CDocument)
END_MESSAGE_MAP()


// C_dx4_Doc construction/destruction

C_dx4_Doc::C_dx4_Doc()
{
	// TODO: add one-time construction code here

}

C_dx4_Doc::~C_dx4_Doc()
{
}

BOOL C_dx4_Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// C_dx4_Doc serialization

void C_dx4_Doc::Serialize(CArchive& ar)
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


// C_dx4_Doc diagnostics

#ifdef _DEBUG
void C_dx4_Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void C_dx4_Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// C_dx4_Doc commands
