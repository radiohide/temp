/* ==========================================================================
	CFlowchartEntity

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CFlowchartEntity is a base class for linked objects used 
					by a CDiagramEditor.	

	Description :	The class contains a few virtual functions that derived 
					classes can implement:
					AllowLink and GetLinkPosition. AllowLink returns what 
					link types are allowed, one or more of the following 
					ORed together:
					LINK_TOP		Links are allowed to the 
									top of the object.
					LINK_BOTTOM		Links are allowed to the 
									bottom.
					LINK_LEFT		Links are allowed to the 
									left.
					LINK_RIGHT		Links are allowed to the 
									right.
					LINK_START		Links are allowed to the 
									start of a line (normally 
									the top-left corner of 
									the non-normalized bounding 
									rect).
					LINK_END		Links are allowed to the 
									end of a line (normally the
									bottom-right corner of the
									non-normalized bounding 
									rect).

					GetLinkPosition will return the position of a link type. 
					The coordinates are not zoomed, so if they should be 
					used for drawing, they must be multiplied with the 
					current zoom factor of the editor.

	Usage :			Use this class as a base class for linked objects.

   ========================================================================*/
#include "stdafx.h"
#include "FlowchartEntity.h"
#include "DiagramEditor/Tokenizer.h"
#include "FlowchartEntityContainer.h"

