// MultBlock.cpp: implementation of the CMultBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "MultBlock.h"
#include "PropertyDlgMult.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultBlock::CMultBlock()
{
	m_strInputEdit="**";
	SetBlockType(_T( "Mult" ) );
	ConstructBlockInputOutput();
}

CMultBlock::~CMultBlock()
{

}
//根据类型新建对象的函数
CCalcBlock* CMultBlock::CreateFromString( const CString& strType )
{
	CMultBlock* obj = new CMultBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CMultBlock::ConstructBlockInputOutput ()
{
    AddInputPort(_T("M1"), VALUE_DOUBLE);
	AddInputPort(_T("M2"), VALUE_DOUBLE);

	AddOutputPort("out",VALUE_DOUBLE,"out");

}
//初始化计算的函数做特殊的初始化
void CMultBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CMultBlock::Calc()//进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	int inNum = GetInputNum();
	double result=1;
	double inValue=0;
	for(int k=0;k<inNum;k++)
	{
		CCalcPort *pPortObj = GetInputPortObj(k);
		pPortObj->GetPortUniValue().GetDoubleVal(&inValue);
		if('*'==m_strInputEdit.GetAt(k))
		{
			result*=inValue;
		}
		else if('/'==m_strInputEdit.GetAt(k))
		{
			if(inValue!=0)
			{
				result/=inValue;
			}
			else
			{
				result=0;
			}
		}
	}
	CUniValue UniVar;
	UniVar.AddData(result);
	pOutPortObj->SetPortUniValue(UniVar);
	OutputResultToGlobalWS();
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CMultBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("StrInputCfg")==0)
	{
		if(!strItem1.IsEmpty())  {m_strInputEdit=strItem1;}
	}
}
//重载以显示不同的属性对象对话框
void CMultBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgMult dlg;
	
	int portnumLinkBefore=this->GetInputNum();//新增
	CStringArray  IDArrLeftOut;//计算块原有的输入端口描述
	IDArrLeftOut.RemoveAll();
	
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		IDArrLeftOut.Add(strPortID);
	}
    //给对话框变量赋值
	dlg.m_strInputEdit=m_strInputEdit;
	if(dlg.DoModal()==IDOK)
	{
		m_strInputEdit=dlg.m_strInputEdit;

		buildInputPort(IDArrLeftOut);
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
void CMultBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="StrInputCfg";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strInputEdit); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	pRecord->Update();
}

void CMultBlock::buildInputPort(CStringArray &IDArrLeftOut)
{
	CStringArray  IDArrLeftIn;//计算块内部因属性改变，而使外部要更新的输入端口描述
	IDArrLeftIn.RemoveAll();

	CString str;
	int aCount=0;
	int dCount=0;
	for(int i=0;i<m_strInputEdit.GetLength();i++)
	{
		if('*'==m_strInputEdit.GetAt(i))
		{
			str.Format("M%d",++aCount);
			IDArrLeftIn.Add(str);
		}
		else if('/'==m_strInputEdit.GetAt(i))
		{
			str.Format("D%d",++dCount);
			IDArrLeftIn.Add(str);	
		}
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
}
//复制图形时，需要拷贝属性。
void CMultBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_strInputEdit=((CMultBlock*)pBlock)->m_strInputEdit;
}
//将属性放置传输数据中
void CMultBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"StrInputCfg");
	
	CString str;
	str.Format("%s",m_strInputEdit);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
