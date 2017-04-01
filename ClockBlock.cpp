// ClockBlock.cpp: implementation of the CClockBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "ClockBlock.h"
#include "PropertyDlgClock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClockBlock::CClockBlock()
{
	SetBlockType(_T("Clock") );
	m_iBeHaveTime=1;//0:不输出，1：输出
	m_iBeHaveBeginEndTime=0;//0:不输出，1：输出
	ConstructBlockInputOutput();
}

CClockBlock::~CClockBlock()
{
	
}
void CClockBlock::ConstructBlockInputOutput ()
{
	//可自由定义输入输出的变量名称。
	ClearOutputPorts();
	if(1==m_iBeHaveTime)
	{
		AddOutputPort("Time", VALUE_DOUBLE);
	}
	if(1==m_iBeHaveBeginEndTime)
	{
		AddOutputPort("StartT", VALUE_DOUBLE);
		AddOutputPort("EndT", VALUE_DOUBLE);
	}
}
	//根据类型新建对象的函数
CCalcBlock* CClockBlock::CreateFromString( const CString& strType )
{
	CClockBlock* obj = new CClockBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CClockBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_iBeHaveTime=((CClockBlock*)pBlock)->m_iBeHaveTime;
	m_iBeHaveBeginEndTime=((CClockBlock*)pBlock)->m_iBeHaveBeginEndTime;
}
	//初始化计算的函数做特殊的初始化
void CClockBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
	//计算函数，实现本模块的计算
void CClockBlock::Calc() //进行计算的函数,需要重载
{
	if((1==m_iBeHaveTime)&&(1==m_iBeHaveBeginEndTime))//全部输出
	{
		CUniValue UniVar1;
		CCalcPort *pOutPortObj=GetOutputPortObj(0);
		UniVar1.AddData(this->m_pModel->m_lDataTime);
		pOutPortObj->SetPortUniValue(UniVar1);

		CUniValue UniVar2;
		pOutPortObj=GetOutputPortObj(1);
		UniVar2.AddData(this->m_pModel->m_lDataSTime);
		pOutPortObj->SetPortUniValue(UniVar2);

		CUniValue UniVar3;
		pOutPortObj=GetOutputPortObj(2);
		UniVar3.AddData(this->m_pModel->m_lDataETime);
		pOutPortObj->SetPortUniValue(UniVar3);
	}
	if((1==m_iBeHaveTime)&&(1!=m_iBeHaveBeginEndTime))//只输出时刻
	{
		CUniValue UniVar1;
		CCalcPort *pOutPortObj=GetOutputPortObj(0);
		UniVar1.AddData(this->m_pModel->m_lDataTime);
		pOutPortObj->SetPortUniValue(UniVar1);
	}
	if((1!=m_iBeHaveTime)&&(1==m_iBeHaveBeginEndTime))//只输出时段
	{
		CUniValue UniVar2;
		CCalcPort *pOutPortObj=GetOutputPortObj(0);
		UniVar2.AddData(this->m_pModel->m_lDataSTime);
		pOutPortObj->SetPortUniValue(UniVar2);

		CUniValue UniVar3;
		pOutPortObj=GetOutputPortObj(1);
		UniVar3.AddData(this->m_pModel->m_lDataETime);
		pOutPortObj->SetPortUniValue(UniVar3);
	}
	
	////////////////////////////////////////////////////////////////////////
   //输出到工作空间中
	OutputResultToGlobalWS();
}
void CClockBlock::SetBlockPropertyToTransferData(
				CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)//将属性放置传输数据中
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"BeHaveTime");
	
	CString str;
	str.Format("%d",m_iBeHaveTime);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"BeHaveBeginEndTime");
	
	str.Format("%d",m_iBeHaveBeginEndTime);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
    //用于根据参数项和值进行属性的设置，当读取值要用
void CClockBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(strPropName.Compare("BeHaveTime")==0)
	{
		if(!strItem1.IsEmpty())
			m_iBeHaveTime=atoi(strItem1);
	}
	else if(strPropName.Compare("BeHaveBeginEndTime")==0)
	{
		if(!strItem1.IsEmpty())
			m_iBeHaveBeginEndTime=atoi(strItem1);
	}
}
	//重载以显示不同的属性对象对话框
void CClockBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//计算块原有的输出端口描述
	IDArrRightOut.RemoveAll();

	CStringArray  IDArrRightIn;//计算块内部的输入端口描述
    IDArrRightIn.RemoveAll();
	

	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}
	//
	CPropertyDlgClock dlg;
    //给对话框变量赋值
	///////////////////////////////////////////////////
	//int tagnum = m_listIn.GetCount();
	dlg.m_iBeHaveTime=m_iBeHaveTime;
	dlg.m_iBeHaveBeginEndTime=m_iBeHaveBeginEndTime;
	////////////////////////////////////////////////////////////////////
	if(dlg.DoModal()==IDOK)
	{
		m_iBeHaveTime = dlg.m_iBeHaveTime;
		m_iBeHaveBeginEndTime=dlg.m_iBeHaveBeginEndTime;
		
		//对输出端口的修改
		if(1==m_iBeHaveTime)
		{
			//AddOutputPort("Time", VALUE_DOUBLE);
			IDArrRightIn.Add("Time");
		}
		if(1==m_iBeHaveBeginEndTime)
		{
			//AddOutputPort("StartT", VALUE_DOUBLE);
			//AddOutputPort("EndT", VALUE_DOUBLE);
			IDArrRightIn.Add("StartT");
			IDArrRightIn.Add("EndT");
		}
		for(int jj=IDArrRightOut.GetSize()-1; jj>-1; jj--)//删除原有而不用的Port,倒着删
		{
			int  runs=0;//遍历次数
			for(int kk=0; kk<IDArrRightIn.GetSize(); kk++)
			{
				if(IDArrRightIn[kk]!=IDArrRightOut[jj])	
				{runs++;}
			}
			if(runs==IDArrRightIn.GetSize())
			{
				this->ClearOutputPortByIndex(jj,parent);
			}
		}		
		for(int mm=0; mm<IDArrRightIn.GetSize(); mm++)//添加新的Port
		{
			int  runs=0;//遍历次数
			for(int nn=0; nn<IDArrRightOut.GetSize(); nn++)
			{
				if(IDArrRightIn[mm]!=IDArrRightOut[nn])
				{runs++;}		
			}
			if(runs==IDArrRightOut.GetSize())
			{
				CString str=IDArrRightIn[mm];
				InSertOutputPortBefore(mm,str, VALUE_DOUBLE, _T(str));
			}
		}
		parent->RedrawWindow();
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
void CClockBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("BeHaveTime"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
	strField.Format("%d",m_iBeHaveTime); 
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(strField) );

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("BeHaveBeginEndTime"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
	strField.Format("%d",m_iBeHaveBeginEndTime); 
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(strField) );

	pRecord->Update();
}
