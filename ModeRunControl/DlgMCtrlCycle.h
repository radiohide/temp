#if !defined(AFX_DLGMCTRLCYCLE_H__5E2FD00C_D7FA_4C80_A083_0AE48A353367__INCLUDED_)
#define AFX_DLGMCTRLCYCLE_H__5E2FD00C_D7FA_4C80_A083_0AE48A353367__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMCtrlCycle.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMCtrlCycle dialog
#include "DlgMCtrlType.h"
#include "MCtrlCycle.h"
class CDlgMCtrlCycle : public CDlgMCtrlType
{
// Construction
public:
	CDlgMCtrlCycle(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMCtrlCycle)
	enum { IDD = IDD_DIALOG_MCTRL_CYCLE };
	CComboBox	m_ComboEWeek;
	CComboBox	m_ComboEMonth;
	CComboBox	m_ComboEDuty;
	CComboBox	m_ComboSDuty;
	CComboBox	m_ComBoSMonth;
	CComboBox	m_ComBoSWeek;
	int		m_iUnit;
	long	m_lCycleLength;
	CTime	m_EDTime;
	CTime	m_EMTime;
	CTime	m_EWTime;
	CTime	m_SDTime;
	CTime	m_SMTime;
	CTime	m_SWTime;
	long	m_lEDay;
	long	m_lEDuty;
	long	m_lEMonth;
	long	m_lESecond;
	long	m_lEWeek;
	long	m_lSDay;
	long	m_lSDuty;
	long	m_lSMonth;
	long	m_lSSecond;
	long	m_lSWeek;
	int		m_iSOffset_Type;
	int		m_iEOffset_Type;
	//}}AFX_DATA
public:
	virtual void ShowCtrlData();
	virtual void SetCtrlData();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMCtrlCycle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMCtrlCycle)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio01();
	afx_msg void OnRadio02();
	afx_msg void OnRadio03();
	afx_msg void OnRadio04();
	afx_msg void OnRadio05();
	afx_msg void OnRadio11();
	afx_msg void OnRadio12();
	afx_msg void OnRadio13();
	afx_msg void OnRadio14();
	afx_msg void OnRadio15();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void enbaleWindowByEOffsetType(MCtrlOffsetSE *pMCtrlOffsetSE);
	void enbaleWindowBySOffsetType(MCtrlOffsetSE *pMCtrlOffsetSE);
	void initOffsetSETime();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMCTRLCYCLE_H__5E2FD00C_D7FA_4C80_A083_0AE48A353367__INCLUDED_)
