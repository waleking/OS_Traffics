// TryMyDD4Doc.h : interface of the CTryMyDD4Doc class
//


#pragma once


class CTryMyDD4Doc : public CDocument
{
protected: // create from serialization only
	CTryMyDD4Doc();
	DECLARE_DYNCREATE(CTryMyDD4Doc)

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
	virtual ~CTryMyDD4Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


