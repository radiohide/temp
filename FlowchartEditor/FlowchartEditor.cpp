/* ==========================================================================
	CFlowchartEditor

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-05-01

	Purpose :		CFlowchartEditor is a flowchart editor, demonstrating 
					several concepts of CDiagramEditor:	

					1.	Linking objects using a CDiagramEntityContainer-
						derived class to manage data
					2.	Putting CDiagramEditor in a MDI-application, using 
						a shared CDiagramClipboard-derived clipboard handler.
					3.	Exporting the diagram to an enhanced metafile that 
						can be pasted into - for example - Word.

	Description :	CFlowcharEditor handles several messages and overrides 
					drawing to manage links. Links are implemented as a 
					special class, stored in a separate array in 
					CFlowchartEntityContainer. Object drawing in 
					CFlowchartEditor also draws the links. 

					The objects derived from CFlowchartEntity has link 
					points. Two selected and unlinked objects with free 
					link points at appropriate position can be linked. If 
					they are linked, a line is automatically drawn between 
					the objects, with an arrow head representing the 
					direction (which can be flipped).

					The link can have a label, that is, a text describing 
					the link.

					When objects are moved, other objects linked to the 
					moved ones might also be moved - depending on the links. 
					For example, if two objects are linked horizontally and 
					one is moved up or down, the linked object will also be 
					moved. 

					A special type of object is the linkable line. Linkable 
					lines are lines that can be linked to other objects, 
					even lines. They can be used to represent more complex 
					flows, such as several links converging on a single spot. 
					Lines will also be moved as other objects, but worth to 
					notice is that they will not be resized, they will keep 
					their original length and might therefore trigger 
					movements far from the current one. Linked lines can 
					only be drawn either vertically or horizontally, that is, 
					no slant is allowed - this is enforced by the editor 
					rather than the line class.

	Usage :			CFlowchartEditor should be instantiated in the same way 
					as a CDiagramEditor. The two public member functions 
					CanLink and IsLinked can be used for command enablers 
					for the commands OnLink, OnUnlink, OnLinkDirection and 
					OnLinkProperties. The commands link, unlink, reverse 
					direction of the link and sets/edit the link label 
					respectively.

					The package uses RTTI (run time type information) to 
					identify object types, using dynamic_cast, so the 
					project must enable this feature.

   ========================================================================*/

#include "stdafx.h"
#include "FlowchartEditor.h"
#include "FlowchartEntityContainer.h"
#include "FlowchartEntity.h"
//#include "FlowchartMenu.h"
//#include "FlowchartLinkPropertiesDialog.h"
//#include "FlowchartLinkableLineSegment.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlowchartEditor
extern CLog g_Log;

