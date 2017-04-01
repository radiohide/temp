/* ==========================================================================
	CFlowchartEntityBox

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CFlowchartEntityBox, derived from CFlowchartEntity, 
					represents a simple linkable box. The box can link 
					from all four sides.	

	Description :	The class implements the necessary functions for 
					cloning and creation from text (Clone and 
					CreateFromString). It draws a rectangle using the 
					title attribute of CDiagramEntity as the text 
					contents. 

	Usage :			Create with CFlowchartControlFactory::CreateFromString

   ========================================================================*/

#include "stdafx.h"
#include "FlowchartEntityBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFlowchartEntityBox::CFlowchartEntityBox()
/* ============================================================
	Function :		CFlowchartEntityBox::CFlowchartEntityBox
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetConstraints( CSize( 60, 80 ), CSize( -1, -1 ) );
	SetType( _T( "flowchart_box" ) );
	CString title;
	title= "flowbox" ;
	SetTitle( title );

}

CFlowchartEntityBox::~CFlowchartEntityBox()
/* ============================================================
	Function :		CFlowchartEntityBox::~CFlowchartEntityBox
	Description :	destructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

void CFlowchartEntityBox::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CFlowchartEntityBox::Draw
	Description :	Draws the object.

	Return :		void
	Parameters :	CDC* dc		-	The CDC to draw to. 
					CRect rect	-	The real rectangle of the 
									object.

	Usage :			The function should clean up all selected 
					objects. Note that the CDC is a memory CDC, 
					so creating a memory CDC in this function 
					will probably not speed up the function.

   ============================================================*/
{
   //计算描述文字的位置r
	int cut = round( ( double ) GetMarkerSize().cx * GetZoom() );
	CRect r( rect );
	r.top = r.bottom - round( cut* 2);
    //选择画笔、刷子、背景
	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );
	int mode = dc->SetBkMode( TRANSPARENT );
    //画外框
	dc->Rectangle( rect );
	////////////////////////////////////////////////////////
	//画输入输出，计算输出输出的位置
	CRect rectSelect;
	CBrush greenBrush;
	greenBrush.CreateSolidBrush( RGB(255, 0, 0 ) );
	dc->SelectObject( &greenBrush );
    int InNum = GetInputNum();
	int OutNum =GetOutputNum();
	for(int k=0;k<InNum;k++) //画输入
	{
		AfxMessageBox("b");
          rectSelect = GetPortMarkerRect(DEHT_INPUT,rect,InNum,k); //获得第几个输入端子
	      dc->Rectangle( rectSelect );
	}
	for( k=0;k<OutNum;k++) //画输入
	{
		AfxMessageBox("a");
          rectSelect = GetPortMarkerRect(DEHT_OUTPUT,rect,OutNum,k); //获得第几个输入端子
	      dc->Ellipse( rectSelect );
	}

	dc->SelectStockObject( BLACK_BRUSH );

	///////////////////////////////////////////////////////

	CFont font;
	font.CreateFont( -round( 12.0 * GetZoom() ), 0,0,0,FW_NORMAL,0,0,0,0,0,0,0,0, _T( "Courier New" ) );
	dc->SelectObject( &font );

	dc->DrawText( GetTitle(), r, DT_NOPREFIX | DT_WORDBREAK | DT_CENTER );

	dc->SelectStockObject( DEFAULT_GUI_FONT );
	dc->SetBkMode( mode );

}

CDiagramEntity* CFlowchartEntityBox::Clone()
/* ============================================================
	Function :		CFlowchartEntityBox::Clone
	Description :	Clone this object to a new object.
					
	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The 
					caller will have to delete the object.

   ============================================================*/
{

	CFlowchartEntityBox* obj = new CFlowchartEntityBox;
	obj->Copy( this );
	return obj;

}

CDiagramEntity* CFlowchartEntityBox::CreateFromString( const CString& str )
/* ============================================================
	Function :		CFlowchartEntityBox::CreateFromString
	Description :	Static factory function that creates and 
					returns an instance of this class if str 
					is a valid representation.
					
	Return :		CDiagramEntity*		-	The object, or NULL 
											if str is not a 
											representation of 
											this type.
	Parameters :	const CString& str	-	The string to create 
											from.
					
	Usage :			Can be used as a factory for text file loads. 
					Each object type should have its own 
					version - the default one is a model 
					implementation.

   ============================================================*/
{

	CFlowchartEntityBox* obj = new CFlowchartEntityBox;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}
