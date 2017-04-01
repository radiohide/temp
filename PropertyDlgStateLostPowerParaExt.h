#if !defined(AFX_PROPERTYDLGSTATELOSTPOWERPARAEXT_H__02890556_7C12_4BE2_B427_BCA95522595F__INCLUDED_)
#define AFX_PROPERTYDLGSTATELOSTPOWERPARAEXT_H__02890556_7C12_4BE2_B427_BCA95522595F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgStateLostPowerParaExt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStateLostPowerParaExt dialog

class CPropertyDlgStateLostPowerParaExt : public CDialog
{
// Construction
public:
	CPropertyDlgStateLostPowerParaExt(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgStateLostPowerParaExt)
	enum { IDD = IDD_PROPERTYDLGSTATELOSTPOWERPARAEXT_DIALOG };
	CListCtrl	m_List2;
	CListCtrl	m_List;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


public:
	CStringArray strInputArr;//输入变量
	CArray<int,int> iValidMin;//弃风时的数值最小值
	CArray<int,int> iValidMax;//弃风时的数值最大值
	CStringArray strTagDespArr;//描述点名
	
	CArray<double,double> dWindSpeedArr;//功率曲线--风速
	CArray<double,double> dPowerArr;//功率曲线--功率
	
	int iTimeSelfOrInput;//标注时间段：0数据本身；1外部输入
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgStateLostPowerParaExt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	void initListBoxHead();
	void initListBoxBody();
	void initListBoxHead2();
	void initListBoxBody2();

// Implementation
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonInput();
	afx_msg void OnButtonOutput();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonInput2();
	afx_msg void OnButtonOutput2();
	afx_msg void OnButtonAdd2();
	afx_msg void OnButtonEdit2();
	afx_msg void OnButtonDel2();
	virtual void OnOK();
	afx_msg void OnDblclkList4(NMHDR* pNMHDR, LRESULT* pResult);
	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgStateLostPowerParaExt)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGSTATELOSTPOWERPARAEXT_H__02890556_7C12_4BE2_B427_BCA95522595F__INCLUDED_)
