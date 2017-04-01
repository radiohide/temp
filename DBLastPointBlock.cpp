// DBLastPointBlock.cpp: implementation of the CDBLastPointBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "DBLastPointBlock.h"
#include "PropertyDlgDBLastPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDBLastPointBlock::CDBLastPointBlock()
{
	m_bConDB=false;
	m_TagTimeOffsetOrInput=1;//1:����ϵͳʱ��+ƫ�ơ�
	m_lOffset=0;
	m_strTagName="";
	m_strTagDesp="";
	m_iForwardOrBack=0;
	m_strTableName="point_data";
	SetBlockType(_T( "DBLastPoint" ) );

	//Ĭ���ֶ�����
	m_strFieldVarName="var_name";
	m_strFieldDateTime="DateTime";
	m_strFieldState="State";
	m_strFieldValue="Value";
	ConstructBlockInputOutput();
}

CDBLastPointBlock::~CDBLastPointBlock()
{
	
}
//���������½�����ĺ���
CCalcBlock* CDBLastPointBlock::CreateFromString( const CString& strType )
{
	CDBLastPointBlock* obj = new CDBLastPointBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CDBLastPointBlock::ConstructBlockInputOutput ()
{
	AddOutputPort("Out",VALUE_DOUBLE);
}
//��ʼ������ĺ���������ĳ�ʼ��
void CDBLastPointBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//�������ԵĶ����ַ���
CString CDBLastPointBlock::GetBlockStrPro()
{
	CString str;
	str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CDBLastPointBlock::SetBlockStrPro(CString str)//���ü������ַ�������
{
	m_strTagName=str.Left(str.Find('@'));
	m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CDBLastPointBlock::Calc()//���м���ĺ���,��Ҫ����
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = m_pModel->GetSourceRDBByDesp(m_strDBDesp,tagBeCon,strDBType);
	if(tagBeCon==0)//��ϵ��δ����
		return;

	double dRetValue=0;
	long lRetTime=0;
	int iRetState=192;
	GetInputValueFromGlobalWS();
    //�������
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	double result=0;
	
	long timeflag=0;
	if(m_TagTimeOffsetOrInput==1)
	{
		timeflag=this->m_pModel->m_lDataTime-m_lOffset;
	}
	else
	{
		double inValue=0;
		CCalcPort *pPortObj = GetInputPortObj(0);
		pPortObj->GetPortUniValue().GetDoubleVal(&inValue);
		timeflag=(long)inValue;
	}
	if(timeflag<0)//ʱ�䲻��Ϊ����
		 timeflag=0;

	CTime t(timeflag);
	CString csTableName="";
	csTableName.Format("%s_%d",m_strTableName,t.GetYear());

	if(m_strTagName!="")
	{
		getValueFromDB(m_pConRDB,strDBType,csTableName,m_strTagName,timeflag,m_iForwardOrBack,
			dRetValue,lRetTime,iRetState);
	}
	CUniValue UniVar;
	UniVar.AddData(dRetValue,timeflag,iRetState);
	pOutPortObj->SetPortUniValue(UniVar);
	OutputResultToGlobalWS();
}
void CDBLastPointBlock::DrawShape(CDC* dc, CRect rect,int CutSize)
{
	//dc->Rectangle(rect);
	CPoint roundness;
	////////////////////////////////////////////////
	CRect rc(rect);
	rc.top = rect.top + 2*CutSize;
	rc.bottom = rect.bottom - 2*CutSize; 
    roundness.x = int(double(rc.Width())*0.6);
	roundness.y = int(double(rc.Height())*0.6);
	dc->RoundRect(rc,roundness);
 
	//������
	int MidX = rc.left+int(double(rc.Width())/2);
	//dc->MoveTo(MidX, rc.top);
	//dc->LineTo(MidX,rc.bottom);
	//���м��԰��ʾʵʱ
	CRect rcCenter;
	int MidY = rc.top+int(double(rc.Height())/2);
	int Dis =  int(double(rc.Height())*0.25);
	rcCenter.SetRect(MidX-Dis,MidY-Dis,MidX+Dis,MidY+Dis);
    dc->Rectangle(rcCenter);
	///////////////////////////////////////////////
}
BOOL CDBLastPointBlock::getValueFromDB(_ConnectionPtr &m_pConRDB,
								   CString strDBType,
								   CString strTableName,
								   CString strTagName,
								   long timeflag,
								   int iForwardOrBack,
								   double &dRetValue,
								   long &lRetTime,
								   int &iRetState)
{
	CTime  t1(timeflag);
	CString timestr1;
	timestr1.Format("%d-%.2d-%.2d %.2d:%.2d:%.2d",t1.GetYear(),t1.GetMonth(),t1.GetDay(),t1.GetHour(),t1.GetMinute(),t1.GetSecond());
	
	_RecordsetPtr m_pRec;

	//�����ڣ���Ϊ�ñ�δ�������ۼ�ֵ���0
	if(!gIsTableExist(m_pConRDB,strTableName))
	{
		dRetValue = 0.0;
		iRetState = 192;//�����ۼ�ֵд��vestoreҪ��192
		return TRUE;
	}
	
	try   
	{
		strDBType.MakeUpper();
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		if(iForwardOrBack==0)
		{
			if(strDBType=="MYSQL")
			{
				strSql.Format("select  * from %s  where %s='%s' and %s<='%s' order by DateTime desc limit 0,1;",
					strTableName,m_strFieldVarName,strTagName,m_strFieldDateTime,timestr1);
			}
			else if(strDBType=="MDB")
			{
				strSql.Format("select top 1 * from %s  where %s='%s' and %s<=#%s# order by DateTime desc;",
					strTableName,m_strFieldVarName,strTagName,m_strFieldDateTime,timestr1);
			}
		}
		else if(iForwardOrBack==1)
		{
			if(strDBType=="MYSQL")
			{
				strSql.Format("select  * from %s  where %s='%s' and %s>='%s' order by DateTime asc limit 0,1;",
					strTableName,m_strFieldVarName,strTagName,m_strFieldDateTime,timestr1);
			}
			else if(strDBType=="MDB")
			{
				strSql.Format("select top 1 * from %s  where %s='%s' and %s>=#%s# order by DateTime asc;",
					strTableName,m_strFieldVarName,strTagName,m_strFieldDateTime,timestr1);
			}
		}
		
		m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pConRDB,true),
			adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error  *e)   
	{   
		CString str;
		str.Format("CDBPointBlock->getValueFromDB(...)->%s",e->ErrorMessage());
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
	BOOL  bRet=FALSE;

	if (m_pRec->adoEOF)
	{
		dRetValue = 0.0;
		iRetState = 192;
		bRet = TRUE;
		return bRet;
	}

	while(!m_pRec->adoEOF)
	{
		// CString strConFieldDateTime=this->m_strFieldDateTime;
		CString U_Input_Date =(char *)_bstr_t(m_pRec->GetCollect((_variant_t)m_strFieldDateTime));
		CString U_Input_Value =(char *)_bstr_t(m_pRec->GetCollect((_variant_t)m_strFieldValue));
		CString U_Input_State =(char *)_bstr_t(m_pRec->GetCollect((_variant_t)m_strFieldState));
		
		dRetValue=atof(U_Input_Value);
		iRetState=atoi(U_Input_State);

		//
		if (iRetState == 0)
		{
			iRetState = 192;//����ǰ���������
		}

		
		int   nYear,   nMonth,   nDate;
		int nHour=0;
		int nMin=0;
		int nSec=0;   

		//ע�͵���ʱ���Ӧ���ϵ�ǰģ�ͼ���ʱ�������Ӧ�����ݿ��е�ʱ���
		/*if(U_Input_Date.Find('/')>=0)
		{
			sscanf(U_Input_Date, "%d/%d/%d   %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);
		}
		else
		{
			sscanf(U_Input_Date, "%d-%d-%d   %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);
		}
		//sscanf(U_Input_Date, "%d-%d-%d   %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);   
		CTime   t(nYear,nMonth,nDate,nHour,nMin,nSec);	
		lRetTime=t.GetTime();*/

		bRet=TRUE;
		break; 
	}
	return bRet;	
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CDBLastPointBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("TimeOffsetOrInput")==0)
	{
		if(!strItem1.IsEmpty())  {m_TagTimeOffsetOrInput=atoi(strItem1);}
	}
	else if(strPropName.Compare("lOffset")==0)
	{
		if(!strItem1.IsEmpty())  {m_lOffset=atol(strItem1);}
	}
	else if(strPropName.Compare("TagName")==0)
	{
		if(!strItem1.IsEmpty())  {m_strTagName=strItem1;}
	}
	else if(strPropName.Compare("TagDesp")==0)
	{
		if(!strItem1.IsEmpty())  {m_strTagDesp=strItem1;}
	}
	else if(strPropName.Compare("ForwardOrBack")==0)
	{
		if(!strItem1.IsEmpty())  {m_iForwardOrBack=atoi(strItem1);}
	}
	else if(strPropName.Compare("TableName")==0)
	{
		if(!strItem1.IsEmpty())  {m_strTableName=strItem1;}
	}
	else if(strPropName.Compare("DBDesp")==0)
	{
		if(!strItem1.IsEmpty())  {m_strDBDesp=strItem1;}
	}
	else if(strPropName.Compare("StrField")==0)
	{
		if(!strItem1.IsEmpty())  {m_strFieldVarName=strItem1;}
		if(!strItem2.IsEmpty())  {m_strFieldDateTime=strItem2;}
		if(!strItem3.IsEmpty())  {m_strFieldState=strItem3;}
		if(!strItem4.IsEmpty())  {m_strFieldValue=strItem4;}
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CDBLastPointBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBefore=this->GetInputNum();//����
	CStringArray  IDArrLeftOut;//�����ԭ�е�����˿�����
	IDArrLeftOut.RemoveAll();
	
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		IDArrLeftOut.Add(strPortID);
	}

	CPropertyDlgDBLastPoint dlg;
    //���Ի��������ֵ
	dlg.m_lOffset=m_lOffset;
	dlg.m_strTagName=m_strTagName;
	dlg.m_strTagDesp=m_strTagDesp;
	dlg.m_TagTimeOffsetOrInput=m_TagTimeOffsetOrInput;
	dlg.m_iForwardOrBack=m_iForwardOrBack;
	dlg.m_strTableName=m_strTableName;
	dlg.m_strDBDesp=m_strDBDesp;
	dlg.m_strFieldVarName=m_strFieldVarName;
	dlg.m_strFieldDateTime=m_strFieldDateTime;
	dlg.m_strFieldState=m_strFieldState;
	dlg.m_strFieldValue=m_strFieldValue;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);
	if(dlg.DoModal()==IDOK)
	{
		m_lOffset=dlg.m_lOffset;
		m_strTagName=dlg.m_strTagName;
		m_strTagDesp=dlg.m_strTagDesp;
		m_TagTimeOffsetOrInput=dlg.m_TagTimeOffsetOrInput;
		m_iForwardOrBack=dlg.m_iForwardOrBack;
		m_strTableName=dlg.m_strTableName;
		m_strDBDesp=dlg.m_strDBDesp;
		m_strFieldVarName=dlg.m_strFieldVarName;
		m_strFieldDateTime=dlg.m_strFieldDateTime;
		m_strFieldState=dlg.m_strFieldState;
		m_strFieldValue=dlg.m_strFieldValue;
		
		if(m_TagTimeOffsetOrInput==0)//�ⲿ
		{
			if(IDArrLeftOut.GetSize()==1){}
			else
			{
				AddInputPort(_T("Offset"), VALUE_DOUBLE, _T("in1"), _T("in1"));
			}
		}
		else
			ClearInputPorts();
		parent->RedrawWindow();
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
void CDBLastPointBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="TimeOffsetOrInput";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_TagTimeOffsetOrInput); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="lOffset";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_lOffset); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="TagName";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strTagName); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="TagDesp";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strTagDesp); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="ForwardOrBack";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iForwardOrBack); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="TableName";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strTableName); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="DBDesp";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strDBDesp); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

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

	pRecord->Update();
}

