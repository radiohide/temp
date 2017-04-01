// StatStandardWindPowerPerf.cpp: implementation of the CStatStandardWindPowerPerf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatStandardWindPowerPerf.h"
#include <vector>
#include "WindPowerTag.h"
#include <math.h>
#include "PropertyStandardWindPowerPerf.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatStandardWindPowerPerf::CStatStandardWindPowerPerf()
{
	m_iPowerPorTagMin=22;//������ʶ
	m_iPowerPorTagMax=24;//������ʶ
	m_dPowerLimit=3150000;//�޵��ʶ
	m_dMinParWindSpeed=3;//��С�������
	m_dMaxParWindSpeed=15;//������85%��������ʱ����

	ConstructBlockInputOutput();
	SetBlockType(_T( "StatStandardWindPowerPerf" ) );
}

CStatStandardWindPowerPerf::~CStatStandardWindPowerPerf()
{

}
//���������½�����ĺ���
CCalcBlock* CStatStandardWindPowerPerf::CreateFromString( const CString& strType )
{
	CStatStandardWindPowerPerf* obj = new CStatStandardWindPowerPerf();
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CStatStandardWindPowerPerf::ConstructBlockInputOutput ()
{
	AddInputPort("WindSpeed",VALUE_RTVALUE_SERIES);//����
	AddInputPort("Power",VALUE_RTVALUE_SERIES);//����
	AddInputPort("PowerProductionTag",VALUE_RTVALUE_SERIES);//������ʶ
	//AddInputPort("PowerLimit",VALUE_RTVALUE_SERIES);//�޵��־
}
//��ʼ������ĺ���������ĳ�ʼ��
void CStatStandardWindPowerPerf::InitCalc()
{
	CCalcBlock::InitCalc();
}
//�������ԵĶ����ַ���
CString CStatStandardWindPowerPerf::GetBlockStrPro()//to do
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CStatStandardWindPowerPerf::SetBlockStrPro(CString str)//���ü������ַ�������//to do
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
void CStatStandardWindPowerPerf::Calc()//���м���ĺ���,��Ҫ����
{
	//�ϸ񱣳��������ݸ���һ�£���һ��ʱ��̶���
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//��ϵ��δ����
		return;

	GetInputValueFromGlobalWS();

	CCalcPort *pPortObjWindSpeed = GetInputPortObj(0);	
	CUniValue  &CUVWindSpeed=pPortObjWindSpeed->GetPortUniValue();
	CCalcPort *pPortObjPower = GetInputPortObj(1);	
	CUniValue  &CUVPower=pPortObjPower->GetPortUniValue();

	
	CCalcPort *pPortObjProductionTag = GetInputPortObj(2);
	CUniValue  &CUVProductionTag=pPortObjProductionTag->GetPortUniValue();
	/*
	CCalcPort *pPortObjLimitTag = GetInputPortObj(3);
	CUniValue  &CUVLimitTag=pPortObjLimitTag->GetPortUniValue();
	*/

	if((CUVWindSpeed.GetDataSize()!=CUVPower.GetDataSize())||(CUVWindSpeed.GetDataSize()==0))
	{
		return;
	}
	int lTimeForYear=0;
	CArray<WindPowerTag,WindPowerTag&> WindPowerArr;
	for(int j=0; j<CUVPower.GetDataSize();j++)
	{
		double value=0;
		long lTime=0;
		CUVPower.GetDoubleValAt(j,&value);
		CUVPower.GetTimeValAt(j,&lTime);
		if(j==0)
		{
			lTimeForYear=lTime;
		}
		WindPowerTag oneTag;
		oneTag.dPower=value;
		oneTag.lTime=lTime;
		
		CUVWindSpeed.GetDoubleValAt(j,&value);
		oneTag.dWindSpeed=value;
		WindPowerArr.Add(oneTag);
	}
	//�������ݣ������޵硢�������Է�Ƕ�
	delTagWindPowerMinMaxArr(WindPowerArr,CUVProductionTag,m_iPowerPorTagMin,m_iPowerPorTagMax);//ɾ��δ��������
	//delTagWindPowerArr(WindPowerArr,CUVLimitTag,m_dPowerLimit,2);//ɾ���޵�����
	delWindPowerArr(WindPowerArr);

	//���ݾ�ֵ�ͷ���ɾ������
	getAvgAndVar(WindPowerArr);//�õ���ֵ�ͷ���
	delWindPowerArrByAvgVar(WindPowerArr);//ɾ����ֵ�ͷ�����������
	
	//bin����
	double dMinWSpeed=m_dMinParWindSpeed-1;
	double dMaxBinWSpeed=m_dMaxParWindSpeed*1.5;
	double dMinBinWSpeed=(int(dMinWSpeed/0.5))*0.5;
	CArray<int,int> iDiffGroupNums;
	getBinGroup(WindPowerArr,dMinBinWSpeed,dMaxBinWSpeed,iDiffGroupNums);
	
	//bin�����ڽ��о�ֵ�ͷ���ɾ������
	getAvgAndVar(WindPowerArr,iDiffGroupNums);
	delWindPowerArrByAvgVar(WindPowerArr);//ɾ����ֵ�ͷ�����������

	//������ֵ
	getAvgAndVar(WindPowerArr,iDiffGroupNums);
	if(WindPowerArr.GetSize()==0)
		return;
	CString strSQL;
	CTime t(lTimeForYear);
	CString strTableName;
	strTableName.Format("%s_%d",m_strTableName,t.GetYear());
	
	_variant_t RecordsAffected;
	if(!gIsTableExist(m_pConRDB,strTableName))
	{
		strSQL.Format("CREATE TABLE `%s` (`turbine` varchar(64) NOT NULL,`time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,`wind_speed` int(11) NOT NULL,`power` double NOT NULL,`reactive_power` double unsigned zerofill DEFAULT NULL,`cosi` double unsigned zerofill DEFAULT NULL, `mark` enum('FALSE','TRUE') NOT NULL DEFAULT 'FALSE', PRIMARY KEY (`turbine`,`time`,`wind_speed`,`power`), KEY `turbine` (`turbine`,`time`)) ENGINE=MyISAM DEFAULT CHARSET=latin1",
			strTableName);
		m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText);
	}
	for(int i=0;i<iDiffGroupNums.GetSize();i++)
	{
		int iBinNum=iDiffGroupNums[i];
		int iFindIndex=-1;
		for(int j=0;j<WindPowerArr.GetSize();j++)
		{
			if(WindPowerArr[j].iBinNum==iBinNum)
			{
				iFindIndex=j;
				break;
			}
		}
		if(iFindIndex==-1)
			continue;
		WindPowerTag &oneTag=WindPowerArr[iFindIndex];
		try
		{
			writeDataToTable(m_strTurbine,strTableName,oneTag);
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
	//OutputResultToGlobalWS();
}
void CStatStandardWindPowerPerf::writeDataToTable(CString strTurbine,CString strTableName,WindPowerTag &oneTag)
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//��ϵ��δ����
		return;
	CString strSQL;
	_variant_t RecordsAffected;
	
	CTime time(oneTag.lTime);
	CString strTime;
	strTime=time.Format("%Y-%m-%d %H:%M:%S");
	strSQL.Format("insert into %s(turbine,time,wind_speed,power,mark) values(\'%s\',\'%s\',\'%d\',\'%f\',\'%s\')",
		strTableName,strTurbine,strTime,(int)(oneTag.dMidSpeed*10),
		oneTag.dPowerAvg,"FALSE");
	m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText);
}
void CStatStandardWindPowerPerf::getBinGroup(
									 CArray<WindPowerTag,WindPowerTag&> &WindPowerArr,
									 double dMinBinWSpeed,
									 double dMaxBinWSpeed,
									 CArray<int,int> &iDiffGroupNums)
{
	int iBinNum=1;
	double dStart=dMinBinWSpeed;
	double dEnd=dMinBinWSpeed+0.25;
	while(dEnd<dMaxBinWSpeed)
	{
		bool bValidNum=false;
		for(int i=0;i<WindPowerArr.GetSize();i++)
		{
			double dWindSpeed=WindPowerArr[i].dWindSpeed;
			if(iBinNum==1)
			{
				if((dWindSpeed>=dStart)&&(dWindSpeed<=dEnd))
				{
					WindPowerArr[i].iBinNum=iBinNum;
					WindPowerArr[i].dMidSpeed=dStart;
					bValidNum=true;
				}
			}
			else
			{
				if((dWindSpeed>dStart)&&(dWindSpeed<=dEnd))
				{
					WindPowerArr[i].iBinNum=iBinNum;
					WindPowerArr[i].dMidSpeed=dStart+0.25;
					bValidNum=true;
				}
			}
		}
		if(bValidNum)
		{
			iDiffGroupNums.Add(iBinNum);
		}
		dStart=dEnd;
		dEnd=dStart+0.5;
		iBinNum++;
	}
}
void CStatStandardWindPowerPerf::delWindPowerArrByAvgVar(
												 CArray<WindPowerTag,WindPowerTag&> &WindPowerArr)
{
	for(int i=WindPowerArr.GetSize()-1;i>=0;i--)
	{
		double dVar=WindPowerArr[i].dPowerVar;
		double dAvg=WindPowerArr[i].dPowerAvg;
		double dPower=WindPowerArr[i].dPower;
		if(((dPower-dAvg)*(dPower-dAvg))>(dVar*dVar))
		{
			WindPowerArr.RemoveAt(i);
		}
	}
}

