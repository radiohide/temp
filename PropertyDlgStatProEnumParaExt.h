#if !defined(AFX_PROPERTYDLGSTATPROENUMPARAEXT_H__8739CAC1_8512_472A_9AA1_92F98CCAF6C6__INCLUDED_)
#define AFX_PROPERTYDLGSTATPROENUMPARAEXT_H__8739CAC1_8512_472A_9AA1_92F98CCAF6C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgStatProEnumParaExt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStatProEnumParaExt dialog

class CPropertyDlgStatProEnumParaExt : public CDialog
{
// Construction
public:
	CPropertyDlgStatProEnumParaExt(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgStatProEnumParaExt)
	enum { IDD = IDD_PROPERTYDLGSTATPROENUMPARAEXT_DIALOG };
	CListCtrl	m_List;
	double	m_dLeft;
	double	m_dRight;
	short	m_iStatType;
	int		m_iFunType;
	double	m_dGain;
		// NOTE: the ClassWizard will add data members here
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
	//{{AFX_VIRTUAL(CPropertyDlgStatProEnumParaExt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgStatProEnumParaExt)
	// NOTE: the ClassWizard will add member functions here	
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

#endif // !defined(AFX_PROPERTYDLGSTATPROENUMPARAEXT_H__8739CAC1_8512_472A_9AA1_92F98CCAF6C6__INCLUDED_)
