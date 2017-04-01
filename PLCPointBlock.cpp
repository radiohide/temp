// PLCPointBlock.cpp: implementation of the CPLCPointBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "PLCPointBlock.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPLCPointBlock::CPLCPointBlock()
{
	m_iPort=502;
	m_iFunNo=0;
	m_iAdd=0;
	m_strIP="192.168.1.1";
	
	 m_strCurrentPointName="";
	 m_strCurentType="BOOL";
		//SYSTME=====��������======
	//��������
	SetBlockType(_T( "PLCPoint" ) );
	ConstructBlockInputOutput();
	//SYSTME=====��ʼ����ģ�������===

}

CPLCPointBlock::~CPLCPointBlock()
{

}
//�����ʼ�������������
void CPLCPointBlock::ConstructBlockInputOutput()
{	
	AddOutputPort("Out",VALUE_DOUBLE,"out1");
}
//���������½�����ĺ���
//ͬʱ��CCalcBlockControlFactory�У�Ҫ������Ӧ�Ĵ���
CCalcBlock* CPLCPointBlock::CreateFromString( const CString& strType )
{
	CPLCPointBlock* obj = new CPLCPointBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CPLCPointBlock::InitCalc()
{
	CCalcBlock::InitCalc();		
    hInst=LoadLibrary("ModbusInterface.dll");
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CPLCPointBlock::Calc() 
{
	typedef int (*pInitConnect)(CString strIP,int iPort);
	pInitConnect InitConnect=(pInitConnect)::GetProcAddress(hInst,"InitConnect");	//���������ݲ�ѯ
    
	typedef int (*pGetDataByTagName_bool)(const PLCGetData  &GetElement,bool &retValue);
	pGetDataByTagName_bool GetDataByTagName_bool=(pGetDataByTagName_bool)::GetProcAddress(hInst,"GetDataByTagName_bool"); 
    typedef int (*pGetDataByTagName_long)(const PLCGetData  &GetElement,long &retValue);
	pGetDataByTagName_long GetDataByTagName_long=(pGetDataByTagName_long)::GetProcAddress(hInst,"GetDataByTagName_long"); 
	typedef int (*pGetDataByTagName_float)(const PLCGetData  &GetElement,float &retValue);
	pGetDataByTagName_float GetDataByTagName_float=(pGetDataByTagName_float)::GetProcAddress(hInst,"GetDataByTagName_float"); 
	
	typedef int (*pReleaseConnect)();
	pReleaseConnect ReleaseConnect=(pReleaseConnect)::GetProcAddress(hInst,"ReleaseConnect");


	int nret;
	float m_Result;
	nret=InitConnect(m_strIP,m_iPort);
	if(m_strCurentType=="BOOL")
	{
		//����Ȧ����BOOL
		PLCGetData  databR;
		strcpy(databR.type,(LPSTR)(LPCTSTR)"BOOL");
		databR.add=m_iAdd;
		databR.funcode=1;
		bool b;
		nret=GetDataByTagName_bool(databR,b);
		if(nret!=0)
		{
			CString str;
			str.Format("PLC��ȡBOOLʧ��!");
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
		else
		{
			CCalcPort *pPortObjOut = GetOutputPortObj(0);
			CUniValue UniVar;
			UniVar.AddData(b);
			pPortObjOut->SetPortUniValue(UniVar);
		}
		ReleaseConnect();
	}
	else if(m_strCurentType=="REAL")
	{
		//�����ּĴ��� float
		PLCGetData  dataf;
		strcpy(dataf.type,(LPSTR)(LPCTSTR)"REAL");
		dataf.add=m_iAdd;
		dataf.funcode=3;
		float f;
		nret=GetDataByTagName_float(dataf,f);
		if(nret!=0)
		{
			CString str;
			str.Format("PLC��ȡREALʧ��!");
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
		else
		{
			CCalcPort *pPortObjOut = GetOutputPortObj(0);
			CUniValue UniVar;
			UniVar.AddData(f);
			pPortObjOut->SetPortUniValue(UniVar);
		}	
		ReleaseConnect();
		
	}
	else if(m_strCurentType=="DINT")
	{
		//�����ּĴ��� ����
		PLCGetData  dataf;
		strcpy(dataf.type,(LPSTR)(LPCTSTR)"DINT");
		dataf.add=m_iAdd;
		dataf.funcode=3;
		long l;
		nret=GetDataByTagName_long(dataf,l);
		if(nret!=0)
		{
			CString str;
			str.Format("PLC��ȡDINTʧ��!");
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
	    else
		{
			CCalcPort *pPortObjOut = GetOutputPortObj(0);
			CUniValue UniVar;
			UniVar.AddData(l);
			pPortObjOut->SetPortUniValue(UniVar);
		}
		
		ReleaseConnect();
	}
	//�������
	OutputResultToGlobalWS();

}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CPLCPointBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("PLCPoint1")==0)
	{
		if(!strItem1.IsEmpty())
			m_strIP=strItem1;
		if(!strItem2.IsEmpty())
			m_iPort=atoi(strItem2);
		if(!strItem3.IsEmpty())
			m_strCurentType=strItem3;
		if(!strItem4.IsEmpty())
			m_strCurrentPointName=strItem4;
	}
	if(strPropName.Compare("PLCPoint2")==0)
	{
		if(!strItem1.IsEmpty())
			m_iAdd=atoi(strItem1);
		if(!strItem2.IsEmpty())
			m_iFunNo=atoi(strItem2);
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CPLCPointBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{

	CPropertyDlgPLCPoint  dlg;

	dlg.m_strIP=m_strIP;
	dlg.m_iPort=m_iPort;
	dlg.m_strCurentType=m_strCurentType;
	dlg.m_strCurrentPointName=m_strCurrentPointName;
	dlg.m_iAdd=m_iAdd;
	dlg.m_iFunNo=m_iFunNo;
	
	if(dlg.DoModal()==IDOK)
	{
		m_strIP=dlg.m_strIP;
		m_iPort=dlg.m_iPort;
		m_strCurentType=dlg.m_strCurentType;
		m_strCurrentPointName=dlg.m_strCurrentPointName;
		m_iAdd=dlg.m_iAdd;
		m_iFunNo=dlg.m_iFunNo;

		ClearOutputPorts();
		AddOutputPort("Out",VALUE_DOUBLE, _T("out"), _T("out"));

		if((m_pModel!=NULL)&&(this->m_pModel->m_iTagModeLocalOrRemote==1))//Զ��
		{
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> dataArr;
			SetBlockPropertyToTransferData(dataArr);//��ȡҪ��������Ե�����������
			SendBlockPropertyTransferData(dataArr);// ��������������
			ClearPropertyTransferData(dataArr);//�ͷ�����
		}
	}	
	
}
//�����Խ���ģ��д�뵽���ݿ���
void CPLCPointBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	//����1-5�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("PLCPoint1"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
	//strFieldValue.Format("%d",m_strInputNum);
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(m_strIP) );
	strFieldValue.Format("%d",m_iPort);
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(strFieldValue) );
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(m_strCurentType) );
	pRecord->PutCollect(_variant_t("value_item4"),_variant_t(m_strCurrentPointName) );

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("PLCPoint2"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iAdd);
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(strFieldValue) );
	strFieldValue.Format("%d",m_iFunNo);
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(strFieldValue) );

	pRecord->Update();

}
//����ͼ��ʱ����Ҫ�������ԡ�
void CPLCPointBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strIP=((CPLCPointBlock*)pBlock)->m_strIP;
	m_iPort=((CPLCPointBlock*)pBlock)->m_iPort;
	m_strCurentType=((CPLCPointBlock*)pBlock)->m_strCurentType;
	m_strCurrentPointName=((CPLCPointBlock*)pBlock)->m_strCurrentPointName;
	m_iAdd=((CPLCPointBlock*)pBlock)->m_iAdd;
	m_iFunNo=((CPLCPointBlock*)pBlock)->m_iFunNo;
				
}
//�����Է��ô���������
void CPLCPointBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CString str;
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"PLCPoint1");

	str.Format("%s",m_strIP);strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iPort);strcpy(pData->m_cValueItem2,str);
	str.Format("%s",m_strCurentType);strcpy(pData->m_cValueItem3,str);
	str.Format("%s",m_strCurrentPointName);strcpy(pData->m_cValueItem4,str);
	dataArr.Add(pData);

    pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"PLCPoint2");
	str.Format("%d",m_iAdd);strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iFunNo);strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);
}
