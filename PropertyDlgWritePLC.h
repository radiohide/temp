#if !defined(AFX_PROPERTYDLGWRITEPLC_H__44BA9645_346F_4F47_B7E5_094D7E9DC0B0__INCLUDED_)
#define AFX_PROPERTYDLGWRITEPLC_H__44BA9645_346F_4F47_B7E5_094D7E9DC0B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgWritePLC.h : header file
//
#include <vector>
/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgWritePLC dialog

class CPropertyDlgWritePLC : public CDialog
{
private:
	CMap<CString,LPCTSTR,int,int>  m_idMap;
	CMap<CString,LPCTSTR,CString,CString&>  m_desMap;
	CMap<CString,LPCTSTR,int,int>  m_addressMap;
	CMap<CString,LPCTSTR,CString,CString&>  m_funcodeMap;
	CMap<CString,LPCTSTR,CString,CString&>  m_typeMap;

	std::vector<int> v_funcodeSer;//π¶ƒ‹¬Î–Ú¡–
// Construction
public:
	BOOL FindRightFunCode(CString dataType,int &FunCode);
	void DealFunCode(CString strFunCodeSer);
	void InitMap();
	CPropertyDlgWritePLC(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgWritePLC)
	enum { IDD = IDD_DIALOG_WRITEPLC };
	CComboBox	m_ComPointName;
	CComboBox	m_ComType;
	int		m_iPort;
	int		m_iFunNo;
	int		m_iAdd;
	CString	m_strIP;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

    CString m_strCurrentPointName;
	CString m_strCurentType;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgWritePLC)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgWritePLC)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnEditchangeCombo2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGWRITEPLC_H__44BA9645_346F_4F47_B7E5_094D7E9DC0B0__INCLUDED_)