CFlowchartEditor::CFlowchartEditor()
/* ============================================================
	Function :		CFlowchartEditor::CFlowchartEditor
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetBackgroundColor( RGB( 250, 250, 230 ) );
	SetSnapToGrid( TRUE );
	SetRestraints( RESTRAINT_VIRTUAL );
	//SetPopupMenu( new CFlowchartMenu );

	m_pObjLinkFrom = NULL;
	m_bInShowOrderMode = FALSE;
	m_bInShowValueMode = FALSE;
}

CFlowchartEditor::~CFlowchartEditor()
/* ============================================================
	Function :		CFlowchartEditor::~CFlowchartEditor
	Description :	destructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

BEGIN_MESSAGE_MAP(CFlowchartEditor, CDiagramEditor)

END_MESSAGE_MAP()
void CFlowchartEditor::SetShowValueMode(BOOL var)
{
	m_bInShowValueMode=var;
}
BOOL CFlowchartEditor::GetShowValueMode()
{
	return m_bInShowValueMode;
}
void CFlowchartEditor::SetShowOrderMode(BOOL var)
{
	m_bInShowOrderMode = var;
}
BOOL CFlowchartEditor::GetShowOrderMode()
{
	return m_bInShowOrderMode;
}
void CFlowchartEditor::OnLButtonDblClk( UINT nFlags, CPoint point ) 
{
	CFlowchartEntity* obj=NULL;
	BOOL bFindOne = FALSE;
	CFlowchartEntity* objFocus=NULL;
	int   hitFocusState=-1;
	int   hitFocusPortID=-1;

	CPoint virtpoint( point );
	ScreenToVirtual( virtpoint );
	int max = GetObjectCount();
	for( int t = 0 ; t < max ; t++ )
	{
		//测试每个对象被点击的情况：没有在该块区域内，名字区域、端口（号）、属性区域
		int portID = -1;
		obj =  (CFlowchartEntity*) GetObject( t );
		int hit = obj->GetHitCodeExt(virtpoint,obj->GetRect(),&portID);
		if(hit == DEHT_NONE)   //该块没有选中,就看其他块
		{
			if(!bFindOne) continue;
			else break;
		}
		else if(hit ==DEHT_BODY ) //点中的是属性区
		{
			bFindOne = TRUE;
			objFocus = obj;
			hitFocusPortID = portID;
			hitFocusState = DEHT_BODY;
			break;
		}
		else if(hit ==DEHT_BLOCKNAME ) //点中的是名字属性区
		{
			bFindOne = TRUE;
			objFocus = obj;
			hitFocusState = DEHT_BLOCKNAME;
			break;
		}
		else if(hit ==DEHT_BLOCKDESP ) //点中的是名字属性区
		{
			bFindOne = TRUE;
			objFocus = obj;
			hitFocusState = DEHT_BLOCKDESP;
			break;
		}
		else if(hit ==DEHT_BLOCKORDER ) //点中的是名字属性区
		{
			bFindOne = TRUE;
			objFocus = obj;
			hitFocusState = DEHT_BLOCKORDER;
			break;
		}
		else if(hit ==DEHT_INPUT )
		{
			bFindOne = TRUE;
			objFocus = obj;
			hitFocusPortID = portID;
			hitFocusState = DEHT_INPUT;
			break;
		}
  	   else if(hit == DEHT_OUTPUT )
		{
		   	bFindOne = TRUE;
			objFocus = obj;
			hitFocusPortID = portID;
			hitFocusState = DEHT_OUTPUT;
			break;
		}
	   /*else if(hit >= DEHT_TOPLEFT && hit<=DEHT_RIGHTMIDDLE ) //位于手柄处
	   {
		   	if(!bFindOne) continue;
			else break;
	   }*/
	}
	////---------------------------------------------------------
	//===========================================================
	if(bFindOne==FALSE) //没有一个被选中，说明是在空白区域的双击，则取消原来的选择
	{
		if(m_pObjLinkFrom!=NULL)  m_pObjLinkFrom->m_bFocusPort =FALSE;
		m_pObjLinkFrom = NULL;
	}
	else  //找到某个
	{
	    switch(hitFocusState)
		{
		case DEHT_BODY:
			{
				if( GetSelectCount() == 1) 
				{
					objFocus->ShowPropDialog(this);	
					RedrawWindow();
				}
			break;
			}
	    case DEHT_BLOCKNAME:
			{
				CString strNameNew = objFocus->ShowBlockNameDialog(this);
				if(!strNameNew.IsEmpty())
				{
					if(strNameNew != objFocus->GetBlockName()) //有变化
					{
						if(GetSameNameBlockCount(strNameNew)>=1)  
						{
							AfxMessageBox("名字已存在，请选用别的块名字.");
						}
						else
						{
							//看链接的端口的问题，如果其他块的连接有此块的输出则，需要改变其他块的指向
							ChangeBlockName(objFocus,strNameNew);
							RedrawWindow();
						}
					}
				}
			break;}
		case DEHT_BLOCKDESP:
			{
               	CString strNameNew = objFocus->ShowBlockDespDialog(this);
				RedrawWindow();
				break;
			}
		  case DEHT_BLOCKORDER:
			{
               if( GetSelectCount() == 1) 
				{
					 objFocus->ShowOrderDialog(this); 
					 RedrawWindow();
				}
				break;
			}
		case DEHT_INPUT:  //要看是否有
			{
				  if(objFocus->GetBlockPortLinked(hitFocusPortID) ) //有连接
				  {
					   objFocus->ClearBlockPortLink(hitFocusPortID);
				  }
				  else //若没有连接，是空的
				  {
						if(m_pObjLinkFrom!=NULL) //当双击到输入且有上次的一个
						{
							if(m_pObjLinkFrom->GetBlockName() != objFocus->GetBlockName())
							{
								//判断端口与端口的数据类型是否一致，一致才能相连
								int k = m_pObjLinkFrom->m_FocusPortID;

								int ObjPortValType = objFocus->GetInputPortValueType(hitFocusPortID);
								int LinkObjPortValType=m_pObjLinkFrom->GetOutputPortValueType(k);
								if(ObjPortValType==LinkObjPortValType)
								{
									objFocus->SetBlockPortLink(hitFocusPortID,m_pObjLinkFrom->GetBlockName(),m_pObjLinkFrom->GetOutPortID(k)); //设置
								}
								else
								{
									AfxMessageBox("连接端口之间的数据类型不匹配!");
								}
							}
						}
				  }
                   RedrawWindow();
					  
			break;}
		case DEHT_OUTPUT:  //取消原来的选中输出端的对象，设置新的
			{
				if(m_pObjLinkFrom!=NULL)  m_pObjLinkFrom->m_bFocusPort = FALSE;
				m_pObjLinkFrom = objFocus;
				m_pObjLinkFrom->m_FocusPortID = hitFocusPortID;
				m_pObjLinkFrom->m_bFocusPort = TRUE;
				RedrawWindow();
			break;}
		}
	}
	////---------------------------------------------------------
	CDiagramEditor::OnLButtonDblClk( nFlags, point );
}

