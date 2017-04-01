// ConstBlock.cpp: implementation of the CConstBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "ConstBlock.h"
#include "PropertyDlgConst.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConstBlock::CConstBlock()
{
	SetBlockType(_T("Const") );
	ConstructBlockInputOutput();
	m_dConstValue=1;

	m_bShowDesp=TRUE;//是否显示DESP
	m_bShowInPortText=FALSE; //是否显示输入端口的名称文字
	m_bShowOutPortText=FALSE; //是否显示输入端口的名称文字
}

CConstBlock::~CConstBlock()
{

}
void CConstBlock::ConstructBlockInputOutput ()
{
	//可自由定义输入输出的变量名称。
    AddOutputPort("out", VALUE_DOUBLE);
}
	//根据类型新建对象的函数
CCalcBlock* CConstBlock::CreateFromString( const CString& strType )
{
	CConstBlock* obj = new CConstBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CConstBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_dConstValue=((CConstBlock*)pBlock)->m_dConstValue;
}
	//初始化计算的函数做特殊的初始化
void CConstBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
	//计算函数，实现本模块的计算
void CConstBlock::Calc() //进行计算的函数,需要重载
{
	CUniValue UniVar;
	CCalcPort *pOutPortObj=GetOutputPortObj(0);
	if(pOutPortObj)
	{	
		UniVar.AddData(m_dConstValue,this->m_pModel->m_lDataTime);
		//UniVar.AddData(m_dConstValue);
		pOutPortObj->SetPortUniValue(UniVar);
	}
	////////////////////////////////////////////////////////////////////////
   //输出到工作空间中
	OutputResultToGlobalWS();
}
//画特征图形
void CConstBlock::DrawShape(CDC* dc, CRect rect,int CutSize)
{
	//rect 为block的外矩形框,在中部写常数值
	//dc->Rectangle(rect);
	CCalcBlock::DrawShape(dc,rect,CutSize);
	CString str;
	if((double)((int)m_dConstValue)==m_dConstValue)
	{
         str.Format("%d", (int)m_dConstValue);
	}
	else
	{
		 str.Format(_T("%f"), m_dConstValue);
	 	 str.TrimRight(_T("0")); 
	}
	CRect rcText(rect);
	rcText.top = rect.top + rect.Height()/2 - CutSize;
	rcText.bottom = rcText.top + 2*CutSize;
	
	dc->DrawText(str,rcText,DT_NOPREFIX | DT_WORDBREAK | DT_CENTER );

}
void CConstBlock::SetBlockPropertyToTransferData(
				CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)//将属性放置传输数据中
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"ConstValue");
	
	CString str;
	str.Format("%f",m_dConstValue);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
    //用于根据参数项和值进行属性的设置，当读取值要用
void CConstBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(strPropName.Compare("ConstValue")==0)
	{
		if(!strItem1.IsEmpty())
			m_dConstValue=atof(strItem1);
	}	
}
	//重载以显示不同的属性对象对话框
void CConstBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgConst dlg;
    //给对话框变量赋值
	///////////////////////////////////////////////////
	//int tagnum = m_listIn.GetCount();
	dlg.m_dConstValue=m_dConstValue;
	////////////////////////////////////////////////////////////////////
	if(dlg.DoModal()==IDOK)
	{
		m_dConstValue = dlg.m_dConstValue;
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
void CConstBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("ConstValue"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(m_dConstValue) );
	pRecord->Update();
}
