// StatAverBlock.cpp: implementation of the CStatAverBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatAverBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatAverBlock::CStatAverBlock()
{
	m_strBlockDesp="ƽ��ֵ�͵�ͳ�ƿ顣����ͳ��ָ���ƽ��ֵ�����������Ӧ�����ݱ��С�";
	m_strTitle="ͳ�ƿ�����ҳ";

	SetBlockType(_T( "StatAver" ) );
	m_iVarType=3;//1:accum;2enum;3aver;4impt
	ConstructBlockInputOutput();
}

CStatAverBlock::~CStatAverBlock()
{

}
//���������½�����ĺ���
CCalcBlock* CStatAverBlock::CreateFromString( const CString& strType )
{
	CStatAverBlock* obj = new CStatAverBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CStatAverBlock::ConstructBlockInputOutput ()
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
		AddInputPort(_T(m_strVarNameArr[i]), VALUE_RTVALUE_SERIES);
	}
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CStatAverBlock::Calc()//���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	m_sTimeArr.RemoveAll();
	m_eTimeArr.RemoveAll();
	m_dAvgArr.RemoveAll();
	m_dMaxArr.RemoveAll();
	m_dMinArr.RemoveAll();
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
		double sum=0;
		double sTime=0;
		double eTime=0;
		double dMax=0;
		double dMin=0;
		for(int j=0; j<CUV.GetDataSize();j++)
		{
			double value;
			long time;
			CUV.GetDoubleValAt(j,&value);
			CUV.GetTimeValAt(j,&time);
			sum+=value;
			if(j==0)
			{
				sTime=time;
				eTime=time;
				dMax=value;
				dMin=value;
				continue;
			}
			if(time<sTime){	sTime=time;}
			if(time>eTime){	eTime=time;}
			if(value>dMax){	dMax=value;}
			if(value<dMin){	dMin=value;}
		}
		if(m_iSETimeType==2)//��������ʱ��
		{
			m_sTimeArr.Add((long)sTime);
			m_eTimeArr.Add((long)eTime);
		}
		if(CUV.GetDataSize()!=0)
			m_dAvgArr.Add(sum/(CUV.GetDataSize()));
		else 
			m_dAvgArr.Add(0);
		m_dMaxArr.Add(dMax);
		m_dMinArr.Add(dMin);
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
		writeDataToDB(strVarName,strUnitSet,m_sTimeArr[i],m_eTimeArr[i],m_dAvgArr[i],m_dMaxArr[i],m_dMinArr[i]);
	}
}
void CStatAverBlock::writeDataToDB(CString strVarName,CString strUnitSet,long lSTime,long lETime,double dAvgValue,double dMaxValue,double dMinValue)
{
	CString strTableName;
	strTableName.Format("st_%s_aver_%s",strUnitSet,strVarName);

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
			strSQL.Format("CREATE TABLE `%s`(`id` bigint(20)  NOT NULL AUTO_INCREMENT,`var_value` double NOT NULL,`start_date` datetime NOT NULL,`end_date` datetime NOT NULL,`in_date` datetime NOT NULL,`max_value` double NOT NULL,`min_value` double NOT NULL, PRIMARY KEY (`id`)) ENGINE=InnoDB DEFAULT CHARSET=gbk;",strTableName);		
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
			strSQL.Format("insert into %s(var_value,start_date,end_date,in_date,max_value,min_value)values(\'%f\',\'%s\',\'%s\',\'%s\',\'%f\',\'%f\')",
				strTableName,dAvgValue,strSTime,strETime,strCurrent,dMaxValue,dMinValue);
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