#include "..\CalcBlockControlFactory.h"
#include "..\DlgCalcOrder.h"
#include "..\DlgBlockName.h"
#include "..\CalcModule.h"
#include "..\CalcModel.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CLog g_Log;
CFlowchartEntity::CFlowchartEntity(CString strType)
{
	// Setting fixed size
	SetConstraints( CSize( 30, 35 ), CSize(-1, -1 ) );
	SetType( _T( "flowchart_entity" ) );
	SetMoved( TRUE );

	//SetPropertyDialog( &m_dlg, CDlgPropRTPoint::IDD );

	m_FocusPortID = -1;
	m_bFocusPort = FALSE;
	//当生成一个对象的时候，就建立一个对应的模块
	m_pBlockInner = NULL;
	m_pBlockInner = CCalcBlockControlFactory::CreateFromString(strType);

	SetBlockType(strType);

}
CFlowchartEntity::~CFlowchartEntity()
/* ============================================================
	Function :		CFlowchartEntity::~CFlowchartEntity
	Description :	destructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
	if(m_pBlockInner) delete m_pBlockInner;
}

CDiagramEntity* CFlowchartEntity::Clone()
/* ============================================================
	Function :		CFlowchartEntity::Clone
	Description :	Clone this object to a new object.
					
	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The 
					caller will have to delete the object.

   ============================================================*/
{

	CFlowchartEntity* obj = new CFlowchartEntity(m_strBlockType);
	obj->Copy( this );
	return obj;

}
void CFlowchartEntity::Copy( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntity::Copy
	Description :	Copy the information in "obj" to this object.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to copy 
											from.
					
	Usage :			Copies basic information. from "obj" to this.
					"GetType" can be used to check for the correct 
					object type in overridden versions.
   ============================================================*/
{

	CDiagramEntity::Copy(obj);//设置基本属性
	//设置和
	CFlowchartEntity* pEntity = static_cast< CFlowchartEntity* >( obj );
	m_strBlockType = pEntity->m_strBlockType;
	if(m_pBlockInner->GetBlockType() == pEntity->m_pBlockInner->GetBlockType() )
	{
		m_pBlockInner->Copy(pEntity->m_pBlockInner);
	}
}

CDiagramEntity* CFlowchartEntity::CreateFromString( const CString& str )
/* ============================================================
	Function :		CFlowchartEntity::CreateFromString
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

	CFlowchartEntity* obj = new CFlowchartEntity(str);
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}


void CFlowchartEntity::DrawSelectionMarkers( CDC* dc, CRect rect ) const
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

	dc->SelectStockObject( BLACK_PEN );
	CBrush greenBrush;
	greenBrush.CreateSolidBrush( RGB(0, 255, 0 ) );
	dc->SelectObject( &greenBrush );


	rectSelect = GetSelectionMarkerRect( DEHT_TOPLEFT, rect );
	dc->Rectangle( rectSelect );


	rectSelect = GetSelectionMarkerRect( DEHT_TOPRIGHT, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMLEFT, rect );
	dc->Rectangle( rectSelect );


	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMRIGHT, rect );
	dc->Rectangle( rectSelect );

	dc->SelectStockObject( BLACK_BRUSH );
	

}

HCURSOR CFlowchartEntity::GetCursor( int hit ) const
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
int CFlowchartEntity::GetHitCode( CPoint point ) const
{

	CRect rect = GetRect();
	return GetHitCode( point, rect );

}
int CFlowchartEntity::GetHitCode( const CPoint& point, const CRect& rect ) const
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
	{
		result = DEHT_TOPLEFT;
		return result;
	}

	rectTest = GetSelectionMarkerRect( DEHT_TOPRIGHT, rect );
	if( rectTest.PtInRect( point ) )
	{	result = DEHT_TOPRIGHT;	return result;}

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMLEFT, rect );
	if( rectTest.PtInRect( point ) )
	{result = DEHT_BOTTOMLEFT;	return result;}

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMRIGHT, rect );
	if( rectTest.PtInRect( point ) )
	{result = DEHT_BOTTOMRIGHT;	return result;}

	rectTest = GetBlockNameRect(rect);
	if( rectTest.PtInRect( point ) )
	{result = DEHT_BLOCKNAME;	return result;}

	rectTest = GetBlockDespRect(rect);
	if( rectTest.PtInRect( point ) )
	{result = DEHT_BLOCKDESP;	return result;}

	rectTest = GetBlockOrderRect(rect);
	if( rectTest.PtInRect( point ) )
	{result = DEHT_BLOCKORDER;	return result;}

	//
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
//得到点击的类型和端口
 int    CFlowchartEntity::GetHitCodeExt( const CPoint& point, const CRect& rect,int* portID ) const
 {
	 int result = DEHT_NONE;

	if( rect.PtInRect( point ) )
		result = DEHT_BODY;
	//else
	//	return result;

	//继续判断到底点击点在块的哪个部位
	CRect rectTest;

	rectTest = GetBlockNameRect(rect);
	if( rectTest.PtInRect( point ) )
	{result = DEHT_BLOCKNAME;	return result;}

	rectTest = GetBlockDespRect(rect);
	if( rectTest.PtInRect( point ) )
	{result = DEHT_BLOCKDESP;	return result;}

	rectTest = GetBlockOrderRect(rect);
	if( rectTest.PtInRect( point ) )
	{result = DEHT_BLOCKORDER;	return result;}

	  //判断其他的部位
	rectTest = GetSelectionMarkerRect( DEHT_TOPLEFT, rect );
	if( rectTest.PtInRect( point ) )
	{	result = DEHT_TOPLEFT;return result;}

	rectTest = GetSelectionMarkerRect( DEHT_TOPRIGHT, rect );
	if( rectTest.PtInRect( point ) )
	{	result = DEHT_TOPRIGHT;return result;}

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMLEFT, rect );
	if( rectTest.PtInRect( point ) )
	{	result = DEHT_BOTTOMLEFT;return result;}

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMRIGHT, rect );
	if( rectTest.PtInRect( point ) )
	{	result = DEHT_BOTTOMRIGHT; return result;}
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
			 (*portID) = k;
			 break;
		  }
	}
	if(result != DEHT_INPUT) 
	{
		for( k=0;k<OutNum;k++) //输出
		{
			  rectTest = GetPortMarkerRect(DEHT_OUTPUT,rect,OutNum,k); //获得第几个输入端子
			   if( rectTest.PtInRect( point ) )
			  {
				result = DEHT_OUTPUT;
				 (*portID) = k;
				break;
			  }
		}
	}

  
 
	return result;
 }
 //获得名字所在的区间
 CRect   CFlowchartEntity::GetBlockNameRect(CRect rect) const
 {
	int cut = round( ( double ) GetMarkerSize().cx * GetZoom() );
	CRect r( rect );
	r.top = r.bottom - round( cut* 2);
	return  r;
 }
  //获得描述的矩形区域,设置在顶部右侧
CRect   CFlowchartEntity::GetBlockDespRect(CRect rect) const
{
	int cut = round( ( double ) GetMarkerSize().cx * GetZoom() );
	CRect rc( rect );
	rc.left  =  rect.left + int(double(rect.Width())*0.3);
	rc.bottom  = rect.top+round( cut* 2);
	return  rc;
}
 //获得计算顺序矩形区域
CRect   CFlowchartEntity::GetBlockOrderRect(CRect rect) const
{
	int cut = round( ( double ) GetMarkerSize().cx * GetZoom() );
	CRect rc( rect );
	rc.right =  rect.left + int(double(rect.Width())*0.3);
	rc.bottom  = rect.top+round( cut* 2);
	return  rc;
}
 CRect   CFlowchartEntity::GetPortMarkerRect(UINT marker,CRect rect,int PortNum,int portID) const
{
	CRect rectMarker;
	int horz =  int(GetMarkerSize().cx* GetZoom() / 2);
	int vert =  int(GetMarkerSize().cy* GetZoom() / 2);
	//计算中心位置点
	double  DivCy = double(rect.Height())/double((PortNum+1));
	int  DeltaY = round(DivCy*double(portID+1));
	switch( marker )
	{
		case DEHT_INPUT:
			rectMarker.SetRect( rect.left - horz,
								rect.top +DeltaY - vert,
								rect.left + horz,
								rect.top + DeltaY+ vert );
		break;

		case DEHT_OUTPUT:
			rectMarker.SetRect( rect.right - horz,
								rect.top +DeltaY - vert,
								rect.right + horz,
								rect.top + DeltaY+ vert );
		break;
	}

	return rectMarker;
}
//获得端口中心点
CPoint  CFlowchartEntity::GetPortPoint(UINT marker,CRect rect,int PortNum,int portID) const
{
	CPoint point(0,0);
	int horz =  GetMarkerSize().cx / 2;
	int vert =  GetMarkerSize().cy / 2;
	//计算中心位置点
	double  DivCy = double(rect.Height())/double((PortNum+1));
	int  DeltaY = round(DivCy*double(portID+1));

	switch( marker )
	{
		case DEHT_INPUT:
			point.x = rect.left;
			point.y = rect.top + DeltaY;
		break;

		case DEHT_OUTPUT:
			point.x = rect.right;
			point.y = rect.top + DeltaY;
		break;
	}

	return point;
}

CPoint   CFlowchartEntity::GetPortPointByName(UINT marker,CString strPortName) const
{
	CPoint pt(-1,-1);
	   //找到第几个
	    int portID=-1;
		if(marker == DEHT_INPUT)
		{
				portID =FindInPortIdByName(strPortName);
				if(portID!=-1)
				{
					pt = GetPortPoint(marker,GetRect(),GetInputNum(),portID);

				}
		}
		else if(marker == DEHT_OUTPUT)
		{
				portID =FindOutPortIdByName(strPortName);
				if(portID!=-1)
				{
					pt = GetPortPoint(marker,GetRect(),GetOutputNum(),portID);

				}
		}
		return pt;
	
}
 //获得输入输出的文字位置
 CRect   CFlowchartEntity::GetPortTextRect(UINT marker,CRect rect,int PortNum,int portID)
 {
	 	CRect rectMarker;

	int horz =  GetMarkerSize().cx;
	int vert =  GetMarkerSize().cy;
	int wid =  rect.Width()-horz;
	//计算中心位置点
	double  DivCy = double(rect.Height())/double((PortNum+1));
	int  DeltaY = round(DivCy*double(portID+1));
	switch( marker )
	{
		case DEHT_INPUT:
			rectMarker.SetRect( rect.left + horz,
								rect.top +DeltaY - vert,
								rect.left + wid,
								rect.top + DeltaY+ vert );
		break;

		case DEHT_OUTPUT:
			rectMarker.SetRect( rect.right - wid,
								rect.top +DeltaY - vert,
								rect.right - horz,
								rect.top + DeltaY+ vert );
		break;
	}

	return rectMarker;
	 /*
	CRect rectMarker;

	int horz =  GetMarkerSize().cx;
	int vert =  GetMarkerSize().cy;
	int wid =  rect.Width()/2-horz;
	//计算中心位置点
	double  DivCy = double(rect.Height())/double((PortNum+1));
	int  DeltaY = round(DivCy*double(portID+1));
	switch( marker )
	{
		case DEHT_INPUT:
			rectMarker.SetRect( rect.left + horz,
								rect.top +DeltaY - vert,
								rect.left + wid,
								rect.top + DeltaY+ vert );
		break;

		case DEHT_OUTPUT:
			rectMarker.SetRect( rect.right - wid,
								rect.top +DeltaY - vert,
								rect.right - horz,
								rect.top + DeltaY+ vert );
		break;
	}

	return rectMarker;*/
 }
 
