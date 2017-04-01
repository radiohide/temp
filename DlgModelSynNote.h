#if !defined(AFX_DLGMODELSYNNOTE_H__15C4584C_804E_49AF_ADA8_3D2D2C9DE6F8__INCLUDED_)
#define AFX_DLGMODELSYNNOTE_H__15C4584C_804E_49AF_ADA8_3D2D2C9DE6F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgModelSynNote.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgModelSynNote dialog

class CDlgModelSynNote : public CDialog
{
// Construction
public:
	CDlgModelSynNote(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgModelSynNote)
	enum { IDD = IDD_DIALOG_MODELSYN_NOTE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	int m_iTag;//2:同步并上传，1:只同步，0：取消

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgModelSynNote)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgModelSynNote)
	afx_msg void OnButtonUpRestar();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonCancle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMODELSYNNOTE_H__15C4584C_804E_49AF_ADA8_3D2D2C9DE6F8__INCLUDED_)
