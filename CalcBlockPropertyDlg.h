#if !defined(AFX_CALCBLOCKPROPERTYDLG_H__11B47F96_0F71_4D50_ACA2_82CDEA0A4885__INCLUDED_)
#define AFX_CALCBLOCKPROPERTYDLG_H__11B47F96_0F71_4D50_ACA2_82CDEA0A4885__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcBlockPropertyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcBlockPropertyDlg dialog

class CCalcBlockPropertyDlg : public CDialog
{
// Construction
public:
	//CCalcBlockPropertyDlg(CWnd* pParent = NULL);   // standard constructor
	CCalcBlockPropertyDlg( UINT res, CWnd* parent );
	BOOL Create( UINT nIDTemplate, CWnd* pParentWnd );

	virtual CWnd*	GetRedrawWnd();
	virtual void	SetRedrawWnd( CWnd* redrawWnd );
	virtual void	Redraw();

private:
	CWnd*			m_redrawWnd;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCBLOCKPROPERTYDLG_H__11B47F96_0F71_4D50_ACA2_82CDEA0A4885__INCLUDED_)
