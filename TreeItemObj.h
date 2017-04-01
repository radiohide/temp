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

	CString	m_strName;  //��������
	 int		m_iItemType;   //���ͣ���=0;�����=1��ϵͳ���Զ�����=2��3
	 CString	m_strBlockType; //�����������
	 CString	m_strBlockTypeDesp; //�����������
	 bool		m_bSysBlock;  //�Ƿ���ϵͳ�飬�����Զ����
};

#endif // !defined(AFX_TREEITEMOBJ_H__279D9C16_0571_4658_8CE3_BE8808785224__INCLUDED_)
