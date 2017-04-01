#if !defined(AFX_FVIEWBLOCKLINK_H__1364A995_7C49_49BC_A3CD_ABF4FF727D8C__INCLUDED_)
#define AFX_FVIEWBLOCKLINK_H__1364A995_7C49_49BC_A3CD_ABF4FF727D8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FViewBlockLink.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFViewBlockLink form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "BlockGridCtrl.h"
#include "MyList.h"
class CCalcBlock;

class CFViewBlockLink : public CFormView
{
protected:
	CFViewBlockLink();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFViewBlockLink)

// Form Data
public:
	//{{AFX_DATA(CFViewBlockLink)
	enum { IDD = IDD_FORMVIEW_BLOCKLINK };
	CStatic	m_wndToolbarLocation;
	//}}AFX_DATA
protected:
	CBlockGridCtrl m_GridCtrl;
	CStatic	m_wndGridLocation;
	CComboBox	m_combCtrl;
	CMyListCtrl	m_lstBlocks;

	CImageList	   m_BlockImages;

	

	//CBCGPToolBar		m_wndToolBar;
// Attributes
public:
	CCalcBlock* m_pSelBlock;
	CString     m_strSelModuleName;
	
// Operations
public:
	CString GetSelBlockOutputLinkID();
	void ShowInOutPage(CCalcBlock* pBlock);
	void UpdateBlockProp(CString strBlockName);
	void RefreshModuleCombCtrl();
	//void SetCellColor(int row,int col,COLORREF bk,COLORREF frt);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFViewBlockLink)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFViewBlockLink();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFViewBlockLink)
	afx_msg void OnButtonAhead();
	afx_msg void OnButtonRear();
	afx_msg void OnSelchangeComboModule();
	afx_msg void OnDblclkListBlocks(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListBlocks(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnBlockLink();
//	afx_msg void OnUpdateBlockLink(CCmdUI* pCmdUI);
//	afx_msg void OnBlockDisLink();
//	afx_msg void OnUpdateBlockDisLink(CCmdUI* pCmdUI);
	afx_msg void OnGridDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FVIEWBLOCKLINK_H__1364A995_7C49_49BC_A3CD_ABF4FF727D8C__INCLUDED_)
