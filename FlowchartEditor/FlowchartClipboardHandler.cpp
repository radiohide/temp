/* ==========================================================================
	CFlowchartClipboardHandler

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-30

	Purpose :		CFlowchartClipboardHandler is a clipboard handler that 
					includes links in the clipboard.	

	Description :	Links are saved and loaded to/from  a separate 
					CObArray.

	Usage :			Declare an instance of CFlowchartClipboardHandler in 
					your document (for a SDI-app), dialog class (for a 
					CDialog-derived app) or centrally for a MDI-application. 
					Use CDiagramEntityContainer::SetClipboardHandler to 
					attach it to application data. If you are not using 
					an external CDiagramEntityContainer, but let editor 
					create the data internally, call 
					CDiagramEditor::GetGetDiagramEntityContainer()->SetClipboardHandler 
					after the CDiagramEditor::Create-call.

   ========================================================================*/
#include "stdafx.h"
#include "FlowchartClipboardHandler.h"
//#include "LinkFactory.h"
#include "DiagramEditor/GroupFactory.h"
#include "DiagramEditor/DiagramEntityContainer.h"
#include "FlowchartEntityContainer.h"
//#include "FlowchartLink.h"
#include "FlowchartEntity.h"

CFlowchartClipboardHandler::CFlowchartClipboardHandler()
/* ============================================================
	Function :		CFlowchartClipboardHandler::CFlowchartClipboardHandler
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

CFlowchartClipboardHandler::~CFlowchartClipboardHandler()
/* ============================================================
	Function :		CFlowchartClipboardHandler::~CFlowchartClipboardHandler
	Description :	destructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	ClearPaste();

}

void CFlowchartClipboardHandler::Copy( CDiagramEntity* obj )
/* ============================================================
	Function :		CFlowchartClipboardHandler::Copy
	Description :	Copies obj to the paste array
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	Object to copy.
					
	Usage :			Overridden to assign a new id.

   ============================================================*/
{

	if( obj )
	{
		ClearPaste();
		CFlowchartEntity* FlowObj = 	(CFlowchartEntity*)obj;
		CFlowchartEntity* FlowNewObj =	(CFlowchartEntity*) FlowObj->Clone();

		FlowNewObj->Select( TRUE );
		FlowNewObj->MoveRect( 10, 10 );
	
	  CFlowchartEntityContainer* pContainer = (CFlowchartEntityContainer*)FlowNewObj->GetParent();
	  CString str = pContainer->MakeSuitableName(FlowObj->GetBlockName());
//	  AfxMessageBox(str);
	  FlowNewObj->SetBlockName(str);

		GetData()->Add( FlowNewObj );
	}

}

void CFlowchartClipboardHandler::Paste( CDiagramEntityContainer* container )
/* ============================================================
	Function :		CFlowchartClipboardHandler::Paste
	Description :	Pastes the contents of the paste array to 
					the data array.
					
	Return :		void
	Parameters :	none

	Usage :			Overridden to paste links as well.

   ============================================================*/
{
    //
	CDWordArray	oldgroup;
	CDWordArray	newgroup;

	int max = GetData()->GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CFlowchartEntity* obj = (CFlowchartEntity*)( GetData()->GetAt( t ) );
		if( obj->GetGroup() )
		{
			int size = oldgroup.GetSize();
			BOOL found = FALSE;
			for( int i = 0 ; i < size ; i++ )
				if( obj->GetGroup() == static_cast< int > ( oldgroup[ i ] ) )
					found = TRUE;

			if( !found )
			{
				oldgroup.Add( obj->GetGroup() );
				newgroup.Add( CGroupFactory::GetNewGroup() );
			}
		}
	}

	for( t = 0 ; t < max ; t++ )
	{
		CFlowchartEntity* obj = (CFlowchartEntity*)( GetData()->GetAt( t ) );
		CFlowchartEntity* clone =(CFlowchartEntity*) obj->Clone();

	    CFlowchartEntityContainer* pContainer = (CFlowchartEntityContainer*)clone->GetParent();
	    CString str = pContainer->MakeSuitableName(clone->GetBlockName());//ÖØÃüÃû
	   // AfxMessageBox(str);
	    clone->SetBlockName(str);

		int group = 0;
		if( obj->GetGroup() )
		{
			int size = oldgroup.GetSize();
			for( int i = 0 ; i < size ; i++ )
				if( obj->GetGroup() == static_cast< int >( oldgroup[ i ] ) )
					group = newgroup[ i ];
		}

		clone->SetGroup( group );
		container->Add( clone );
	}

	//CDiagramClipboardHandler::Paste( container );

}

void CFlowchartClipboardHandler::CopyAllSelected( CDiagramEntityContainer* container )
/* ============================================================
	Function :		CFlowchartClipboardHandler::CopyAllSelected
	Description :	Copies all the selected items to the paste 
					array.
					
	Return :		void
	Parameters :	none

	Usage :			Overridden to add links as well. New ids 
					are assigned to the copied objects, and the 
					copied links are updated.

   ============================================================*/
{

	CDiagramClipboardHandler::CopyAllSelected( container );


}

void CFlowchartClipboardHandler::ClearPaste()
/* ============================================================
	Function :		CFlowchartClipboardHandler::ClearPaste
	Description :	Clears the paste array.
					
	Return :		void
	Parameters :	none

	Usage :			Overridden to clear the paste link array as 
					well.

   ============================================================*/
{

	CDiagramClipboardHandler::ClearPaste();



}

