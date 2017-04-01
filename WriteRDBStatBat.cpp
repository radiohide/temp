// WriteRDBStatBat.cpp: implementation of the CWriteRDBStatBat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WriteRDBStatBat.h"
#include "PropertyWriteRDBStatBat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWriteRDBStatBat::CWriteRDBStatBat()
{
	SetBlockType(_T( "WriteRDBStatBat" ) );
	ConstructBlockInputOutput();
}

CWriteRDBStatBat::~CWriteRDBStatBat()
{

}
//根据类型新建对象的函数
CCalcBlock* CWriteRDBStatBat::CreateFromString( const CString& strType )
{
	CWriteRDBStatBat* obj = new CWriteRDBStatBat;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//返回属性的逗点字符串
CString CWriteRDBStatBat::GetBlockStrPro()
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CWriteRDBStatBat::SetBlockStrPro(CString str)//设置计算块的字符串属性
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CWriteRDBStatBat::ConstructBlockInputOutput ()
{
	
}
//初始化计算的函数做特殊的初始化
void CWriteRDBStatBat::InitCalc()
{
	CCalcBlock::InitCalc();
}
void CWriteRDBStatBat::Calc()//进行计算的函数,需要重载
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
		return;
	GetInputValueFromGlobalWS();
	//循环插入每一个表
	for(int i=0;i<strOutPutNameArr.GetSize();i++)
	{
		if(strOutPutNameArr[i]=="")
			continue;
		StatData statData;
		CCalcPort *pPortObj = GetInputPortObj(i);
		pPortObj->GetPortUniValue().GetStatData(&statData);

		CTime t(statData.lSTime);
		CString strTableName="";
		strTableName.Format("%s_%d",strOutPutNameArr[i],t.GetYear());
		CString strSQL;
		_variant_t RecordsAffected;
		if(!gIsTableExist(m_pConRDB,strTableName))
		{
			strSQL.Format("create table %s (id bigint not null auto_increment, start_date datetime,end_date datetime,in_date datetime, max_value decimal(18,6), min_value decimal(18,6), state_type tinyint, total_value decimal(18,6),count int, primary key (id));",strTableName);
			m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText);
		}
		
		try
		{
			writeDataToTable(strTableName,statData.lSTime,statData.lETime,statData.dMax,statData.dMin,statData.iType,statData.dValue,statData.iCount);
		}
		catch(...)
		{
			CString str;
			str.Format("写入表'%s'失败!",strOutPutNameArr[i]);
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
	}
}
void CWriteRDBStatBat::writeDataToTable(
									 CString strTable,
									 long lSTime,
									 long lETime,
									 double dMaxValue,
									 double dMinValue,
									 short iType,
									 double dSum,
									 long iCount)
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
		return;
	CString strSQL;
	_variant_t RecordsAffected;

	CTime sTime(lSTime);
	CTime eTime(lETime);
	CString strSTime,strETime,strCurTime;
	strSTime=sTime.Format("%Y-%m-%d %H:%M:%S");
	strETime=eTime.Format("%Y-%m-%d %H:%M:%S");
	strCurTime=CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
	getInRangeValue(dMaxValue);
	getInRangeValue(dMinValue);
	getInRangeValue(dSum);
	strSQL.Format("insert into %s(start_date,end_date,in_date,max_value,min_value,state_type,total_value,count) values(\'%s\',\'%s\',\'%s\',\'%f\',\'%f\',\'%d\',\'%f\',\'%d\')",
		strTable,strSTime,strETime,strCurTime,
		dMaxValue,dMinValue,iType,dSum,iCount);
	m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText);
}
void CWriteRDBStatBat::getInRangeValue(double &dValue)
{
	if(dValue>99999999999.999999)
	{
		//dValue=99999999999.999999;
		dValue=0.0123456789;
	}
	else if(dValue<-99999999999.999999)
	{
		//dValue=-99999999999.999999;
		dValue=-0.0123456789;
	}
}
BOOL CWriteRDBStatBat::gIsTableExist(_ConnectionPtr &pConnection, CString strTableName)
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	pRecord.CreateInstance(__uuidof(Recordset));
	pRecord->CursorLocation = adUseClient;
	
	strSQL.Format("select * from %s limit 0,1",strTableName);
	try
	{
		pRecord->Open((const char*)strSQL,pConnection.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error e)
	{
		//AfxMessageBox(e.Description());
		return FALSE;
	} 
	return TRUE;
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CWriteRDBStatBat::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
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
			if(!strItem1.IsEmpty())  {tagNameArr.SetAtGrow(propIndex,strItem1);}
			if(!strItem2.IsEmpty())  {strOutPutNameArr.SetAtGrow(propIndex,strItem2);}
			if(!strItem3.IsEmpty())  {unitArr.SetAtGrow(propIndex,strItem3);}
			if(!strItem4.IsEmpty())  {dispArr.SetAtGrow(propIndex,strItem4);}
		}
	}
}
void CWriteRDBStatBat::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
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
void CWriteRDBStatBat::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyWriteRDBStatBat dlg;
	
	int portnumLinkBefore=this->GetInputNum();//新增
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrLeftOut;//计算块原有的输入端口描述	
	CStringArray  IDArrLeftIn;//计算块内部的输入端口描述
	
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		int iPortType=this->GetInputPortObj(q)->GetPortValueType();
		IDArrLeftOut.Add(strPortID);
	}
	
	dlg.m_strDBName=m_strDBName;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);
	dlg.tagNameArr.Copy(tagNameArr);
	dlg.unitArr.Copy(unitArr);
	dlg.dispArr.Copy(dispArr);
	dlg.strOutPutNameArr.Copy(strOutPutNameArr);

	if(dlg.DoModal()==IDOK)
	{
		m_strDBName=dlg.m_strDBName;
		tagNameArr.Copy(dlg.tagNameArr);
		unitArr.Copy(dlg.unitArr);
		dispArr.Copy(dlg.dispArr);
		strOutPutNameArr.Copy(dlg.strOutPutNameArr);

		//1、对输入端口的修改
		for(int i=0;i<tagNameArr.GetSize();i++)
		{
			if(tagNameArr[i]=="")
				break;
			IDArrLeftIn.Add(tagNameArr[i]);
		}	
		for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)	//删除原有而不用的Port,倒着删
		{
			int  runs=0;//遍历次数
			for(int k=0; k<IDArrLeftIn.GetSize(); k++)
			{
				if((IDArrLeftIn[k]!=IDArrLeftOut[j]))	
				{runs++;}
			}
			if(runs==IDArrLeftIn.GetSize())
			{
				this->ClearInputPortByIndex(j);
			}
		}	
		for(int m=0; m<IDArrLeftIn.GetSize(); m++)	//添加新的Port
		{
			int  runs=0;//遍历次数
			for(int n=0; n<IDArrLeftOut.GetSize(); n++)
			{
				if((IDArrLeftIn[m]!=IDArrLeftOut[n]))
				{runs++;}		
			}
			if(runs==IDArrLeftOut.GetSize())
			{
				CString str=IDArrLeftIn[m];
				InSertInputPortBefore(m,str, VALUE_STAT, _T(str));
			}
		}
		parent->RedrawWindow();
	}
}
//重载以将本模块写入到数据库中
void CWriteRDBStatBat::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	for(int i=0;i<tagNameArr.GetSize();i++)//输出
	{
		CString strOutPut=tagNameArr[i];
		if(strOutPut=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("input.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%s",tagNameArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",strOutPutNameArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",unitArr[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",dispArr[i]); //属性名
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
void CWriteRDBStatBat::CopyPropertyData(CCalcBlock* pBlock)
{
	this->tagNameArr.Copy(((CWriteRDBStatBat*)pBlock)->tagNameArr);
	this->strOutPutNameArr.Copy(((CWriteRDBStatBat*)pBlock)->strOutPutNameArr);
	this->unitArr.Copy(((CWriteRDBStatBat*)pBlock)->unitArr);
	this->dispArr.Copy(((CWriteRDBStatBat*)pBlock)->dispArr);
	this->m_strDBName=((CWriteRDBStatBat*)pBlock)->m_strDBName;//数据库名称
}

//将属性放置传输数据中
void CWriteRDBStatBat::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}
