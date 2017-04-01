#if !defined(AFX_VIEWDEBUGFRM_H__78ED8CBB_6DF4_4F93_9936_51F87DA8509D__INCLUDED_)
#define AFX_VIEWDEBUGFRM_H__78ED8CBB_6DF4_4F93_9936_51F87DA8509D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewDebugFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewDebugFrm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CViewDebugFrm : public CFormView
{
public:
	CViewDebugFrm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewDebugFrm)

	CBCGPListCtrl	m_wndList;
	CFont		m_Font;
// Attributes
private:
	void debugSingleStep();
	CCalcModel* m_pModel;
public:
		long  m_lFocusID;  //当前落到第几个ID
// Operations
public:
		void LoadModel(CCalcModel* pModel);
// Form Data
public:
	//{{AFX_DATA(CViewDebugFrm)
	enum { IDD = IDD_FORMVIEW_MODELVIEWDEBUG };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void SetFocusListCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewDebugFrm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewDebugFrm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewDebugFrm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListDebug(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDebugSingleStep();
	afx_msg void OnUpdateDebugSingleStep(CCmdUI* pCmdUI);
	afx_msg void OnDebugGo();
	afx_msg void OnUpdateDebugGo(CCmdUI* pCmdUI);
	afx_msg void OnDebugGotoLine();
	afx_msg void OnUpdateDebugGotoLine(CCmdUI* pCmdUI);
	afx_msg void OnDebugCancel();
	afx_msg void OnUpdateDebugCancel(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWDEBUGFRM_H__78ED8CBB_6DF4_4F93_9936_51F87DA8509D__INCLUDED_)
