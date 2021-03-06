// LinkToBlock.cpp: implementation of the CLinkToBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "LinkToBlock.h"
#include "PropertyDlgLinkTo.h"
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

CLinkToBlock::CLinkToBlock()
{
	m_sOutPut1="";
	m_sOutPut2="";
	m_sOutPut3="";
	m_sOutPut4="";
	m_sOutPut5="";

	m_iDataType1=0;
	m_iDataType2=0;
	m_iDataType3=0;
	m_iDataType4=0;
	m_iDataType5=0;

	SetBlockType(_T( "LinkTo" ) );//设置类型
	ConstructBlockInputOutput ();
}

CLinkToBlock::~CLinkToBlock()
{
	
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CLinkToBlock::ConstructBlockInputOutput ()
{
	
}
//根据类型新建对象的函数
CCalcBlock* CLinkToBlock::CreateFromString( const CString& strType )
{
	CLinkToBlock* obj = new CLinkToBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CLinkToBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//返回属性的逗点字符串
CString CLinkToBlock::GetBlockStrPro()
{
	CString str;
	str.Format("%s@%s@%s@%s@%s",m_sOutPut1,m_sOutPut2,m_sOutPut3,m_sOutPut4,m_sOutPut5);
	return str;
}
void CLinkToBlock::SetBlockStrPro(CString str)//设置计算块的字符串属性
{
	//CStringArray  IDArrLeftOut;//计算块原有的输出端口描述
	//IDArrLeftOut.RemoveAll();
	//std::vector<int> ArrLeftDatytypeOut;
	//ArrLeftDatytypeOut.clear();
	
	//if(m_sOutPut1!=""){IDArrLeftOut.Add(m_sOutPut1);	ArrLeftDatytypeOut.push_back(this->m_iDataType1);}
	//if(m_sOutPut2!=""){IDArrLeftOut.Add(m_sOutPut2);	ArrLeftDatytypeOut.push_back(this->m_iDataType2);}
	//if(m_sOutPut3!=""){IDArrLeftOut.Add(m_sOutPut3);	ArrLeftDatytypeOut.push_back(this->m_iDataType3);}
	//if(m_sOutPut4!=""){IDArrLeftOut.Add(m_sOutPut4);	ArrLeftDatytypeOut.push_back(this->m_iDataType4);}	
	//if(m_sOutPut5!=""){IDArrLeftOut.Add(m_sOutPut5);	ArrLeftDatytypeOut.push_back(this->m_iDataType5);}

	CTokenizer tok(str,_T('@'));
	CString strOutPut;
	for(int i=0;i<tok.GetSize();i++)
	{
		tok.GetAt(i,strOutPut);
		if(i==0)
		{
			if(m_sOutPut1!=strOutPut)
			{
				ChangeInPortName(m_sOutPut1,strOutPut);
				m_sOutPut1=strOutPut;
			}
		}
		else if(i==1)
		{
			if(m_sOutPut2!=strOutPut)
			{
				ChangeInPortName(m_sOutPut2,strOutPut);
				m_sOutPut2=strOutPut;
			}
		}
		else if(i==2)
		{
			if(m_sOutPut3!=strOutPut)
			{
				ChangeInPortName(m_sOutPut3,strOutPut);
				m_sOutPut3=strOutPut;
			}
		}
		else if(i==3)
		{
			if(m_sOutPut4!=strOutPut)
			{
				ChangeInPortName(m_sOutPut4,strOutPut);
				m_sOutPut4=strOutPut;
			}
		}
		else if(i==4)
		{
			if(m_sOutPut5!=strOutPut)
			{
				ChangeInPortName(m_sOutPut5,strOutPut);
				m_sOutPut5=strOutPut;
			}
		}
	}
	//removeElemFromWSByKey(m_sOutPut1);
	//removeElemFromWSByKey(m_sOutPut2);
	//removeElemFromWSByKey(m_sOutPut3);
	//removeElemFromWSByKey(m_sOutPut4);
	//removeElemFromWSByKey(m_sOutPut5);
	//reviewPort(NULL,IDArrLeftOut,ArrLeftDatytypeOut);
}
//计算函数，实现本模块的计算
void CLinkToBlock::Calc() //进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
	int inNum = GetInputNum();
	for(int k=0;k<inNum;k++)
	{
		CCalcPort *pPortObj = GetInputPortObj(k);
		CString strVarName = getRefToWSTagName(k);
		CCalcPort *pPortObjFind;
		if(g_WorkSpace.Lookup(strVarName,pPortObjFind))
		{
			g_WorkSpace.RemoveKey(strVarName);
		}
		g_WorkSpace.SetAt(strVarName,pPortObj);
	}
}
void CLinkToBlock::DrawShape(CDC* dc, CRect rect,int CutSize)
{
	//dc->Rectangle(rect);
	CCalcBlock::DrawShape(dc,rect,CutSize);
	////////////////////////////////////////////////
	CPoint midPt;
	midPt.x = rect.right ;
	midPt.y = rect.top + rect.Height()/2;
    //
	int XBegin =rect.left+ int(double(rect.Width())*0.7 );
	int XEnd = rect.top+2*CutSize;
	int YBegin = rect.top+int(2.5*double(CutSize));
	int YEnd =rect.bottom - 2*CutSize; 

	dc->MoveTo(rect.left, YBegin);
	dc->LineTo(XBegin, YBegin);

	dc->LineTo(midPt);
	dc->LineTo(XBegin,YEnd);
	dc->LineTo(rect.left,YEnd);
}

CString CLinkToBlock::getRefToWSTagName(int index)
{
	if((!m_sOutPut1.IsEmpty())&&(index<1))return m_sOutPut1;
	else if((!m_sOutPut2.IsEmpty())&&(index<2))return m_sOutPut2;
	else if((!m_sOutPut3.IsEmpty())&&(index<3))return m_sOutPut3;
	else if((!m_sOutPut4.IsEmpty())&&(index<4))return m_sOutPut4;
	else return m_sOutPut5;
}

//用于根据参数项和值进行属性的设置，当读取值要用
void CLinkToBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("outputTag")==0)
	{
		if(!strItem1.IsEmpty())  {m_sOutPut1=strItem1;}
		if(!strItem2.IsEmpty())  {m_sOutPut2=strItem2;}
		if(!strItem3.IsEmpty())  {m_sOutPut3=strItem3;}
		if(!strItem4.IsEmpty())  {m_sOutPut4=strItem4;}
		if(!strItem5.IsEmpty())  {m_sOutPut5=strItem5;}
	}
	if(strPropName.Compare("outputType")==0)
	{
		if(!strItem1.IsEmpty())  {m_iDataType1=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_iDataType2=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {m_iDataType3=atoi(strItem3);}
		if(!strItem4.IsEmpty())  {m_iDataType4=atoi(strItem4);}
		if(!strItem5.IsEmpty())  {m_iDataType5=atoi(strItem5);}
	}
}
//重载以显示不同的属性对象对话框
void CLinkToBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	//计算块原有的输出端口描述
	CStringArray  IDArrLeftOut;
	IDArrLeftOut.RemoveAll();
	std::vector<int> ArrLeftDatytypeOut;
	ArrLeftDatytypeOut.clear();
	
	if(m_sOutPut1!=""){IDArrLeftOut.Add(m_sOutPut1);	ArrLeftDatytypeOut.push_back(this->m_iDataType1);}
	if(m_sOutPut2!=""){IDArrLeftOut.Add(m_sOutPut2);	ArrLeftDatytypeOut.push_back(this->m_iDataType2);}
	if(m_sOutPut3!=""){IDArrLeftOut.Add(m_sOutPut3);	ArrLeftDatytypeOut.push_back(this->m_iDataType3);}
	if(m_sOutPut4!=""){IDArrLeftOut.Add(m_sOutPut4);	ArrLeftDatytypeOut.push_back(this->m_iDataType4);}	
	if(m_sOutPut5!=""){IDArrLeftOut.Add(m_sOutPut5);	ArrLeftDatytypeOut.push_back(this->m_iDataType5);}
	CPropertyDlgLinkTo dlg;
    //给对话框变量赋值	
    dlg.m_VarTag1 = m_sOutPut1;
	dlg.m_VarTag2 = m_sOutPut2;
	dlg.m_VarTag3 = m_sOutPut3;
	dlg.m_VarTag4 = m_sOutPut4;
	dlg.m_VarTag5 = m_sOutPut5;

	dlg.SetType(1,this->m_iDataType1);
	dlg.SetType(2,this->m_iDataType2);
	dlg.SetType(3,this->m_iDataType3);
	dlg.SetType(4,this->m_iDataType4);
	dlg.SetType(5,this->m_iDataType5);
	///////////////////////////////////////////////////
	if(dlg.DoModal()==IDOK)
	{
		//从公共区删除
		removeElemFromWSByKey(m_sOutPut1);
		removeElemFromWSByKey(m_sOutPut2);
		removeElemFromWSByKey(m_sOutPut3);
		removeElemFromWSByKey(m_sOutPut4);
		removeElemFromWSByKey(m_sOutPut5);

		m_sOutPut1 = dlg.m_VarTag1 ;
		m_sOutPut2 = dlg.m_VarTag2 ;
		m_sOutPut3 = dlg.m_VarTag3 ;
		m_sOutPut4 = dlg.m_VarTag4 ;
		m_sOutPut5 = dlg.m_VarTag5 ;

		m_iDataType1=dlg.GetType(1);
		m_iDataType2=dlg.GetType(2);
		m_iDataType3=dlg.GetType(3);
		m_iDataType4=dlg.GetType(4);
		m_iDataType5=dlg.GetType(5);

		//端口修改处理
		reviewPort(parent,IDArrLeftOut,ArrLeftDatytypeOut);
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
void CLinkToBlock::reviewPort(CWnd* parent,CStringArray &IDArrLeftOut,std::vector<int> &ArrLeftDatytypeOut)
{
	CStringArray  IDArrLeftIn;//计算块内部的输出端口描述
	IDArrLeftIn.RemoveAll();
	std::vector<int> ArrLeftDatytypeIn;
	ArrLeftDatytypeIn.clear();
	//2、对输出端口的修改
	if(!m_sOutPut1.IsEmpty()){IDArrLeftIn.Add(m_sOutPut1);	ArrLeftDatytypeIn.push_back(m_iDataType1);	}
	if(!m_sOutPut2.IsEmpty()){IDArrLeftIn.Add(m_sOutPut2);	ArrLeftDatytypeIn.push_back(m_iDataType2);	}
	if(!m_sOutPut3.IsEmpty()){IDArrLeftIn.Add(m_sOutPut3);	ArrLeftDatytypeIn.push_back(m_iDataType3);	}
	if(!m_sOutPut4.IsEmpty()){IDArrLeftIn.Add(m_sOutPut4);	ArrLeftDatytypeIn.push_back(m_iDataType4);	}
	if(!m_sOutPut5.IsEmpty()){IDArrLeftIn.Add(m_sOutPut5);	ArrLeftDatytypeIn.push_back(m_iDataType5);	}
	
	for(int jj=IDArrLeftOut.GetSize()-1; jj>-1; jj--)//删除原有而不用的Port,倒着删
	{
		int  runs=0;//遍历次数
		for(int kk=0; kk<IDArrLeftIn.GetSize(); kk++)
		{
			if((IDArrLeftIn[kk]!=IDArrLeftOut[jj])
				||(ArrLeftDatytypeIn[kk]!=ArrLeftDatytypeOut[kk]))	
			{
				runs++;
			}
		}
		if(runs==IDArrLeftIn.GetSize())
		{
			this->ClearInputPortByIndex(jj);
		}
	}	
	for(int mm=0; mm<IDArrLeftIn.GetSize(); mm++)//添加新的Port
	{
		int  runs=0;//遍历次数
		int  PortType=ArrLeftDatytypeIn[mm];
		for(int nn=0; nn<IDArrLeftOut.GetSize(); nn++)
		{
			if((IDArrLeftIn[mm]!=IDArrLeftOut[nn])
				||(ArrLeftDatytypeIn[mm]!=ArrLeftDatytypeOut[mm]))
			{
				runs++;
				PortType=ArrLeftDatytypeIn[mm];
			}		
		}
		if(runs==IDArrLeftOut.GetSize())
		{
			CString str=IDArrLeftIn[mm];
			if(PortType==0){InSertInputPortBefore(mm,str, VALUE_DOUBLE, _T(str));}
			else  {InSertInputPortBefore(mm,str, VALUE_RTVALUE_SERIES, _T(str));}
		}
	}
}
void CLinkToBlock::removeElemFromWSByKey(CString strKey)
{
	if(strKey=="")
		return;
	CCalcPort *pPortObj;
	if(g_WorkSpace.Lookup(strKey,pPortObj))
	{
		g_WorkSpace.RemoveKey(strKey);
	}
}
//重载以将本模块写入到数据库中
void CLinkToBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="outputTag";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_sOutPut1); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_sOutPut2); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_sOutPut3); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_sOutPut4); //属性名
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_sOutPut5); //属性名
	pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="outputType";//属性名
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
void CLinkToBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_sOutPut1=((CLinkToBlock*)pBlock)->m_sOutPut1;
	m_sOutPut2=((CLinkToBlock*)pBlock)->m_sOutPut2;
	m_sOutPut3=((CLinkToBlock*)pBlock)->m_sOutPut3;
	m_sOutPut4=((CLinkToBlock*)pBlock)->m_sOutPut4;
	m_sOutPut5=((CLinkToBlock*)pBlock)->m_sOutPut5;

	m_iDataType1=((CLinkToBlock*)pBlock)->m_iDataType1;
	m_iDataType2=((CLinkToBlock*)pBlock)->m_iDataType2;
	m_iDataType3=((CLinkToBlock*)pBlock)->m_iDataType3;
	m_iDataType4=((CLinkToBlock*)pBlock)->m_iDataType4;
	m_iDataType5=((CLinkToBlock*)pBlock)->m_iDataType5;
}
//将属性放置传输数据中
void CLinkToBlock::SetBlockPropertyToTransferData(
									CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"inputTag");
	
	CString str;
	str.Format("%s",m_sOutPut1);strcpy(pData->m_cValueItem1,str);
	str.Format("%s",m_sOutPut2);strcpy(pData->m_cValueItem2,str);
	str.Format("%s",m_sOutPut3);strcpy(pData->m_cValueItem3,str);
	str.Format("%s",m_sOutPut4);strcpy(pData->m_cValueItem4,str);
	str.Format("%s",m_sOutPut5);strcpy(pData->m_cValueItem5,str);
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