void CStatStandardWindPowerPerf::getAvgAndVar(
									  CArray<WindPowerTag,WindPowerTag&> &WindPowerArr,
									  CArray<int,int> &iDiffGroupNums)
{
	for(int i=0;i<iDiffGroupNums.GetSize();i++)
	{
		int iBinNum=iDiffGroupNums[i];
		double dSum=0;
		double dAvg=0;
		int iCount=0;
		double dVar=0;//����
		for(int j=0;j<WindPowerArr.GetSize();j++)
		{
			if(WindPowerArr[j].iBinNum==iBinNum)
			{
				dSum+=WindPowerArr[j].dPower;
				iCount++;
			}
		}
		if(iCount>0)
		{
			dAvg=dSum/iCount;
		}
		double dSum2=0;
		for(j=0;j<WindPowerArr.GetSize();j++)
		{
			if(WindPowerArr[j].iBinNum==iBinNum)
			{
				WindPowerArr[j].dPowerAvg=dAvg;
				dSum2+=(WindPowerArr[j].dPower-dAvg)*(WindPowerArr[j].dPower-dAvg);
			}
		}
		if(iCount>1)
		{
			dVar=sqrt(double(dSum2/(iCount-1)));
		}
		for(j=0;j<WindPowerArr.GetSize();j++)
		{
			if(WindPowerArr[j].iBinNum==iBinNum)
			{
				WindPowerArr[j].dPowerVar=dVar;//������
			}
		}
	}
}
void CStatStandardWindPowerPerf::getAvgAndVar(
									  CArray<WindPowerTag,WindPowerTag&> &WindPowerArr)
{
	double dSum=0;
	double dAvg=0;
	double dVar=0;//����
	for(int i=0;i<WindPowerArr.GetSize();i++)
	{
		dSum+=WindPowerArr[i].dPower;
	}
	if(WindPowerArr.GetSize()>0)
	{
		dAvg=dSum/WindPowerArr.GetSize();
	}
	double dSum2=0;
	for(i=0;i<WindPowerArr.GetSize();i++)
	{
		WindPowerArr[i].dPowerAvg=dAvg;//����ֵ
		dSum2+=(WindPowerArr[i].dPower-dAvg)*(WindPowerArr[i].dPower-dAvg);
	}
	if(WindPowerArr.GetSize()>1)
	{
		dVar=sqrt(double(dSum2/(WindPowerArr.GetSize()-1)));
	}
	for(i=0;i<WindPowerArr.GetSize();i++)
	{
		WindPowerArr[i].dPowerVar=dVar;//������
	}
}
void CStatStandardWindPowerPerf::delWindPowerArr(
										 CArray<WindPowerTag,WindPowerTag&> &WindPowerArr)
{
	for(int i=WindPowerArr.GetSize()-1;i>=0;i--)
	{
		if(WindPowerArr[i].bDel)
		{
			WindPowerArr.RemoveAt(i);
		}
	}
}
void CStatStandardWindPowerPerf::delTagWindPowerMinMaxArr(
												  CArray<WindPowerTag,WindPowerTag&> &WindPowerArr,
												  CUniValue  &CUV,
												  int iMin,
												  int iMax											
												  )
{
	for(int i=CUV.GetDataSize()-1;i>=0;i--)
	{
		double value=0;
		CUV.GetDoubleValAt(i,&value);
		if((value<iMin)||(value>iMax))
		{
			WindPowerArr[i].bDel=true;
		}
	}
}
void CStatStandardWindPowerPerf::delTagWindPowerArr(
											CArray<WindPowerTag,WindPowerTag&> &WindPowerArr,
											CUniValue  &CUV,
											double iDelTag,//ɾ��ֵ�ı�ʶ
											int iMaxMinTag//�Ƿ���˴����趨ֵ
											)
{
	for(int i=CUV.GetDataSize()-1;i>=0;i--)
	{
		double value=0;
		CUV.GetDoubleValAt(i,&value);
		if(iMaxMinTag==0)//ɾ�����ֵ
		{
			if(value==iDelTag)
			{
				WindPowerArr[i].bDel=true;
			}
		}
		else if(iMaxMinTag==1)//ɾ������ֵ
		{
			if(value>iDelTag)
			{
				WindPowerArr[i].bDel=true;
			}
		}
		else//2ɾ��С��ֵ
		{
			if(value<iDelTag)
			{
				WindPowerArr[i].bDel=true;
			}
		}
	}
}

