// TreeItemObj.cpp: implementation of the CTreeItemObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "TreeItemObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTreeItemObj::CTreeItemObj(CString strName,int ItemType,CString BlockType,CString BlockTypeDesp,bool bSystBlock)
:
m_strName(strName),
m_iItemType(ItemType),
m_strBlockType(BlockType),
m_strBlockTypeDesp(BlockTypeDesp),
m_bSysBlock(bSystBlock)
{

}

CTreeItemObj::~CTreeItemObj()
{

}
