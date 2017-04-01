// ClassViewModelBar.h: interface for the CClassViewModelBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLASSVIEWMODELBAR_H__335ED56D_C6E6_45D9_87BD_DF4DA82FFD1E__INCLUDED_)
#define AFX_CLASSVIEWMODELBAR_H__335ED56D_C6E6_45D9_87BD_DF4DA82FFD1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ModuleTree.h"

class CCalcModel;
class CClassViewModelBar : public CBCGPDockingControlBar  
{
public:
	CClassViewModelBar();
	virtual ~CClassViewModelBar();
	void AdjustLayout ();

	
   void RefreshModelTree(CCalcModel* pModel);
public:
	CModuleTree	    m_wndClassView;
	CImageList		m_ClassViewImages;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClassViewBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CClassViewBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();

	afx_msg void  OnModuleAddBlock();
	afx_msg void  OnModuleProp();
	afx_msg void  OnMRename();
	afx_msg void OnUpdateModuleAddBlock(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModuleProp(CCmdUI* pCmdUI);

	afx_msg void OnDblclkModelTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CLASSVIEWMODELBAR_H__335ED56D_C6E6_45D9_87BD_DF4DA82FFD1E__INCLUDED_)
