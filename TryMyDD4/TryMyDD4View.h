// TryMyDD4View.h : interface of the CTryMyDD4View class
//


#pragma once


class CTryMyDD4View : public CView
{
protected: // create from serialization only
	CTryMyDD4View();
	DECLARE_DYNCREATE(CTryMyDD4View)

// Attributes
public:
	CTryMyDD4Doc* GetDocument() const;

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
	virtual ~CTryMyDD4View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TryMyDD4View.cpp
inline CTryMyDD4Doc* CTryMyDD4View::GetDocument() const
   { return reinterpret_cast<CTryMyDD4Doc*>(m_pDocument); }
#endif

