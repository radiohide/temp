// BackCalc.cpp: implementation of the CBackCalc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "BackCalc.h"
#include "PropertyDlgBackCalc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBackCalc::CBackCalc()
{
	SetBlockType(_T( "BackCalc" ) );
	ConstructBlockInputOutput();
}

CBackCalc::~CBackCalc()
{

}

//根据类型新建对象的函数
CCalcBlock* CBackCalc::CreateFromString( const CString& strType )
{
	CBackCalc* obj = new CBackCalc;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CBackCalc::ConstructBlockInputOutput ()
{
	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//初始化计算的函数做特殊的初始化
void CBackCalc::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CBackCalc::Calc()//进行计算的函数,需要重载
{
	//从全局获取输出值到本计算块输入准备
	GetInputValueFromGlobalWS();
	
	CUniValue UniVar;
	UniVar.AddData(this->m_pModel->m_iBackCalcTag,0,0);
	
    CCalcPort *pOutPortObj = GetOutputPortObj(0);
	pOutPortObj->SetPortUniValue(UniVar);
	OutputResultToGlobalWS();
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CBackCalc::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{

}
//重载以显示不同的属性对象对话框
void CBackCalc::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgBackCalc dlg;
	dlg.m_iBackCalc = this->m_pModel->m_iBackCalcTag;
	dlg.DoModal();
}
//重载以将本模块写入到数据库中
void CBackCalc::PutPropertyToRecord(_RecordsetPtr& pRecord)
{

}

//复制图形时，需要拷贝属性。
void CBackCalc::CopyPropertyData(CCalcBlock* pBlock)
{
}
//将属性放置传输数据中
void CBackCalc::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
}
