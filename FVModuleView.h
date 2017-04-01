#if !defined(AFX_FVMODULEVIEW_H__DD6C65C5_A294_4672_8384_D41CD74F32BE__INCLUDED_)
#define AFX_FVMODULEVIEW_H__DD6C65C5_A294_4672_8384_D41CD74F32BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FVModuleView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFVModuleView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMyBlockListCtrl : public CBCGPListCtrl
{
	virtual COLORREF OnGetCellTextColor (int nRow, int nColum);
	virtual COLORREF OnGetCellBkColor (int nRow, int nColum);
	virtual HFONT OnGetCellFont (int nRow, int nColum, DWORD dwData = 0);

public:
	BOOL	m_bColor;
	BOOL	m_bModifyFont;
};


#include "MyList.h"
#include "BlockGridCtrl.h"

class CCalcBlock;
class CEPerfManageDoc;

class CFVModuleView : public CFormView
{
protected:
	CFVModuleView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFVModuleView)
public:
	CEPerfManageDoc* GetDocument();
	///////////////////////////////
	CString m_strModuleName; //该view显示的模块名称
	CCalcBlock* m_pSelBlock;
// Form Data
public:
	//{{AFX_DATA(CFVModuleView)
	enum { IDD = IDD_MODULE_VIEW };
	//CStatic	m_wndGridLocation;
	//}}AFX_DATA
protected:
	CMyBlockListCtrl	m_lstBlocks;
	//CStatic	m_wndPropListLocation;
	//CBlockGridCtrl m_GridCtrl;
// Attributes
	CImageList	   m_BlockImages;
public:

// Operations
public:
	void ShowInOutPage(CCalcBlock* pBlock);
	void UpdateBlockList();
	void UpdateBlockProp(CString strBlockName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFVModuleView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL
    afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
// Implementation
protected:
	virtual ~CFVModuleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFVModuleView)
	afx_msg void OnClickListBlock(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#ifndef _DEBUG  // debug version in BCGPOutlookDemoView.cpp
inline CEPerfManageDoc* CFVModuleView::GetDocument()
   { return (CEPerfManageDoc*)m_pDocument; }
#endif
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FVMODULEVIEW_H__DD6C65C5_A294_4672_8384_D41CD74F32BE__INCLUDED_)
