#if !defined(AFX_DLGCFGFINDBLOCK_H__0A56CEC0_A4CE_4484_B5F1_12F293DECDA9__INCLUDED_)
#define AFX_DLGCFGFINDBLOCK_H__0A56CEC0_A4CE_4484_B5F1_12F293DECDA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCfgFindBlock.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgFindBlock dialog
#include "DlgTabFindBlock.h"
#include "CalcBlock.h"
class CDlgCfgFindBlock : public CDialog
{
// Construction
public:
	CDlgCfgFindBlock(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCfgFindBlock)
	enum { IDD = IDD_CFG_FIND_BLOCK };
	CListCtrl	m_List;
	CTabCtrl	m_TabList;
	//}}AFX_DATA
	//BOOL Create( UINT nIDTemplate, CWnd* pParentWnd = NULL );

	int m_iIndexFindNext;
	int m_iTagFindOrReplace;//0:≤È’“£¨1:ÃÊªª

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCfgFindBlock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
	CArray<CCalcBlock*,CCalcBlock*&> m_blockFindArr;
private:
	CDlgTabFindBlock *pDlgFindAndReplace;
private:
	void initTabCtrl();
	void findAllBlock();
	void InitListBoxBody();
	void InitListBoxHead();
	void setListBeSel(int index);
	void selBlockInView(int index);
	void replaceBlockInView(int index);
	void replaceListByIndex(int index);
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCfgFindBlock)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnBtnFindNext();
	afx_msg void OnBtnFindAll();
	afx_msg void OnBtnReplace();
	afx_msg void OnBtnReplaceall();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCFGFINDBLOCK_H__0A56CEC0_A4CE_4484_B5F1_12F293DECDA9__INCLUDED_)
