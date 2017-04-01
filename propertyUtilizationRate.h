#if !defined(AFX_PROPERTYUTILIZATIONRATE_H__9D9DFEC2_8F38_4BC5_85C1_3BA9B8BE62FF__INCLUDED_)
#define AFX_PROPERTYUTILIZATIONRATE_H__9D9DFEC2_8F38_4BC5_85C1_3BA9B8BE62FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// propertyUtilizationRate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CpropertyUtilizationRate dialog

class CpropertyUtilizationRate : public CDialog
{
// Construction
public:
	CpropertyUtilizationRate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CpropertyUtilizationRate)
	enum { IDD = IDD_DIALOG_UtiliRatePlg };
	CComboBox	m_comboBoxDBSel;
	CString	m_strTableName;
	CString	m_strTurbine;
	int		m_iPowerPorTag;
	/*
	double	m_dMinParWindSpeed;
	double	m_dMaxParWindSpeed;
	double	m_dPowerLimit;
	int		m_iPowerPorTagMin;
	int		m_iPowerPorTagMax;*/
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	CString m_strDBName;//数据库名称
	CArray<int,int> m_ArrSourceRDBTagCon;//连接标志
	CStringArray m_ArrSourceRDBName;//连接名称
	CStringArray m_ArrSourceRDBType;//连接类型
private:
	CStringArray despArr;
	CStringArray typeArr;
public:
	void SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
		CStringArray &m_ArrSourceRDBName,
		CStringArray &m_ArrSourceRDBType
		);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CpropertyUtilizationRate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CpropertyUtilizationRate)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDbsel();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYUTILIZATIONRATE_H__9D9DFEC2_8F38_4BC5_85C1_3BA9B8BE62FF__INCLUDED_)
