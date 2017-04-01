#if !defined(AFX_PROPERTYDLGLIMITLOSTPOWERPARAEXT_H__071883C2_438C_4A38_83A7_6F45AF3DA16A__INCLUDED_)
#define AFX_PROPERTYDLGLIMITLOSTPOWERPARAEXT_H__071883C2_438C_4A38_83A7_6F45AF3DA16A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgLimitLostPowerParaExt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgLimitLostPowerParaExt dialog

class CPropertyDlgLimitLostPowerParaExt : public CDialog
{
// Construction
public:
	CPropertyDlgLimitLostPowerParaExt(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgLimitLostPowerParaExt)
	enum { IDD = IDD_PROPERTYDLGLIMITLOSTPOWERPARAEXT_DIALOG };
	CListCtrl	m_List2;
	double	dMaxPower;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CArray<double,double> dWindSpeedArr;//功率曲线--风速
	CArray<double,double> dPowerArr;//功率曲线--功率
	
	int iTimeSelfOrInput;//标注时间段：0数据本身；1外部输入


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgLimitLostPowerParaExt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	void initListBoxHead2();
	void initListBoxBody2();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgLimitLostPowerParaExt)
		// NOTE: the ClassWizard will add member functions here
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

#endif // !defined(AFX_PROPERTYDLGLIMITLOSTPOWERPARAEXT_H__071883C2_438C_4A38_83A7_6F45AF3DA16A__INCLUDED_)
