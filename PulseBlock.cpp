// PulseBlock.cpp: implementation of the CPulseBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "PulseBlock.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPulseBlock::CPulseBlock()
{
	m_iComIndex=0;

	m_dFengzhi = 3.0;
	m_dFengzhiNegative = 0.0;
	m_dZhouqi = 2.0;
	m_dYanchi = 0.0;
	m_dMaichongKD = 0.5;

	m_dInitValue = 0.0;
	m_dStep = 3.0;

	m_iInputNum=0;
		//SYSTME=====设置类型======
	//设置类型
	SetBlockType(_T( "Pulse" ) );
	ConstructBlockInputOutput();
	//SYSTME=====初始化本模块的属性===

}

CPulseBlock::~CPulseBlock()
{

}

//构造初始的输入输出个数
void CPulseBlock::ConstructBlockInputOutput()
{
	AddInputPort(_T("In"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	
	AddOutputPort("Out",VALUE_DOUBLE,"out1");	
}
//根据类型新建对象的函数
//同时在CCalcBlockControlFactory中，要加入相应的代码
CCalcBlock* CPulseBlock::CreateFromString( const CString& strType )
{
	CPulseBlock* obj = new CPulseBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CPulseBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CPulseBlock::Calc() 
{
	//获得输入端口的数据
	GetInputValueFromGlobalWS();
	//进行计算
	int inNum = GetInputNum();
	double m_Result=0;
    if(m_iInputNum==1)//外部输入
	{
		CCalcPort *pInPutPort = GetInputPortObj(0);		
		double dInput;
		pInPutPort->GetPortUniValue().GetDoubleVal(&dInput);
		
		if(dInput<m_dYanchi)
		{
			m_Result=m_dFengzhiNegative;
		}
		else
		{
			double dYushu=0;
			dYushu=dInput-m_dYanchi;
			double dInside=dYushu-(int)(dYushu/m_dZhouqi)*m_dZhouqi;
			if(dInside<=m_dZhouqi*m_dMaichongKD)
			{
				m_Result=m_dFengzhi;
			}
			else
			{
				m_Result=m_dFengzhiNegative;
			}	
		}
		m_dInitValue+=m_dStep;         
	}
	else if(m_iInputNum==0)
	{
		if(m_dInitValue<m_dYanchi)
		{
			m_Result=m_dFengzhiNegative;
		}
		else
		{
			double dYushu=0;
			dYushu=m_dInitValue-m_dYanchi;
			double dInside=dYushu-(int)(dYushu/m_dZhouqi)*m_dZhouqi;
			if(dInside<=m_dZhouqi*m_dMaichongKD)
			{
				m_Result=m_dFengzhi;
			}
			else
			{
				m_Result=m_dFengzhiNegative;
			}	
		}
		m_dInitValue+=m_dStep;
	}
	///////////////////////////////////
	CCalcPort *pPortObjOut = GetOutputPortObj(0);
	CUniValue UniVar;
	UniVar.AddData(m_Result);
	pPortObjOut->SetPortUniValue(UniVar);
	///////////////////////////////////
	//输出计算
	OutputResultToGlobalWS();
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CPulseBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("Pulse1")==0)
	{
		if(!strItem1.IsEmpty())
			m_dFengzhi=atof(strItem1);
		if(!strItem2.IsEmpty())
			m_dFengzhiNegative=atof(strItem2);
		if(!strItem3.IsEmpty())
			m_dZhouqi=atof(strItem3);
		if(!strItem4.IsEmpty())
			m_dYanchi=atof(strItem4);
		if(!strItem5.IsEmpty())
			m_dMaichongKD=atof(strItem5);		
	}
	if(strPropName.Compare("Pulse2")==0)
	{
		if(!strItem1.IsEmpty())
			m_iComIndex=atoi(strItem1);
		if(!strItem2.IsEmpty())
			m_dInitValue=atof(strItem2);
		if(!strItem3.IsEmpty())
			m_dStep=atof(strItem3);
	}
}
//重载以显示不同的属性对象对话框
void CPulseBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBefore=this->GetInputNum();//新增
	CStringArray  IDArrLeftOut;//计算块原有的输入端口描述
	IDArrLeftOut.RemoveAll();
	
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		IDArrLeftOut.Add(strPortID);
	}

	CPropertyDlgPulse  dlg;

	dlg.m_index =m_iComIndex;

	dlg.m_dFengzhi=m_dFengzhi;
	dlg.m_dFengzhiNegative= m_dFengzhiNegative;
	dlg.m_dZhouqi=	m_dZhouqi;
	dlg.m_dYanchi=	m_dYanchi;
	dlg.m_dMaichongKD=	m_dMaichongKD;
	dlg.m_dInitValue=	m_dInitValue;
	dlg.m_dStep=	m_dStep;
    ///
	
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.m_index==0)
		{
			m_iComIndex  =dlg.m_index;

			m_dFengzhi   =	dlg.m_dFengzhi;
			m_dFengzhiNegative =dlg.m_dFengzhiNegative;
			m_dZhouqi    =	dlg.m_dZhouqi;
			m_dYanchi    =	dlg.m_dYanchi;
			m_dMaichongKD    =	dlg.m_dMaichongKD;

			m_dInitValue =	dlg.m_dInitValue;
			m_dStep      =	dlg.m_dStep;

			m_iInputNum=1;
		}
		else if(dlg.m_index==1)
		{
			m_iComIndex  =dlg.m_index;

			m_dFengzhi   =	dlg.m_dFengzhi;
			m_dFengzhiNegative = dlg.m_dFengzhiNegative;
			m_dZhouqi    =	dlg.m_dZhouqi;
			m_dYanchi    =	dlg.m_dYanchi;
			m_dMaichongKD    =	dlg.m_dMaichongKD;
			m_dInitValue =	dlg.m_dInitValue;
			m_dStep      =	dlg.m_dStep;

			m_iInputNum=0;
		}
		if(IDArrLeftOut.GetSize()==m_iInputNum){}
		else
		{
			if(m_iInputNum==1)
			{
			ClearInputPorts();
			AddInputPort(_T("In"), VALUE_DOUBLE, _T("in1"), _T("in1"));
			}
			else
			{
				ClearInputPorts();
			}
		}
		parent->RedrawWindow();
	}
}
//重载以将本模块写入到数据库中
void CPulseBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1-4：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("Pulse1"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(m_dFengzhi) );
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(m_dFengzhiNegative) );
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(m_dZhouqi) );
	pRecord->PutCollect(_variant_t("value_item4"),_variant_t(m_dYanchi) );
	pRecord->PutCollect(_variant_t("value_item5"),_variant_t(m_dMaichongKD) );

	//属性5-6：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("Pulse2"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
    pRecord->PutCollect(_variant_t("value_item1"),_variant_t((double)m_iComIndex) );
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(m_dInitValue) );
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(m_dStep) );

	pRecord->Update();

}
void CPulseBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_dFengzhi=((CPulseBlock*)pBlock)->m_dFengzhi;
	m_dFengzhiNegative=((CPulseBlock*)pBlock)->m_dFengzhiNegative;
	m_dZhouqi=((CPulseBlock*)pBlock)->m_dZhouqi;
	m_dYanchi=((CPulseBlock*)pBlock)->m_dYanchi;
	m_dMaichongKD=((CPulseBlock*)pBlock)->m_dMaichongKD;
	m_iComIndex=((CPulseBlock*)pBlock)->m_iComIndex;
	m_dInitValue=((CPulseBlock*)pBlock)->m_dInitValue;
	m_dStep=((CPulseBlock*)pBlock)->m_dStep;
}
void CPulseBlock::SetBlockPropertyToTransferData(
				CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)//将属性放置传输数据中
{
	CString str;
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Pulse1");
	str.Format("%f",m_dFengzhi);strcpy(pData->m_cValueItem1,str);
	str.Format("%f",m_dFengzhiNegative);strcpy(pData->m_cValueItem2,str);
	str.Format("%f",m_dZhouqi);strcpy(pData->m_cValueItem3,str);
	str.Format("%f",m_dYanchi);strcpy(pData->m_cValueItem4,str);
	str.Format("%f",m_dMaichongKD);strcpy(pData->m_cValueItem5,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Pulse2");
	str.Format("%d",m_iComIndex);strcpy(pData->m_cValueItem1,str);
	str.Format("%f",m_dInitValue);strcpy(pData->m_cValueItem2,str);
	str.Format("%f",m_dStep);strcpy(pData->m_cValueItem3,str);
	dataArr.Add(pData);
}
