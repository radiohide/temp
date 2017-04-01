// StrMaptoNumBlock.cpp: implementation of the CStrMaptoNumBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StrMaptoNumBlock.h"
#include "PropertyDlgStrMapToNum.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStrMaptoNumBlock::CStrMaptoNumBlock()
{
	SetBlockType(_T( "StrMaptoNum" ) );
	ConstructBlockInputOutput();
}

CStrMaptoNumBlock::~CStrMaptoNumBlock()
{
	m_strDataArr.RemoveAll();
	m_dDataArr.RemoveAll();
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CStrMaptoNumBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("Str"), VALUE_RTVALUE_SERIES);
	AddOutputPort(_T("bMatch"), VALUE_DOUBLE);
	AddOutputPort(_T("Num"), VALUE_DOUBLE);
}
//根据类型新建对象的函数
CCalcBlock* CStrMaptoNumBlock::CreateFromString( const CString& strType )
{
	CStrMaptoNumBlock* obj = new CStrMaptoNumBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CStrMaptoNumBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBDesp,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
	{
		CString str;
		str.Format("数据源%s连接失败，字符串映射到数字块将无法正常工作，请检查数据源配置或网络！",m_strDBDesp);
		AfxMessageBox(str);
		return;
	}
	getTableData(pConRDB);
}
void CStrMaptoNumBlock::getTableData(_ConnectionPtr &pConRDB)
{
	m_strDataArr.RemoveAll();
	m_dDataArr.RemoveAll();

	_RecordsetPtr  pRecord;
	//检索到所有属性
	try   
	{   
		HRESULT hr;
		hr = pRecord.CreateInstance(__uuidof(Recordset)); 
		CString strSQL;
		strSQL.Format("SELECT * FROM %s",m_strTableName);
		pRecord->Open((char *)_bstr_t(strSQL),_variant_t((IDispatch*)pConRDB,true),
			adOpenStatic,adLockOptimistic,adCmdText);		
	}   
	catch(_com_error e)
	{
		return ;
	}
	while(!pRecord->adoEOF)
	{
		CString strValue =(char *)_bstr_t(pRecord->GetCollect("StrValue"));
		double dValue=atof((char *)_bstr_t(pRecord->GetCollect("NumValue")));
		m_strDataArr.Add(strValue);
		m_dDataArr.Add(dValue);
		pRecord->MoveNext();
	}
	pRecord->Close();
}
//计算函数，实现本模块的计算
void CStrMaptoNumBlock::Calc() //进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
	CCalcPort *pPortObjIn = GetInputPortObj(0);			
	CUniValue  &uniValue=pPortObjIn->GetPortUniValue();
	CString strConCode="";
	for(int i=0; i<uniValue.GetDataSize();i++)
	{
		double dChar;
		uniValue.GetDoubleValAt(i,&dChar);
		strConCode+=(char)dChar;
	}
	bool beFind=false;
	double dValue=0;
	for(i=0;i<m_strDataArr.GetSize();i++)
	{
		if(m_strDataArr[i]==strConCode)
		{
			beFind=true;
			dValue=m_dDataArr[i];
		}
	}
	CCalcPort *pOutPortObj0 = GetOutputPortObj(0);
	CCalcPort *pOutPortObj1 = GetOutputPortObj(1);
	CUniValue UniVar0;
	CUniValue UniVar1;
	if(beFind)
	{
		UniVar0.AddData(1);
		UniVar1.AddData(dValue);
	}
	else
	{
		UniVar0.AddData(0);
		UniVar1.AddData(dValue);
	}
	pOutPortObj0->SetPortUniValue(UniVar0);
	pOutPortObj1->SetPortUniValue(UniVar1);
	OutputResultToGlobalWS();
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CStrMaptoNumBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("TableName")==0)
	{
		if(!strItem1.IsEmpty())  {m_strTableName=strItem1;}
	}
	else if(strPropName.Compare("DBDesp")==0)
	{
		if(!strItem1.IsEmpty())  {m_strDBDesp=strItem1;}
	}
}
//重载以显示不同的属性对象对话框
void CStrMaptoNumBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStrMapToNum dlg;
	dlg.m_strTableName=m_strTableName;
	dlg.m_strDBDesp=m_strDBDesp;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);
	dlg.SetModelPtr(this->m_pModel);
	if(dlg.DoModal()==IDOK)
	{
		m_strTableName=dlg.m_strTableName;
		m_strDBDesp=dlg.m_strDBDesp;
		if((m_pModel!=NULL)&&(this->m_pModel->m_iTagModeLocalOrRemote==1))//远程
		{
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> dataArr;
			SetBlockPropertyToTransferData(dataArr);//获取要传输的属性到传送数组中
			SendBlockPropertyTransferData(dataArr);// 传送数组中数据
			ClearPropertyTransferData(dataArr);//释放数据
		}
	}
}
//重载以将本模块写入到数据库中
void CStrMaptoNumBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="TableName";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strTableName); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="DBDesp";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strDBDesp); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CStrMaptoNumBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_strTableName=((CStrMaptoNumBlock*)pBlock)->m_strTableName;
	this->m_strDBDesp=((CStrMaptoNumBlock*)pBlock)->m_strDBDesp;
}
//将属性放置传输数据中
void CStrMaptoNumBlock::SetBlockPropertyToTransferData(
											CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TableName");
	
	CString str;
	str.Format("%s",m_strTableName);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"DBDesp");
	
	str.Format("%s",m_strDBDesp);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}	