void CFlowchartEntity::SetMoved( BOOL moved )
/* ============================================================
	Function :		CFlowchartEntity::SetMoved
	Description :	Sets the moved-flag of the object.
					
	Return :		void
	Parameters :	BOOL moved	-	TRUE if the object is moved 
									while moving linked objects, 
									FALSE if not.
					
	Usage :			To avoid infinite recursion while moving 
					linked objects, we use a flag to mark this 
					object as already moved.

   ============================================================*/
{

	m_moved = moved;

}

BOOL CFlowchartEntity::GetMoved()
/* ============================================================
	Function :		CFlowchartEntity::GetMoved
	Description :	Gets the moved-flag of the object.
					
	Return :		BOOL	-	TRUE if the object is moved 
									while moving linked objects, 
									FALSE if not.
					
	Parameters :	none

	Usage :			To avoid infinite recursion while moving 
					linked objects, we use a flag to mark this 
					object as already moved.

   ============================================================*/
{

	return m_moved;

}
void CFlowchartEntity::Draw( CDC* dc, CRect rect )
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
   /*-----------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------
   不同的图形采用不同的方法绘制内部，在CALCBLOCK中设计一个函数即可；另外可在头部加描述和块号；
    底部还是名字修改。
  ------------------------------------------------------------------------------------------------------ */
	 //选择画笔、刷子、背景、字体**************************************************************************
	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );
	int mode = dc->SetBkMode( TRANSPARENT );
	CFont font;
	font.CreateFont( -round( 10.0 * GetZoom() ), 0,0,0,FW_NORMAL,0,0,0,0,0,0,0,0, _T( "Arial" ) );
	dc->SelectObject( &font );
	//画图形部分*********************************************************************************************
	CRect rectText;
	//计算一些特征边界数据---------------------
	int cut = round( ( double ) GetMarkerSize().cx * GetZoom() );
	int seg = cut/2;
	//画块特征------------------------------------
	m_pBlockInner->DrawShape(dc,rect,cut);

    //画块序号 [200],在左上角区域----------------
	CFlowchartEntityContainer* pContainer = (CFlowchartEntityContainer*)GetParent();
	//if(pContainer->GetShowOrderMode())
	{
		COLORREF oldtext = dc->GetTextColor();
		dc->SetTextColor( RGB( 255, 0, 0 ) );
		rectText = GetBlockOrderRect(rect);
		dc->Rectangle(rectText);
		CString str;
		str.Format("[%d]",GetCalcOrderID());
		dc->DrawText( str, rectText, DT_NOPREFIX | DT_WORDBREAK | DT_CENTER );
		dc->SetTextColor( oldtext );
	}
	//画块描述----------------------------------------------------------
	//COLORREF oldback = dc->SetBkColor( RGB( 128, 128, 128 ) );
	rectText = GetBlockDespRect(rect);
	dc->Rectangle(rectText.left-1,rectText.top,rectText.right,rectText.bottom );
	dc->DrawText( GetBlockDesp(), rectText, DT_NOPREFIX | DT_WORDBREAK | DT_CENTER );
	//dc->SetBkColor( oldback );
	//画块名称-----------------------------------------------------------
	rectText = GetBlockNameRect(rect);
	dc->DrawText( GetBlockName(),rectText , DT_NOPREFIX | DT_WORDBREAK | DT_VCENTER|DT_CENTER );
	//画输入输出端口-----------------------------------------------------
	int InNum = GetInputNum();
	int OutNum =GetOutputNum();
	POINT pts[ 3 ];
	CRect rectPort;
	for(int k=0;k<InNum;k++) //画输入
	{
		rectPort = GetPortMarkerRect(DEHT_INPUT,rect,InNum,k); //获得第几个输入端子
		
		CPoint topLeftPoint=rectPort.TopLeft( );
		CPoint bottomRightPoint=rectPort.BottomRight();
		int tLX=topLeftPoint.x;
		int tLY=topLeftPoint.y;
		int bRX=bottomRightPoint.x;
		int bRY=bottomRightPoint.y;
		
		CPoint ptPort = GetPortPoint(DEHT_INPUT,GetRect(),InNum,k);
		BOOL bLink = GetBlockPortLinked(k); //检测是否有连接
		int ValueType = GetInputPortValueType(k);
		if(bLink) //存在连接
		{
			dc->SelectStockObject( BLACK_BRUSH );
			//画成三角输入端
			pts[ 0 ].x=ptPort.x;
			pts[ 0 ].y =ptPort.y;
			pts[ 1 ].x = ptPort.x - seg * 2;
			pts[ 1 ].y = ptPort.y - seg;
			pts[ 2 ].x = ptPort.x - seg * 2;
			pts[ 2 ].y = ptPort.y + seg;
			dc->Polygon( pts, 3 );
		}
		else  //不存在连接
		{
			dc->SelectStockObject( WHITE_BRUSH );
			if(ValueType ==VALUE_DOUBLE) 
			{
				dc->Ellipse( rectPort );  //当是数值型时
			}
			else if(ValueType ==VALUE_RTVALUE_SERIES)
			{
				dc->Rectangle(rectPort);  //当是序列值时
			}
			else if(ValueType ==VALUE_STAT)//统计值
			{
				POINT polygonPts[3]={{tLX+(bRX-tLX)/2,tLY},{tLX,bRY},{bRX,bRY}};
				dc->Polygon(polygonPts,3);
			}
			else ////统计值序列
			{
				POINT polygonPts[4]={{tLX+(bRX-tLX)/2,tLY},{tLX,tLY-(tLY-bRY)/2},{tLX+(bRX-tLX)/2,bRY},{bRX,tLY-(tLY-bRY)/2}};
				dc->Polygon(polygonPts,4);
			}
		}
		rectText = GetPortTextRect(DEHT_INPUT,rect,InNum,k);
		if(GetIsShowInPortText())
		{
			dc->DrawText( GetInPortID(k), rectText, DT_NOPREFIX | DT_WORDBREAK | DT_LEFT );
		}
		if(pContainer->GetShowValueMode())
		{
			COLORREF oldtext = dc->GetTextColor();
			dc->SetTextColor( RGB( 0, 0, 255 ) );
			CString str;
			str.Format("%s",GetInPortValue(k));
			rectText.top+=10;rectText.bottom+=10;
			dc->DrawText( str, rectText, DT_NOPREFIX | DT_WORDBREAK | DT_LEFT );
			dc->SetTextColor( oldtext );
		}	  
	}
	//画输出
	dc->SelectStockObject( WHITE_BRUSH );
	CBrush HighLightBrush;
	HighLightBrush.CreateSolidBrush( RGB(255, 0, 0 ) );
	for( k=0;k<OutNum;k++) //画输出端口
	{
		rectPort = GetPortMarkerRect(DEHT_OUTPUT,rect,OutNum,k); //获得第几个输入端子
		
		CPoint topLeftPoint=rectPort.TopLeft( );
		CPoint bottomRightPoint=rectPort.BottomRight();
		int tLX=topLeftPoint.x;
		int tLY=topLeftPoint.y;
		int bRX=bottomRightPoint.x;
		int bRY=bottomRightPoint.y;
		
		int ValueType = GetOutputPortValueType(k);
		if(m_bFocusPort)
		{
			if(k==m_FocusPortID)
			{
				dc->SelectObject( &HighLightBrush ); //选中端子的颜色刷子
				if(ValueType ==VALUE_DOUBLE) 
				{
					dc->Ellipse( rectPort );  //当是数值型时
				}
				else if(ValueType ==VALUE_RTVALUE_SERIES)
				{
					dc->Rectangle(rectPort);  //当是序列值时
				}
				else if(ValueType ==VALUE_STAT)//统计值
				{
					POINT polygonPts[3]={{tLX+(bRX-tLX)/2,tLY},{tLX,bRY},{bRX,bRY}};
					dc->Polygon(polygonPts,3);
				}
				else //统计值序列
				{
					POINT polygonPts[4]={{tLX+(bRX-tLX)/2,tLY},{tLX,tLY-(tLY-bRY)/2},{tLX+(bRX-tLX)/2,bRY},{bRX,tLY-(tLY-bRY)/2}};
					dc->Polygon(polygonPts,4);
				}
				dc->SelectStockObject( WHITE_BRUSH );
			}
			else
			{
				if(ValueType ==VALUE_DOUBLE) 
				{
					dc->Ellipse( rectPort );  //当是数值型时
				}
				else if(ValueType ==VALUE_RTVALUE_SERIES)
				{
					dc->Rectangle(rectPort);  //当是序列值时
				}
				else if(ValueType ==VALUE_STAT)//统计值
				{
					POINT polygonPts[3]={{tLX+(bRX-tLX)/2,tLY},{tLX,bRY},{bRX,bRY}};
					dc->Polygon(polygonPts,3);
				}
				else ////统计值序列
				{
					POINT polygonPts[4]={{tLX+(bRX-tLX)/2,tLY},{tLX,tLY-(tLY-bRY)/2},{tLX+(bRX-tLX)/2,bRY},{bRX,tLY-(tLY-bRY)/2}};
					dc->Polygon(polygonPts,4);
				}
			}
		}
		else
		{
			if(ValueType ==VALUE_DOUBLE) 
			{
				dc->Ellipse( rectPort );  //当是数值型时
			}
			else if(ValueType ==VALUE_RTVALUE_SERIES)
			{
				dc->Rectangle(rectPort);  //当是序列值时
			}
			else if(ValueType ==VALUE_STAT)//统计值
			{
				POINT polygonPts[3]={{tLX+(bRX-tLX)/2,tLY},{tLX,bRY},{bRX,bRY}};
				dc->Polygon(polygonPts,3);
			}
			else ////统计值序列
			{
				POINT polygonPts[4]={{tLX+(bRX-tLX)/2,tLY},{tLX,tLY-(tLY-bRY)/2},{tLX+(bRX-tLX)/2,bRY},{bRX,tLY-(tLY-bRY)/2}};
				dc->Polygon(polygonPts,4);
			}
		}
		rectText = GetPortTextRect(DEHT_OUTPUT,rect,OutNum,k);
		if(GetIsShowOutPortText())
		{
			dc->DrawText( GetOutPortID(k), rectText, DT_NOPREFIX | DT_WORDBREAK | DT_RIGHT );
		}
		//画处于显示数字模式的情况
		if(pContainer->GetShowValueMode())
		{
			COLORREF oldtext = dc->GetTextColor();
			dc->SetTextColor( RGB( 0, 0, 255 ) );
			CString str;
			str.Format("%s",GetOutPortValue(k)); //获得输入端的数据值
			rectText.top+=10;rectText.bottom+=10;
			dc->DrawText( str, rectText, DT_NOPREFIX | DT_WORDBREAK | DT_RIGHT );	
			dc->SetTextColor( oldtext );
		}	  
	}
	//恢复默认画图的方式********************************************************************************
	dc->SelectStockObject( DEFAULT_GUI_FONT );
	dc->SetBkMode( mode );
}