BOOL CStatStandardWindPowerPerf::gIsTableExist(_ConnectionPtr &pConnection, CString strTableName)
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
void CStatStandardWindPowerPerf::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("DBInf")==0)
	{
		if(!strItem1.IsEmpty())  {m_strDBName=(strItem1);}
		if(!strItem2.IsEmpty())  {m_strTableName=(strItem2);}
	}
	else if(strPropName.Compare("CalcInfo")==0)
	{
		if(!strItem1.IsEmpty())  {m_strTurbine=(strItem1);}
		if(!strItem2.IsEmpty())  {m_dPowerLimit=atof(strItem2);}
		if(!strItem3.IsEmpty())  {m_dMinParWindSpeed=atof(strItem3);}
		if(!strItem4.IsEmpty())  {m_dMaxParWindSpeed=atof(strItem4);}
	}
	else if(strPropName.Compare("PowerPro")==0)
	{
		if(!strItem1.IsEmpty())  {m_iPowerPorTagMin=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_iPowerPorTagMax=atoi(strItem2);}
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CStatStandardWindPowerPerf::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyStandardWindPowerPerf dlg;
	dlg.m_strDBName=m_strDBName;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);
	dlg.m_strTableName=m_strTableName;
	dlg.m_strTurbine=m_strTurbine;
	dlg.m_iPowerPorTagMin=m_iPowerPorTagMin;//������ʶ��Сֵ
	dlg.m_iPowerPorTagMax=m_iPowerPorTagMax;//������ʶ��Сֵ

