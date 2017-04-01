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
	this->m_iSaveSeconds=5400;//Ĭ�ϱ���1.5Сʱ

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

//���������½�����ĺ���
CCalcBlock* CWriteRDBProBat::CreateFromString( const CString& strType )
{
	CWriteRDBProBat* obj = new CWriteRDBProBat;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//�������ԵĶ����ַ���
CString CWriteRDBProBat::GetBlockStrPro()
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CWriteRDBProBat::SetBlockStrPro(CString str)//���ü������ַ�������
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}

//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CWriteRDBProBat::ConstructBlockInputOutput ()
{
	
}
//��ʼ������ĺ���������ĳ�ʼ��
void CWriteRDBProBat::InitCalc()
{
	CCalcBlock::InitCalc();
}
void CWriteRDBProBat::Calc()//���м���ĺ���,��Ҫ����
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//��ϵ��δ����
		return;
	GetInputValueFromGlobalWS();
	//������һ�����ٸ���ÿ�������һ�β���
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
	//ѭ������ÿһ����
	for(i=0;i<tableArr.GetSize();i++)
	{
		if(tableArr[i]=="")
			continue;
		bool bFindTime=false;//Ѱ����Чʱ��
		double dValue=0;
		long lTime=0;
		long lrealTime=0;
		short sState=0;
		for(int j=0;j<strOutPutNameArr.GetSize();j++)//�ҵ�һ����Чʱ��
		{
			if(strOutPutNameArr[j]==tableArr[i])
			{
				CCalcPort *pPortObj = GetInputPortObj(j);
				
				pPortObj->GetPortUniValue().GetDoubleVal(&dValue);
				pPortObj->GetPortUniValue().GetTimeVal(&lTime);
				pPortObj->GetPortUniValue().GetRealTimeVal(&lrealTime);
				pPortObj->GetPortUniValue().GetStatusVal(&sState);
				if((sState>=0)&&(lTime!=0))//����״̬����0����ʱ�䲻Ϊ0
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
			//�����realtime�ֶΣ����realtime�ֶΣ�Ϊ��Щ�ֳ��Ѳ����������ʱ�Ĳ��书�ܣ����ֳ�����Ҫ�ù���
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
		for(j=0;j<strOutPutNameArr.GetSize();j++)//�ҵ�һ����Чʱ��
		{
			if(strOutPutNameArr[j]==tableArr[i])
			{
				CCalcPort *pPortObj = GetInputPortObj(j);
				
				pPortObj->GetPortUniValue().GetDoubleVal(&dValue);
				pPortObj->GetPortUniValue().GetTimeVal(&lTime);
				pPortObj->GetPortUniValue().GetRealTimeVal(&lrealTime);//��
				pPortObj->GetPortUniValue().GetStatusVal(&sState);
				if((sState>=0)&&(lTime!=0))//����״̬����0����ʱ�䲻Ϊ0
				{
					strSQLPart.Format("(\'%s\',\'%s\',\'%d\',\'%f\',\'%s\'),",tagNameArr[j],strTime,sState,dValue,strRealTime);
					strSQL+=strSQLPart;
				}
			}
		}
		strSQL.Delete(strSQL.GetLength()-1,1);//ȥ�����Ķ��š�
		try
		{
			m_pConRDB->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText); //��Ӽ�¼
		}
		catch(...)
		{
			CString str;
			str.Format("д���'%s'ʧ��!",tableArr[i]);
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
		if(m_iOnlySave==1)
		{
			CTime t(lTime-m_iSaveSeconds);
			CString strTime = t.Format("%Y-%m-%d %H:%M:%S");;
			strSQL.Format("DELETE FROM %s WHERE %s < '%s'",strTableName,m_strFieldDateTime,strTime);
			try
			{
				m_pConRDB->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText); //��Ӽ�¼
			}
			catch(...)
			{
				CString str;
				str.Format("ɾ����'%s'�м�¼ʧ��!",tableArr[i]);
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

	//2014-7-20 rao ע�͵�������mysql�����ӣ�ȷ������ƽ̨ռ����Դ
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
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
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
void CWriteRDBProBat::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyWriteRDBProBat dlg;
	
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
				InSertInputPortBefore(m,str, VALUE_DOUBLE, _T(str));
			}
		}
		parent->RedrawWindow();
	}
}
//�����Խ���ģ��д�뵽���ݿ���
void CWriteRDBProBat::PutPropertyToRecord(_RecordsetPtr& pRecord)
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
	strFieldValue="StrField";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strFieldVarName); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%s",m_strFieldDateTime); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%s",m_strFieldState); //������
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%s",m_strFieldValue); //������
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="OnlySave";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iOnlySave); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iSaveSeconds); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%s",m_strDBName); //������
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
	
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CWriteRDBProBat::CopyPropertyData(CCalcBlock* pBlock)
{
	this->tagNameArr.Copy(((CWriteRDBProBat*)pBlock)->tagNameArr);
	this->strOutPutNameArr.Copy(((CWriteRDBProBat*)pBlock)->strOutPutNameArr);
	this->unitArr.Copy(((CWriteRDBProBat*)pBlock)->unitArr);
	this->dispArr.Copy(((CWriteRDBProBat*)pBlock)->dispArr);
	this->m_iOnlySave=((CWriteRDBProBat*)pBlock)->m_iOnlySave;//ֻ�����־
	this->m_iSaveSeconds=((CWriteRDBProBat*)pBlock)->m_iSaveSeconds;//ֻ�������n���ڵ����ݡ�
	this->m_strDBName=((CWriteRDBProBat*)pBlock)->m_strDBName;//���ݿ�����

	this->m_strFieldVarName=((CWriteRDBProBat*)pBlock)->m_strFieldVarName;
	this->m_strFieldDateTime=((CWriteRDBProBat*)pBlock)->m_strFieldDateTime;
	this->m_strFieldState=((CWriteRDBProBat*)pBlock)->m_strFieldState;
	this->m_strFieldValue=((CWriteRDBProBat*)pBlock)->m_strFieldValue;
}

//�����Է��ô���������
void CWriteRDBProBat::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}

