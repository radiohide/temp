#if !defined(AFX_DLGSYSVAR_H__7A39DB91_B158_40E3_9695_624D1BB0CCBB__INCLUDED_)
#define AFX_DLGSYSVAR_H__7A39DB91_B158_40E3_9695_624D1BB0CCBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysVar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSysVar dialog
class SysVarAll;
class SysVarCG;
class SysVarG;

class CDlgSysVar : public CDialog
{
// Construction
public:
	CDlgSysVar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSysVar)
	enum { IDD = IDD_DIALOG_SYSVAR };
	CListCtrl	m_List2;
	CListCtrl	m_List;
	CTreeCtrl	m_ctlTree;
	//}}AFX_DATA
private:
	SysVarAll *m_SysVarAll;
	CCalcModel *m_pModel;
	CString m_strDBName;
	int m_iUnit;
	int m_iSet;

	HICON m_hIcon;

    HTREEITEM m_hRoot;//根节点
	CImageList m_ImageList;//图像列表
public:
	CArray<int,int> m_iCGArr;
	CStringArray m_strVarNameArr;
	CStringArray m_strVarDespArr;
	CStringArray m_strVarUnitArr;

public:
	void SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet);
	void InitTree();
	void InitListBoxHead();
	void InitListBox2Head();
	void InitListBoxBodyByAllVar();
	void InitListBoxBodyByCG(SysVarCG *pRSysVarCG);
	void InitListBoxBodyByG(SysVarG *pRSysVarG);
private:
	BOOL beExistInSel(CString strVarName);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysVar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysVar)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangedTreeGroup(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnDblclkListVar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListSelvar(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSVAR_H__7A39DB91_B158_40E3_9695_624D1BB0CCBB__INCLUDED_)
