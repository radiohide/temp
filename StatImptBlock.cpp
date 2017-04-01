// StatImptBlock.cpp: implementation of the CStatImptBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatImptBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatImptBlock::CStatImptBlock()
{
	m_strBlockDesp="�����͵�ͳ�ƿ顣����ͳ���ֶ����������������ָ��㣬���������Ӧ�����ݱ��С�";
	m_strTitle="ͳ�ƿ�����ҳ";

	SetBlockType(_T( "StatImpt" ) );
	m_iVarType=4;//1:accum;2enum;3aver;4impt
	ConstructBlockInputOutput();
}

CStatImptBlock::~CStatImptBlock()
{

}
//���������½�����ĺ���
CCalcBlock* CStatImptBlock::CreateFromString( const CString& strType )
{
	CStatImptBlock* obj = new CStatImptBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CStatImptBlock::ConstructBlockInputOutput ()
{
	if((m_iSETimeType==0)||(m_iSETimeType==2))//ϵͳʱ���,����ʱ���
	{
		ClearInputPorts();
	}
	else if(m_iSETimeType==1)//�ⲿ����ʱ���
	{
		ClearInputPorts();	
		AddInputPort(_T("STime"), VALUE_DOUBLE);
		AddInputPort(_T("ETime"), VALUE_DOUBLE);
	}
	for(int i=0;i<this->m_strVarNameArr.GetSize();i++)
	{
		AddInputPort(_T(m_strVarNameArr[i]), VALUE_DOUBLE);
	}
}
void CStatImptBlock::Calc()//���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	m_dValueArr.RemoveAll();
	m_sTimeArr.RemoveAll();
	m_eTimeArr.RemoveAll();
	int inNum = GetInputNum();
	int portIndexStart=0;
	if((m_iSETimeType==0)||(m_iSETimeType==1))//0:ϵͳʱ��Ρ�1:�ⲿ����ʱ���.2:����ʱ���
	{
		portIndexStart=0;
	}
	else
		portIndexStart=2;
	for(int i=portIndexStart; i<inNum; i++)//ȡ�����Сֵ����ֵ����ʱ��ο�ʼʱ��ͽ���ʱ��
	{
		CCalcPort *pPortObjIn = GetInputPortObj(i);		
		CUniValue  &CUV=pPortObjIn->GetPortUniValue();
		double value;
		CUV.GetDoubleVal(&value);
		m_dValueArr.Add(value);
	}
	if((m_iSETimeType==0)||(m_iSETimeType==1))
	{
		double dSTime,dETime;
		if(m_iSETimeType==0)//0:ϵͳʱ��Ρ�1:�ⲿ����ʱ���.2:����ʱ���
		{
			dSTime=this->m_pModel->m_lDataSTime;
			dETime=this->m_pModel->m_lDataETime;
		}
		else if(m_iSETimeType==1)//�ⲿʱ��
		{
			CCalcPort *pPortObj = GetInputPortObj(0);
			pPortObj->GetPortUniValue().GetDoubleVal(&dSTime);
			pPortObj = GetInputPortObj(1);
			pPortObj->GetPortUniValue().GetDoubleVal(&dETime);
		}
		for(int i=0;i<m_strVarNameArr.GetSize();i++)
		{
			m_sTimeArr.Add((long)dSTime);
			m_eTimeArr.Add((long)dETime);
		}
	}
	for(i=0;i<m_strVarNameArr.GetSize();i++)
	{
		CString strVarName=m_strVarNameArr[i];
		CString strUnitSet=m_strUnitSetArr[i];
		if((m_sTimeArr[i]==0)||(m_eTimeArr[i]==0))//���ʱ��Ϊ0����ʾ�����������������
			continue;
		writeDataToDB(strVarName,strUnitSet,m_sTimeArr[i],m_eTimeArr[i],m_dValueArr[i]);
	}
}
void CStatImptBlock::writeDataToDB(CString strVarName,CString strUnitSet,long lSTime,long lETime,double dValue)
{
	CString strTableName;
	strTableName.Format("st_%s_impt_%s",strUnitSet,strVarName);

	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//��ϵ��δ����
		return;
	CString strSQL;
	_variant_t RecordsAffected;
	if(!gIsTableExist(m_pConRDB, strTableName))
	{	
		strDBType.MakeUpper();
		if(strDBType=="MYSQL")
			strSQL.Format("CREATE TABLE `%s`(`id` bigint(20)  NOT NULL AUTO_INCREMENT,`var_value` double NOT NULL,`start_date` datetime NOT NULL,`end_date` datetime NOT NULL,`in_date` datetime NOT NULL, PRIMARY KEY (`id`)) ENGINE=InnoDB DEFAULT CHARSET=gbk;",strTableName);		
		else if(strDBType=="MDB")//������Access�д����ñ��Sql���
		{
			//strSQL.Format("Create TABLE %s(ID COUNTER(1, 1) CONSTRAINT PK_TVIPLevel26 PRIMARY KEY,LongID varchar(250), DateTime DATETIME, State SMALLINT,Value varchar(250)",m_strTableName);
		}
		m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText); 
	}
	try
	{		
		CTime tS(lSTime);
		CString strSTime=tS.Format("%Y-%m-%d %H:%M:%S");
		CTime tE(lETime);
		CString strETime=tE.Format("%Y-%m-%d %H:%M:%S");
		CString strCurrent=(CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
		if(strDBType=="MYSQL")
		{
			strSQL.Format("insert into %s(var_value,start_date,end_date,in_date)values(\'%f\',\'%s\',\'%s\',\'%s\')",
				strTableName,dValue,strSTime,strETime,strCurrent);
		}
		else if(strDBType=="MDB")
		{
			//strSQL.Format("insert into [%s]([LongID],[DateTime],[State],[Value]) values(\'%s\',\'%s\',\'%d\',\'%f\')",
			//	strTableName,this->m_strTagName,strTime,this->m_iPointStatus,inValue);
		}
		m_pConRDB->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText); //��Ӽ�¼
	}
	catch(...)
	{
		AfxMessageBox("д���ϵ��ʧ�ܣ�");
	}
}

