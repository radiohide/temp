// WriteRDBProBat.cpp: implementation of the CWriteRDBProBat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WriteRDBProBat.h"
#include "PropertyWriteRDBProBat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWriteRDBProBat::CWriteRDBProBat()
{
	this->m_iOnlySave=0;
	this->m_iSaveSeconds=5400;//默认保存1.5小时

	m_strFieldVarName="VarName";
	m_strFieldDateTime="DateTime";
	m_strFieldRealDateTime="RealDateTime";
	m_strFieldState="State";
	m_strFieldValue="Value";

	SetBlockType(_T( "WriteRDBProBat" ) );
	ConstructBlockInputOutput();
}

CWriteRDBProBat::~CWriteRDBProBat()
{

}

//根据类型新建对象的函数
CCalcBlock* CWriteRDBProBat::CreateFromString( const CString& strType )
{
	CWriteRDBProBat* obj = new CWriteRDBProBat;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//返回属性的逗点字符串
CString CWriteRDBProBat::GetBlockStrPro()
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CWriteRDBProBat::SetBlockStrPro(CString str)//设置计算块的字符串属性
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}

//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CWriteRDBProBat::ConstructBlockInputOutput ()
{
	
}
//初始化计算的函数做特殊的初始化
void CWriteRDBProBat::InitCalc()
{
	CCalcBlock::InitCalc();
}
void CWriteRDBProBat::Calc()//进行计算的函数,需要重载
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
		return;
	GetInputValueFromGlobalWS();
	//搜索出一共多少个表，每个表进行一次插入
	CStringArray tableArr;
	for(int i=0;i<strOutPutNameArr.GetSize();i++)
	{
		bool bFind=false;
		for(int j=0;j<tableArr.GetSize();j++)
		{
			if(tableArr[j]==strOutPutNameArr[i])
			{
				bFind=true;
				break;
			}
		}
		if(!bFind)
		{
			tableArr.Add(strOutPutNameArr[i]);
		}
	}
	//循环插入每一个表
	for(i=0;i<tableArr.GetSize();i++)
	{
		if(tableArr[i]=="")
			continue;
		bool bFindTime=false;//寻找有效时间
		double dValue=0;
		long lTime=0;
		long lrealTime=0;
		short sState=0;
		for(int j=0;j<strOutPutNameArr.GetSize();j++)//找第一个有效时间
		{
			if(strOutPutNameArr[j]==tableArr[i])
			{
				CCalcPort *pPortObj = GetInputPortObj(j);
				
				pPortObj->GetPortUniValue().GetDoubleVal(&dValue);
				pPortObj->GetPortUniValue().GetTimeVal(&lTime);
				pPortObj->GetPortUniValue().GetRealTimeVal(&lrealTime);
				pPortObj->GetPortUniValue().GetStatusVal(&sState);
				if((sState>=0)&&(lTime!=0))//数据状态大于0，且时间不为0
				{
					bFindTime=true;
					break;
				}
			}
		}
		if(!bFindTime)
			continue;
		CTime t(lTime);
		CTime rt(lrealTime);
		CString strTableName="";
		strTableName.Format("%s_%d",tableArr[j],t.GetYear());
		CString strSQL;
		_variant_t RecordsAffected;
		if(!gIsTableExist(m_pConRDB,strTableName))
		{
			strSQL.Format("CREATE TABLE `%s` (  `ID` int(11) NOT NULL AUTO_INCREMENT,  `%s` varchar(50) NOT NULL,`%s` datetime DEFAULT NULL,  `%s` int(1) NOT NULL DEFAULT '0',  `%s` double NOT NULL DEFAULT '0',`%s` datetime DEFAULT NULL,PRIMARY KEY (`ID`)) ENGINE=InnoDB DEFAULT CHARSET=gbk",
					strTableName,m_strFieldVarName,m_strFieldDateTime,m_strFieldState,m_strFieldValue,m_strFieldRealDateTime);	
			m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText); 
		}
		{
			//如果无realtime字段，添加realtime字段，为有些现场已产生表和数据时的补充功能，新现场不需要该功能
			strSQL.Format("show columns from %s like 'RealDateTime'",strTableName);
			m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText);
			int nRow=0;
			nRow=RecordsAffected.iVal;
			if (nRow==0)
			{
				strSQL.Format("alter table %s add %s datetime",strTableName,m_strFieldRealDateTime);
			    m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText);
			}
		}
	
		strSQL.Format("insert into %s(%s,%s,%s,%s,%s) values",
					strTableName,m_strFieldVarName,m_strFieldDateTime,m_strFieldState,m_strFieldValue,m_strFieldRealDateTime);
		CString strTime = t.Format("%Y-%m-%d %H:%M:%S");
		CString strRealTime = rt.Format("%Y-%m-%d %H:%M:%S");
		CString strSQLPart;
		for(j=0;j<strOutPutNameArr.GetSize();j++)//找第一个有效时间
		{
			if(strOutPutNameArr[j]==tableArr[i])
			{
				CCalcPort *pPortObj = GetInputPortObj(j);
				
				pPortObj->GetPortUniValue().GetDoubleVal(&dValue);
				pPortObj->GetPortUniValue().GetTimeVal(&lTime);
				pPortObj->GetPortUniValue().GetRealTimeVal(&lrealTime);//无
				pPortObj->GetPortUniValue().GetStatusVal(&sState);
				if((sState>=0)&&(lTime!=0))//数据状态大于0，且时间不为0
				{
					strSQLPart.Format("(\'%s\',\'%s\',\'%d\',\'%f\',\'%s\'),",tagNameArr[j],strTime,sState,dValue,strRealTime);
					strSQL+=strSQLPart;
				}
			}
		}
		strSQL.Delete(strSQL.GetLength()-1,1);//去掉最后的逗号。
		try
		{
			m_pConRDB->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText); //添加记录
		}
		catch(...)
		{
			CString str;
			str.Format("写入表'%s'失败!",tableArr[i]);
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
		if(m_iOnlySave==1)
		{
			CTime t(lTime-m_iSaveSeconds);
			CString strTime = t.Format("%Y-%m-%d %H:%M:%S");;
			strSQL.Format("DELETE FROM %s WHERE %s < '%s'",strTableName,m_strFieldDateTime,strTime);
			try
			{
				m_pConRDB->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText); //添加记录
			}
			catch(...)
			{
				CString str;
				str.Format("删除表'%s'中记录失败!",tableArr[i]);
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

	//2014-7-20 rao 注释掉、保持mysql长连接，确保计算平台占用资源
	/*
	if (m_pConRDB->GetState() == adStateOpen)
	{
		m_pConRDB->Close();
	}*/
}
BOOL CWriteRDBProBat::gIsTableExist(_ConnectionPtr &pConnection, CString strTableName)
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
void CWriteRDBProBat::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("StrField")==0)
	{
		if(!strItem1.IsEmpty())  {m_strFieldVarName=strItem1;}
		if(!strItem2.IsEmpty())  {m_strFieldDateTime=strItem2;}
		if(!strItem3.IsEmpty())  {m_strFieldState=strItem3;}
		if(!strItem4.IsEmpty())  {m_strFieldValue=strItem4;}
	}
	else if(strPropName.Compare("OnlySave")==0)
	{
		if(!strItem1.IsEmpty())  {m_iOnlySave=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_iSaveSeconds=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {m_strDBName=strItem3;}
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

void CWriteRDBProBat::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
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
void CWriteRDBProBat::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyWriteRDBProBat dlg;
	
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
	
	dlg.m_strFieldVarName=m_strFieldVarName;
	dlg.m_strFieldDateTime=m_strFieldDateTime;
	dlg.m_strFieldState=m_strFieldState;
	dlg.m_strFieldValue=m_strFieldValue;
	
	dlg.m_BOnlySave=this->m_iOnlySave;
	dlg.m_iSaveSeconds=this->m_iSaveSeconds;
	if(this->m_iOnlySave)
	{
		dlg.m_BOnlySave=TRUE;
	}
	if(dlg.DoModal()==IDOK)
	{
		m_strDBName=dlg.m_strDBName;
		tagNameArr.Copy(dlg.tagNameArr);
		unitArr.Copy(dlg.unitArr);
		dispArr.Copy(dlg.dispArr);
		strOutPutNameArr.Copy(dlg.strOutPutNameArr);
		m_strFieldVarName=dlg.m_strFieldVarName;
		m_strFieldDateTime=dlg.m_strFieldDateTime;
		m_strFieldState=dlg.m_strFieldState;
		m_strFieldValue=dlg.m_strFieldValue;
		
		this->m_iOnlySave=dlg.m_BOnlySave;
		this->m_iSaveSeconds=dlg.m_iSaveSeconds;
		
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
				InSertInputPortBefore(m,str, VALUE_DOUBLE, _T(str));
			}
		}
		parent->RedrawWindow();
	}
}
//重载以将本模块写入到数据库中
void CWriteRDBProBat::PutPropertyToRecord(_RecordsetPtr& pRecord)
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

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="OnlySave";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iOnlySave); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_iSaveSeconds); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_strDBName); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	
	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CWriteRDBProBat::CopyPropertyData(CCalcBlock* pBlock)
{
	this->tagNameArr.Copy(((CWriteRDBProBat*)pBlock)->tagNameArr);
	this->strOutPutNameArr.Copy(((CWriteRDBProBat*)pBlock)->strOutPutNameArr);
	this->unitArr.Copy(((CWriteRDBProBat*)pBlock)->unitArr);
	this->dispArr.Copy(((CWriteRDBProBat*)pBlock)->dispArr);
	this->m_iOnlySave=((CWriteRDBProBat*)pBlock)->m_iOnlySave;//只保存标志
	this->m_iSaveSeconds=((CWriteRDBProBat*)pBlock)->m_iSaveSeconds;//只保存最近n秒内的数据。
	this->m_strDBName=((CWriteRDBProBat*)pBlock)->m_strDBName;//数据库名称

	this->m_strFieldVarName=((CWriteRDBProBat*)pBlock)->m_strFieldVarName;
	this->m_strFieldDateTime=((CWriteRDBProBat*)pBlock)->m_strFieldDateTime;
	this->m_strFieldState=((CWriteRDBProBat*)pBlock)->m_strFieldState;
	this->m_strFieldValue=((CWriteRDBProBat*)pBlock)->m_strFieldValue;
}

//将属性放置传输数据中
void CWriteRDBProBat::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}

