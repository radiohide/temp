// WASPCalcBlock.cpp: implementation of the CWASPCalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WASPCalcBlock.h"
#include "PropertyDlgWASP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPtrArray ListPoteDll;
CWASPCalcBlock::CWASPCalcBlock()
{
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;
}

CWASPCalcBlock::~CWASPCalcBlock()
{
	if (PSourceValue)
	{
		delete[] PSourceValue;
		PSourceValue=NULL;
	}
	if (pG)
	{
		delete pG;
		pG=NULL;
	}
}

//计算函数，实现本模块的计算
void CWASPCalcBlock::Calc() //进行计算的函数,需要重载
{
	rtFunc = NULL;
	for (int i=0; i<(ListPoteDll).GetSize(); i++)
	{
		CPoteDll* PoteDll = (CPoteDll*)((ListPoteDll)[i]);
		for (int flag=0; flag<PoteDll->lstPoteFuncName.GetSize();flag++)
		{
			if (PoteDll->lstPoteFuncName[flag] == m_strFuncName)
			{
				rtFunc = (RTFUNC)PoteDll->lstPoteFuncAddr[flag];
				if (rtFunc)	break;
			}
		}
		if (rtFunc)	break;
	}
	if(rtFunc)
	{
		GetInputValueFromGlobalWS();
		//计算过程
		CCalcPort *pOutPortObj = GetOutputPortObj(0);
		CCalcPort *pInput1Obj=GetInputPortObj(0);
		CCalcPort *pInput2Obj=GetInputPortObj(1);
		
		double inValue1,inValue2;
		pInput1Obj->GetPortUniValue().GetDoubleVal(&inValue1);
		pInput2Obj->GetPortUniValue().GetDoubleVal(&inValue2);
		
		PSourceValue[0].dValue = inValue1;
		PSourceValue[0].wState = 1;
		PSourceValue[0].tTime=CTime::GetCurrentTime().GetTime();
		
		PSourceValue[1].dValue = inValue2;
		PSourceValue[1].wState = 1;
		PSourceValue[1].tTime=CTime::GetCurrentTime().GetTime();
		
		*pG = rtFunc(PSourceValue, 2);
		CUniValue UniVar;
		UniVar.AddData(pG->dValue);
		pOutPortObj->SetPortUniValue(UniVar);
		//输出到工作空间中
		OutputResultToGlobalWS();
	}
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CWASPCalcBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("strFuncName")==0)
	{
		if(!strItem1.IsEmpty())  m_strFuncName=strItem1;
	}
}
//重载以显示不同的属性对象对话框
void CWASPCalcBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//计算块原有的输出端口描述
	IDArrRightOut.RemoveAll();

	CStringArray  IDArrRightIn;//计算块内部的输出端口描述
    IDArrRightIn.RemoveAll();

	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}

	CPropertyDlgWASP dlg;
	dlg.m_pStrArrFunDisp=&m_strArrFunDisp;
	dlg.m_pStrArrFunName=&m_strArrFunName;
	dlg.m_strBlockDesp=this->m_strBlockDisp;
	dlg.m_strFunName=this->m_strFuncName;
	if(dlg.DoModal()==IDOK)
	{
		m_strFuncName=dlg.m_strFunName;
		int index=-1;
		for(int i=0;i<m_strArrFunName.GetSize();i++)
		{
			if(m_strFuncName==m_strArrFunName[i])
			{
				index=i;
				break;
			}
		}

	    //对输出端口的修改
		if(index!=-1)
			IDArrRightIn.Add(m_strArrOutPortName[index]);
		else
			IDArrRightIn.Add("out");
		
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
}
//重载以将本模块写入到数据库中
void CWASPCalcBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
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
void CWASPCalcBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strFuncName=((CWASPCalcBlock*)pBlock)->m_strFuncName;
}
//将属性放置传输数据中
void CWASPCalcBlock::SetBlockPropertyToTransferData(
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
