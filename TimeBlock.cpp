// TimeBlock.cpp: implementation of the CTimeBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "TimeBlock.h"
#include "PropertyDlgTime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeBlock::CTimeBlock()
{
	SetBlockType(_T("Time") );
	m_strDateTime="";
	m_iTheTimeOrCurrent=0;
	ConstructBlockInputOutput();
}

CTimeBlock::~CTimeBlock()
{

}

void CTimeBlock::ConstructBlockInputOutput ()
{
	//可自由定义输入输出的变量名称。
	AddOutputPort("T", VALUE_DOUBLE);
}
//根据类型新建对象的函数
CCalcBlock* CTimeBlock::CreateFromString( const CString& strType )
{
	CTimeBlock* obj = new CTimeBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CTimeBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CTimeBlock::Calc()//进行计算的函数,需要重载
{
	CUniValue UniVar1;
	CCalcPort *pOutPortObj=GetOutputPortObj(0);
	if(0==m_iTheTimeOrCurrent)//当前时间
	{
		UniVar1.AddData(CTime::GetCurrentTime().GetTime());
	}
	else
	{
		int   nYear,   nMonth,   nDate;
		int   nHour=0;
		int   nMin=0;
		int   nSec=0;    
		sscanf(m_strDateTime, "%d-%d-%d %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);
		CTime tDateTime(nYear,   nMonth,   nDate,nHour,nMin,nSec);
		UniVar1.AddData(tDateTime.GetTime());
	}
	pOutPortObj->SetPortUniValue(UniVar1);
   //输出到工作空间中
	OutputResultToGlobalWS();
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CTimeBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(strPropName.Compare("BeTheTimeOrCurrent")==0)
	{
		if(!strItem1.IsEmpty())
			m_iTheTimeOrCurrent=atoi(strItem1);
		if(!strItem2.IsEmpty())
			m_strDateTime=strItem2;
		else
			m_strDateTime="";
	}
}
//重载以显示不同的属性对象对话框
void CTimeBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgTime dlg;
	dlg.m_iTheTimeOrCurrent=this->m_iTheTimeOrCurrent;
	dlg.m_strDateTime=m_strDateTime;
	if(IDOK==dlg.DoModal())
	{
		this->m_iTheTimeOrCurrent=dlg.m_iTheTimeOrCurrent;
		m_strDateTime=dlg.m_strDateTime;
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
void CTimeBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("BeTheTimeOrCurrent"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
	strField.Format("%d",m_iTheTimeOrCurrent); 
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(strField) );
	strField.Format("%s",m_strDateTime); 
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(strField) );
		
	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CTimeBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_iTheTimeOrCurrent=((CTimeBlock*)pBlock)->m_iTheTimeOrCurrent;
	m_strDateTime=((CTimeBlock*)pBlock)->m_strDateTime;
}
//将属性放置传输数据中
void CTimeBlock::SetBlockPropertyToTransferData(
											CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"BeTheTimeOrCurrent");
	
	CString str;
	str.Format("%d",m_iTheTimeOrCurrent);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%s",m_strDateTime);
	strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);
}
