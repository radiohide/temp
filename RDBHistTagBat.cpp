// RDBHistTagBat.cpp: implementation of the CRDBHistTagBat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RDBHistTagBat.h"
#include "PropertyDlgRDBHistTagBat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRDBHistTagBat::CRDBHistTagBat()
{
	m_strFieldVarName="VarName";
	m_strFieldDateTime="DateTime";
	m_strFieldState="State";
	m_strFieldValue="Value";

	m_iNearSec=300;

	SetBlockType(_T( "RDBHistTagBat" ) );
	ConstructBlockInputOutput();
}

CRDBHistTagBat::~CRDBHistTagBat()
{

}
//根据类型新建对象的函数
CCalcBlock* CRDBHistTagBat::CreateFromString( const CString& strType )
{
	CRDBHistTagBat* obj = new CRDBHistTagBat;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CRDBHistTagBat::ConstructBlockInputOutput ()
{
	//AddInputPort("Enable",VALUE_DOUBLE);
	AddInputPort("Time",VALUE_DOUBLE);
}

//初始化计算的函数做特殊的初始化
void CRDBHistTagBat::InitCalc()
{
	CCalcBlock::InitCalc();
}

//返回属性的逗点字符串
CString CRDBHistTagBat::GetBlockStrPro()//to do
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CRDBHistTagBat::SetBlockStrPro(CString str)//设置计算块的字符串属性//to do
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
void CRDBHistTagBat::Calc()//进行计算的函数,需要重载
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
		return;
	GetInputValueFromGlobalWS();
	
	double dInValue=0;
	long lSTime=0;
	long lETime=0;
	CCalcPort *pInPortObj1 = GetInputPortObj(0);
	pInPortObj1->GetPortUniValue().GetDoubleVal(&dInValue);
	lSTime=(long)dInValue;

	
	lETime=lSTime-m_iNearSec;
	if(lSTime>lETime)
	{
		long lTemp=lSTime;
		lSTime=lETime;
		lETime=lTemp;
	}
	CTime st(lSTime);
	CTime et(lETime);
	CString strSTime = st.Format("%Y-%m-%d %H:%M:%S");
	CString strETime = et.Format("%Y-%m-%d %H:%M:%S");
	CString strSql;
	for(int i=0;i<strTableArr.GetSize();i++)
	{
		CCalcPort *pPortObjOut = GetOutputPortObj(i);
		CUniValue UniVar;//用于输出

		CString strTableName="";
		strTableName.Format("%s_%d",strTableArr[i],st.GetYear());
		CString strSQL;
		_RecordsetPtr m_pRec;
		_variant_t RecordsAffected;
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		try
		{
			strSql.Format("select  * from %s  where %s='%s' and %s>='%s' and %s<='%s' order by %s desc limit 0,1;",
				strTableName,m_strFieldVarName,strTagArr[i],m_strFieldDateTime,strSTime,m_strFieldDateTime,strETime,m_strFieldDateTime);
			m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pConRDB,true),
				adOpenStatic,adLockOptimistic,adCmdText);
			double dRetValue=0;
			short iRetState=0;
			long lRetTime=0;
			if(!m_pRec->adoEOF)
			{
				// CString strConFieldDateTime=this->m_strFieldDateTime;
				CString U_Input_Date =(char *)_bstr_t(m_pRec->GetCollect((_variant_t)m_strFieldDateTime));
				CString U_Input_Value =(char *)_bstr_t(m_pRec->GetCollect((_variant_t)m_strFieldValue));
				CString U_Input_State =(char *)_bstr_t(m_pRec->GetCollect((_variant_t)m_strFieldState));
				
				dRetValue=atof(U_Input_Value);
				iRetState=atoi(U_Input_State);
				
				int   nYear=0;int nMonth=0;int nDate=0;int nHour=0;int nMin=0;int nSec=0; 
				if(U_Input_Date.Find('/')>=0)
					sscanf(U_Input_Date, "%d/%d/%d   %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);
				else
					sscanf(U_Input_Date, "%d-%d-%d   %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);
				CTime   t(nYear,nMonth,nDate,nHour,nMin,nSec);	
				lRetTime=t.GetTime();
				UniVar.AddData(dRetValue,lRetTime,iRetState);
			}
			else
			{
				UniVar.AddData(dRetValue,lRetTime,-1);//没有获取到数据则取状态为-1
			}
		}
		catch(_com_error  *e)
		{   
			CString str;
			str.Format("CRDBHistTagBat读取点'%s'失败,错误信息:%s",strTagArr[i],e->ErrorMessage());
			if(m_iDebugOn)//调试模式
				AfxMessageBox(str);
			else if(m_iTraceOn)//运行模式,且追踪信息
			{
				if(m_pWnd!=NULL)
					m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
			}
			if(m_iLogOnOrOff)
			{
				if(m_pLog!=NULL)
					m_pLog->WriteFileData(str);
			}
		}
		pPortObjOut->SetPortUniValue(UniVar);
	}
	OutputResultToGlobalWS();
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CRDBHistTagBat::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("StrField")==0)
	{
		if(!strItem1.IsEmpty())  {m_strFieldVarName=strItem1;}
		if(!strItem2.IsEmpty())  {m_strFieldDateTime=strItem2;}
		if(!strItem3.IsEmpty())  {m_strFieldState=strItem3;}
		if(!strItem4.IsEmpty())  {m_strFieldValue=strItem4;}
		if(!strItem5.IsEmpty())  {m_strDBName=strItem5;}
	}
	else if(strPropName.Compare("NearSec")==0)
	{
		if(!strItem1.IsEmpty())  {m_iNearSec=atoi(strItem1);}
	}
	else
	{
		int propType,propIndex;
		getPropTypeByName(strPropName,propType,propIndex);
		if(propType==1)
		{
			if(!strItem1.IsEmpty())  {strTableArr.SetAtGrow(propIndex,strItem1);}
			if(!strItem2.IsEmpty())  {strTagArr.SetAtGrow(propIndex,strItem2);}
			if(!strItem3.IsEmpty())  {strTagDespArr.SetAtGrow(propIndex,strItem3);}
			if(!strItem4.IsEmpty())  {strUnitArr.SetAtGrow(propIndex,strItem4);}
		}
	}
}
void CRDBHistTagBat::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
{
	int dIndex=strPropName.ReverseFind('.');
	CString strIndex=strPropName.Right(strPropName.GetLength()-dIndex-1);
	CString strName=strPropName.Left(dIndex);
	propIndex=atoi(strIndex);

	if(strName=="input")//输入为1
	{
		propType=1;
	}
	else if(strName=="output")//输出为2
	{
		propType=2;
	}
	else if(strName=="formula")//公式为3
	{
		propType=3;
	}
}
//重载以显示不同的属性对象对话框
void CRDBHistTagBat::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgRDBHistTagBat dlg;

	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//计算块原有的输出端口描述
	CStringArray  IDArrRightIn;//计算块内部的输出端口描述

	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}

	dlg.strTableArr.Copy(strTableArr);
	dlg.strTagArr.Copy(strTagArr);
	dlg.strTagDespArr.Copy(strTagDespArr);
	dlg.strUnitArr.Copy(strUnitArr);
	dlg.m_strDBName=m_strDBName;
	dlg.m_iNearSec=m_iNearSec;

	dlg.m_strFieldVarName=m_strFieldVarName;
	dlg.m_strFieldDateTime=m_strFieldDateTime;
	dlg.m_strFieldState=m_strFieldState;
	dlg.m_strFieldValue=m_strFieldValue;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);
	if(dlg.DoModal()==IDOK)
	{
		strTableArr.Copy(dlg.strTableArr);
		strTagArr.Copy(dlg.strTagArr);
		strTagDespArr.Copy(dlg.strTagDespArr);
		strUnitArr.Copy(dlg.strUnitArr);
		m_strDBName=dlg.m_strDBName;
		
		m_strFieldVarName=dlg.m_strFieldVarName;
		m_strFieldDateTime=dlg.m_strFieldDateTime;
		m_strFieldState=dlg.m_strFieldState;
		m_strFieldValue=dlg.m_strFieldValue;

		m_iNearSec=dlg.m_iNearSec;
		//对输出端口的修改
		for(int i=0;i<strTagArr.GetSize();i++)
		{
			IDArrRightIn.Add(strTagArr[i]);
		}		
		for(int jj=IDArrRightOut.GetSize()-1; jj>-1; jj--)//删除原有而不用的Port,倒着删
		{
			int  runs=0;//遍历次数
			for(int kk=0; kk<IDArrRightIn.GetSize(); kk++)
			{
				if((IDArrRightIn[kk]!=IDArrRightOut[jj]))	
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
				if((IDArrRightIn[mm]!=IDArrRightOut[nn]))
				{runs++;}		
			}
			if(runs==IDArrRightOut.GetSize())
			{
				CString str=IDArrRightIn[mm];
				InSertOutputPortBefore(mm,str, VALUE_DOUBLE, _T(str));
			}
		}
		parent->RedrawWindow();
	}
}
//重载以将本模块写入到数据库中
void CRDBHistTagBat::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	for(int i=0;i<strTableArr.GetSize();i++)//输出
	{
		CString strInPut=strTableArr[i];
		if(strInPut=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("input.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%s",strTableArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",strTagArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",strTagDespArr[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",strUnitArr[i]); //属性名
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名
	}
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="StrField";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strFieldVarName); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_strFieldDateTime); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_strFieldState); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_strFieldValue); //属性名
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_strDBName); //属性名
	pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //属性名	
	
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="NearSec";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iNearSec); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CRDBHistTagBat::CopyPropertyData(CCalcBlock* pBlock)
{
	this->strTableArr.Copy(((CRDBHistTagBat*)pBlock)->strTableArr);
	this->strTagArr.Copy(((CRDBHistTagBat*)pBlock)->strTagArr);
	this->strTagDespArr.Copy(((CRDBHistTagBat*)pBlock)->strTagDespArr);
	this->strUnitArr.Copy(((CRDBHistTagBat*)pBlock)->strUnitArr);

	this->m_strFieldVarName=((CRDBHistTagBat*)pBlock)->m_strFieldVarName;
	this->m_strFieldDateTime=((CRDBHistTagBat*)pBlock)->m_strFieldDateTime;
	this->m_strFieldState=((CRDBHistTagBat*)pBlock)->m_strFieldState;
	this->m_strFieldValue=((CRDBHistTagBat*)pBlock)->m_strFieldValue;
	this->m_strDBName=((CRDBHistTagBat*)pBlock)->m_strDBName;//数据库名称

	this->m_iNearSec=((CRDBHistTagBat*)pBlock)->m_iNearSec;//数据库名称	
}
//将属性放置传输数据中
void CRDBHistTagBat::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	
}
