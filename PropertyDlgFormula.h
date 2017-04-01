#if !defined(AFX_PROPERTYDLGFORMULA_H__CCB6352E_D6F4_4A5B_9B11_F9058F128043__INCLUDED_)
#define AFX_PROPERTYDLGFORMULA_H__CCB6352E_D6F4_4A5B_9B11_F9058F128043__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgFormula.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgFormula dialog
#include "PropertyDlgFormulaPort.h"
#include "DlgFormulaData.h"
class CPropertyDlgFormula : public CDialog
{
// Construction
public:
	CPropertyDlgFormula(CWnd* pParent = NULL);   // standard constructor
	void SetPropData(
		const CStringArray &inPutArr,
		const CArray<int,int> &inPutTypeArr,
		const CStringArray &outPutArr,
		const CArray<int,int> &outPutTypeArr,
		const CStringArray &varArr,
		const CArray<int,int> &varTypeArr,
		const CStringArray &formulaArr);
	void InitFormulaListBoxHead();
	void InitFormulaListBoxBody();
	void initTreeCtrl();
private:
	void initTabCtrl();//初始化Tab控件的时候，创建每页所需要的数据
	int getAvailFormulaCount();//得到公式的有效个数
private:
	int currentSelFormulaIndex;
public:
	CStringArray inPutArr;
	CArray<int,int> inPutTypeArr;
	CArray<int,int> inPortCanBeEdit;

	CStringArray outPutArr;
	CArray<int,int> outPutTypeArr;
	CArray<int,int> outPortCanBeEdit;

	CStringArray varArr;
	CStringArray formulaArr;
	CArray<int,int> varTypeArr;//0,数值；1,序列

	CArray<CPropertyDlgFormulaPort*,CPropertyDlgFormulaPort*&> DlgTabArr;

// Dialog Data
	//{{AFX_DATA(CPropertyDlgFormula)
	enum { IDD = IDD_DIALOG_FORMULA };
	CTreeCtrl	m_ctlTree;
	CTabCtrl	m_TabList;
	CListCtrl	m_List;
	CString	m_StrSelFormula;
	CString	m_StrCelVarName;
	//}}AFX_DATA
	CImageList m_imageList;//定义图像列表控件

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgFormula)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgFormula)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTablist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnAddPort();
	afx_msg void OnBtnEditPort();
	afx_msg void OnBtnDelPort();
	afx_msg void OnBtnAddFormula();
	afx_msg void OnBtnEditFormula();
	afx_msg void OnBtnDelFormula();
	afx_msg void OnClickListFormula(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnUpFormula();
	afx_msg void OnBtnDownFormula();
	afx_msg void OnBtnUpPort();
	afx_msg void OnBtnDownPort();
	afx_msg void OnDblclkListFormula(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonTestformula();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGFORMULA_H__CCB6352E_D6F4_4A5B_9B11_F9058F128043__INCLUDED_)
