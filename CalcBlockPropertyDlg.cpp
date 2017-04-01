// CalcBlockPropertyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "CalcBlockPropertyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcBlockPropertyDlg dialog

CCalcBlockPropertyDlg::CCalcBlockPropertyDlg( UINT res, CWnd* parent ) : CDialog( res, parent )
{
	 m_redrawWnd = NULL;
}

BOOL CCalcBlockPropertyDlg::Create( UINT nIDTemplate, CWnd* pParentWnd )
/* ============================================================
	Function :		CCalcBlockPropertyDlg::Create
	Description :	Creates the dialog.
	Access :		Public

	Return :		BOOL				-	"TRUE" if window was 
											created ok.
	Parameters :	UINT nIDTemplate	-	Resource id of 
											dialog template.
					CWnd* pParentWnd	-	Parent of dialog 
											(normally the 
											"CDiagramEditor")
					
	Usage :			Called internally to create the property 
					dialog.

   ============================================================*/
{

	BOOL result;

	result = CDialog::Create( nIDTemplate, pParentWnd );
	SetRedrawWnd( pParentWnd );

	return result;

}

CWnd* CCalcBlockPropertyDlg::GetRedrawWnd() 
/* ============================================================
	Function :		CCalcBlockPropertyDlg::GetRedrawWnd
	Description :	Get the window that should be redrawn when 
					changes are made in this dialog.
	Access :		Public

	Return :		CWnd*	-	The window
	Parameters :	none

	Usage :			Call to get the window that should be 
					redrawn when applying changes in the box. 
					This member is used as the editor will not 
					redraw properly in a MDI-application 
					("GetParent()" returns the frame instead of 
					the editor).

   ============================================================*/
{ 

	return m_redrawWnd; 

}

void CCalcBlockPropertyDlg::SetRedrawWnd( CWnd* redrawWnd ) 
/* ============================================================
	Function :		CCalcBlockPropertyDlg::SetRedrawWnd
	Description :	Set the window that should be redrawn in 
					response to changes in this dialog.
	Access :		Public

	Return :		void
	Parameters :	CWnd* redrawWnd	-	Normally the editor.
					
	Usage :			Call to set the window that should be 
					redrawn when applying changes in the box. 
					This member is used as the editor will not 
					redraw properly in a MDI-application 
					("GetParent()" returns the frame instead of 
					the editor).

   ============================================================*/
{ 

	m_redrawWnd = redrawWnd; 

}

void CCalcBlockPropertyDlg::Redraw() 
/* ============================================================
	Function :		CCalcBlockPropertyDlg::Redraw
	Description :	Redraw the parent window of the dialog.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to redraw the editor window when 
					applying changes in the box. 
					This member is used as the editor will not 
					redraw properly in a MDI-application 
					("GetParent()" returns the frame instead of 
					the editor).


   ============================================================*/
{ 

	CWnd* wnd = GetRedrawWnd();
	if( wnd )
		wnd->RedrawWindow();

}