//	dlg.m_dPowerLimit=m_dPowerLimit;//�޵��ʶ
	dlg.m_dMinParWindSpeed=m_dMinParWindSpeed;//��С�������
	dlg.m_dMaxParWindSpeed=m_dMaxParWindSpeed;//������85%��������ʱ����
	if(dlg.DoModal()==IDOK)
	{
		m_strDBName=dlg.m_strDBName;
		m_strTableName=dlg.m_strTableName;
		m_strTurbine=dlg.m_strTurbine;
		m_iPowerPorTagMin=dlg.m_iPowerPorTagMin;//������ʶ��Сֵ
		m_iPowerPorTagMax=dlg.m_iPowerPorTagMax;//������ʶ��Сֵ
//		m_dPowerLimit=dlg.m_dPowerLimit;//�޵��ʶ
		m_dMinParWindSpeed=dlg.m_dMinParWindSpeed;//��С�������
		m_dMaxParWindSpeed=dlg.m_dMaxParWindSpeed;//������85%��������ʱ����
	}
}
//�����Խ���ģ��д�뵽���ݿ���
void CStatStandardWindPowerPerf::PutPropertyToRecord(_RecordsetPtr& pRecord)
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
	strFieldValue.Format("%f",m_dPowerLimit); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%f",m_dMinParWindSpeed); //������
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%f",m_dMaxParWindSpeed); //������
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������
	
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="PowerPro";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iPowerPorTagMin); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iPowerPorTagMax); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CStatStandardWindPowerPerf::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_strDBName=((CStatStandardWindPowerPerf*)pBlock)->m_strDBName;//���ݿ�����
	this->m_strTableName=((CStatStandardWindPowerPerf*)pBlock)->m_strTableName;
	this->m_strTurbine=((CStatStandardWindPowerPerf*)pBlock)->m_strTurbine;
	this->m_iPowerPorTagMin=((CStatStandardWindPowerPerf*)pBlock)->m_iPowerPorTagMin;
	this->m_iPowerPorTagMax=((CStatStandardWindPowerPerf*)pBlock)->m_iPowerPorTagMax;
	this->m_dPowerLimit=((CStatStandardWindPowerPerf*)pBlock)->m_dPowerLimit;
	this->m_dMinParWindSpeed=((CStatStandardWindPowerPerf*)pBlock)->m_dMinParWindSpeed;
	this->m_dMaxParWindSpeed=((CStatStandardWindPowerPerf*)pBlock)->m_dMaxParWindSpeed;
}
//�����Է��ô���������
void CStatStandardWindPowerPerf::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}
