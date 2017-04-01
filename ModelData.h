// ModelData.h: interface for the ModelData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODELDATA_H__7054667A_8FF5_4F7F_AB16_607341B74B7A__INCLUDED_)
#define AFX_MODELDATA_H__7054667A_8FF5_4F7F_AB16_607341B74B7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ModuleData.h"
#include "DBOperator.h"
class ModelData  
{
public:
	int id;
	CString model_name;
	CString model_desp;
	CString model_IP;
	CString model_port;
	CArray<ModuleData*,ModuleData*&> ModuleArr;

	CStringArray strSourceNameArr;
	CStringArray strRDBTypeArr;
	CStringArray strIPArr;
	CStringArray strRDBNameArr;
	CStringArray strRDBUserArr;
	CStringArray strRDBPassArr;
	CStringArray strFileNameArr;
public:
	ModelData();
	virtual ~ModelData();
	void SetIPAndPort(CString strIP,CString strPort);//设置IP和端口
	void GetIPAndPort(CString &strIP,CString &strPort);//获得IP和端口信息
	void ReadDate(_ConnectionPtr &pCon,CString strModelName);

	BOOL DelModelInModelListTable(_ConnectionPtr &pCon,CString strModelName);
	void DelTheModelTables(_ConnectionPtr &pCon,CString strModelName);

	void CreatTheModelTablesInMySql(_ConnectionPtr &pCon,CString strModelName);
	void CreatTheModelTablesInMDB(_ConnectionPtr &pCon,CString strModelName);

	void WriteData(_ConnectionPtr &pCon,CString strModelName);
private:
	void readModuleDate(_ConnectionPtr &pCon,CString strModelName);
	void readRDBSourceData(_ConnectionPtr &pCon,CString strModelName);

	void writeModuleDate(_ConnectionPtr &pCon,CString strModelName);
	void writeRDBSourceData(_ConnectionPtr &pCon,CString strModelName);
};

#endif // !defined(AFX_MODELDATA_H__7054667A_8FF5_4F7F_AB16_607341B74B7A__INCLUDED_)
