#if !defined(AFX_DLGLOCALRUNSETTING_H__015F90E8_CD00_4199_8222_E85A0F645817__INCLUDED_)
#define AFX_DLGLOCALRUNSETTING_H__015F90E8_CD00_4199_8222_E85A0F645817__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLocalRunSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLocalRunSetting dialog

class CDlgLocalRunSetting : public CDialog
{
// Construction
public:
	CDlgLocalRunSetting(CWnd* pParent = NULL);   // standard constructor
    int  m_nCycleT;  //实时的周期
	int  m_nInterval; //间隔
// Dialog Data
	//{{AFX_DATA(CDlgLocalRunSetting)
	enum { IDD = IDD_DIALOG_LOCALRUNSETTING };
	CComboBox	m_combInterval;
	CDateTimeCtrl	m_CtrlStartTime;
	CDateTimeCtrl	m_CtrlStartDate;
	CDateTimeCtrl	m_CtrlEndTime;
	CDateTimeCtrl	m_CtrlEndDate;
	CComboBox	m_combCycleT;
	int		m_nCalcMode;
	CTime	m_EndDate;
	CTime	m_EndTime;
	CTime	m_StartDate;
	CTime	m_StartTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLocalRunSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLocalRunSetting)
	afx_msg void OnSelchangeComboCycel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboMode();
	virtual void OnOK();
	afx_msg void OnEditchangeComboInterval();
	afx_msg void OnEditchangeComboCycel();
	afx_msg void OnButtonAllonetime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOCALRUNSETTING_H__015F90E8_CD00_4199_8222_E85A0F645817__INCLUDED_)
