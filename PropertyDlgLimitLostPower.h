#if !defined(AFX_PROPERTYDLGLIMITLOSTPOWER_H__5AED0A98_E808_4B55_9E93_288DCFA30AE6__INCLUDED_)
#define AFX_PROPERTYDLGLIMITLOSTPOWER_H__5AED0A98_E808_4B55_9E93_288DCFA30AE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgLimitLostPower.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgLimitLostPower dialog

class CPropertyDlgLimitLostPower : public CDialog
{
// Construction
public:
	CPropertyDlgLimitLostPower(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgLimitLostPower)
	enum { IDD = IDD_PROPERTYDLGLIMITLOSTPOWER_DIALOG };
	CListCtrl	m_List2;
	double	dMaxPower;
	//}}AFX_DATA

	CArray<double,double> dWindSpeedArr;//功率曲线--风速
	CArray<double,double> dPowerArr;//功率曲线--功率

	int iTimeSelfOrInput;//标注时间段：0数据本身；1外部输入

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgLimitLostPower)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	private:
	void initListBoxHead2();
	void initListBoxBody2();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgLimitLostPower)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonInput2();
	afx_msg void OnButtonOutput2();
	afx_msg void OnButtonAdd2();
	afx_msg void OnButtonEdit2();
	afx_msg void OnButtonDel2();
	virtual void OnOK();
	afx_msg void OnDblclkList4(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGLIMITLOSTPOWER_H__5AED0A98_E808_4B55_9E93_288DCFA30AE6__INCLUDED_)
