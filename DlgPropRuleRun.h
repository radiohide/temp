#if !defined(AFX_DLGPROPRULERUN_H__F3EE2879_E771_4757_AF6A_909E30B64BDE__INCLUDED_)
#define AFX_DLGPROPRULERUN_H__F3EE2879_E771_4757_AF6A_909E30B64BDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPropRuleRun.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPropRuleRun dialog
#include "DlgFBVar.h"

class CDlgPropRuleRun : public CDialog
{
// Construction
public:
	CDlgPropRuleRun(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPropRuleRun)
	enum { IDD = IDD_PROPDLG_RULERUN };
	CTabCtrl	m_TabList;
	CString	m_strDBConnect;
	CString	m_strRuleDBName;
	CString	m_strRuleDBDesp;
	//}}AFX_DATA
	_ConnectionPtr m_pConRule; //���ӵ��������
	CDlgFBVar *pDlgFVar;
	CDlgFBVar *pDlgBVar;

	CStringArray nameFArr;
	CStringArray despFArr;
	CArray<int,int> tagBeInFArr;//��������Ƶ���ǰ���������,1:����,0:������

	CStringArray nameBArr;
	CStringArray despBArr;
	CArray<int,int> tagBeInBArr;//��������Ƶ��ĺ���������,1:����,0:������
	
	int  m_iTagToFOrB;//ǰ������ͷ��������ǣ�0:ǰ��(eg:��״̬������)(Ĭ��),1:����(eg:��״̬������)
	int  m_iTagWeightOrTime;//����Ȩ������ǰ���ʱ����������й���ƥ�䡣0:Ȩ�����1:ʱ�����
	void displayFBVar();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPropRuleRun)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CString getABSDirRuleDB();
	bool m_bCon;
	BOOL EstablishMDBConnectPtr(CString strFileName);
	void initTabCtrl();
	void readFBVar();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPropRuleRun)
	afx_msg void OnButtonConstring();
	afx_msg void OnButtonSelRuleID();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROPRULERUN_H__F3EE2879_E771_4757_AF6A_909E30B64BDE__INCLUDED_)
