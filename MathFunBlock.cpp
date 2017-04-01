// MathFunBlock.cpp: implementation of the CMathFunBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "MathFunBlock.h"
#include "PropertyDlgMathFun.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMathFunBlock::CMathFunBlock()
{
	SetBlockType(_T( "MathFun" ) );//设置类型
	m_strFuncName="Power";
	m_strBlockDisp="数学函数计算块。请选择数学计算函数进行计算。";
	m_strPropertityTableName="数学函数计算块";
	
	m_strArrFunName.Add("Power");m_strArrFunDisp.Add("乘方");
	m_strArrInPortName.Add("Base");m_iArrInFunIndex.Add(0);
	m_strArrInPortName.Add("Exponent");m_iArrInFunIndex.Add(0);
	m_strArrOutPortName.Add("Power");m_iArrOutFunIndex.Add(0);

	m_strArrFunName.Add("Log");m_strArrFunDisp.Add("对数");
	m_strArrInPortName.Add("Base");m_iArrInFunIndex.Add(1);
	m_strArrInPortName.Add("Real");m_iArrInFunIndex.Add(1);
	m_strArrOutPortName.Add("Log");m_iArrOutFunIndex.Add(1);
	
	m_strArrFunName.Add("Abs");m_strArrFunDisp.Add("绝对值");
	m_strArrInPortName.Add("In");m_iArrInFunIndex.Add(2);
	m_strArrOutPortName.Add("Out");m_iArrOutFunIndex.Add(2);

	ConstructBlockInputOutput();
}

CMathFunBlock::~CMathFunBlock()
{
	
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CMathFunBlock::ConstructBlockInputOutput ()
{
	ClearInputPorts();
	ClearOutputPorts();
	int index=-1;
	for(int i=0;i<m_strArrFunName.GetSize();i++)
	{
		if(m_strFuncName==m_strArrFunName[i])
		{
			index=i;
			break;
		}
	}
	if(index!=-1)//有选取函数
	{
		for(int i=0;i<m_iArrInFunIndex.GetSize();i++)
		{
			if(m_iArrInFunIndex[i]==index)
			{
				AddInputPort(m_strArrInPortName[i], VALUE_DOUBLE);
			}
		}
		for(i=0;i<m_iArrOutFunIndex.GetSize();i++)
		{
			if(m_iArrOutFunIndex[i]==index)
			{
				AddOutputPort(m_strArrOutPortName[i], VALUE_DOUBLE);
			}
		}
	}
}
//根据类型新建对象的函数
CCalcBlock* CMathFunBlock::CreateFromString( const CString& strType )
{
	CMathFunBlock* obj = new CMathFunBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CMathFunBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CMathFunBlock::Calc() //进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	double inValue1,inValue2; 
	CUniValue UniVar;
	if("Power"==m_strFuncName)
	{
		CCalcPort *pInPortObj1 = GetInputPortObj(0);
		CCalcPort *pInPortObj2 = GetInputPortObj(1);
		pInPortObj1->GetPortUniValue().GetDoubleVal(&inValue1);
		pInPortObj2->GetPortUniValue().GetDoubleVal(&inValue2);
		if(inValue1<0)
			UniVar.AddData(-1*pow(-1*inValue1,inValue2));
		else
			UniVar.AddData(pow(inValue1,inValue2));
	}
	else if("Log"==m_strFuncName)
	{
		CCalcPort *pInPortObj1 = GetInputPortObj(0);
		CCalcPort *pInPortObj2 = GetInputPortObj(1);
		pInPortObj1->GetPortUniValue().GetDoubleVal(&inValue1);
		pInPortObj2->GetPortUniValue().GetDoubleVal(&inValue2);
		UniVar.AddData(log(inValue2>=0?inValue2:(-1)*inValue2)/log(inValue1>=0?inValue1:(-1)*inValue1));
	}
	else if("Abs"==m_strFuncName)
	{
		CCalcPort *pInPortObj1 = GetInputPortObj(0);
		pInPortObj1->GetPortUniValue().GetDoubleVal(&inValue1);
		UniVar.AddData(inValue1>=0?inValue1:(-1)*inValue1);
	}
	pOutPortObj->SetPortUniValue(UniVar);
	OutputResultToGlobalWS();	
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CMathFunBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("strFuncName")==0)
	{
		if(!strItem1.IsEmpty())  m_strFuncName=strItem1;
	}
}
//重载以显示不同的属性对象对话框
void CMathFunBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgMathFun dlg;
	dlg.m_pStrArrFunDisp=&m_strArrFunDisp;
	dlg.m_pStrArrFunName=&m_strArrFunName;
	dlg.m_strBlockDesp=this->m_strBlockDisp;
	dlg.m_strFunName=this->m_strFuncName;
	dlg.m_strPropertityTableName=m_strPropertityTableName;
	
	int portnumLinkBefore=this->GetInputNum();//新增
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrLeftOut;//计算块原有的输入端口描述
	CStringArray  IDArrRightOut;//计算块原有的输出端口描述
	IDArrLeftOut.RemoveAll();
	IDArrRightOut.RemoveAll();
	
	CStringArray  IDArrLeftIn;//计算块内部的输入端口描述
	CStringArray  IDArrRightIn;//计算块内部的输出端口描述
	IDArrLeftIn.RemoveAll();
    IDArrRightIn.RemoveAll();
	
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		IDArrLeftOut.Add(strPortID);
	}
	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}
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
		if(index!=-1)//有选取函数
		{
			//1、对输入端口的修改
			for(int i=0;i<m_iArrInFunIndex.GetSize();i++)
			{
				if(m_iArrInFunIndex[i]==index)
				{
					//AddInputPort(m_strArrInPortName[i], VALUE_DOUBLE);
					IDArrLeftIn.Add(m_strArrInPortName[i]);
				}
			}
			for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)	//删除原有而不用的Port,倒着删
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
			for(int m=0; m<IDArrLeftIn.GetSize(); m++)	//添加新的Port
			{
				int  runs=0;//遍历次数
				for(int n=0; n<IDArrLeftOut.GetSize(); n++)
				{
					if(IDArrLeftIn[m]!=IDArrLeftOut[n])
					{runs++;}		
				}
				if(runs==IDArrLeftOut.GetSize())
				{
					CString str=IDArrLeftIn[m];
					InSertInputPortBefore(m,str, VALUE_DOUBLE, _T(str));
				}
			}
			//2、对输出端口的修改
			for(i=0;i<m_iArrOutFunIndex.GetSize();i++)
			{
				if(m_iArrOutFunIndex[i]==index)
				{
					//AddOutputPort(m_strArrOutPortName[i], VALUE_DOUBLE);
					IDArrRightIn.Add(m_strArrOutPortName[i]);
				}
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
void CMathFunBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
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
void CMathFunBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strFuncName=((CMathFunBlock*)pBlock)->m_strFuncName;
}
//将属性放置传输数据中
void CMathFunBlock::SetBlockPropertyToTransferData(
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
