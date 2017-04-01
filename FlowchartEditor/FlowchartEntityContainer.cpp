/* ==========================================================================
	CFlowchartEntityContainer

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CFlowchartEntityContainer is a CDiagramEntityContainer-
					derived class, holding the data for a CFlowchartEditor. 
					In addition to CDiagramEntityContainer, this class keeps 
					and manages a list of links. This includes copy/paste 
					and undo-handling.

	Description :	The class uses a derived CDiagramClipboardHandler.

	Usage :			Use as a normal CDiagramEntityContainer class. The 
					editor class exposes functions for command enabling.

   ========================================================================*/
#include "stdafx.h"
#include "FlowchartEntityContainer.h"
#include <math.h>

CFlowchartEntityContainer::CFlowchartEntityContainer()
/* ============================================================
	Function :		CFlowchartEntityContainer::CFlowchartEntityContainer
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
	m_bInShowOrderMode = FALSE;
	m_bInShowValueMode = FALSE;
}

CFlowchartEntityContainer::~CFlowchartEntityContainer()
/* ============================================================
	Function :		CFlowchartEntityContainer::~CFlowchartEntityContainer
	Description :	destructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	
	//ClearUndo();

}

void CFlowchartEntityContainer::RemoveAt( int index )
/* ============================================================
	Function :		CFlowchartEntityContainer::RemoveAt
	Description :	Removes the object at index. Will also 
					remove all links refering to this object.
					
	Return :		void
	Parameters :	int index	-	Index for object to remove.
					
	Usage :			Overridden to remove links as well.

   ============================================================*/
{

	CDiagramEntity* obj = GetAt( index );
	if( obj )
	{
		//CString name = obj->GetName();
		//看是否有哪个块和它相连，所有则对该块进行清除其连接关系
		ClearAllLinkWithObject(index);
		CDiagramEntityContainer::RemoveAt( index );
	}

}


void CFlowchartEntityContainer::Undo()
/* ============================================================
	Function :		CFlowchartEntityContainer::Undo
	Description :	Undo the latest operation
					
	Return :		void
	Parameters :	none

	Usage :			Overridden to also undo link operations.

   ============================================================*/
{

	CDiagramEntityContainer::Undo();

}

void CFlowchartEntityContainer::Snapshot()
/* ============================================================
	Function :		CFlowchartEntityContainer::Snapshot
	Description :	Creates a snapshot of the current data 
					state for the undo-functionality.
					
	Return :		void
	Parameters :	none

	Usage :			Overridden to save the link state as well.

   ============================================================*/
{

	CDiagramEntityContainer::Snapshot();


}

void CFlowchartEntityContainer::ClearUndo()
/* ============================================================
	Function :		CFlowchartEntityContainer::ClearUndo
	Description :	Clears the undo-array
					
	Return :		void
	Parameters :	none

	Usage :			Overridden to also clear the link undo 
					states.

   ============================================================*/
{

	CDiagramEntityContainer::ClearUndo();
}


double CFlowchartEntityContainer::Dist( CPoint point1, CPoint point2 )
/* ============================================================
	Function :		CFlowchartEntityContainer::Dist
	Description :	Calculates the distance between point1 and 
					point2.
					
	Return :		double			-	Resulting distance
	Parameters :	CPoint point1	-	First point to test
					CPoint point2	-	Second point to test
					
	Usage :			Used to find the closest link points between 
					two objects.

   ============================================================*/
{

	double width = abs( point1.x - point2.x );
	double height = abs( point1.y - point2.y );

	double hyp = _hypot( width, height );

	return hyp;

}


