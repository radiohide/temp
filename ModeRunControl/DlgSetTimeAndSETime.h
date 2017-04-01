#if !defined(AFX_DLGSETTIMEANDSETIME_H__D93A59CB_BA10_4B88_9430_C08E802982F0__INCLUDED_)
#define AFX_DLGSETTIMEANDSETIME_H__D93A59CB_BA10_4B88_9430_C08E802982F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetTimeAndSETime.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgSetTimeAndSETime dialog
#include "MCtrlOffsetSE.h"

class DlgSetTimeAndSETime : public CDialog
{
// Construction
public:
	DlgSetTimeAndSETime(CWnd* pParent = NULL);   // standard constructor
	BOOL m_bBeDuty;
	BOOL m_bBeMonth;
	int m_iDay;
	CString m_strTime;
	MCtrlOffsetSE m_MCtrlOffsetSE;

	CStringArray m_ArrAllDuty;
	CString m_strDutyName;
	int m_iDutySOrE;

	void SetBeDutyStyle();
	void SetAllDuty(CStringArray &m_ArrAllDuty);
	void SetDutyTimeData(CString strDutyName,int dutySOrE);
	void SetMonthTheDay(int day);
	void SetBeMonthStyle();
	void SetTimeAndOffSetSETime(CString strTime,MCtrlOffsetSE *pMCtrlOffsetSE);
// Dialog Data
	//{{AFX_DATA(DlgSetTimeAndSETime)
	enum { IDD = IDD_DIALOG_TIME_SETIME };
	CComboBox	m_ComboDutySOrE;
	CComboBox	m_ComboDutyNum;
	CComboBox	m_ComboMonthDay;
	CTime	m_Time;
	CComboBox	m_ComboEWeek;
	CComboBox	m_ComboEMonth;
	CComboBox	m_ComboEDuty;
	CComboBox	m_ComboSDuty;
	CComboBox	m_ComBoSMonth;
	CComboBox	m_ComBoSWeek;
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


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgSetTimeAndSETime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgSetTimeAndSETime)
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
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void enbaleWindowByEOffsetType(MCtrlOffsetSE *pMCtrlOffsetSE);
	void enbaleWindowBySOffsetType(MCtrlOffsetSE *pMCtrlOffsetSE);
	void initOffsetSETime();
	void setCtrlData();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETTIMEANDSETIME_H__D93A59CB_BA10_4B88_9430_C08E802982F0__INCLUDED_)