void CFlowchartEntityBox::DrawSelectionMarkers( CDC* dc, CRect rect ) const
/* ============================================================
	Function :		CFlowchartEntity::DrawSelectionMarkers
	Description :	Draws the selection markers for the 
					object.
					
	Return :		void
	Parameters :	CDC* dc		-	The CDC to draw to
					CRect rect	-	The real object rectangle.
					
	Usage :			rect is the true rectangle (zoomed) of the 
					object.

   ============================================================*/
{
	CRect rectSelect;

	dc->SelectStockObject( BLACK_BRUSH );

	rectSelect = GetSelectionMarkerRect( DEHT_TOPLEFT, rect );
	dc->Rectangle( rectSelect );


	rectSelect = GetSelectionMarkerRect( DEHT_TOPRIGHT, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMLEFT, rect );
	dc->Rectangle( rectSelect );


	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMRIGHT, rect );
	dc->Rectangle( rectSelect );
}

int CFlowchartEntityBox::GetHitCode( const CPoint& point, const CRect& rect ) const
/* ============================================================
	Function :		CFlowchartEntity::GetHitCode
	Description :	Returns the hit point constant for point.
					
	Return :		int				-	The hit point, 
										DEHT_NONE if none.
	Parameters :	CPoint point	-	The point to check
					
	Usage :			Call to see in what part of the object point 
					lies.

   ============================================================*/
{
   	
	int result = DEHT_NONE;

	if( rect.PtInRect( point ) )
		result = DEHT_BODY;

	CRect rectTest;

	rectTest = GetSelectionMarkerRect( DEHT_TOPLEFT, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_TOPLEFT;

	rectTest = GetSelectionMarkerRect( DEHT_TOPRIGHT, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_TOPRIGHT;

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMLEFT, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_BOTTOMLEFT;

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMRIGHT, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_BOTTOMRIGHT;
	////////////////////////////////////////////////////////////////////////////
	//判断在哪个输入还是输出中
    int InNum = GetInputNum();
	int OutNum =GetOutputNum();
	for(int k=0;k<InNum;k++) //画输入
	{
          rectTest = GetPortMarkerRect(DEHT_INPUT,rect,InNum,k); //获得第几个输入端子
		  if( rectTest.PtInRect( point ) )
		  {
			result = DEHT_INPUT;
			break;
		  }
	}
	for( k=0;k<OutNum;k++) //画输入
	{
          rectTest = GetPortMarkerRect(DEHT_OUTPUT,rect,OutNum,k); //获得第几个输入端子
		   if( rectTest.PtInRect( point ) )
		  {
			result = DEHT_OUTPUT;
			break;
		  }
	}
 
	return result;


}

HCURSOR CFlowchartEntityBox::GetCursor( int hit ) const
{
	HCURSOR cursor = NULL;
	switch( hit )
	{
		case DEHT_BODY:
			cursor = LoadCursor( NULL, IDC_SIZEALL );
		break;
		case DEHT_TOPLEFT:
			cursor = LoadCursor( NULL, IDC_SIZENWSE );
		break;
		
		case DEHT_TOPRIGHT:
			cursor = LoadCursor( NULL, IDC_SIZENESW );
		break;
		case DEHT_BOTTOMLEFT:
			cursor = LoadCursor( NULL, IDC_SIZENESW );
		break;
	
		case DEHT_BOTTOMRIGHT:
			cursor = LoadCursor( NULL, IDC_SIZENWSE );
		break;
        ///////////////////////////////////////////////////////
		//输入输出端口
		case DEHT_INPUT:
				cursor = LoadCursor(NULL, IDC_SIZEWE );
		break;
		case DEHT_OUTPUT:
				cursor = LoadCursor(NULL, IDC_SIZEALL );
		break;
	
	}
	return cursor;
}