void CFlowchartEditor::DrawObjects( CDC* dc, double zoom ) const
{
	int max = GetObjectCount();
	CFlowchartEntity* obj;
	int seg = round( ( double ) GetMarkerSize().cx * zoom / 2 );
	for( int t = 0 ; t < max ; t++ )
	{
		obj =  (CFlowchartEntity*)( GetObject( t ) );
		obj->DrawObject( dc, zoom );
		//画连接关系
		int InNum = obj->GetInputNum();
		for(int k=0;k<InNum;k++)
		{
			//看是否有连接
			BOOL bLink = obj->GetBlockPortLinked(k);
			if(bLink)
			{
				//找是哪个块、第几个输出，根据BlockName,portName,
				CString strFromBlockName = obj->GetPortLinkBlockName(k);
				CString strFromBlockPortName = obj->GetPortLinkBlockPortName(k);
                //获得起点的位置
				CPoint ptFrom = GetPointFromByLinkString(strFromBlockName,strFromBlockPortName);
				//获得终点的中心位置
				CPoint ptTo = obj->GetPortPoint(DEHT_INPUT,obj->GetRect(),InNum,k);
				CPoint ptMidTo;
				CPoint ptMidFrom;
				CPoint ptEnd;
				ptMidTo.x = ptTo.x - (2+(InNum-k)*2)*seg;//ptTo.x - int(double(nLinkWidth)*double(InNum-k)/double(InNum));
				ptMidTo.y = ptTo.y;
				ptMidFrom.x = ptMidTo.x;
				ptMidFrom.y = ptFrom.y;
				ptEnd.x = ptTo.x - seg * 2;
				ptEnd.y = ptTo.y;
				//判断是否两个中间点的y的差别
				int Ydis = ptMidTo.y - ptMidFrom.y;
				if(Ydis<0) Ydis = -Ydis;
				if(Ydis<=0) //完全相等则，直接画
				{
					dc->MoveTo(ptFrom);
					dc->LineTo(ptEnd);
				}
				else //不等,则看是否在一定小范围内
				{
					dc->MoveTo(ptFrom);
					dc->LineTo(ptMidFrom);
					dc->LineTo(ptMidTo);
					dc->LineTo(ptEnd);
				}
				
			}
		}
		
	}

}
//通过模块名，获得该模块的
CPoint CFlowchartEditor::GetPointFromByLinkString(CString strBlock,CString strPortName) const
{
	CPoint point(-1,-1);

	CFlowchartEntity* pObj = FindByName(strBlock);
	if(pObj)
	{
	     point = pObj->GetPortPointByName(DEHT_OUTPUT,strPortName);
	}
	return point;
}

CFlowchartEntity*  CFlowchartEditor::FindByName(CString strBlockName) const
{
	CFlowchartEntity* pObj= NULL;
	int max = GetObjectCount();
	int portID=-1;
	for( int t = 0 ; t < max ; t++ )
	{
		pObj =  (CFlowchartEntity*)( GetObject( t ) );
		if(pObj)
		{
			if(pObj->GetName() ==strBlockName)
			{
				break;
			}
		}
	}
	return pObj;
}
int CFlowchartEditor::GetSameNameBlockCount(CString strBlockName)
{
	CFlowchartEntity* pObj= NULL;
	int max = GetObjectCount();
	int Count=0;
	for( int t = 0 ; t < max ; t++ )
	{
		pObj =  (CFlowchartEntity*)( GetObject( t ) );
		if(pObj)
		{
			if(pObj->GetName() ==strBlockName)
			{
				Count++;
			}
		}
	}
	return Count;
}
//改变块的名称
void CFlowchartEditor::ChangeBlockName(CFlowchartEntity* obj,CString strNameNew )
{
	CFlowchartEntity* pObj= NULL;
	
	CString strOldName = obj->GetBlockName();
	int max = GetObjectCount();
	for( int t = 0 ; t < max ; t++ )
	{
		pObj =  (CFlowchartEntity*)( GetObject( t ) );
		//遍历此OBJ的输入端口，看是否有来自于
		if(pObj->GetBlockName()!=strOldName)
		{
				int InNum = pObj->GetInputNum();
				for(int k=0;k<InNum;k++)
				{
					//看是否有连接
					BOOL bLink = pObj->GetBlockPortLinked(k);
					if(bLink)
					{
						//找是哪个块、第几个输出，根据BlockName,portName,
						CString strFromBlockName = pObj->GetPortLinkBlockName(k);
						CString strFromBlockPortName = pObj->GetPortLinkBlockPortName(k);
						if(strFromBlockName==strOldName)
						{
							pObj->SetBlockPortLink(k,strNameNew,strFromBlockPortName); //设置
						}
					}
				}

		}

	}
    obj->SetBlockName(strNameNew);
}