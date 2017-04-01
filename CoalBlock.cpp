// CoalBlock.cpp: implementation of the CCoalBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "CoalBlock.h"
#include "PropertyDlgCoal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCoalBlock::CCoalBlock()
{
	m_bFixMf=1;
	m_dMf=10;
	m_iMs=0;
	m_iS=0;
	m_iAshAr=0;
	m_iLwCoalAr=0;
	m_iH=0;

	SetBlockType(_T( "Coal" ) );//设置类型
	ConstructBlockInputOutput ();
}

CCoalBlock::~CCoalBlock()
{

}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CCoalBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("iMs"), VALUE_DOUBLE, _T("in1"), _T("in1"));
	AddInputPort(_T("iAshAr"), VALUE_DOUBLE, _T("in2"), _T("in2"));
	AddInputPort(_T("iLwCoalAr"), VALUE_DOUBLE, _T("in3"), _T("in3"));
	
	AddOutputPort("Ms",VALUE_DOUBLE,"out1");
	AddOutputPort("AshAr",VALUE_DOUBLE,"out2");
	AddOutputPort("LwCoalAr",VALUE_DOUBLE,"out3");
	AddOutputPort("H",VALUE_DOUBLE,"out4");
}
//根据类型新建对象的函数
CCalcBlock* CCoalBlock::CreateFromString( const CString& strType )
{
	CCoalBlock* obj = new CCoalBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CCoalBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CCoalBlock::Calc()//进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程
	CCalcPort *pOutPort_Ms = GetOutputPortObj(0);
	CCalcPort *pOutPort_AshAr = GetOutputPortObj(1);
	CCalcPort *pOutPort_LwCoalAr = GetOutputPortObj(2);
	CCalcPort *pOutPort_H = GetOutputPortObj(3);

	CCalcPort *pInput_Ms=GetInputPortObj(0);
	CCalcPort *pInput_AshAr=GetInputPortObj(1);
	CCalcPort *pInput_LwCoalAr=GetInputPortObj(2);
	CCalcPort *pInput_Mf=NULL;
	if(!m_bFixMf)
		pInput_Mf=GetInputPortObj(3);
	
	//水分0.全水分（收到基水分）1.分析基水分
	double dMs,dMf,dMad,dResultMs;
	pInput_Ms->GetPortUniValue().GetDoubleVal(&dMs);
	if(!m_bFixMf)
		pInput_Mf->GetPortUniValue().GetDoubleVal(&dMf);
	else
		dMf=m_dMf;
	CUniValue UniVarMs;
	if(this->m_iMs==0)
	{
		dResultMs=dMs;
		UniVarMs.AddData(dResultMs);
		dMad=(dMs - dMf)*100/(100 - dMf);
	}
	else if(this->m_iMs==1)
	{
		dResultMs=dMf + dMs*(100-dMf)/100;
		UniVarMs.AddData(dResultMs);
		dMad=dMs;
	}
	pOutPort_Ms->SetPortUniValue(UniVarMs);
	
	//灰分计算  0.应用基灰分 1.分析基灰分 2.干燥基灰分 3.干燥无灰基灰分
	double dAshAr,dResultAshAr;
	pInput_AshAr->GetPortUniValue().GetDoubleVal(&dAshAr);
	CUniValue UniVarAshAr;
	if(this->m_iAshAr==0)
	{
		UniVarAshAr.AddData(dAshAr);
	}
	else if(this->m_iAshAr==1)
	{
		dResultAshAr=dAshAr*(100-dResultMs)/(100-dMad);
		UniVarAshAr.AddData(dResultAshAr);
	}
	else if(this->m_iAshAr==2)
	{
		dResultAshAr=dAshAr*(100-dResultMs)/100;
		UniVarAshAr.AddData(dResultAshAr);
	}
	else if(this->m_iAshAr==3)
	{
		dResultAshAr=dAshAr*(100-dResultMs)/(100+dAshAr);
		UniVarAshAr.AddData(dResultAshAr);
	}
	pInput_AshAr->SetPortUniValue(UniVarAshAr);

	//低位发热量计算 0.收到基低位发热量 1.分析基低位发热量 2.干燥基低位发热量 3.干燥无灰基低位发热量
	double dLwCoalAr,dResultLwCoalAr;
	pInput_LwCoalAr->GetPortUniValue().GetDoubleVal(&dLwCoalAr);
	CUniValue UniVarLwCoalAr;
	if(this->m_iLwCoalAr==0)
	{
		dResultLwCoalAr=dLwCoalAr;
		UniVarLwCoalAr.AddData(dResultLwCoalAr);
	}
	else if(this->m_iLwCoalAr==1)
	{
		dResultLwCoalAr=dLwCoalAr*(100-dResultMs)/(100-dMad);
		UniVarLwCoalAr.AddData(dResultLwCoalAr);
	}
	else if(this->m_iLwCoalAr==2)
	{
		dResultLwCoalAr=dLwCoalAr*(100-dResultMs)/100;
		UniVarLwCoalAr.AddData(dResultLwCoalAr);
	}
	else if(this->m_iLwCoalAr==3)
	{
		dResultLwCoalAr=dLwCoalAr*(100-dResultMs-dResultAshAr)/100;
		UniVarLwCoalAr.AddData(dResultLwCoalAr);
	}
	pInput_LwCoalAr->SetPortUniValue(UniVarLwCoalAr);
	
	//氢含量计算 0.无烟煤 1.贫煤 2.烟煤(20~30) 3.烟煤(30~40) 4.长焰煤 5.褐煤
	double dResultH;
	CUniValue UniVarH;
	if(this->m_iH==0)
	{
		dResultH=2;
		UniVarH.AddData(dResultH);
	}
	else if(this->m_iH==1)
	{
		dResultH=3;
		UniVarH.AddData(dResultH);
	}
	else if(this->m_iH==1)
	{
		dResultH=3;
		UniVarH.AddData(dResultH);
	}
	else if(this->m_iH==1)
	{
		dResultH=3.8;
		UniVarH.AddData(dResultH);
	}
	else if(this->m_iH==1)
	{
		dResultH=3.5;
		UniVarH.AddData(dResultH);
	}
	else if(this->m_iH==1)
	{
		dResultH=3.5;
		UniVarH.AddData(dResultH);
	}
	pOutPort_H->SetPortUniValue(UniVarH);

	//输出到工作空间中
	OutputResultToGlobalWS();
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CCoalBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("Mf")==0)
	{
		if(!strItem1.IsEmpty())  m_bFixMf=atoi(strItem1);
		if(!strItem2.IsEmpty())  m_dMf=atof(strItem2);
	}
	if(strPropName.Compare("Ms")==0)
	{
		if(!strItem1.IsEmpty())  m_iMs=atoi(strItem1);
	}
	if(strPropName.Compare("S")==0)
	{
		if(!strItem1.IsEmpty())  m_iS=atoi(strItem1);
	}
	if(strPropName.Compare("AshAr")==0)
	{
		if(!strItem1.IsEmpty())  m_iAshAr=atoi(strItem1);
	}
	if(strPropName.Compare("LwCoalAr")==0)
	{
		if(!strItem1.IsEmpty())  m_iLwCoalAr=atoi(strItem1);
	}
	if(strPropName.Compare("H")==0)
	{
		if(!strItem1.IsEmpty())  m_iH=atoi(strItem1);
	}
}
//重载以显示不同的属性对象对话框
void CCoalBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgCoal dlg;
    //给对话框变量赋值
	dlg.m_bFixMf = this->m_bFixMf;
	dlg.m_dMf=this->m_dMf;
	dlg.m_iAshAr=this->m_iAshAr;
	dlg.m_iH=this->m_iH;
	dlg.m_iLwCoalAr=this->m_iLwCoalAr;
	dlg.m_iMs=this->m_iMs;
	dlg.m_iS=this->m_iS;
	///////////////////////////////////////////////////
	if(dlg.DoModal()==IDOK)
	{
		m_bFixMf = dlg.m_bFixMf;
		m_dMf=dlg.m_dMf;
		m_iAshAr=dlg.m_iAshAr;
		m_iH=dlg.m_iH;
		m_iLwCoalAr=dlg.m_iLwCoalAr;
		m_iMs=dlg.m_iMs;
		m_iS=dlg.m_iS;

		if((this->GetInputNum()==3)&&(!m_bFixMf))
		{
			AddInputPort(_T("iMf"), VALUE_DOUBLE, _T("in4"), _T("in4"));
		}
		else if((this->GetInputNum()==4)&&(m_bFixMf))
		{
			this->ClearInputPortByIndex(3);
		}
		if((m_pModel!=NULL)&&(this->m_pModel->m_iTagModeLocalOrRemote==1))//远程
		{
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> dataArr;
			SetBlockPropertyToTransferData(dataArr);//获取要传输的属性到传送数组中
			SendBlockPropertyTransferData(dataArr);// 传送数组中数据
			ClearPropertyTransferData(dataArr);//释放数据
		}
	}
	//////////////////////////////////////////////////
}
//重载以将本模块写入到数据库中
void CCoalBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="Mf";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_bFixMf); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%f",m_dMf); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="Ms";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iMs); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="S";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iS); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="AshAr";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iAshAr); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="LwCoalAr";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iLwCoalAr); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="H";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iH); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->Update();
}
void CCoalBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_dMf=((CCoalBlock*)pBlock)->m_dMf;
	m_bFixMf=((CCoalBlock*)pBlock)->m_bFixMf;
	m_iH=((CCoalBlock*)pBlock)->m_iH;
	m_iLwCoalAr=((CCoalBlock*)pBlock)->m_iLwCoalAr;
	m_iMs=((CCoalBlock*)pBlock)->m_iMs;
	m_iS=((CCoalBlock*)pBlock)->m_iS;
	m_iAshAr=((CCoalBlock*)pBlock)->m_iAshAr;
}
	//将属性放置传输数据中
void CCoalBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Mf");
	CString str;
	str.Format("%d",m_bFixMf);strcpy(pData->m_cValueItem1,str);
	str.Format("%f",m_dMf);strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Ms");
	str.Format("%d",m_iMs);strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"S");
	str.Format("%d",m_iS);strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"AshAr");
	str.Format("%d",m_iAshAr);strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"LwCoalAr");
	str.Format("%d",m_iLwCoalAr);strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"H");
	str.Format("%d",m_iH);strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
