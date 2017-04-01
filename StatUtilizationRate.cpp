// StatUtilizationRate.cpp: implementation of the CStatUtilizationRate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatUtilizationRate.h"

#include <vector>
#include <math.h>
//#include "PropertyWindPowerPerf.h"

#include "propertyUtilizationRate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatUtilizationRate::CStatUtilizationRate()
{
	ConstructBlockInputOutput();
	SetBlockType(_T( "StatUtilizationRate" ) );
}

CStatUtilizationRate::~CStatUtilizationRate()
{

}

//���������½�����ĺ���
CCalcBlock* CStatUtilizationRate::CreateFromString( const CString& strType )
{
	CStatUtilizationRate* obj = new CStatUtilizationRate();
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CStatUtilizationRate::ConstructBlockInputOutput ()
{
	AddInputPort("PowerUpTime",VALUE_DOUBLE);
	AddInputPort("StandyByTime",VALUE_DOUBLE);
	AddInputPort("ManualDoTime",VALUE_DOUBLE);
	AddInputPort("StartUpTime",VALUE_DOUBLE);
	AddInputPort("ErrorShutDownTime",VALUE_DOUBLE);
	AddInputPort("PowerProdDoTime",VALUE_DOUBLE);
	AddInputPort("NormalStopTime",VALUE_DOUBLE);
	AddInputPort("ErrGlbTime",VALUE_DOUBLE);
}
//��ʼ������ĺ���������ĳ�ʼ��
void CStatUtilizationRate::InitCalc()
{
	CCalcBlock::InitCalc();
}

void CStatUtilizationRate::Calc()//���м���ĺ���,��Ҫ����
{
	//�ϸ񱣳��������ݸ���һ�£���һ��ʱ��̶���
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//��ϵ��δ����
		return;

	GetInputValueFromGlobalWS();

	//��ȡģ�����������
	int inNum = GetInputNum();
	TagData stateTable[8] = {0};
	double dSum = 0.0;//�ۼ�ֵ
	for(int k=0;k<inNum;k++)
	{
		CCalcPort *pPortObj = GetInputPortObj(k);
		pPortObj->GetPortUniValue().GetDoubleVal(&(stateTable[k].value));
		pPortObj->GetPortUniValue().GetTimeVal(&(stateTable[k].time));
		short status = 0;
		pPortObj->GetPortUniValue().GetStatusVal(&status);
		stateTable[k].status = status;
		dSum = dSum + stateTable[k].value;
	}

	//�����������������ײ�PLC�ۼƳ�ʱ���������ۼƳ�24������
	//����˼·����ʵ�ʱ������Ż����������������
	//DataRevise(stateTable,dSum);//ɽ������Դ����Ҫ����ӣ������ֳ�ȥ��

	//����������
	double dUtiliRate = 0.0;
	double fzero = 0.00001;
	if(dSum > fzero)
	{
		dUtiliRate = (dSum-stateTable[4].value)/dSum;
	}

	//��������
	CString strSQL;
	long timeflag=this->m_pModel->m_lDataTime;
	CTime t(timeflag);
	CString strTableName;
	strTableName.Format("%s_%d",m_strTableName,t.GetYear());
	
	//�ޱ���
	_variant_t RecordsAffected;
	if(!gIsTableExist(m_pConRDB,strTableName))
	{
		strSQL.Format("CREATE TABLE `%s` (`ID` int(11) NOT NULL AUTO_INCREMENT,`WID` varchar(20) NOT NULL,`time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,`PowerUpTime` double NOT NULL,`StandyByTime` double NOT NULL,`ManualDoTime` double NOT NULL,`StartUpTime` double NOT NULL,`ErrorShutDownTime` double NOT NULL,`PowerProdDoTime` double NOT NULL,`NormalStopTime` double NOT NULL,`ErrGlbTime` double NOT NULL, `UtiliRate` double NOT NULL,PRIMARY KEY (`ID`)) ENGINE=InnoDB DEFAULT CHARSET=gbk",
			strTableName);
		m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText);
	}

	try
	{
		writeDataToTable(m_strTurbine,strTableName,stateTable,dUtiliRate);
	}
	catch(...)
	{
		CString str;
		//str.Format("д���'%s'ʧ��!",strOutPutNameArr[i]);
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
void CStatUtilizationRate::writeDataToTable(CString strTurbine,CString strTableName,TagData stateTable[],double dUtiliRate)
{
	
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//��ϵ��δ����
		return;
	CString strSQL;

	_variant_t RecordsAffected;
	
	//��ȡ����ʱ��
	long timeflag=this->m_pModel->m_lDataTime;

	CTime time(timeflag);
	CString strTime;
	strTime=time.Format("%Y-%m-%d %H:%M:%S");
	strSQL.Format("insert into %s (WID,time,PowerUpTime,StandyByTime,ManualDoTime,StartUpTime,ErrorShutDownTime,PowerProdDoTime,NormalStopTime,ErrGlbTime,UtiliRate) values('%s','%s','%lf','%lf','%lf','%lf','%lf','%lf','%lf','%lf','%lf')",
		strTableName,strTurbine,strTime,stateTable[0].value,stateTable[1].value,stateTable[2].value,stateTable[3].value,stateTable[4].value,stateTable[5].value,stateTable[6].value,stateTable[7].value,dUtiliRate);

	m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText);
	
}

BOOL CStatUtilizationRate::gIsTableExist(_ConnectionPtr &pConnection, CString strTableName)
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
void CStatUtilizationRate::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	
	if(strPropName.Compare("DBInf")==0)
	{
		if(!strItem1.IsEmpty())  {m_strDBName=(strItem1);}
		if(!strItem2.IsEmpty())  {m_strTableName=(strItem2);}
	}
	else if(strPropName.Compare("CalcInfo")==0)
	{
		if(!strItem1.IsEmpty())  {m_strTurbine=(strItem1);}
	}
	
}
//��������ʾ��ͬ�����Զ���Ի���
void CStatUtilizationRate::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	
	//CPropertyWindPowerPerf dlg;
	CpropertyUtilizationRate dlg;
	dlg.m_strDBName=m_strDBName;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);
	dlg.m_strTableName=m_strTableName;
	dlg.m_strTurbine=m_strTurbine;

	if(dlg.DoModal()==IDOK)
	{
		m_strDBName=dlg.m_strDBName;
		m_strTableName=dlg.m_strTableName;
		m_strTurbine=dlg.m_strTurbine;
	}
	
}
//�����Խ���ģ��д�뵽���ݿ���
void CStatUtilizationRate::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="DBInf";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strDBName); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strTableName); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="CalcInfo";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strTurbine); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������

	pRecord->Update();
	
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CStatUtilizationRate::CopyPropertyData(CCalcBlock* pBlock)
{	
	this->m_strDBName=((CStatUtilizationRate*)pBlock)->m_strDBName;//���ݿ�����
	this->m_strTableName=((CStatUtilizationRate*)pBlock)->m_strTableName;
	this->m_strTurbine=((CStatUtilizationRate*)pBlock)->m_strTurbine;
}
//�����Է��ô���������
void CStatUtilizationRate::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}

