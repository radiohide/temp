#if !defined(AFX_MODELRUNVIEW_H__1C24012D_AADA_4257_B9A1_6B6C7596D2B9__INCLUDED_)
#define AFX_MODELRUNVIEW_H__1C24012D_AADA_4257_B9A1_6B6C7596D2B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelRunView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModelRunView view

class CModelRunView : public CView
{
protected:
	CModelRunView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CModelRunView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelRunView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CModelRunView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CModelRunView)
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELRUNVIEW_H__1C24012D_AADA_4257_B9A1_6B6C7596D2B9__INCLUDED_)
