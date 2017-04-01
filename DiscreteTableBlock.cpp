// DiscreteTableBlock.cpp: implementation of the CDiscreteTableBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "DiscreteTableBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDiscreteTableBlock::CDiscreteTableBlock()
{
	iComIndex=0;

    m_strDBDesp="";
	m_strDBType ="";
	m_strTableName="t_u1s2_envt_division";
	//SYSTME=====设置类型======
	//设置类型
	SetBlockType(_T( "DiscreteTable" ) );
	ConstructBlockInputOutput();
	//SYSTME=====初始化本模块的属性===

}

CDiscreteTableBlock::~CDiscreteTableBlock()
{

}
//构造初始的输入输出个数
void CDiscreteTableBlock::ConstructBlockInputOutput()
{
	AddInputPort(_T("In"), VALUE_DOUBLE, _T("in1"), _T("in1"));	

	AddOutputPort("Out",VALUE_RTVALUE_SERIES, _T("out1"), _T("out1"));
}
//根据类型新建对象的函数
//同时在CCalcBlockControlFactory中，要加入相应的代码
CCalcBlock* CDiscreteTableBlock::CreateFromString( const CString& strType )
{
	CDiscreteTableBlock* obj = new CDiscreteTableBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CDiscreteTableBlock::InitCalc()
{
	CCalcBlock::InitCalc();

	//初始化时加载表内容
	m_DiscreteContent.RemoveAll();

	int tagBeCon=0;
	CString strDBType="";
	m_strDBDesp=this->m_pModel->m_ArrSourceRDBName[iComIndex];
	_ConnectionPtr m_pConRDB = m_pModel->GetSourceRDBByDesp(m_strDBDesp,tagBeCon,strDBType);
	//m_pConRDB-> Execute("SET   NAMES   gbk",&vtMissing,adCmdText); 
	
	_RecordsetPtr  pRecord;
	//检索到所有属性
	try   
	{   
		HRESULT hr;
		hr = pRecord.CreateInstance(__uuidof(Recordset)); 
		CString strSQL;
		strSQL.Format("SELECT * FROM %s",m_strTableName);
		pRecord->Open((char *)_bstr_t(strSQL),_variant_t((IDispatch*)m_pConRDB,true),
			adOpenStatic,adLockOptimistic,adCmdText);		
	}   
	
	catch(_com_error e)
	{
		pRecord->Close() ;
		return ;
	}
	//
	while(!pRecord->adoEOF)
	{
		DiscreteRecord *pData=new DiscreteRecord();

		CString U_division_code =(char *)_bstr_t(pRecord->GetCollect("division_code"));		
		pData->str_division_code=U_division_code;
		CString U_code_despe =(char *)_bstr_t(pRecord->GetCollect("code_desp"));		
		pData->str_code_desp=U_code_despe;
		CString U_low_value =(char *)_bstr_t(pRecord->GetCollect("low_value"));		
		pData->str_low_value=U_low_value;
		CString U_high_value =(char *)_bstr_t(pRecord->GetCollect("high_value"));		
		pData->str_high_value=U_high_value;	

		m_DiscreteContent.Add(pData);
		
		pRecord->MoveNext();
	}
	pRecord->Close();

}
//计算函数，实现本模块的计算
void CDiscreteTableBlock::Calc() 
{
	//获得输入端口的数据
	GetInputValueFromGlobalWS();
    
	CCalcPort *pInPutPort = GetInputPortObj(0);		
	double dInput;
	pInPutPort->GetPortUniValue().GetDoubleVal(&dInput);
	//查表
    for(int i=0; i<m_DiscreteContent.GetSize(); i++)
	{
		if(dInput>=atof(m_DiscreteContent[i]->str_low_value)  && dInput<atof(m_DiscreteContent[i]->str_high_value) )
		{
			CCalcPort *pPortObjOut = GetOutputPortObj(0);
			CUniValue UniVar;
			UniVar.SetValueType(1);//设置为VALUE_RTVALUE_SERIES类型
			for(int j=0; j<m_DiscreteContent[i]->str_division_code.GetLength(); j++)
			{
				int shiValue=m_DiscreteContent[i]->str_division_code.GetAt(j);				
				UniVar.AddData((double)shiValue);							
			}
			///////////////////////////////////	
			pPortObjOut->SetPortUniValue(UniVar);
			//输出计算
			OutputResultToGlobalWS();
			break;	
				
		}
	}
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CDiscreteTableBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("DiscreteTable")==0)
	{
		if(!strItem1.IsEmpty())
			iComIndex=atoi(strItem1);	
		if(!strItem2.IsEmpty())
			m_strDBType=strItem2;
		if(!strItem3.IsEmpty())
			m_strTableName=strItem3;
	}
}
//重载以显示不同的属性对象对话框
void CDiscreteTableBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgDiscreteTable  dlg;
	//用于属性窗口
	dlg.iComIndex=iComIndex;
	m_strDBDesp=dlg.m_strDBDesp;
	dlg.m_strDBType=m_strDBType;
	dlg.m_strTableName=m_strTableName;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);
	
    //用于属性窗口中的查询
	//获得输入端口的数据
	GetInputValueFromGlobalWS();
	CCalcPort *pInPutPort = GetInputPortObj(0);		
	double dInput;
	pInPutPort->GetPortUniValue().GetDoubleVal(&dInput);
	dlg.m_dFromInput=dInput;
	
	dlg.m_pModelPro=m_pModel;

	//保存
	if(dlg.DoModal()==IDOK)
	{
		iComIndex=dlg.iComIndex;
		m_strDBType=dlg.m_strDBType;
		m_strTableName=dlg.m_strTableName;

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
void CDiscreteTableBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{

	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1-4：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("DiscreteTable"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t((double)iComIndex) );
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(m_strDBType) );
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(m_strTableName) );

	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CDiscreteTableBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	iComIndex=((CDiscreteTableBlock*)pBlock)->iComIndex;
	m_strDBType=((CDiscreteTableBlock*)pBlock)->m_strDBType;
	m_strTableName=((CDiscreteTableBlock*)pBlock)->m_strTableName;
}
//将属性放置传输数据中
void CDiscreteTableBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

	CString str;
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"DiscreteTable");
	str.Format("%d",iComIndex);strcpy(pData->m_cValueItem1,str);
	str.Format("%s",m_strDBType);strcpy(pData->m_cValueItem2,str);
	str.Format("%s",m_strTableName);strcpy(pData->m_cValueItem2,str);

	dataArr.Add(pData);

}