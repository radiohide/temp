#if !defined(AFX_PROPERTYDLGPLCPOINT_H__498C5693_07AD_499B_BBC5_2411B58DFCA7__INCLUDED_)
#define AFX_PROPERTYDLGPLCPOINT_H__498C5693_07AD_499B_BBC5_2411B58DFCA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgPLCPoint.h : header file
//
#include <vector>
/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgPLCPoint dialog

class CPropertyDlgPLCPoint : public CDialog
{
private:
	CMap<CString,LPCTSTR,int,int>  m_idMap;
	CMap<CString,LPCTSTR,CString,CString&>  m_desMap;
	CMap<CString,LPCTSTR,int,int>  m_addressMap;
	CMap<CString,LPCTSTR,CString,CString&>  m_funcodeMap;
	CMap<CString,LPCTSTR,CString,CString&>  m_typeMap;

	std::vector<int> v_funcodeSer;//����������
// Construction
public:
	BOOL FindRightFunCode(CString dataType,int &FunCode);
	void DealFunCode(CString strFunCodeSer);
	void InitMap();
	CPropertyDlgPLCPoint(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgPLCPoint)
	enum { IDD = IDD_DIALOG_PLCPOINT };
	CComboBox	m_ComPointName;
	CComboBox	m_ComType;
	int		m_iPort;
	int		m_iFunNo;
	int		m_iAdd;
	CString	m_strIP;
	//}}AFX_DATA
	CString m_strCurrentPointName;
	CString m_strCurentType;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgPLCPoint)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgPLCPoint)
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

#endif // !defined(AFX_PROPERTYDLGPLCPOINT_H__498C5693_07AD_499B_BBC5_2411B58DFCA7__INCLUDED_)
