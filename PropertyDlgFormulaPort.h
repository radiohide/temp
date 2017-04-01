#if !defined(AFX_PROPERTYDLGFORMULAPORT_H__A00AA225_CFED_4379_A4D9_4F749525976F__INCLUDED_)
#define AFX_PROPERTYDLGFORMULAPORT_H__A00AA225_CFED_4379_A4D9_4F749525976F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgFormulaPort.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgFormulaPort dialog
#include "DlgPortData.h"

class CPropertyDlgFormulaPort : public CDialog
{
// Construction
public:
	void SetDlgIsInOrOut(int tag);

	bool AddPort();//返回值表示加入成功
	bool EditPort();//返回值表示编辑成功
	bool DelPort();//返回值表示编辑成功
	bool MoveDownPort();//向下移动端口
	bool MoveUpPort();//向上移动端口

	CPropertyDlgFormulaPort(CWnd* pParent = NULL);   // standard constructor
	void InitPortListBoxHead();
	void InitPortListBoxBody();
	void SetPortPropData(CStringArray *pPortNameArr,CArray<int,int> *pPortTypeArr);
	void SetCanBeEdit(CArray<int,int> *pPortCanBeEdit);
// Dialog Data
	//{{AFX_DATA(CPropertyDlgFormulaPort)
	enum { IDD = IDD_DIALOG_FORMULA_PORT };
	CListCtrl	m_List;
	//}}AFX_DATA

	CStringArray *pPortNameArr;
	CArray<int,int> *pPortTypeArr;
	CArray<int,int> *pPortCanBeEdit;
	int tagInOrOut;//1：IN；2：out；
private:
	int getAvailPortCount();
	int currentSelPortIndex;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgFormulaPort)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgFormulaPort)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGFORMULAPORT_H__A00AA225_CFED_4379_A4D9_4F749525976F__INCLUDED_)
