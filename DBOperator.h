// DBOperator.h: interface for the CDBOperator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBOPERATOR_H__CC1875FB_0E63_4C07_AB6C_15D4F8AC20FC__INCLUDED_)
#define AFX_DBOPERATOR_H__CC1875FB_0E63_4C07_AB6C_15D4F8AC20FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDBOperator  
{
public:
	CDBOperator();
	virtual ~CDBOperator();
public:
	//����
	static BOOL  CreateTable(_ConnectionPtr &pCon,CString strTableName,CStringArray& Fields,CStringArray& FieldTypes);
	static BOOL  CreateBlockListTableInMySql(_ConnectionPtr &pCon,CString strModelName,CString strModuleName);
	static BOOL  CreateBlockPropTableInMySql(_ConnectionPtr &pCon,CString strModelName,CString strModuleName);
	static BOOL  CreateModuleListTableInMySql(_ConnectionPtr &pCon,CString strModelName);
	static BOOL  CheckRecordExisted(_ConnectionPtr &pCon,CString strTable,CString strKeyField,CString strValue);
	static BOOL  DelTable(_ConnectionPtr &pCon,CString strTable);
	static BOOL  RenameTable(_ConnectionPtr &pCon,CString strOldName,CString strNewName);
	//ɾ������������
	//BOOL DeleteTableContent();
	//ɾ������ĳ����¼�������ֶ�ֵΪʲô�ġ�
	
};

#endif // !defined(AFX_DBOPERATOR_H__CC1875FB_0E63_4C07_AB6C_15D4F8AC20FC__INCLUDED_)
