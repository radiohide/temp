#if !defined(AFX_FOLDERSTREE_H__A0EAE6DE_C7A4_11D4_95C8_00A0C9289F1B__INCLUDED_)
#define AFX_FOLDERSTREE_H__A0EAE6DE_C7A4_11D4_95C8_00A0C9289F1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FoldersTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModuleTree window
class CCalcModel;
class CTreeItemObj;
class CModuleTree : public CTreeCtrl
{
// Construction
public:
	//CModuleTree(const CObList& lstWorkspaces, CImageList& images);
	CModuleTree();
    void RefreshModelTree(CCalcModel* pModel);
	
	CObList	m_lstWorkspaces;
	CMap<CTreeItemObj*, CTreeItemObj*, HTREEITEM, HTREEITEM> m_Items;
// Attributes
protected:
	//CTreeCtrl		m_wndFolders;
	//const CObList&	m_lstWorkspaces;
	//CImageList&		m_images;

	//CMap<CWorkspaceObj*, CWorkspaceObj*, HTREEITEM, HTREEITEM>	m_Items;

// Operations
public:
	//BOOL SelectWorkspace (CWorkspaceObj* pWS);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModuleTree)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CModuleTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CModuleTree)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOLDERSTREE_H__A0EAE6DE_C7A4_11D4_95C8_00A0C9289F1B__INCLUDED_)
