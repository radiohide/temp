// RDBStatSeriesBat.cpp: implementation of the CRDBStatSeriesBat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RDBStatSeriesBat.h"
#include "PropertyDlgRDBStatSeriesBat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRDBStatSeriesBat::CRDBStatSeriesBat()
{
	SetBlockType(_T( "RDBStatSeriesBat" ) );
	ConstructBlockInputOutput();
}

CRDBStatSeriesBat::~CRDBStatSeriesBat()
{

}
//根据类型新建对象的函数
CCalcBlock* CRDBStatSeriesBat::CreateFromString( const CString& strType )
{
	CRDBStatSeriesBat* obj = new CRDBStatSeriesBat;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CRDBStatSeriesBat::ConstructBlockInputOutput ()
{
	AddInputPort("StartTime",VALUE_DOUBLE);
	AddInputPort("EndTime",VALUE_DOUBLE);
}
//初始化计算的函数做特殊的初始化
void CRDBStatSeriesBat::InitCalc()
{
	CCalcBlock::InitCalc();
}//返回属性的逗点字符串
CString CRDBStatSeriesBat::GetBlockStrPro()//to do
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CRDBStatSeriesBat::SetBlockStrPro(CString str)//设置计算块的字符串属性//to do
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
void CRDBStatSeriesBat::Calc()//进行计算的函数,需要重载
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
	CCalcPort *pInPortObj2 = GetInputPortObj(1);
	pInPortObj2->GetPortUniValue().GetDoubleVal(&dInValue);
	lETime=(long)dInValue;
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
			strSql.Format("select  * from %s  where start_date>='%s' and end_date<='%s';",
				strTableName,strSTime,strETime);
			m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pConRDB,true),
				adOpenStatic,adLockOptimistic,adCmdText);
			double dRetValue=0;
			short iRetState=0;
			long lRetTime=0;
			while(!m_pRec->adoEOF)
			{
				StatData statData;
				CString str_start_date =(char *)_bstr_t(m_pRec->GetCollect("start_date"));
				CString str_end_date =(char *)_bstr_t(m_pRec->GetCollect("end_date"));
				CString str_in_date =(char *)_bstr_t(m_pRec->GetCollect("in_date"));
				CString str_max_value=(char *)_bstr_t(m_pRec->GetCollect("max_value"));
				CString str_min_value=(char *)_bstr_t(m_pRec->GetCollect("min_value"));
				CString str_state_type=(char *)_bstr_t(m_pRec->GetCollect("state_type"));
				CString str_total_value=(char *)_bstr_t(m_pRec->GetCollect("total_value"));
				CString str_count=(char *)_bstr_t(m_pRec->GetCollect("count"));
				
				statData.lSTime=getLTimeFromStr(str_start_date);
				statData.lETime=getLTimeFromStr(str_end_date);
				statData.lInsertTime=getLTimeFromStr(str_in_date);
				statData.dMax=atof(str_max_value);
				statData.dMin=atof(str_min_value);
				statData.iType=atoi(str_state_type);
				statData.dValue=atof(str_total_value);
				statData.iCount=atoi(str_count);
				
				UniVar.AddStatData(statData);
				m_pRec->MoveNext();
			}
		}
		catch(_com_error  *e)
		{   
			CString str;
			str.Format("CRDBStatSeriesBat读取点'%s'失败,错误信息:%s",strTagArr[i],e->ErrorMessage());
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
long CRDBStatSeriesBat::getLTimeFromStr(const CString &strTime)
{
	long lRetTime=0;
	try
	{
		int   nYear=0;int nMonth=0;int nDate=0;int nHour=0;int nMin=0;int nSec=0; 
		if(strTime.Find('/')>=0)
			sscanf(strTime, "%d/%d/%d   %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);
		else
			sscanf(strTime, "%d-%d-%d   %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);
		CTime   t(nYear,nMonth,nDate,nHour,nMin,nSec);	
		lRetTime=t.GetTime();
	}
	catch(...)
	{
		
	}
	return lRetTime;
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CRDBStatSeriesBat::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("DBName")==0)
	{
		if(!strItem1.IsEmpty())  {m_strDBName=strItem1;}
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
void CRDBStatSeriesBat::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
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
void CRDBStatSeriesBat::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgRDBStatSeriesBat dlg;

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
				InSertOutputPortBefore(mm,str, VALUE_STAT_SERIES, _T(str));
			}
		}
		parent->RedrawWindow();
	}
}
//重载以将本模块写入到数据库中
void CRDBStatSeriesBat::PutPropertyToRecord(_RecordsetPtr& pRecord)
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
	strFieldValue="DBName";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strDBName); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	
	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CRDBStatSeriesBat::CopyPropertyData(CCalcBlock* pBlock)
{
	this->strTableArr.Copy(((CRDBStatSeriesBat*)pBlock)->strTableArr);
	this->strTagArr.Copy(((CRDBStatSeriesBat*)pBlock)->strTagArr);
	this->strTagDespArr.Copy(((CRDBStatSeriesBat*)pBlock)->strTagDespArr);
	this->strUnitArr.Copy(((CRDBStatSeriesBat*)pBlock)->strUnitArr);

	this->m_strDBName=((CRDBStatSeriesBat*)pBlock)->m_strDBName;//数据库名称
}
//将属性放置传输数据中
void CRDBStatSeriesBat::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}
