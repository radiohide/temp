// GainBlock.cpp: implementation of the CGainBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "GainBlock.h"
#include "PropertyDlgGain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGainBlock::CGainBlock()
{
	m_dGainValue=1;
	SetBlockType(_T( "Gain" ) );
	ConstructBlockInputOutput();

	m_bShowDesp=TRUE;//是否显示DESP
	m_bShowInPortText=FALSE; //是否显示输入端口的名称文字
	m_bShowOutPortText=FALSE; //是否显示输入端口的名称文字
}

CGainBlock::~CGainBlock()
{

}
//根据类型新建对象的函数
CCalcBlock* CGainBlock::CreateFromString( const CString& strType )
{
	CGainBlock* obj = new CGainBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CGainBlock::ConstructBlockInputOutput ()
{
	AddInputPort("In",VALUE_DOUBLE);
	AddOutputPort("Out",VALUE_DOUBLE);
}
//初始化计算的函数做特殊的初始化
void CGainBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CGainBlock::Calc()//进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	int inNum = GetInputNum();
	double inValue=0;
	CCalcPort *pInPortObj = GetInputPortObj(0);
	pInPortObj->GetPortUniValue().GetDoubleVal(&inValue);

	CUniValue UniVar;
	UniVar.AddData(inValue*m_dGainValue);
	pOutPortObj->SetPortUniValue(UniVar);
	OutputResultToGlobalWS();
}
//画特征图形
void CGainBlock::DrawShape(CDC* dc, CRect rect,int CutSize)
{
	//rect 为
	//dc->Rectangle(rect);
	CCalcBlock::DrawShape(dc,rect,CutSize);
	CString str;
	if((double)((int)m_dGainValue)==m_dGainValue)
	{
              str.Format("%d", (int)m_dGainValue);
	}
	else
	{
		 str.Format(_T("%f"), m_dGainValue);
	 	 str.TrimRight(_T("0")); 
	}
	CRect rcText(rect);
	rcText.top = rect.top + rect.Height()/2 - CutSize;
	rcText.bottom = rcText.top + 2*CutSize;
	//
	CPoint midPt;
	midPt.x = rect.right ;
	midPt.y = rect.top + rect.Height()/2;
	dc->MoveTo(rect.left, rect.top+2*CutSize);
	dc->LineTo(midPt);
	dc->LineTo(rect.left,rect.bottom - 2*CutSize);
	dc->DrawText(str,rcText,DT_NOPREFIX | DT_WORDBREAK | DT_CENTER );

}
//用于根据参数项和值进行属性的设置，当读取值要用
void CGainBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("GainValue")==0)
	{
		if(!strItem1.IsEmpty())  {m_dGainValue=atof(strItem1);}
	}
}
//重载以显示不同的属性对象对话框
void CGainBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgGain dlg;
    //给对话框变量赋值
	dlg.m_dGainValue=m_dGainValue;
	if(dlg.DoModal()==IDOK)
	{
		m_dGainValue=dlg.m_dGainValue;
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
void CGainBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="GainValue";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%f",m_dGainValue); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	pRecord->Update();
}

//复制图形时，需要拷贝属性。
void CGainBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_dGainValue=((CGainBlock*)pBlock)->m_dGainValue;
}
//将属性放置传输数据中
void CGainBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"GainValue");
	
	CString str;
	str.Format("%f",m_dGainValue);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
