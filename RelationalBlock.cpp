// RelationalBlock.cpp: implementation of the CRelationalBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RelationalBlock.h"
#include "PropertyDlgMathFun.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRelationalBlock::CRelationalBlock()
{
	SetBlockType(_T( "Relational" ) );//设置类型
	m_strFuncName=">";
	m_strBlockDisp="关系计算块。请选择关系计算函数进行计算。";
	m_strPropertityTableName="关系计算块";
	
	m_strArrFunName.Add(">");m_strArrFunDisp.Add("大于");
	m_strArrFunName.Add(">=");m_strArrFunDisp.Add("大于等于");	
	m_strArrFunName.Add("<");m_strArrFunDisp.Add("小于");
	m_strArrFunName.Add("<=");m_strArrFunDisp.Add("小于等于");
	m_strArrFunName.Add("==");m_strArrFunDisp.Add("等于");
	m_strArrFunName.Add("!=");m_strArrFunDisp.Add("不等于");
	ConstructBlockInputOutput();
}

CRelationalBlock::~CRelationalBlock()
{

}
//根据类型新建对象的函数
CCalcBlock* CRelationalBlock::CreateFromString( const CString& strType )
{
	CRelationalBlock* obj = new CRelationalBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CRelationalBlock::ConstructBlockInputOutput ()
{
	AddInputPort("In1", VALUE_DOUBLE);
	AddInputPort("In2", VALUE_DOUBLE);
	AddOutputPort("Out", VALUE_DOUBLE);
}
//初始化计算的函数做特殊的初始化
void CRelationalBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CRelationalBlock::Calc()//进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	CCalcPort *pInput1Obj=GetInputPortObj(0);
	CCalcPort *pInput2Obj=GetInputPortObj(1);
	
	double inValue1,inValue2;
	pInput1Obj->GetPortUniValue().GetDoubleVal(&inValue1);
	pInput2Obj->GetPortUniValue().GetDoubleVal(&inValue2);
	
	CUniValue UniVar;
	if(m_strFuncName==">")
	{
		UniVar.AddData(inValue1>inValue2);
	}
	else if(m_strFuncName==">=")
	{
		UniVar.AddData(inValue1>=inValue2);
	}
	else if(m_strFuncName=="<")
	{
		UniVar.AddData(inValue1<inValue2);
	}
	else if(m_strFuncName=="<=")
	{
		UniVar.AddData(inValue1<=inValue2);
	}
	else if(m_strFuncName=="==")
	{
		UniVar.AddData(inValue1==inValue2);
	}
	else if(m_strFuncName=="!=")
	{
		UniVar.AddData(inValue1!=inValue2);
	}
	pOutPortObj->SetPortUniValue(UniVar);
	//输出到工作空间中
	OutputResultToGlobalWS();
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CRelationalBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("strFuncName")==0)
	{
		if(!strItem1.IsEmpty())  m_strFuncName=strItem1;
	}
}
//重载以显示不同的属性对象对话框
void CRelationalBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgMathFun dlg;
	dlg.m_pStrArrFunDisp=&m_strArrFunDisp;
	dlg.m_pStrArrFunName=&m_strArrFunName;
	dlg.m_strBlockDesp=this->m_strBlockDisp;
	dlg.m_strFunName=this->m_strFuncName;
	dlg.m_strPropertityTableName=m_strPropertityTableName;
	if(dlg.DoModal()==IDOK)
	{
		m_strFuncName=dlg.m_strFunName;
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
void CRelationalBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="strFuncName";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strFuncName); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	pRecord->Update();
}

//复制图形时，需要拷贝属性。
void CRelationalBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strFuncName=((CRelationalBlock*)pBlock)->m_strFuncName;
}
//将属性放置传输数据中
void CRelationalBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"strFuncName");
	
	CString str;
	str.Format("%s",m_strFuncName);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