int CFlowchartEntity::GetInputNum() const
{
	if(m_pBlockInner)
		return m_pBlockInner->GetInputNum();
	else 
		return 0;
}

int CFlowchartEntity::GetOutputNum() const
{
	if(m_pBlockInner)
		return m_pBlockInner->GetOutputNum();
	else 
		return 0;
}
CString CFlowchartEntity::GetInPortValue(int k)
{
	if(m_pBlockInner)
	{
		//由于计算的模型中有一个block对象，其中存储这计算结果所有的数据。
		//而图形中的block对象并没有计算的结果数据
		//在此获取计算的模型中的数据
		CString strModule=(m_pBlockInner->GetModulePtr())->m_Name;
		CString strBlockName=m_pBlockInner->m_sName;
		CCalcBlock *pBlockForClac=(m_pBlockInner->GetModelPtr())->FindBlockByName(strModule,strBlockName);
		if(pBlockForClac==NULL)
			return _T("N/A");
		CCalcPort* pPort=NULL;
		pPort = pBlockForClac->GetInputPortObj(k);
		//if(pPort==NULL)
		//	m_pBlockInner->GetInputPortObj(k);
		if(pPort==NULL)
			return _T("");
		double value;
		CString str;
		if((pPort->GetPortValueType()==VALUE_STAT)||(pPort->GetPortValueType()==VALUE_STAT_SERIES))
		{
			StatData statData;
			pPort->GetPortUniValue().GetStatData(&statData);
			value=statData.dValue;
		}
		else
		{
			pPort->GetPortUniValue().GetDoubleVal(&value);
		}
		
		//str.Format("%.2f",value);
		if((double)((int)value)==value)
		{
			str.Format("%d", (int)value);
		}
		else
		{
			str.Format(_T("%f"), value);
	 		str.TrimRight(_T("0")); 
		}
		return str;
	}
	else 
		return _T("");
}
CString CFlowchartEntity::GetOutPortValue(int k)
{
	if(m_pBlockInner)
	{
		CString strModule=(m_pBlockInner->GetModulePtr())->m_Name;
		CString strBlockName=m_pBlockInner->m_sName;
		CCalcBlock *pBlockForClac=(m_pBlockInner->GetModelPtr())->FindBlockByName(strModule,strBlockName);
		CCalcPort* pPort=NULL;
		if(pBlockForClac==NULL)
			return _T("N/A");
		pPort = pBlockForClac->GetOutputPortObj(k);
		if(pPort==NULL)
			return _T("");

		double value;
		CString str;
		if((pPort->GetPortValueType()==VALUE_STAT)||(pPort->GetPortValueType()==VALUE_STAT_SERIES))
		{
			StatData statData;
			pPort->GetPortUniValue().GetStatData(&statData);
			value=statData.dValue;
		}
		else
		{
			pPort->GetPortUniValue().GetDoubleVal(&value);
		}

		//str.Format("%.2f",value);
		if((double)((int)value)==value)
		{
				  str.Format("%d", (int)value);
		}
		else
		{
			 str.Format(_T("%f"), value);
	 		 str.TrimRight(_T("0")); 
		}
		return str;
	}
	else 
		return _T("");
}
CString CFlowchartEntity::GetInPortID(int k)
{
	if(m_pBlockInner)
	{
			CCalcPort* pPort = m_pBlockInner->GetInputPortObj(k);
			return pPort->GetPortID(); 
	}
	else 
		return _T("");
}
CString CFlowchartEntity::GetOutPortID(int k)
{
	if(m_pBlockInner)
	{
			CCalcPort* pPort = m_pBlockInner->GetOutputPortObj(k);
			return pPort->GetPortID(); 
	}
	else 
		return _T("");
}
int CFlowchartEntity::GetInputPortValueType(int k)
{
	if(m_pBlockInner)
	{
			CCalcPort* pPort = m_pBlockInner->GetInputPortObj(k);
			return pPort->GetPortValueType();
	}
	else 
		return -1;
}
void CFlowchartEntity::SetInputPortValueType(int k,int type)
{
	if(m_pBlockInner)
	{
			CCalcPort* pPort = m_pBlockInner->GetInputPortObj(k);
			pPort->SetPortValueType(type);
	}
}

