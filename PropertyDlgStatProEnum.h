#if !defined(AFX_PROPERTYDLGSTATPROENUM_H__4C54EC95_5D3F_4F13_99D1_D359B333DF0F__INCLUDED_)
#define AFX_PROPERTYDLGSTATPROENUM_H__4C54EC95_5D3F_4F13_99D1_D359B333DF0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgStatProEnum.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStatProEnum dialog

class CPropertyDlgStatProEnum : public CDialog
{
// Construction
public:
	CPropertyDlgStatProEnum(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgStatProEnum)
	enum { IDD = IDD_PROPERTYDLGSTATPROENUM_DIALOG };
	CListCtrl	m_List;
	double	m_dLeft;
	double	m_dRight;
	short	m_iStatType;
	int		m_iFunType;
	double	m_dGain;
	//}}AFX_DATA
public:
	CStringArray strInputArr;//输入变量
	CStringArray strOutputArr;//输出变量
	CStringArray strTagDespArr;//描述点名
	CStringArray strUnitArr;//单位
	int iTimeSelfOrInput;//标注时间段：0数据本身；1外部输入
private:
	void initListBoxHead();
	void initListBoxBody();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgStatProEnum)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgStatProEnum)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonInput();
	afx_msg void OnButtonOutput();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSTATPROENUM_H__4C54EC95_5D3F_4F13_99D1_D359B333DF0F__INCLUDED_)
