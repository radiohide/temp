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
//���������½�����ĺ���
CCalcBlock* CWriteRDBStatBat::CreateFromString( const CString& strType )
{
	CWriteRDBStatBat* obj = new CWriteRDBStatBat;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//�������ԵĶ����ַ���
CString CWriteRDBStatBat::GetBlockStrPro()
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CWriteRDBStatBat::SetBlockStrPro(CString str)//���ü������ַ�������
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CWriteRDBStatBat::ConstructBlockInputOutput ()
{
	
}
//��ʼ������ĺ���������ĳ�ʼ��
void CWriteRDBStatBat::InitCalc()
{
	CCalcBlock::InitCalc();
}
void CWriteRDBStatBat::Calc()//���м���ĺ���,��Ҫ����
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//��ϵ��δ����
		return;
	GetInputValueFromGlobalWS();
	//ѭ������ÿһ����
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
			str.Format("д���'%s'ʧ��!",strOutPutNameArr[i]);
			if(m_iDebugOn)//����ģʽ
				AfxMessageBox(str);
			else if(m_iTraceOn)//����ģʽ,��׷����Ϣ
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
	if(tagBeCon==0)//��ϵ��δ����
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
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
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

	if(strName=="input")//����Ϊ1
	{
		propType=1;
	}
	else if(strName=="output")//���Ϊ2
	{
		propType=2;
	}
	else if(strName=="formula")//��ʽΪ3
	{
		propType=3;
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CWriteRDBStatBat::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyWriteRDBStatBat dlg;
	
	int portnumLinkBefore=this->GetInputNum();//����
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrLeftOut;//�����ԭ�е�����˿�����	
	CStringArray  IDArrLeftIn;//������ڲ�������˿�����
	
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

		//1��������˿ڵ��޸�
		for(int i=0;i<tagNameArr.GetSize();i++)
		{
			if(tagNameArr[i]=="")
				break;
			IDArrLeftIn.Add(tagNameArr[i]);
		}	
		for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)	//ɾ��ԭ�ж����õ�Port,����ɾ
		{
			int  runs=0;//��������
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
		for(int m=0; m<IDArrLeftIn.GetSize(); m++)	//����µ�Port
		{
			int  runs=0;//��������
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
//�����Խ���ģ��д�뵽���ݿ���
void CWriteRDBStatBat::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	for(int i=0;i<tagNameArr.GetSize();i++)//���
	{
		CString strOutPut=tagNameArr[i];
		if(strOutPut=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("input.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������

		strFieldValue.Format("%s",tagNameArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",strOutPutNameArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",unitArr[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",dispArr[i]); //������
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������
	}

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="DBName";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strDBName); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CWriteRDBStatBat::CopyPropertyData(CCalcBlock* pBlock)
{
	this->tagNameArr.Copy(((CWriteRDBStatBat*)pBlock)->tagNameArr);
	this->strOutPutNameArr.Copy(((CWriteRDBStatBat*)pBlock)->strOutPutNameArr);
	this->unitArr.Copy(((CWriteRDBStatBat*)pBlock)->unitArr);
	this->dispArr.Copy(((CWriteRDBStatBat*)pBlock)->dispArr);
	this->m_strDBName=((CWriteRDBStatBat*)pBlock)->m_strDBName;//���ݿ�����
}

//�����Է��ô���������
void CWriteRDBStatBat::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}
