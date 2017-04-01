// RDBHistSeriesBat.cpp: implementation of the CRDBHistSeriesBat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RDBHistSeriesBat.h"
#include "PropertyDlgRDBHistSeriesBat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRDBHistSeriesBat::CRDBHistSeriesBat()
{
	m_strFieldVarName="VarName";
	m_strFieldDateTime="DateTime";
	m_strFieldRealDateTime="RealDateTime";
	m_strFieldState="State";
	m_strFieldValue="Value";

	SetBlockType(_T( "RDBHistSeriesBat" ));
	ConstructBlockInputOutput();
}

CRDBHistSeriesBat::~CRDBHistSeriesBat()
{

}
//���������½�����ĺ���
CCalcBlock* CRDBHistSeriesBat::CreateFromString( const CString& strType )
{
	CRDBHistSeriesBat* obj = new CRDBHistSeriesBat;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CRDBHistSeriesBat::ConstructBlockInputOutput ()
{
	AddInputPort("StartTime",VALUE_DOUBLE);
	AddInputPort("EndTime",VALUE_DOUBLE);
}

//��ʼ������ĺ���������ĳ�ʼ��
void CRDBHistSeriesBat::InitCalc()
{
	CCalcBlock::InitCalc();
}

//�������ԵĶ����ַ���
CString CRDBHistSeriesBat::GetBlockStrPro()//to do
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CRDBHistSeriesBat::SetBlockStrPro(CString str)//���ü������ַ�������//to do
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
void CRDBHistSeriesBat::Calc()//���м���ĺ���,��Ҫ����
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//��ϵ��δ����
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
		CUniValue UniVar;//�������
		UniVar.SetValueType(1);//����ΪVALUE_RTVALUE_SERIES����

		CString strTableName="";
		strTableName.Format("%s_%d",strTableArr[i],st.GetYear());
		CString strSQL;
		_RecordsetPtr m_pRec;
		_variant_t RecordsAffected;
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		try
		{
			strSql.Format("select  * from %s  where %s='%s' and %s>='%s' and %s<='%s';",
				strTableName,m_strFieldVarName,strTagArr[i],m_strFieldDateTime,strSTime,m_strFieldDateTime,strETime);
			m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pConRDB,true),
				adOpenStatic,adLockOptimistic,adCmdText);

			double dRetValue=0;
			short iRetState=0;
			long lRetTime=0;
			long lRetRealTime=0;
			while(!m_pRec->adoEOF)
			{
				//CString strConFieldDateTime=this->m_strFieldDateTime;
				CString U_Input_Date =(char *)_bstr_t(m_pRec->GetCollect((_variant_t)m_strFieldDateTime));
				CString U_Input_RealDate =(char *)_bstr_t(m_pRec->GetCollect((_variant_t)m_strFieldRealDateTime));
				CString U_Input_Value =(char *)_bstr_t(m_pRec->GetCollect((_variant_t)m_strFieldValue));
				CString U_Input_State =(char *)_bstr_t(m_pRec->GetCollect((_variant_t)m_strFieldState));
				
				dRetValue=atof(U_Input_Value);
				iRetState=atoi(U_Input_State);
				
				int   nYear=0;int nMonth=0;int nDate=0;int nHour=0;int nMin=0;int nSec=0; 
				int   nrYear=0;int nrMonth=0;int nrDate=0;int nrHour=0;int nrMin=0;int nrSec=0;
				if(U_Input_Date.Find('/')>=0)
					sscanf(U_Input_Date, "%d/%d/%d   %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);
				else
					sscanf(U_Input_Date, "%d-%d-%d   %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);
				CTime   t(nYear,nMonth,nDate,nHour,nMin,nSec);	
				lRetTime=t.GetTime();

				if(U_Input_RealDate.Find('/')>=0)
					sscanf(U_Input_RealDate, "%d/%d/%d   %d:%d:%d",   &nrYear,   &nrMonth,   &nrDate,   &nrHour,   &nrMin,   &nrSec);
				else
					sscanf(U_Input_RealDate, "%d-%d-%d   %d:%d:%d",   &nrYear,   &nrMonth,   &nrDate,   &nrHour,   &nrMin,   &nrSec);
				CTime   rt(nrYear,nrMonth,nrDate,nrHour,nrMin,nrSec);	
				lRetRealTime=rt.GetTime();

				UniVar.AddData(dRetValue,lRetTime,iRetState,lRetRealTime);
				m_pRec->MoveNext();
			}
		}
		catch(_com_error  *e)
		{   
			CString str;
			str.Format("CRDBHistSeriesBat��ȡ��'%s'ʧ��,������Ϣ:%s",strTagArr[i],e->ErrorMessage());
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
		pPortObjOut->SetPortUniValue(UniVar);
	}
	OutputResultToGlobalWS();
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CRDBHistSeriesBat::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("StrField")==0)
	{
		if(!strItem1.IsEmpty())  {m_strFieldVarName=strItem1;}
		if(!strItem2.IsEmpty())  {m_strFieldDateTime=strItem2;}
		if(!strItem3.IsEmpty())  {m_strFieldState=strItem3;}
		if(!strItem4.IsEmpty())  {m_strFieldValue=strItem4;}
		if(!strItem5.IsEmpty())  {m_strDBName=strItem5;}
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
void CRDBHistSeriesBat::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
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
void CRDBHistSeriesBat::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgRDBHistSeriesBat dlg;
	
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//�����ԭ�е�����˿�����
	CStringArray  IDArrRightIn;//������ڲ�������˿�����
	
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
		
		//������˿ڵ��޸�
		for(int i=0;i<strTagArr.GetSize();i++)
		{
			IDArrRightIn.Add(strTagArr[i]);
		}		
		for(int jj=IDArrRightOut.GetSize()-1; jj>-1; jj--)//ɾ��ԭ�ж����õ�Port,����ɾ
		{
			int  runs=0;//��������
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
		for(int mm=0; mm<IDArrRightIn.GetSize(); mm++)//����µ�Port
		{
			int  runs=0;//��������
			for(int nn=0; nn<IDArrRightOut.GetSize(); nn++)
			{
				if((IDArrRightIn[mm]!=IDArrRightOut[nn]))
				{runs++;}		
			}
			if(runs==IDArrRightOut.GetSize())
			{
				CString str=IDArrRightIn[mm];
				InSertOutputPortBefore(mm,str, VALUE_RTVALUE_SERIES, _T(str));
			}
		}
		parent->RedrawWindow();
	}
}
//�����Խ���ģ��д�뵽���ݿ���
void CRDBHistSeriesBat::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	for(int i=0;i<strTableArr.GetSize();i++)//���
	{
		CString strInPut=strTableArr[i];
		if(strInPut=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("input.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������

		strFieldValue.Format("%s",strTableArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",strTagArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",strTagDespArr[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",strUnitArr[i]); //������
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
	strFieldValue.Format("%s",m_strDBName); //������
	pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //������	
	
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CRDBHistSeriesBat::CopyPropertyData(CCalcBlock* pBlock)
{
	this->strTableArr.Copy(((CRDBHistSeriesBat*)pBlock)->strTableArr);
	this->strTagArr.Copy(((CRDBHistSeriesBat*)pBlock)->strTagArr);
	this->strTagDespArr.Copy(((CRDBHistSeriesBat*)pBlock)->strTagDespArr);
	this->strUnitArr.Copy(((CRDBHistSeriesBat*)pBlock)->strUnitArr);

	this->m_strFieldVarName=((CRDBHistSeriesBat*)pBlock)->m_strFieldVarName;
	this->m_strFieldDateTime=((CRDBHistSeriesBat*)pBlock)->m_strFieldDateTime;
	this->m_strFieldState=((CRDBHistSeriesBat*)pBlock)->m_strFieldState;
	this->m_strFieldValue=((CRDBHistSeriesBat*)pBlock)->m_strFieldValue;
	this->m_strDBName=((CRDBHistSeriesBat*)pBlock)->m_strDBName;//���ݿ�����
}
//�����Է��ô���������
void CRDBHistSeriesBat::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}

