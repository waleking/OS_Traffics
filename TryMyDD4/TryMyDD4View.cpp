// TryMyDD4View.cpp : implementation of the CTryMyDD4View class
//

#include "stdafx.h"
#include "TryMyDD4.h"

#include "TryMyDD4Doc.h"
#include "TryMyDD4View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTryMyDD4View

IMPLEMENT_DYNCREATE(CTryMyDD4View, CView)

BEGIN_MESSAGE_MAP(CTryMyDD4View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTryMyDD4View construction/destruction

CTryMyDD4View::CTryMyDD4View()
{
	// TODO: add construction code here

}

CTryMyDD4View::~CTryMyDD4View()
{
}

BOOL CTryMyDD4View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTryMyDD4View drawing

void CTryMyDD4View::OnDraw(CDC* /*pDC*/)
{
	CTryMyDD4Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CTryMyDD4View printing

BOOL CTryMyDD4View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTryMyDD4View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTryMyDD4View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTryMyDD4View diagnostics

#ifdef _DEBUG
void CTryMyDD4View::AssertValid() const
{
	CView::AssertValid();
}

void CTryMyDD4View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTryMyDD4Doc* CTryMyDD4View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTryMyDD4Doc)));
	return (CTryMyDD4Doc*)m_pDocument;
}
#endif //_DEBUG


// CTryMyDD4View message handlers