int CFlowchartEntity::GetOutputPortValueType(int k)
{
	if(m_pBlockInner)
	{
			CCalcPort* pPort = m_pBlockInner->GetOutputPortObj(k);
			return pPort->GetPortValueType();
	}
	else 
		return -1;
}

void CFlowchartEntity::SetOutputPortValueType(int k ,int type)
{
	if(m_pBlockInner)
	{
			CCalcPort* pPort = m_pBlockInner->GetOutputPortObj(k);
			pPort->SetPortValueType(type);
	}
}

//只针对输入
void  CFlowchartEntity::SetBlockPortLink(int k,CString strBlockName,CString strPortName)
{

	if(m_pBlockInner!=NULL)
	{
			CCalcPort* pPort = m_pBlockInner->GetInputPortObj(k);
			pPort->SetPortLinked(TRUE);
			pPort->SetLinkFormBlockName(strBlockName);
			pPort->SetLinkFromBlockPortName(strPortName);
	}
	
}
CString CFlowchartEntity::GetPortLinkBlockName(int k)
{
	if(m_pBlockInner!=NULL)
	{
			CCalcPort* pPort = m_pBlockInner->GetInputPortObj(k);
		    return pPort->GetLinkFormBlockName();
	}
	else
		return _T("");
}

CString CFlowchartEntity::GetPortLinkBlockPortName(int k)
{
	if(m_pBlockInner!=NULL)
	{
			CCalcPort* pPort = m_pBlockInner->GetInputPortObj(k);
		    return pPort->GetLinkFromBlockPortName();
	}
	else
		return _T("");
}

