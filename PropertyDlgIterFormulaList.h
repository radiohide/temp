#if !defined(AFX_PROPERTYDLGITERFORMULALIST_H__B74D79F0_DB2A_4314_82EB_2363D7651D7F__INCLUDED_)
#define AFX_PROPERTYDLGITERFORMULALIST_H__B74D79F0_DB2A_4314_82EB_2363D7651D7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgIterFormulaList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgIterFormulaList dialog

class CPropertyDlgIterFormulaList : public CDialog
{
// Construction
public:
	CPropertyDlgIterFormulaList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgIterFormulaList)
	enum { IDD = IDD_DIALOG_ITER_FORMULALIST };
	CListCtrl	m_List;
	int		m_iConFOrB;
	//}}AFX_DATA
public:
	CStringArray *pVNameArr;
	CArray<int,int> *pTypeArr;
	CStringArray *pFormulaArr;
	CArray<int,int> *pCanBeEditArr;
	CArray<int,int> *pCanBeShowArr;
private:
	int *m_pCurrSelFormulaIndex;
	int *m_pConFOrB;
private:
	int getAvailFormulaCount();//得到公式的有效个数
public:
	void SetPtrTagConFOrB(int *ptrTagConFOrB);
	void ShowConFOrBCtrl(CRect rc);
	void HideConFOrBCtrl();
	bool MoveDownVar();
	bool MoveUpVar();
	bool DelVar();
	bool EditVar();
	bool AddVar();
	void SetCurrentSelIndex(int *m_pCurrSelFormulaIndex);
	void SetFormulaData(CStringArray *pVNameArr,CArray<int,int> *pTypeArr,CStringArray *pFormulaArr);
	void SetCanBeEditCanBeShow(CArray<int,int> *pCanBeEditArr,CArray<int,int> *pCanBeShowArr);
	void InitFormulaListBoxHead();
	void InitFormulaListBoxBody();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgIterFormulaList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgIterFormulaList)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGITERFORMULALIST_H__B74D79F0_DB2A_4314_82EB_2363D7651D7F__INCLUDED_)
