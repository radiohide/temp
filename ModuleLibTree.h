#if !defined(AFX_MODULELIBTREE_H__1C3C9436_6A64_437A_A8E5_2C72E57B2A4C__INCLUDED_)
#define AFX_MODULELIBTREE_H__1C3C9436_6A64_437A_A8E5_2C72E57B2A4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModuleLibTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModuleLibTree window
class CTreeItemObj;
class CModuleLibTree : public CTreeCtrl
{
// Construction
public:
	CModuleLibTree();

// Attributes
protected:
	CObList	m_lstWorkspaces;
	CMap<CTreeItemObj*, CTreeItemObj*, HTREEITEM, HTREEITEM> m_Items;

	CMenu m_menu;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModuleLibTree)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void LoadLibBlocks(_ConnectionPtr &pCon);
	virtual ~CModuleLibTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CModuleLibTree)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODULELIBTREE_H__1C3C9436_6A64_437A_A8E5_2C72E57B2A4C__INCLUDED_)
