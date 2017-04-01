#if !defined(AFX_PROPERTYDLGSTATTOPROC_H__8F48FFC0_F4D9_4CEF_908D_F4439F349DE1__INCLUDED_)
#define AFX_PROPERTYDLGSTATTOPROC_H__8F48FFC0_F4D9_4CEF_908D_F4439F349DE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgStatToProc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStatToProc dialog

class CPropertyDlgStatToProc : public CDialog
{
// Construction
public:
	CPropertyDlgStatToProc(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgStatToProc)
	enum { IDD = IDD_DIALOG_STATISTOPROCESS };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgStatToProc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CStringArray strInputArr;//�������
	CStringArray strOutputArr;//�������
	CStringArray strTagDespArr;//��������
	CStringArray strUnitArr;//��λ
	int iTimeSelect;//��עʱ�䣺0 ��ʼʱ�䣻1 ��ֹʱ�� 2 ����ʱ��
	int	m_nProcType;//����ֵ���ͣ�0 - 2 Ĭ�� 0

public:
	void initListBoxBody();
	void initListBoxHead();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgStatToProc)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonInput();
	afx_msg void OnButtonOutput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSTATTOPROC_H__8F48FFC0_F4D9_4CEF_908D_F4439F349DE1__INCLUDED_)
