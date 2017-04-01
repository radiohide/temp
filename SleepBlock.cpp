// SleepBlock.cpp: implementation of the CSleepBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "SleepBlock.h"
#include "PropertyDlgSleep.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSleepBlock::CSleepBlock()
{
	SetBlockType(_T("Sleep") );
	ConstructBlockInputOutput();
	m_iSleepMS=1000;

	m_bShowDesp=TRUE;//是否显示DESP
	m_bShowInPortText=FALSE; //是否显示输入端口的名称文字
	m_bShowOutPortText=FALSE; //是否显示输入端口的名称文字
}

CSleepBlock::~CSleepBlock()
{

}
void CSleepBlock::ConstructBlockInputOutput ()
{

}
	//根据类型新建对象的函数
CCalcBlock* CSleepBlock::CreateFromString( const CString& strType )
{
	CSleepBlock* obj = new CSleepBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CSleepBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_iSleepMS=((CSleepBlock*)pBlock)->m_iSleepMS;
}
	//初始化计算的函数做特殊的初始化
void CSleepBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
	//计算函数，实现本模块的计算
void CSleepBlock::Calc() //进行计算的函数,需要重载
{
	if(m_iSleepMS>0)
		Sleep(m_iSleepMS);
}
//画特征图形
void CSleepBlock::DrawShape(CDC* dc, CRect rect,int CutSize)
{
	//rect 为block的外矩形框,在中部写常数值
	//dc->Rectangle(rect);
	CCalcBlock::DrawShape(dc,rect,CutSize);
	CString str;
	str.Format(_T("%d ms"), m_iSleepMS);
	//str.TrimRight(_T("0")); 
	CRect rcText(rect);
	rcText.top = rect.top + rect.Height()/2 - CutSize;
	rcText.bottom = rcText.top + 2*CutSize;
	
	dc->DrawText(str,rcText,DT_NOPREFIX | DT_WORDBREAK | DT_CENTER );

}
void CSleepBlock::SetBlockPropertyToTransferData(
				CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)//将属性放置传输数据中
{
	
}
    //用于根据参数项和值进行属性的设置，当读取值要用
void CSleepBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(strPropName.Compare("SleepValue")==0)
	{
		if(!strItem1.IsEmpty())
			m_iSleepMS=atoi(strItem1);
	}	
}
	//重载以显示不同的属性对象对话框
void CSleepBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgSleep dlg;
    //给对话框变量赋值
	///////////////////////////////////////////////////
	//int tagnum = m_listIn.GetCount();
	dlg.m_iSleep=m_iSleepMS;
	////////////////////////////////////////////////////////////////////
	if(dlg.DoModal()==IDOK)
	{
		m_iSleepMS = dlg.m_iSleep;
		
	}
	//////////////////////////////////////////////////
}
	//重载以将本模块写入到数据库中
void CSleepBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("SleepValue"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(m_iSleepMS) );
	pRecord->Update();
}
