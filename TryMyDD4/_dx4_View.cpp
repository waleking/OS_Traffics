// _dx4_View.cpp : implementation of the C_dx4_View class
//

#include "stdafx.h"
#include "_dx4_.h"

#include "_dx4_Doc.h"
#include "_dx4_View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C_dx4_View

IMPLEMENT_DYNCREATE(C_dx4_View, CView)

BEGIN_MESSAGE_MAP(C_dx4_View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// C_dx4_View construction/destruction

C_dx4_View::C_dx4_View()
{
	// TODO: add construction code here

}

C_dx4_View::~C_dx4_View()
{
}

BOOL C_dx4_View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// C_dx4_View drawing

void C_dx4_View::OnDraw(CDC* /*pDC*/)
{
	C_dx4_Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// C_dx4_View printing

BOOL C_dx4_View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void C_dx4_View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void C_dx4_View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// C_dx4_View diagnostics

#ifdef _DEBUG
void C_dx4_View::AssertValid() const
{
	CView::AssertValid();
}

void C_dx4_View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

C_dx4_Doc* C_dx4_View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(C_dx4_Doc)));
	return (C_dx4_Doc*)m_pDocument;
}
#endif //_DEBUG


// C_dx4_View message handlers
