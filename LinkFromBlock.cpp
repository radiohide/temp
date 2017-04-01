// LinkFromBlock.cpp: implementation of the CLinkFromBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "LinkFromBlock.h"
#include "PropertyDlgLinkFrom.h"
#include "FlowchartEditor/DiagramEditor/Tokenizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CMap<CString,LPCTSTR,CCalcPort*,CCalcPort*&> g_WorkSpace;
CLinkFromBlock::CLinkFromBlock()
{
	m_sInPut1="";
	m_sInPut2="";
	m_sInPut3="";
	m_sInPut4="";
	m_sInPut5="";
	

	m_iDataType1=0;
	m_iDataType2=0;
	m_iDataType3=0;
	m_iDataType4=0;
	m_iDataType5=0;

	SetBlockType(_T( "LinkFrom" ) );//设置类型
	ConstructBlockInputOutput ();
}

CLinkFromBlock::~CLinkFromBlock()
{

}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CLinkFromBlock::ConstructBlockInputOutput ()
{
	
}
//根据类型新建对象的函数
CCalcBlock* CLinkFromBlock::CreateFromString( const CString& strType )
{
	CLinkFromBlock* obj = new CLinkFromBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CLinkFromBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//返回属性的逗点字符串
CString CLinkFromBlock::GetBlockStrPro()
{
	CString str;
	str.Format("%s@%s@%s@%s@%s",m_sInPut1,m_sInPut2,m_sInPut3,m_sInPut4,m_sInPut5);
	return str;
}
void CLinkFromBlock::SetBlockStrPro(CString str)//设置计算块的字符串属性
{
	//CStringArray  IDArrRightOut;//计算块原有的输出端口描述
	//IDArrRightOut.RemoveAll();
	//std::vector<int> ArrRightDatytypeOut;
	//ArrRightDatytypeOut.clear();
	
	//if(m_sInPut1!=""){IDArrRightOut.Add(m_sInPut1);	ArrRightDatytypeOut.push_back(this->m_iDataType1);}
	//if(m_sInPut2!=""){IDArrRightOut.Add(m_sInPut2);	ArrRightDatytypeOut.push_back(this->m_iDataType2);}
	//if(m_sInPut3!=""){IDArrRightOut.Add(m_sInPut3);	ArrRightDatytypeOut.push_back(this->m_iDataType3);}
	//if(m_sInPut4!=""){IDArrRightOut.Add(m_sInPut4);	ArrRightDatytypeOut.push_back(this->m_iDataType4);}	
	//if(m_sInPut5!=""){IDArrRightOut.Add(m_sInPut5);	ArrRightDatytypeOut.push_back(this->m_iDataType5);}

	CTokenizer tok(str,_T('@'));
	CString strInPut;
	for(int i=0;i<tok.GetSize();i++)
	{
		tok.GetAt(i,strInPut);
		if(i==0)
		{
			if(m_sInPut1!=strInPut)
			{
				ChangeOutPortName(m_sInPut1,strInPut);
				m_sInPut1=strInPut;
			}	
		}
		else if(i==1)
		{
			if(m_sInPut2!=strInPut)
			{
				ChangeOutPortName(m_sInPut1,strInPut);
				m_sInPut2=strInPut;
			}
		}
		else if(i==2)
		{
			if(m_sInPut3!=strInPut)
			{
				ChangeOutPortName(m_sInPut1,strInPut);
				m_sInPut3=strInPut;
			}
		}
		else if(i==3)
		{
			if(m_sInPut4!=strInPut)
			{
				ChangeOutPortName(m_sInPut1,strInPut);
				m_sInPut4=strInPut;
			}
		}
		else if(i==4)
		{
			if(m_sInPut5!=strInPut)
			{
				ChangeOutPortName(m_sInPut1,strInPut);
				m_sInPut5=strInPut;
			}
		}
	}
	//reviewPort((CWnd *)m_pEditorObj,IDArrRightOut,ArrRightDatytypeOut);
}

//计算函数，实现本模块的计算
void CLinkFromBlock::Calc() //进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
	//计算过程
	int outNum = GetOutputNum();
	for(int k=0;k<outNum;k++)
	{
		CCalcPort *pPortObj = GetOutputPortObj(k);
		CCalcPort *pPortInObj = getRefFromWSByTagIndex(k);
		if(pPortInObj!=NULL)
			pPortObj->SetPortUniValue(pPortInObj->GetPortUniValue());
	}
	////////////////////////////////////////////////////////////////////////
   //输出到工作空间中
	OutputResultToGlobalWS();
}
void CLinkFromBlock::DrawShape(CDC* dc, CRect rect,int CutSize)
{
	//dc->Rectangle(rect);
	CCalcBlock::DrawShape(dc,rect,CutSize);
	////////////////////////////////////////////////
	CPoint midPt;
	midPt.x = rect.left ;
	midPt.y = rect.top + rect.Height()/2;
    //
	int XBegin =rect.left+ int(double(rect.Width())*0.3 );
	int XEnd = rect.top+2*CutSize;
	int YBegin = rect.top+int(2.5*double(CutSize));
	int YEnd =rect.bottom - 2*CutSize; 
    //从最右边画
	dc->MoveTo(rect.right, YBegin);
	dc->LineTo(XBegin, YBegin);
    //画到中点
	dc->LineTo(midPt);
	dc->LineTo(XBegin,YEnd);
	dc->LineTo(rect.right,YEnd);
}
CCalcPort* CLinkFromBlock::getRefFromWSByTagIndex(int index)
{
	CString str;
	CCalcPort *pPortObj=NULL;
	if((!m_sInPut1.IsEmpty())&&(index<1)) str = m_sInPut1;
	else if((!m_sInPut2.IsEmpty())&&(index<2))str = m_sInPut2;
	else if((!m_sInPut3.IsEmpty())&&(index<3))str = m_sInPut3;
	else if((!m_sInPut4.IsEmpty())&&(index<4))str = m_sInPut4;
	else str = m_sInPut5;

	if(g_WorkSpace.Lookup(str,pPortObj))
	{}
	return pPortObj;
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CLinkFromBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("inputTag")==0)
	{
		if(!strItem1.IsEmpty())  {m_sInPut1=strItem1;}
		if(!strItem2.IsEmpty())  {m_sInPut2=strItem2;}
		if(!strItem3.IsEmpty())  {m_sInPut3=strItem3;}
		if(!strItem4.IsEmpty())  {m_sInPut4=strItem4;}
		if(!strItem5.IsEmpty())  {m_sInPut5=strItem5;}
	}
	if(strPropName.Compare("inputType")==0)
	{
		if(!strItem1.IsEmpty())  {m_iDataType1=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_iDataType2=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {m_iDataType3=atoi(strItem3);}
		if(!strItem4.IsEmpty())  {m_iDataType4=atoi(strItem4);}
		if(!strItem5.IsEmpty())  {m_iDataType5=atoi(strItem5);}
	}
}
//重载以显示不同的属性对象对话框
void CLinkFromBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CStringArray  IDArrRightOut;//计算块原有的输出端口描述
	IDArrRightOut.RemoveAll();
	std::vector<int> ArrRightDatytypeOut;
	ArrRightDatytypeOut.clear();
	
	if(m_sInPut1!=""){IDArrRightOut.Add(m_sInPut1);	ArrRightDatytypeOut.push_back(this->m_iDataType1);}
	if(m_sInPut2!=""){IDArrRightOut.Add(m_sInPut2);	ArrRightDatytypeOut.push_back(this->m_iDataType2);}
	if(m_sInPut3!=""){IDArrRightOut.Add(m_sInPut3);	ArrRightDatytypeOut.push_back(this->m_iDataType3);}
	if(m_sInPut4!=""){IDArrRightOut.Add(m_sInPut4);	ArrRightDatytypeOut.push_back(this->m_iDataType4);}	
	if(m_sInPut5!=""){IDArrRightOut.Add(m_sInPut5);	ArrRightDatytypeOut.push_back(this->m_iDataType5);}
	CPropertyDlgLinkFrom dlg;
    //给对话框变量赋值	
    dlg.m_VarTag1 = m_sInPut1;
	dlg.m_VarTag2 = m_sInPut2;
	dlg.m_VarTag3 = m_sInPut3;
	dlg.m_VarTag4 = m_sInPut4;
	dlg.m_VarTag5 = m_sInPut5;
	
	dlg.SetType(1,this->m_iDataType1);
	dlg.SetType(2,this->m_iDataType2);
	dlg.SetType(3,this->m_iDataType3);
	dlg.SetType(4,this->m_iDataType4);
	dlg.SetType(5,this->m_iDataType5);
	///////////////////////////////////////////////////
	if(dlg.DoModal()==IDOK)
	{
		m_sInPut1 = dlg.m_VarTag1 ;
		m_sInPut2 = dlg.m_VarTag2 ;
		m_sInPut3 = dlg.m_VarTag3 ;
		m_sInPut4 = dlg.m_VarTag4 ;
		m_sInPut5 = dlg.m_VarTag5 ;
		
		m_iDataType1=dlg.GetType(1);
		m_iDataType2=dlg.GetType(2);
		m_iDataType3=dlg.GetType(3);
		m_iDataType4=dlg.GetType(4);
		m_iDataType5=dlg.GetType(5);

		reviewPort(parent,IDArrRightOut,ArrRightDatytypeOut);
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
void CLinkFromBlock::reviewPort(CWnd* parent,CStringArray &IDArrRightOut,std::vector<int> &ArrRightDatytypeOut)
{
	CStringArray  IDArrRightIn;//计算块内部的输出端口描述
	IDArrRightIn.RemoveAll();
	std::vector<int> ArrRightDatytypeIn;
	ArrRightDatytypeIn.clear();
	//2、对输出端口的修改
	if(!m_sInPut1.IsEmpty()){IDArrRightIn.Add(m_sInPut1);	ArrRightDatytypeIn.push_back(m_iDataType1);	}
	if(!m_sInPut2.IsEmpty()){IDArrRightIn.Add(m_sInPut2);	ArrRightDatytypeIn.push_back(m_iDataType2);	}
	if(!m_sInPut3.IsEmpty()){IDArrRightIn.Add(m_sInPut3);	ArrRightDatytypeIn.push_back(m_iDataType3);	}
	if(!m_sInPut4.IsEmpty()){IDArrRightIn.Add(m_sInPut4);	ArrRightDatytypeIn.push_back(m_iDataType4);	}
	if(!m_sInPut5.IsEmpty()){IDArrRightIn.Add(m_sInPut5);	ArrRightDatytypeIn.push_back(m_iDataType5);	}
	
	for(int jj=IDArrRightOut.GetSize()-1; jj>-1; jj--)//删除原有而不用的Port,倒着删
	{
		int  runs=0;//遍历次数
		for(int kk=0; kk<IDArrRightIn.GetSize(); kk++)
		{
			if((IDArrRightIn[kk]!=IDArrRightOut[jj])
				||(ArrRightDatytypeIn[kk]!=ArrRightDatytypeOut[kk]))	
			{
				runs++;
			}
		}
		if(runs==IDArrRightIn.GetSize())
		{
			if(parent!=NULL)
				this->ClearOutputPortByIndex(jj,parent);
		}
	}	
	for(int mm=0; mm<IDArrRightIn.GetSize(); mm++)//添加新的Port
	{
		int  runs=0;//遍历次数
		int  PortType=ArrRightDatytypeIn[mm];
		for(int nn=0; nn<IDArrRightOut.GetSize(); nn++)
		{
			if((IDArrRightIn[mm]!=IDArrRightOut[nn])
				||(ArrRightDatytypeIn[mm]!=ArrRightDatytypeOut[mm]))
			{
				runs++;
				PortType=ArrRightDatytypeIn[mm];
			}		
		}
		if(runs==IDArrRightOut.GetSize())
		{
			CString str=IDArrRightIn[mm];
			if(PortType==0){InSertOutputPortBefore(mm,str, VALUE_DOUBLE, _T(str));}
			else  {InSertOutputPortBefore(mm,str, VALUE_RTVALUE_SERIES, _T(str));}
		}
	}
}
//重载以将本模块写入到数据库中
void CLinkFromBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="inputTag";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_sInPut1); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_sInPut2); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_sInPut3); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_sInPut4); //属性名
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_sInPut5); //属性名
	pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="inputType";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iDataType1); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_iDataType2); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_iDataType3); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_iDataType4); //属性名
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_iDataType5); //属性名
	pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //属性名	

	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CLinkFromBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_sInPut1=((CLinkFromBlock*)pBlock)->m_sInPut1;
	m_sInPut2=((CLinkFromBlock*)pBlock)->m_sInPut2;
	m_sInPut3=((CLinkFromBlock*)pBlock)->m_sInPut3;
	m_sInPut4=((CLinkFromBlock*)pBlock)->m_sInPut4;
	m_sInPut5=((CLinkFromBlock*)pBlock)->m_sInPut5;

	m_iDataType1=((CLinkFromBlock*)pBlock)->m_iDataType1;
	m_iDataType2=((CLinkFromBlock*)pBlock)->m_iDataType2;
	m_iDataType3=((CLinkFromBlock*)pBlock)->m_iDataType3;
	m_iDataType4=((CLinkFromBlock*)pBlock)->m_iDataType4;
	m_iDataType5=((CLinkFromBlock*)pBlock)->m_iDataType5;
}
//将属性放置传输数据中
void CLinkFromBlock::SetBlockPropertyToTransferData(
									CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"inputTag");
	
	CString str;
	str.Format("%s",m_sInPut1);strcpy(pData->m_cValueItem1,str);
	str.Format("%s",m_sInPut2);strcpy(pData->m_cValueItem2,str);
	str.Format("%s",m_sInPut3);strcpy(pData->m_cValueItem3,str);
	str.Format("%s",m_sInPut4);strcpy(pData->m_cValueItem4,str);
	str.Format("%s",m_sInPut5);strcpy(pData->m_cValueItem5,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"inputType");
	
	str.Format("%d",m_iDataType1);strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iDataType2);strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_iDataType3);strcpy(pData->m_cValueItem3,str);
	str.Format("%d",m_iDataType4);strcpy(pData->m_cValueItem4,str);
	str.Format("%d",m_iDataType5);strcpy(pData->m_cValueItem5,str);
	dataArr.Add(pData);
}
