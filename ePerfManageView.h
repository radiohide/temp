// ePerfManageView.h : interface of the CEPerfManageView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPERFMANAGEVIEW_H__830B2DD7_4496_4039_BD38_0E250FA36DCD__INCLUDED_)
#define AFX_EPERFMANAGEVIEW_H__830B2DD7_4496_4039_BD38_0E250FA36DCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCalcModel;
class CEPerfManageDoc;
class CEPerfManageView : public CBCGPTabView
{
protected: // create from serialization only
	CEPerfManageView();
	DECLARE_DYNCREATE(CEPerfManageView)
// Attributes
public:
	CEPerfManageDoc* GetDocument();
	CView* GetTabViewByIndex(int index);
// Operations
public:
	CEPerfManageDoc *GetDocumentFromView();
	//显示不同类型的页面，并根据条件进行显示具体的内容
	void ShowCurrentModel(); //显示模型的基本信息
	void ShowSelectedBlock(CString strModuleName,CString strBlockName);
	void CloseUserTabByName(CString strName);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEPerfManageView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetTabShowCount();
	int GetTabIndexByLable(CString tabText);
	BOOL FindTabWndByLable(CWnd*& pWnd,CString str);
	virtual ~CEPerfManageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEPerfManageView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	LRESULT OnUserGuid(WPARAM wParam,LPARAM lParam);
	LRESULT OnUserTabClose(WPARAM wParam,LPARAM lParam);
	LRESULT OnUserAllTabClose(WPARAM wParam,LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ePerfManageView.cpp
inline CEPerfManageDoc* CEPerfManageView::GetDocument()
   { return (CEPerfManageDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EPERFMANAGEVIEW_H__830B2DD7_4496_4039_BD38_0E250FA36DCD__INCLUDED_)