void CStatUtilizationRate::DataRevise(TagData stateTable[],double &dSum)
{
	long lTime = this->m_pModel->m_lDataTime;
	CTime ctCurCalc = (CTime)lTime;
	int nMonth = 1;
	if (ctCurCalc.GetMonth() != 1)
	{
		nMonth = ctCurCalc.GetMonth() -1;
	}
	else
	{
		nMonth = 12;
	}
	
	int nYear = ctCurCalc.GetYear() -1;

	if (dSum>0 && dSum<48)//�������ۼ�ʱ�䲻�ᳬ��48Сʱ
	{
		if (dSum>24)
		{
			dSum = 24;
			DataReviseR(stateTable,dSum);
		}
		
	}
	
	if (dSum>(2*24) && dSum<(35*24))//�������ۼ�ʱ�䲻�ᳬ��60*24Сʱ
	{
		switch(nMonth)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			{
				if (dSum > (24*31))
				{
					dSum = 24*31;
			        DataReviseR(stateTable,dSum);
				}
			}
			break;

		case 4:
		case 6:
		case 9:
		case 11:
			{
				if (dSum > (24*30))
				{
					dSum = 24*30;
					DataReviseR(stateTable,dSum);
				}
			}
			break;

		case 2:
			{
				if (nYear%4 == 0)
				{
					if (dSum > (24*29))
					{
						dSum = 24*29;
			            DataReviseR(stateTable,dSum);
					}
				}
				else
				{
					if (dSum > (24*28))
					{
						dSum = 24*28;
			            DataReviseR(stateTable,dSum);
					}
				}
			}
			break;
		}
	}
	
	if (dSum>(35*24) && dSum<(400*24))//�������ۼ�ʱ�䲻�ᳬ��500*24Сʱ
	{
		if (nYear%4 == 0)
		{
			if (dSum > (24*366))
			{
				dSum = 24*366;
				DataReviseR(stateTable,dSum);
			}
		}
		else
		{
			if (dSum > (24*365))
			{
				dSum = 24*365;
				DataReviseR(stateTable,dSum);
			}
		}
	}
}

//�����ȱ�������
void CStatUtilizationRate::DataReviseR(TagData stateTable[],double dSum)
{
	double dSum2 = 0.0;
	for (int i=0;i<8;i++)
	{
		dSum2 = stateTable[i].value + dSum2;
	}

	for (i=0;i<8;i++)
	{
		stateTable[i].value = (stateTable[i].value*dSum)/dSum2;
	}
}
