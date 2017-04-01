#if !defined(AFX_DLGMODERUNCTRL_H__3958F334_C95E_41A7_AB0C_845B6BC2B743__INCLUDED_)
#define AFX_DLGMODERUNCTRL_H__3958F334_C95E_41A7_AB0C_845B6BC2B743__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgModeRunCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgModeRunCtrl dialog
#include "DlgMCtrlDay.h"
#include "DlgMCtrlCycle.h"
#include "DlgMCtrlDuty.h"
#include "DlgMCtrlMonth.h"
#include "DlgMCtrlWeek.h"
#include "DlgMCtrlType.h"
#include "ModelCalcControl.h"
class CDlgModeRunCtrl : public CDialog
{
// Construction
public:
	CDlgModeRunCtrl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgModeRunCtrl)
	enum { IDD = IDD_DIALOG_MODERUN_CONCTRL };
	CComboBox	m_ComBoTaskType;
	//}}AFX_DATA
public:
	CArray<CDlgMCtrlType*,CDlgMCtrlType*&> dlgAllTypeArr;
	ModelCalcControl *m_pMCalControl;
public:
	void SetModelCalcControl(ModelCalcControl *pMCalControl);
private:
	void showMCtrlDlgByIndex(int index);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgModeRunCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgModeRunCtrl)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboTasktype();
	afx_msg void OnClose();
	afx_msg void OnButtonSetRunABSTime();
	afx_msg void OnButtonSetRunDataTime();
	afx_msg void OnButtonApply();
	afx_msg void OnButtonOk();
	afx_msg void OnButtonCancle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMODERUNCTRL_H__3958F334_C95E_41A7_AB0C_845B6BC2B743__INCLUDED_)
