// ModuleManageTree.h: interface for the CModuleManageTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODULEMANAGETREE_H__1DE3335C_6F81_4FD2_A359_3EB7EE31585C__INCLUDED_)
#define AFX_MODULEMANAGETREE_H__1DE3335C_6F81_4FD2_A359_3EB7EE31585C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTreeItemObj;
class CModuleManageTree : public CTreeCtrl  
{
public:
	void LoadManageNodes();
	CModuleManageTree();
	virtual ~CModuleManageTree();
protected:
	CObList	m_lstWorkspaces;
	CMap<CTreeItemObj*, CTreeItemObj*, HTREEITEM, HTREEITEM> m_Items;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	//{{AFX_MSG(CModuleTree)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_MODULEMANAGETREE_H__1DE3335C_6F81_4FD2_A359_3EB7EE31585C__INCLUDED_)
