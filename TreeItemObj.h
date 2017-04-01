// TreeItemObj.h: interface for the CTreeItemObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREEITEMOBJ_H__279D9C16_0571_4658_8CE3_BE8808785224__INCLUDED_)
#define AFX_TREEITEMOBJ_H__279D9C16_0571_4658_8CE3_BE8808785224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTreeItemObj : public CObject  
{
public:
	CTreeItemObj(CString strName,int ItemType,CString BlockType,CString BlockTypeDesp,bool bSystBlock);
	virtual ~CTreeItemObj();

	CString	m_strName;  //名称描述
	 int		m_iItemType;   //类型：块=0;分类号=1；系统或自定义组=2，3
	 CString	m_strBlockType; //块的类型名称
	 CString	m_strBlockTypeDesp; //块的类型描述
	 bool		m_bSysBlock;  //是否是系统块，还是自定义块
};

#endif // !defined(AFX_TREEITEMOBJ_H__279D9C16_0571_4658_8CE3_BE8808785224__INCLUDED_)
