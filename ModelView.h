#if !defined(AFX_MODELVIEW_H__C53166CF_0F6D_484C_A60C_08A79E3CBDA1__INCLUDED_)
#define AFX_MODELVIEW_H__C53166CF_0F6D_484C_A60C_08A79E3CBDA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModelView view

class CModelView : public CListView
{
protected:
	CModelView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CModelView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CModelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CModelView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELVIEW_H__C53166CF_0F6D_484C_A60C_08A79E3CBDA1__INCLUDED_)
