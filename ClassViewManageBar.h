// ClassViewManageBar.h: interface for the CClassViewManageBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLASSVIEWMANAGEBAR_H__F5EDF72B_B0DF_426D_9C85_10ECF7F2F717__INCLUDED_)
#define AFX_CLASSVIEWMANAGEBAR_H__F5EDF72B_B0DF_426D_9C85_10ECF7F2F717__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//本控制条主要是负责：测点维护、煤质数据等维护类
//从模型中获得所有的测点信息，并一种统一的形式进行维护，例如可以修改，修改后会影响；
//但不能修改
#include "ModuleManageTree.h"
class CClassViewManageBar : public CBCGPDockingControlBar  
{
public:
	void RefreshTreeContent();
	CClassViewManageBar();
	virtual ~CClassViewManageBar();
protected:
	CModuleManageTree	    m_wndManageTree;
	CImageList		m_Images;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClassViewBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CClassViewBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();

	afx_msg void  OnModuleProp();

	afx_msg void OnDblclkModelTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CLASSVIEWMANAGEBAR_H__F5EDF72B_B0DF_426D_9C85_10ECF7F2F717__INCLUDED_)
