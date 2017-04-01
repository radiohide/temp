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
		//����ÿ�����󱻵���������û���ڸÿ������ڣ��������򡢶˿ڣ��ţ�����������
		int portID = -1;
		obj =  (CFlowchartEntity*) GetObject( t );
		int hit = obj->GetHitCodeExt(virtpoint,obj->GetRect(),&portID);
		if(hit == DEHT_NONE)   //�ÿ�û��ѡ��,�Ϳ�������
		{
			if(!bFindOne) continue;
			else break;
		}
		else if(hit ==DEHT_BODY ) //���е���������
		{
			bFindOne = TRUE;
			objFocus = obj;
			hitFocusPortID = portID;
			hitFocusState = DEHT_BODY;
			break;
		}
		else if(hit ==DEHT_BLOCKNAME ) //���е�������������
		{
			bFindOne = TRUE;
			objFocus = obj;
			hitFocusState = DEHT_BLOCKNAME;
			break;
		}
		else if(hit ==DEHT_BLOCKDESP ) //���е�������������
		{
			bFindOne = TRUE;
			objFocus = obj;
			hitFocusState = DEHT_BLOCKDESP;
			break;
		}
		else if(hit ==DEHT_BLOCKORDER ) //���е�������������
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
	   /*else if(hit >= DEHT_TOPLEFT && hit<=DEHT_RIGHTMIDDLE ) //λ���ֱ���
	   {
		   	if(!bFindOne) continue;
			else break;
	   }*/
	}
	////---------------------------------------------------------
	//===========================================================
	if(bFindOne==FALSE) //û��һ����ѡ�У�˵�����ڿհ������˫������ȡ��ԭ����ѡ��
	{
		if(m_pObjLinkFrom!=NULL)  m_pObjLinkFrom->m_bFocusPort =FALSE;
		m_pObjLinkFrom = NULL;
	}
	else  //�ҵ�ĳ��
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
					if(strNameNew != objFocus->GetBlockName()) //�б仯
					{
						if(GetSameNameBlockCount(strNameNew)>=1)  
						{
							AfxMessageBox("�����Ѵ��ڣ���ѡ�ñ�Ŀ�����.");
						}
						else
						{
							//�����ӵĶ˿ڵ����⣬���������������д˿���������Ҫ�ı��������ָ��
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
		case DEHT_INPUT:  //Ҫ���Ƿ���
			{
				  if(objFocus->GetBlockPortLinked(hitFocusPortID) ) //������
				  {
					   objFocus->ClearBlockPortLink(hitFocusPortID);
				  }
				  else //��û�����ӣ��ǿյ�
				  {
						if(m_pObjLinkFrom!=NULL) //��˫�������������ϴε�һ��
						{
							if(m_pObjLinkFrom->GetBlockName() != objFocus->GetBlockName())
							{
								//�ж϶˿���˿ڵ����������Ƿ�һ�£�һ�²�������
								int k = m_pObjLinkFrom->m_FocusPortID;

								int ObjPortValType = objFocus->GetInputPortValueType(hitFocusPortID);
								int LinkObjPortValType=m_pObjLinkFrom->GetOutputPortValueType(k);
								if(ObjPortValType==LinkObjPortValType)
								{
									objFocus->SetBlockPortLink(hitFocusPortID,m_pObjLinkFrom->GetBlockName(),m_pObjLinkFrom->GetOutPortID(k)); //����
								}
								else
								{
									AfxMessageBox("���Ӷ˿�֮����������Ͳ�ƥ��!");
								}
							}
						}
				  }
                   RedrawWindow();
					  
			break;}
		case DEHT_OUTPUT:  //ȡ��ԭ����ѡ������˵Ķ��������µ�
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
		//�����ӹ�ϵ
		int InNum = obj->GetInputNum();
		for(int k=0;k<InNum;k++)
		{
			//���Ƿ�������
			BOOL bLink = obj->GetBlockPortLinked(k);
			if(bLink)
			{
				//�����ĸ��顢�ڼ������������BlockName,portName,
				CString strFromBlockName = obj->GetPortLinkBlockName(k);
				CString strFromBlockPortName = obj->GetPortLinkBlockPortName(k);
                //�������λ��
				CPoint ptFrom = GetPointFromByLinkString(strFromBlockName,strFromBlockPortName);
				//����յ������λ��
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
				//�ж��Ƿ������м���y�Ĳ��
				int Ydis = ptMidTo.y - ptMidFrom.y;
				if(Ydis<0) Ydis = -Ydis;
				if(Ydis<=0) //��ȫ�����ֱ�ӻ�
				{
					dc->MoveTo(ptFrom);
					dc->LineTo(ptEnd);
				}
				else //����,���Ƿ���һ��С��Χ��
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
//ͨ��ģ��������ø�ģ���
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
//�ı�������
void CFlowchartEditor::ChangeBlockName(CFlowchartEntity* obj,CString strNameNew )
{
	CFlowchartEntity* pObj= NULL;
	
	CString strOldName = obj->GetBlockName();
	int max = GetObjectCount();
	for( int t = 0 ; t < max ; t++ )
	{
		pObj =  (CFlowchartEntity*)( GetObject( t ) );
		//������OBJ������˿ڣ����Ƿ���������
		if(pObj->GetBlockName()!=strOldName)
		{
				int InNum = pObj->GetInputNum();
				for(int k=0;k<InNum;k++)
				{
					//���Ƿ�������
					BOOL bLink = pObj->GetBlockPortLinked(k);
					if(bLink)
					{
						//�����ĸ��顢�ڼ������������BlockName,portName,
						CString strFromBlockName = pObj->GetPortLinkBlockName(k);
						CString strFromBlockPortName = pObj->GetPortLinkBlockPortName(k);
						if(strFromBlockName==strOldName)
						{
							pObj->SetBlockPortLink(k,strNameNew,strFromBlockPortName); //����
						}
					}
				}

		}

	}
    obj->SetBlockName(strNameNew);
}