#if !defined(AFX_PROPERTYDLGSTATPROAVG_H__64E1E99B_0C5F_4677_A305_A5BE341361B2__INCLUDED_)
#define AFX_PROPERTYDLGSTATPROAVG_H__64E1E99B_0C5F_4677_A305_A5BE341361B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgStatProAvg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStatProAvg dialog

class CPropertyDlgStatProAvg : public CDialog
{
// Construction
public:
	CPropertyDlgStatProAvg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgStatProAvg)
	enum { IDD = IDD_PROPERTYDLGSTATPROAVG_DIALOG };
	CListCtrl	m_List;
	CString	m_strBlockDesp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgStatProAvg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	CStringArray strInputArr;//�������
	CStringArray strOutputArr;//�������
	CStringArray strTagDespArr;//��������
	CStringArray strUnitArr;//��λ
	int iTimeSelfOrInput;//��עʱ��Σ�0���ݱ���1�ⲿ����
public:
	int m_iProOrStatOrSum;//ͳ�ƹ������ݻ���ͳ��ͳ�����ݣ�0:��������(Ĭ��)��1ͳ�����ݾ�ֵ��2ͳ��������ͣ�
// Implementation
protected:
private:
	void initListBoxHead();
	void initListBoxBody();
	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgStatProAvg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonInput();
	afx_msg void OnButtonOutput();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSTATPROAVG_H__64E1E99B_0C5F_4677_A305_A5BE341361B2__INCLUDED_)
