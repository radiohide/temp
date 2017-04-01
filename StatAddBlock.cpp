// StatAddBlock.cpp: implementation of the CStatAddBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatAddBlock.h"
#include "PropertyDlgStatAdd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatAddBlock::CStatAddBlock()
{
	m_strInputEdit="++";
	SetBlockType(_T( "StatAdd" ) );
	ConstructBlockInputOutput();
}

CStatAddBlock::~CStatAddBlock()
{

}
//根据类型新建对象的函数
CCalcBlock* CStatAddBlock::CreateFromString( const CString& strType )
{
	CStatAddBlock* obj = new CStatAddBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CStatAddBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("A1"), VALUE_STAT);
	AddInputPort(_T("A2"), VALUE_STAT);

	AddOutputPort("out",VALUE_STAT,"out");
}
//初始化计算的函数做特殊的初始化
void CStatAddBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CStatAddBlock::Calc()//进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();

    //计算过程
	int inNum = GetInputNum();
	double sum=0;
	StatData oneStatData;
	for(int k=0;k<inNum;k++)
	{
		StatData statData;
		CCalcPort *pPortObj = GetInputPortObj(k);
		pPortObj->GetPortUniValue().GetStatData(&statData);
		if(k==0)
		{
			oneStatData=statData;
		}

		if('+'==m_strInputEdit.GetAt(k))
		{
			sum+=statData.dValue;
		}
		else if('-'==m_strInputEdit.GetAt(k))
		{
			sum-=statData.dValue;
		}
	}

	oneStatData.dValue=sum;
	CUniValue UniVarOut;

	CCalcPort *pPortObjOut = GetOutputPortObj(0);
	UniVarOut.AddStatData(oneStatData);
	pPortObjOut->SetPortUniValue(UniVarOut);
	OutputResultToGlobalWS();
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CStatAddBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("StrInputCfg")==0)
	{
		if(!strItem1.IsEmpty())  {m_strInputEdit=strItem1;}
	}
}
//重载以显示不同的属性对象对话框
void CStatAddBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStatAdd dlg;
    //给对话框变量赋值
	dlg.m_strInputEdit=m_strInputEdit;
    
	int portnumLinkBefore=this->GetInputNum();//新增
	CStringArray  IDArrLeftOut;//计算块原有的输入端口描述
    IDArrLeftOut.RemoveAll();

    for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		IDArrLeftOut.Add(strPortID);
	}
	if(dlg.DoModal()==IDOK)
	{
		m_strInputEdit=dlg.m_strInputEdit;

		buildInputPort(IDArrLeftOut);
		parent->RedrawWindow();
	}
}
//重载以将本模块写入到数据库中
void CStatAddBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
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

void CStatAddBlock::buildInputPort(CStringArray &IDArrLeftOut)
{
	CStringArray  IDArrLeftIn;//计算块内部因属性改变，而使外部要更新的输入端口描述
	IDArrLeftIn.RemoveAll();

	CString str;
	int aCount=0;
	int dCount=0;
	for(int i=0;i<m_strInputEdit.GetLength();i++)
	{
		if('+'==m_strInputEdit.GetAt(i))
		{
			str.Format("A%d",++aCount);
			IDArrLeftIn.Add(str);
		}
		else if('-'==m_strInputEdit.GetAt(i))
		{
			str.Format("S%d",++dCount);
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
			InSertInputPortBefore(m,str, VALUE_STAT, _T(str));
		}
	}
}
//复制图形时，需要拷贝属性。
void CStatAddBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_strInputEdit=((CStatAddBlock*)pBlock)->m_strInputEdit;
}
//将属性放置传输数据中
void CStatAddBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}
