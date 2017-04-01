#if !defined(AFX_PROPERTYDLGTIMEOFFSET_H__C2C67B85_A738_4369_BDA5_FF282496936E__INCLUDED_)
#define AFX_PROPERTYDLGTIMEOFFSET_H__C2C67B85_A738_4369_BDA5_FF282496936E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgTimeOffset.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgTimeOffset dialog

class CPropertyDlgTimeOffset : public CDialog
{
// Construction
public:
	CPropertyDlgTimeOffset(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgTimeOffset)
	enum { IDD = IDD_DIALOG_TIME_OFFSET };
	int		m_iOffsetType;
	int		m_iSecond;
	int		m_iDay;
	int		m_iMonth;
	int		m_iMonthDay;
	CTime	m_DTime;
	CTime	m_MTime;
	int		m_iWeekDay;
	int		m_iYearDay;
	int		m_iYearMonth;
	CTime	m_HTime;
	CTime	m_WTime;
	CTime	m_YTime;
	int		m_iYear;
	int		m_iWeek;
	int		m_iHour;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgTimeOffset)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	void initUI();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgTimeOffset)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio01();
	afx_msg void OnRadio02();
	afx_msg void OnRadio04();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGTIMEOFFSET_H__C2C67B85_A738_4369_BDA5_FF282496936E__INCLUDED_)
