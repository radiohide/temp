#if !defined(AFX_DLGSTATVARSEL_H__2C81E9A4_46C6_49E4_8C31_865B232B2B6C__INCLUDED_)
#define AFX_DLGSTATVARSEL_H__2C81E9A4_46C6_49E4_8C31_865B232B2B6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStatVarSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgStatVarSel dialog

class CDlgStatVarSel : public CDialog
{
// Construction
public:
	CDlgStatVarSel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgStatVarSel)
	enum { IDD = IDD_DIALOG_STATVAR_SEL };
	CListCtrl	m_List;
	int		m_iEditSet;
	int		m_iEditUnit;
	//}}AFX_DATA
	CCalcModel *m_pModel;
	CString m_strDBName;
	int m_iVarType;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStatVarSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	CStringArray m_strVarNameArr;
	CStringArray m_strVarUnitArr;
	CStringArray m_strVarDespArr;
public:
	CStringArray m_strSelVarNameArr;
	CStringArray m_strSelVarUnitArr;
	CStringArray m_strSelVarDespArr;

private:
	void initListBoxHead();
	void initListBoxBody();
	void readVarList();
	BOOL gIsTableExist(_ConnectionPtr &pConnection, CString strTableName);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgStatVarSel)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTATVARSEL_H__2C81E9A4_46C6_49E4_8C31_865B232B2B6C__INCLUDED_)
