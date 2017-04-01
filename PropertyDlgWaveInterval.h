#if !defined(AFX_PROPERTYDLGWAVEINTERVAL_H__1F2DE565_D6FF_4494_AE28_0BA50BBE554E__INCLUDED_)
#define AFX_PROPERTYDLGWAVEINTERVAL_H__1F2DE565_D6FF_4494_AE28_0BA50BBE554E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgWaveInterval.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgWaveInterval dialog

class CPropertyDlgWaveInterval : public CDialog
{
// Construction
public:
	CPropertyDlgWaveInterval(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgWaveInterval)
	enum { IDD = IDD_DIALOG_WAVEINTERVAL };
	CListCtrl	m_List;
	double	m_dSpanIntPercent;
	int		m_iSpanKeepTime;
	int		m_iSmoothStep;
	int		m_iSmoothWidth;
	//}}AFX_DATA
	CArray<double,double> m_dArrDisValue;//离散值
	CArray<double,double> m_dArrIntStart;//区间开始值
	CArray<double,double> m_dArrIntEnd;//区间结束值
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgWaveInterval)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgWaveInterval)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InitListBoxHead();
	void InitListBoxBody();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGWAVEINTERVAL_H__1F2DE565_D6FF_4494_AE28_0BA50BBE554E__INCLUDED_)