BOOL CFlowchartEntity::GetBlockPortLinked(int portID)
{
	if(m_pBlockInner!=NULL)
	{
			CCalcPort* pPort = m_pBlockInner->GetInputPortObj(portID);
		    return  pPort->GetPortLinked();
	}
	else
		return FALSE;
}

void  CFlowchartEntity::ClearBlockPortLink(int k)
{
	if(m_pBlockInner!=NULL)
	{
			CCalcPort* pPort = m_pBlockInner->GetInputPortObj(k);
		    CString str;
			str.Empty();
			pPort->SetPortLinked(FALSE);
			pPort->SetLinkFormBlockName(str);
			pPort->SetLinkFromBlockPortName(str);
	}
	
}
int CFlowchartEntity::FindInPortIdByName(CString strName) const
{
	int index=-1;
	if(m_pBlockInner!=NULL)
	{
		int portnum =  m_pBlockInner->GetInputNum();
		for(int t =0;t<portnum;t++)
		{
			CCalcPort* pPort = m_pBlockInner->GetInputPortObj(t);
			if(pPort->GetPortID() == strName)
			{
				index = t;
				break;
			}
		}
	}
	return index;

	
}
int CFlowchartEntity::FindOutPortIdByName(CString strName) const
{
	////////////////////////////////
	int index=-1;
	if(m_pBlockInner!=NULL)
	{
		int portnum =  m_pBlockInner->GetOutputNum();
		for(int t =0;t<portnum;t++)
		{
			CCalcPort* pPort = m_pBlockInner->GetOutputPortObj(t);
			if(pPort->GetPortID() == strName)
			{
				index = t;
				break;
			}
		}
	}
	return index;

	
}

