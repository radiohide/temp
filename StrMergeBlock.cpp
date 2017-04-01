// StrMergeBlock.cpp: implementation of the CStrMergeBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StrMergeBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStrMergeBlock::CStrMergeBlock()
{
	m_strInputNum=0;

	//SYSTME=====设置类型======
	//设置类型
	SetBlockType(_T( "StrMerge" ) );
	ConstructBlockInputOutput();
	//SYSTME=====初始化本模块的属性===

}

CStrMergeBlock::~CStrMergeBlock()
{

}
//构造初始的输入输出个数
void CStrMergeBlock::ConstructBlockInputOutput()
{	
	AddOutputPort("Out",VALUE_RTVALUE_SERIES, _T("out1"), _T("out1"));	
}
//根据类型新建对象的函数
//同时在CCalcBlockControlFactory中，要加入相应的代码
CCalcBlock* CStrMergeBlock::CreateFromString( const CString& strType )
{
	CStrMergeBlock* obj = new CStrMergeBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CStrMergeBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CStrMergeBlock::Calc() 
{
	//获得输入端口的数据
	GetInputValueFromGlobalWS();
	//
	CUniValue UniVar;//用于输出
	UniVar.SetValueType(1);//设置为VALUE_RTVALUE_SERIES类型
	//进行计算
	int inNum = GetInputNum();
    for(int i=0; i<inNum; i++)
	{
		CCalcPort *pPortObjIn = GetInputPortObj(i);		
		
		CUniValue  &CUV=pPortObjIn->GetPortUniValue();
		for(int i=0; i<CUV.GetDataSize();i++)
		{
			double value;
			CUV.GetDoubleValAt(i,&value);
	        UniVar.AddData(value);	
		}
	}
	///////////////////////////////////
	CCalcPort *pPortObjOut = GetOutputPortObj(0);
	pPortObjOut->SetPortUniValue(UniVar);
	//输出计算
	OutputResultToGlobalWS();

}
//用于根据参数项和值进行属性的设置，当读取值要用
void CStrMergeBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("StrMerge")==0)
	{
		if(!strItem1.IsEmpty())
			m_strInputNum=atoi(strItem1);	
	}	
}
//重载以显示不同的属性对象对话框
void CStrMergeBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBefore=this->GetInputNum();//D???
	CStringArray  IDArrLeftOut;//?????é?-óDμ?ê?è????ú?èê?
	IDArrLeftOut.RemoveAll();
	
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		IDArrLeftOut.Add(strPortID);
	}
	
	CPropertyDlgStrMerge  dlg;
	dlg.m_strInputNum=m_strInputNum;
	if(dlg.DoModal()==IDOK)
	{
		CStringArray  IDArrLeftIn;//计算块内部因属性改变，而使外部要更新的输入端口描述
		IDArrLeftIn.RemoveAll();
		
		m_strInputNum=dlg.m_strInputNum;
		for(int i=0; i<m_strInputNum; i++)
		{
			CString str;
			str.Format("In%d",i);
			IDArrLeftIn.Add(str);
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
				CString str=IDArrLeftIn[m];
				InSertInputPortBefore(m,str, VALUE_RTVALUE_SERIES, _T(str));
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
void CStrMergeBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1-4：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("StrMerge"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
	strFieldValue.Format("%d",m_strInputNum);
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(strFieldValue) );

	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CStrMergeBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strInputNum=((CStrMergeBlock*)pBlock)->m_strInputNum;
}
//将属性放置传输数据中
void CStrMergeBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

	CString str;
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"StrMerge");
	str.Format("%f",m_strInputNum);strcpy(pData->m_cValueItem1,str);

	dataArr.Add(pData);

}