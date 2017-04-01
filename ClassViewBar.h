// ClassViewBar.h: interface for the CClassViewBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLASSVIEWBAR_H__5186B76A_E94C_4AE1_A2E6_8BFBA1C5A50E__INCLUDED_)
#define AFX_CLASSVIEWBAR_H__5186B76A_E94C_4AE1_A2E6_8BFBA1C5A50E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ModuleLibTree.h"

class CClassViewBar : public CBCGPDockingControlBar  
{
public:
	CClassViewBar();
	virtual ~CClassViewBar();
	void AdjustLayout ();

	//void LoadLibBlocks(_ConnectionPtr &pCon);
    void FillClassView (_ConnectionPtr &pCon);
public:
	CModuleLibTree	m_wndClassView;
protected:
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
	afx_msg void OnProperties();
	afx_msg void OnAddLibBlock();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CLASSVIEWBAR_H__5186B76A_E94C_4AE1_A2E6_8BFBA1C5A50E__INCLUDED_)
