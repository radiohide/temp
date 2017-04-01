#if !defined(AFX_FRMVIEWDESIGNDATA_H__88B10707_918D_4705_9689_21AC7FDF57F8__INCLUDED_)
#define AFX_FRMVIEWDESIGNDATA_H__88B10707_918D_4705_9689_21AC7FDF57F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrmViewDesignData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrmViewDesignData form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFrmViewDesignData : public CFormView
{
protected:
	CFrmViewDesignData();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFrmViewDesignData)

// Form Data
public:
	//{{AFX_DATA(CFrmViewDesignData)
	enum { IDD = IDD_FORMVIEW1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrmViewDesignData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFrmViewDesignData();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFrmViewDesignData)
//	afx_msg void OnCfgAddBlock();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRMVIEWDESIGNDATA_H__88B10707_918D_4705_9689_21AC7FDF57F8__INCLUDED_)
