#if !defined(AFX_VIEWWSPACEFRM_H__0CEF2D95_ED59_4BE1_B6D8_6B418E442CFB__INCLUDED_)
#define AFX_VIEWWSPACEFRM_H__0CEF2D95_ED59_4BE1_B6D8_6B418E442CFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewWSpaceFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewWSpaceFrm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "GridCtrl.h"
#include "ViewUserFrm.h"
class CViewWSpaceFrm : public CViewUserFrm
{
protected:
	CViewWSpaceFrm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewWSpaceFrm)

// Form Data
public:
	//{{AFX_DATA(CViewWSpaceFrm)
	enum { IDD = IDD_FORMVIEW_WORKSPACE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	COLORREF Color;
	COLORREF BKColor;
	COLORREF FKColor;
	
	const int m_rowHeight;//行高
	int m_CurrRowNum;
	int m_CurrColNum;
	CGridCtrl m_Grid;
	CImageList m_ImageList;

	int TagType;//1：实测点；2关系测点；3工作空间
	CStringArray m_ArrBlockRTType;//实时测点类型
	CStringArray m_ArrBlockRType;//关系测点类型
// Attributes
public:
	virtual void RefreshWindow();
private:
	int getBlockTypeByPort(CCalcPort* pPort);
// Operations
public:
	void FillGCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewWSpaceFrm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewWSpaceFrm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewWSpaceFrm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWWSPACEFRM_H__0CEF2D95_ED59_4BE1_B6D8_6B418E442CFB__INCLUDED_)
