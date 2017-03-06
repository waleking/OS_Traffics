// _dx4_Doc.h : interface of the C_dx4_Doc class
//


#pragma once


class C_dx4_Doc : public CDocument
{
protected: // create from serialization only
	C_dx4_Doc();
	DECLARE_DYNCREATE(C_dx4_Doc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~C_dx4_Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


