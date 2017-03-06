// _dx4_View.h : interface of the C_dx4_View class
//


#pragma once


class C_dx4_View : public CView
{
protected: // create from serialization only
	C_dx4_View();
	DECLARE_DYNCREATE(C_dx4_View)

// Attributes
public:
	C_dx4_Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~C_dx4_View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in _dx4_View.cpp
inline C_dx4_Doc* C_dx4_View::GetDocument() const
   { return reinterpret_cast<C_dx4_Doc*>(m_pDocument); }
#endif