void CFlowchartEntityContainer::ClearAllLinkWithObject(int index)
{
	int max = GetSize();
	CFlowchartEntity* obj;
	CFlowchartEntity* TheObj = (CFlowchartEntity*)( GetAt( index) );
	if(TheObj==NULL) return; 

	for( int t = 0 ; t < max ; t++ )
	{
		if(t!=index)
		{
			obj =  (CFlowchartEntity*)( GetAt( t ) );
			//判断是否和要处理的对象有连接，若有则删除，要检查每个入口
			int InNum = obj->GetInputNum();
			for(int k=0;k<InNum;k++)
			{
				//看是否有连接
				BOOL bLink = obj->GetBlockPortLinked(k);
				if(bLink) //该入口有连接
				{
					//找是哪个块、第几个输出，根据BlockName,portName,
					CString strFromBlockName = obj->GetPortLinkBlockName(k);
					CString strFromBlockPortName = obj->GetPortLinkBlockPortName(k);
					if(strFromBlockName==TheObj->GetBlockName())
					{
						obj->ClearBlockPortLink(k);
					}
				}
			}
		}
		
	}
}


void CFlowchartEntityContainer::SetShowOrderMode(BOOL var)
{
	m_bInShowOrderMode = var;
}
void CFlowchartEntityContainer::SetShowValueMode(BOOL var)
{
	m_bInShowValueMode = var;
}
BOOL CFlowchartEntityContainer::GetShowOrderMode()
{
	return m_bInShowOrderMode;
}
BOOL CFlowchartEntityContainer::GetShowValueMode()
{
	return m_bInShowValueMode;
}
 void CFlowchartEntityContainer::Add( CDiagramEntity* obj )
 {
	obj->SetParent( this );
	GetData()->Add( obj );
	SetModified( TRUE );
 }
//根据给定的一个名字，自动生成一个不相同的名字
 //看这个视图中，各个块的名字是否有去掉数字后;
 CString CFlowchartEntityContainer::MakeSuitableName(CString strOldBlockName)
 {
	 CString strNewName=_T("N/A");
	 int NumberID = 0;
	 int maxID=0;

	 CString strName0 =  DeleteNameStringNum(strOldBlockName,&NumberID);

	 BOOL bFind = FALSE;
	 int max = GetSize();
 	 for( int t = 0 ; t < max ; t++ )
	 {
		CFlowchartEntity* obj =  (CFlowchartEntity*) GetAt( t ) ;
		CString strName1 =  DeleteNameStringNum(obj->GetBlockName(),&NumberID);
		if(strName1==strName0)
		{
			bFind = TRUE;
			if(maxID< NumberID)  maxID = NumberID;
		}
	 }
	 //////////////////////////////////////////////////
	 strNewName.Format("%s%d",strName0,maxID+1);
	 return strNewName;
 }

CString CFlowchartEntityContainer::DeleteNameStringNum(CString strName,int* Number)
{
    CString str;
	int len = strName.GetLength();
	int Count=0;
	for(int k=0;k<len;k++)
	{
        CString str1 = strName.Right(k+1);
		if(str1.SpanIncluding("1234567890")!=str1)
		{
			Count =k;
			break;
		}
		//int num = atoi(str1);
		//if(num==0)
		//{
		//	Count =k;
		//	break;
		//}
	}
	if(Count>=1)
	{
		str = strName.Left(len-Count);
		CString str1 = strName.Right(Count);
		(*Number) = atoi(str1);
	}
	else
	{
		str =  strName;
		(*Number)=0;
	}
	return str;
}


void CFlowchartEntityContainer::Duplicate( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Duplicate
	Description :	Duplicates the object and adds the new 
					one 10 pixels offset down and right.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to 
											duplicate.	
					
	Usage :			Call to create a copy of the selected 
					element.

   ============================================================*/
{

	int index = Find( obj );
	if( index != -1 )
	{
		CFlowchartEntity* FlowObj = 	(CFlowchartEntity*)obj;
		CFlowchartEntity* FlowNewObj =	(CFlowchartEntity*) FlowObj->Clone();
		FlowNewObj->Select( TRUE );
		FlowNewObj->MoveRect( 10, 10 );
	   CString str = MakeSuitableName(FlowObj->GetBlockName());
	   //AfxMessageBox(str);
	   FlowNewObj->SetBlockName(str);

		Add( FlowNewObj );
	}

	

}