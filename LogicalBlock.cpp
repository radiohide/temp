// LogicalBlock.cpp: implementation of the CLogicalBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "LogicalBlock.h"
#include "PropertyDlgMathFun.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogicalBlock::CLogicalBlock()
{
	SetBlockType(_T( "Logical" ) );//设置类型
	m_strFuncName="&&";
	m_strBlockDisp="逻辑计算块。请选择逻辑计算函数进行计算。";
	m_strPropertityTableName="逻辑计算块";
	m_iInputCount=2;
	
	m_strArrFunName.Add("&&");m_strArrFunDisp.Add("与");
	m_strArrFunName.Add("||");m_strArrFunDisp.Add("或");	
	m_strArrFunName.Add("!");m_strArrFunDisp.Add("非");
	m_strArrFunName.Add("~&");m_strArrFunDisp.Add("非与");
	m_strArrFunName.Add("~|");m_strArrFunDisp.Add("非或");
	m_strArrFunName.Add("^");m_strArrFunDisp.Add("异或");
	ConstructBlockInputOutput();
}

CLogicalBlock::~CLogicalBlock()
{
	
}

//根据类型新建对象的函数
CCalcBlock* CLogicalBlock::CreateFromString( const CString& strType )
{
	CLogicalBlock* obj = new CLogicalBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CLogicalBlock::ConstructBlockInputOutput ()
{
	ClearInputPorts();
	ClearOutputPorts();
	CString str;
	for(UINT i=0;i<m_iInputCount;i++)
	{
		str.Format("In%d",i+1);
		AddInputPort(str, VALUE_DOUBLE);
	}
	AddOutputPort("Out", VALUE_DOUBLE);
}
//初始化计算的函数做特殊的初始化
void CLogicalBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CLogicalBlock::Calc()//进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	int result=0;
	double inValue;
	if(m_strFuncName=="&&")
	{
		result=1;
		for(UINT i=0;i<m_iInputCount;i++)
		{
			CCalcPort *pInputObj=GetInputPortObj(i);
			pInputObj->GetPortUniValue().GetDoubleVal(&inValue);
			result=result&&inValue;
		}
	}
	else if(m_strFuncName=="||")
	{
		result=0;
		for(UINT i=0;i<m_iInputCount;i++)
		{
			CCalcPort *pInputObj=GetInputPortObj(i);
			pInputObj->GetPortUniValue().GetDoubleVal(&inValue);
			result=result||inValue;
		}
	}
	else if(m_strFuncName=="!")
	{
		CCalcPort *pInputObj=GetInputPortObj(0);
		pInputObj->GetPortUniValue().GetDoubleVal(&inValue);
		result=!inValue;
	}
	else if(m_strFuncName=="~&")
	{
		result=1;
		for(UINT i=0;i<m_iInputCount;i++)
		{
			CCalcPort *pInputObj=GetInputPortObj(i);
			pInputObj->GetPortUniValue().GetDoubleVal(&inValue);
			result=result&&inValue;
		}
		result=!result;
	}
	else if(m_strFuncName=="~|")
	{
		result=0;
		for(UINT i=0;i<m_iInputCount;i++)
		{
			CCalcPort *pInputObj=GetInputPortObj(i);
			pInputObj->GetPortUniValue().GetDoubleVal(&inValue);
			result=result||inValue;
		}
		result=!result;
	}
	else if(m_strFuncName=="^")
	{
		for(UINT i=0;i<m_iInputCount;i++)
		{
			CCalcPort *pInputObj=GetInputPortObj(i);
			pInputObj->GetPortUniValue().GetDoubleVal(&inValue);
			if(i==0)
			{
				result=inValue>0;
			}
			else
				result=(result>0)!=(inValue>0);
		}
	}
	
	CUniValue UniVar;
	UniVar.AddData(result);
	pOutPortObj->SetPortUniValue(UniVar);
	//输出到工作空间中
	OutputResultToGlobalWS();
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CLogicalBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("strFuncName")==0)
	{
		if(!strItem1.IsEmpty())  m_strFuncName=strItem1;
	}
	if(strPropName.Compare("InputCount")==0)
	{
		if(!strItem1.IsEmpty())  m_iInputCount=atoi(strItem1);
	}
}
//重载以显示不同的属性对象对话框
void CLogicalBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
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

	CPropertyDlgMathFun dlg;
	dlg.m_pStrArrFunDisp=&m_strArrFunDisp;
	dlg.m_pStrArrFunName=&m_strArrFunName;
	dlg.m_strBlockDesp=this->m_strBlockDisp;
	dlg.m_strFunName=this->m_strFuncName;
	dlg.m_strPropertityTableName=m_strPropertityTableName;
	dlg.m_iInputCount=m_iInputCount;
	if(dlg.DoModal()==IDOK)
	{
		m_iInputCount=dlg.m_iInputCount;
		m_strFuncName=dlg.m_strFunName;
		if(m_strFuncName=="!")
			m_iInputCount=1;
	

		CStringArray  IDArrLeftIn;//计算块内部因属性改变，而使外部要更新的输入端口描述
		IDArrLeftIn.RemoveAll();		

		CString str;
		for(UINT i=0;i<m_iInputCount;i++)
		{
			str.Format("In%d",i+1);
			IDArrLeftIn.Add(str);
			//	AddInputPort(str, VALUE_DOUBLE);
		}
		//删除原有而不用的Port,倒着删
		for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)
		{
			int  runs=0;//遍历次数
			for(int k=0; k<IDArrLeftIn.GetSize(); k++)
			{
				if(IDArrLeftIn[k]!=IDArrLeftOut[j])	
				{runs++;}
			}
			if(runs==IDArrLeftIn.GetSize())
			{
				this->ClearInputPortByIndex(j);
			}
		}
		//添加新的Port
		for(int m=0; m<IDArrLeftIn.GetSize(); m++)
		{
			int  runs=0;//遍历次数
			for(int n=0; n<IDArrLeftOut.GetSize(); n++)
			{
				if(IDArrLeftIn[m]!=IDArrLeftOut[n])
				{runs++;}		
			}
			if(runs==IDArrLeftOut.GetSize())
			{
				str=IDArrLeftIn[m];
				InSertInputPortBefore(m,str, VALUE_DOUBLE, _T(str));
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
void CLogicalBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
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

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="InputCount";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iInputCount); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	pRecord->Update();
}

//复制图形时，需要拷贝属性。
void CLogicalBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strFuncName=((CLogicalBlock*)pBlock)->m_strFuncName;
	m_iInputCount=((CLogicalBlock*)pBlock)->m_iInputCount;
}
//将属性放置传输数据中
void CLogicalBlock::SetBlockPropertyToTransferData(
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

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"InputCount");
	
	str.Format("%d",m_iInputCount);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