//����ͼ��ʱ����Ҫ�������ԡ�
void CDBLastPointBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_TagTimeOffsetOrInput=((CDBLastPointBlock*)pBlock)->m_TagTimeOffsetOrInput;
	this->m_lOffset=((CDBLastPointBlock*)pBlock)->m_lOffset;
	this->m_strTagName=((CDBLastPointBlock*)pBlock)->m_strTagName;
	this->m_strTagDesp=((CDBLastPointBlock*)pBlock)->m_strTagDesp;
	this->m_iForwardOrBack=((CDBLastPointBlock*)pBlock)->m_iForwardOrBack;
	this->m_strTableName=((CDBLastPointBlock*)pBlock)->m_strTableName;
	this->m_strDBDesp=((CDBLastPointBlock*)pBlock)->m_strDBDesp;\

	this->m_strFieldVarName=((CDBLastPointBlock*)pBlock)->m_strFieldVarName;
	this->m_strFieldDateTime=((CDBLastPointBlock*)pBlock)->m_strFieldDateTime;
	this->m_strFieldState=((CDBLastPointBlock*)pBlock)->m_strFieldState;
	this->m_strFieldValue=((CDBLastPointBlock*)pBlock)->m_strFieldValue;
}
//�����Է��ô���������
void CDBLastPointBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TimeOffsetOrInput");	
	CString str;
	str.Format("%d",m_TagTimeOffsetOrInput);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"lOffset");	
	str.Format("%d",m_lOffset);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TagName");	
	str.Format("%s",m_strTagName);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TagDesp");	
	str.Format("%s",m_strTagDesp);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"ForwardOrBack");	
	str.Format("%d",m_iForwardOrBack);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TableName");	
	str.Format("%s",m_strTableName);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"DBDesp");	
	str.Format("%s",m_strDBDesp);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"StrField");
	str.Format("%s",m_strFieldVarName);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%s",m_strFieldDateTime);
	strcpy(pData->m_cValueItem2,str);
	str.Format("%s",m_strFieldState);
	strcpy(pData->m_cValueItem3,str);
	str.Format("%s",m_strFieldValue);
	strcpy(pData->m_cValueItem4,str);
	dataArr.Add(pData);
}