CString CFlowchartEntity::GetBlockType()
{
	return m_strBlockType;
}
void CFlowchartEntity::SetBlockType(CString str)
{
      m_strBlockType = str;
}
CString CFlowchartEntity::GetBlockName()
{
	return m_pBlockInner->GetBlockName();
}
void CFlowchartEntity::SetBlockName(CString str)
{
	m_pBlockInner->SetBlockName(str);
	SetName(str); //
}

CString CFlowchartEntity::GetBlockDesp()
{
	return m_pBlockInner->GetBlockDesp();
}
void CFlowchartEntity::SetBlockDesp(CString str)
{
	m_pBlockInner->SetBlockDesp(str);
	//SetTitle(str); //
}
CString CFlowchartEntity::GetBlockStrPro()//得到计算块的字符串属性
{
	return m_pBlockInner->GetBlockStrPro();
}
void CFlowchartEntity::SetBlockStrPro(CString str)//设置计算块的字符串属性
{
	m_pBlockInner->SetBlockStrPro(str);
}
void CFlowchartEntity::SetModelPtr(CCalcModel* p)
{
	m_pBlockInner->SetModelPtr(p);
}
void CFlowchartEntity::SetModulePtr(CCalcModule* p) 
{
	m_pBlockInner->SetModulePtr(p);
}
void CFlowchartEntity::SetCalcOrderID(int id)
{
	if(m_pBlockInner!=NULL)
	{
			m_pBlockInner->SetOrderID(id);
	}
}
int CFlowchartEntity::GetCalcOrderID()
{
	if(m_pBlockInner!=NULL)
	{
			return m_pBlockInner->GetOrderID();
	}
	else
		return -1;
}


