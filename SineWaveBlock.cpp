// SineWaveBlock.cpp: implementation of the CSineWaveBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "SineWaveBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include<math.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSineWaveBlock::CSineWaveBlock()
{
	m_iComIndex=0;

	m_dFengzhi=1;
	m_dZhouqi=1;
	m_dLeftRight=0;
	m_dUpDown=0;
	m_dInitValue=0;
	m_dStep=3;

	m_iInputNum=0;
	//SYSTME=====设置类型======
	//设置类型
	SetBlockType(_T( "SineWave" ) );
	ConstructBlockInputOutput();
	//SYSTME=====初始化本模块的属性===

}

CSineWaveBlock::~CSineWaveBlock()
{

}
//构造初始的输入输出个数
void CSineWaveBlock::ConstructBlockInputOutput()
{
	AddInputPort(_T("In"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	
	AddOutputPort("Out",VALUE_DOUBLE,"out1");	
}
//根据类型新建对象的函数
//同时在CCalcBlockControlFactory中，要加入相应的代码
CCalcBlock* CSineWaveBlock::CreateFromString( const CString& strType )
{
	CSineWaveBlock* obj = new CSineWaveBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CSineWaveBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CSineWaveBlock::Calc() 
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
		m_Result=m_dFengzhi*sin(m_dZhouqi*dInput+m_dLeftRight)+m_dUpDown;          
	}
	else if(m_iInputNum==0)
	{
		m_Result=m_dFengzhi*sin(m_dZhouqi*m_dInitValue+m_dLeftRight)+m_dUpDown; 
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
void CSineWaveBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("SineWave1")==0)
	{
		if(!strItem1.IsEmpty())
			m_dFengzhi=atof(strItem1);
		if(!strItem2.IsEmpty())
			m_dZhouqi=atof(strItem2);
		if(!strItem3.IsEmpty())
			m_dLeftRight=atof(strItem3);
		if(!strItem4.IsEmpty())
			m_dUpDown=atof(strItem4);		
	}
	if(strPropName.Compare("SineWave2")==0)
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
void CSineWaveBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
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

	CPropertyDlgSineWave  dlg;

	dlg.m_index =m_iComIndex;

	dlg.m_dFengzhi=m_dFengzhi;
	dlg.m_dZhouqi=	m_dZhouqi;
	dlg.m_dLeftRight=	m_dLeftRight;
	dlg.m_dUpDown=	m_dUpDown;
	dlg.m_dInitValue=	m_dInitValue;
	dlg.m_dStep=	m_dStep;
    ///
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.m_index==0)
		{
			m_iComIndex  =dlg.m_index;

			m_dFengzhi   =	dlg.m_dFengzhi;
			m_dZhouqi    =	dlg.m_dZhouqi;
			m_dLeftRight =	dlg.m_dLeftRight;
			m_dUpDown    =	dlg.m_dUpDown;

		//	m_dInitValue =	dlg.m_dInitValue;
		//	m_dStep      =	dlg.m_dStep;

			m_iInputNum=1;

		}
		else if(dlg.m_index==1)
		{
			m_iComIndex  =dlg.m_index;

			m_dFengzhi   =	dlg.m_dFengzhi;
			m_dZhouqi    =	dlg.m_dZhouqi;
			m_dLeftRight =	dlg.m_dLeftRight;
			m_dUpDown    =	dlg.m_dUpDown;
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
void CSineWaveBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1-4：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("SineWave1"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(m_dFengzhi) );
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(m_dZhouqi) );
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(m_dLeftRight) );
	pRecord->PutCollect(_variant_t("value_item4"),_variant_t(m_dUpDown) );

	//属性5-6：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("SineWave2"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
    pRecord->PutCollect(_variant_t("value_item1"),_variant_t((double)m_iComIndex) );
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(m_dInitValue) );
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(m_dStep) );

	pRecord->Update();

}
//复制图形时，需要拷贝属性。
void CSineWaveBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_iComIndex=((CSineWaveBlock*)pBlock)->m_iComIndex;
	m_dFengzhi=((CSineWaveBlock*)pBlock)->m_dFengzhi;
	m_dZhouqi=((CSineWaveBlock*)pBlock)->m_dZhouqi;
	m_dLeftRight=((CSineWaveBlock*)pBlock)->m_dLeftRight;
	m_dUpDown=((CSineWaveBlock*)pBlock)->m_dUpDown;
	m_dInitValue=((CSineWaveBlock*)pBlock)->m_dInitValue;
	m_dStep=((CSineWaveBlock*)pBlock)->m_dStep;
}
//将属性放置传输数据中
void CSineWaveBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CString str;
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"SineWave1");
	str.Format("%f",m_dFengzhi);strcpy(pData->m_cValueItem1,str);
	str.Format("%f",m_dZhouqi);strcpy(pData->m_cValueItem2,str);
	str.Format("%f",m_dLeftRight);strcpy(pData->m_cValueItem3,str);
	str.Format("%f",m_dUpDown);strcpy(pData->m_cValueItem4,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"SineWave2");
	str.Format("%d",m_iComIndex);strcpy(pData->m_cValueItem1,str);
	str.Format("%f",m_dInitValue);strcpy(pData->m_cValueItem2,str);
	str.Format("%f",m_dStep);strcpy(pData->m_cValueItem3,str);
	dataArr.Add(pData);
}