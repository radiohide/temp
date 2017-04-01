#if !defined(AFX_PROPERTYDLGRDBFIELD_H__C9460CE2_46CA_473D_BEFB_56CC8C4C67CB__INCLUDED_)
#define AFX_PROPERTYDLGRDBFIELD_H__C9460CE2_46CA_473D_BEFB_56CC8C4C67CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgRDBField.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRDBField dialog

class CPropertyDlgRDBField : public CDialog
{
// Construction
public:
	CPropertyDlgRDBField(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgRDBField)
	enum { IDD = IDD_DIALOG_RDBFIELD };
	CString	m_strName;
	CString	m_strState;
	CString	m_strTime;
	CString	m_strValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgRDBField)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	public:
	CString *m_pStrFieldVarName;
	CString *m_pStrFieldDateTime;
	CString *m_pStrFieldState;
	CString *m_pStrFieldValue;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgRDBField)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGRDBFIELD_H__C9460CE2_46CA_473D_BEFB_56CC8C4C67CB__INCLUDED_)