//显示属性对话框
void CFlowchartEntity::ShowPropDialog(CWnd* parent, BOOL show )
{
		if(m_pBlockInner!=NULL)
		{
			m_pBlockInner->ShowPropertyDialog(parent,show);
		}
}
void  CFlowchartEntity::ShowOrderDialog(CWnd* parent, BOOL show )
{
	CDlgCalcOrder  dlg;
	dlg.m_iOrderID = GetCalcOrderID();
	if(dlg.DoModal()==IDOK)
	{
		SetCalcOrderID(dlg.m_iOrderID);
	}
}
CString CFlowchartEntity::ShowBlockDespDialog(CWnd* parent, BOOL show)
{
	CString str;
	str.Empty();
    CDlgBlockName  dlg;
	dlg.m_strWndTitle = _T("计算块描述属性");
	dlg.m_strName = GetBlockDesp();
	if(dlg.DoModal()==IDOK)
	{
		 str = dlg.m_strName ;
	     SetBlockDesp(dlg.m_strName);
	}
	return str;
}
CString  CFlowchartEntity::ShowBlockNameDialog(CWnd* parent, BOOL show )
{
	CString str;
	str.Empty();
    CDlgBlockName  dlg;
	dlg.m_strWndTitle = _T("计算块名字属性");
	dlg.m_strName = GetBlockName();
	if(dlg.DoModal()==IDOK)
	{
		str = dlg.m_strName;
	}
	return str;
}

//根据读取的配置，设置端口，直接影响的是
void CFlowchartEntity::CreatePortsFromString(int InNum,int OutNum,CString InNames,CString strInValueTypes,CString OutNames,CString strOutValueTypes, CString InputLinkStrs)
{
	if(m_pBlockInner==NULL) return;
	m_pBlockInner->CreatePortsFromString(InNum,OutNum,InNames, strInValueTypes, OutNames, strOutValueTypes,  InputLinkStrs);
}

void CFlowchartEntity::ShowProperties( CWnd* parent, BOOL show )
{
	 //CDiagramEntity::ShowProperties(parent,show);
		//if(m_pBlockInner!=NULL) 
		//	m_pBlockInner->ShowPropertyDialog(parent,show);

}
void CFlowchartEntity::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	if(m_pBlockInner!=NULL) 
		m_pBlockInner->PutPropertyToRecord(pRecord);
}
void CFlowchartEntity::ReadBlockProperties(_ConnectionPtr &pCon,CString strPropTable)
{
	if(m_pBlockInner!=NULL) 
		m_pBlockInner->ReadBlock(pCon,strPropTable);
}
//add at 2011-10-30----------begin--------------------
BOOL CFlowchartEntity::GetIsShowDesp()//是否显示DESP
{
	return m_pBlockInner->m_bShowDesp ;
}
BOOL CFlowchartEntity::GetIsShowInPortText()
{
	return m_pBlockInner->m_bShowInPortText ;
}
BOOL CFlowchartEntity::GetIsShowOutPortText()
{
	return m_pBlockInner->m_bShowOutPortText ;
}

//add at 2011-10-30---above end---------------------------