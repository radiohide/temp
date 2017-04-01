// RTPointBlock.cpp: implementation of the CRTPointBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RTPointBlock.h"
#include "PropertyDlgRTPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRTPointBlock::CRTPointBlock()
{
	m_TagTimeOffsetOrInput=1;//1:����ϵͳʱ��+ƫ�ơ�
	m_lOffset=0;
	m_strTagName="";
	m_strTagDesp="";

	m_iQueryType=0;
	m_lTimeLength=300;
	m_TagQueryTimeSpan=0;
	SetBlockType(_T( "RTPoint" ) );
	ConstructBlockInputOutput();
}

CRTPointBlock::~CRTPointBlock()
{
	
}
//���������½�����ĺ���
CCalcBlock* CRTPointBlock::CreateFromString( const CString& strType )
{
	CRTPointBlock* obj = new CRTPointBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CRTPointBlock::ConstructBlockInputOutput ()
{
	AddOutputPort("Out",VALUE_DOUBLE);
}
//��ʼ������ĺ���������ĳ�ʼ��
void CRTPointBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	this->m_pModel->LoadRTDBDllAndCon();
}
//�������ԵĶ����ַ���
CString CRTPointBlock::GetBlockStrPro()
{
	CString str;
	str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CRTPointBlock::SetBlockStrPro(CString str)//���ü������ַ�������
{
	m_strTagName=str.Left(str.Find('@'));
	m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CRTPointBlock::Calc()//���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	double result=0;
	if(this->m_pModel->m_bConRTDB)
	{
		typedef  int (*pGetHistoryDataByTime)(const char * tagName,long time, TagData *pTagData);
		pGetHistoryDataByTime m_GetHistoryDataByTime =(pGetHistoryDataByTime)GetProcAddress(this->m_pModel->hDLL,"GetHistoryDataByTime");
		
		typedef int (*pGetAggregationDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData * pTagData);
		pGetAggregationDataByTagName GetAggregationDataByTagName=(pGetAggregationDataByTagName)GetProcAddress(this->m_pModel->hDLL,"GetAggregationDataByTagName");
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
		if(timeflag<0)
			timeflag=0;
		if(m_strTagName!="")
		{
			int nRet=-1;
			if(m_TagQueryTimeSpan==0)
			{
				TagData *tagData=new TagData;
				memset(tagData,0,sizeof(TagData));
				nRet = m_GetHistoryDataByTime(m_strTagName,timeflag, tagData);
				if(nRet!=0)
				{
					for(int i=0;i<50;i++)//����ȡʧ�ܣ�����50�ζ�ȡ��
					{
						memset(tagData,0,sizeof(TagData));
						::Sleep(5);//��ʱ��ȷʵ���ȡʧ�ܣ��ڵڶ��ζ�ȡ��ʱ���ɹ�
						nRet = m_GetHistoryDataByTime(m_strTagName,timeflag, tagData);
						if(nRet==0)
							break;
					}
					if(nRet!=0)
					{
						tagData->status=0;
						tagData->time=0;
						tagData->value=0;
					}
				}
				result=tagData->value;
				if(tagData!=NULL)
				{
					delete tagData;
					tagData=NULL;
				}
			}
			else
			{
				TagData *pTagData=new TagData[1];
				memset(pTagData,0,sizeof(TagData)*1);
				ReadHiDataRequest req;
				memset(&req,0,sizeof(ReadHiDataRequest));
				strcpy(req.pointName,m_strTagName);
				req.stTime= timeflag-m_lTimeLength;
				req.enTime= timeflag;
				if(req.enTime<0)
				{
					req.enTime= 0;
				}
				if(m_iQueryType==0)//��ֵ
				{
					req.reqType=3;
				}
				else if(m_iQueryType==1)
				{
					req.reqType=1;
				}
				else if(m_iQueryType==2)
				{
					req.reqType=2;
				}
				req.tPeriod=m_lTimeLength;
				nRet = GetAggregationDataByTagName(&req,pTagData);
				if(nRet==0)
				{
					result=pTagData[0].value;
				}
				else
				{
					for(int i=0;i<50;i++)
					{
						memset(pTagData,0,sizeof(TagData)*1);
						::Sleep(5);
						nRet = GetAggregationDataByTagName(&req,pTagData);
						if(nRet==0)
						{
							result=pTagData[0].value;
							break;
						}
					}
				}
				if(pTagData!=NULL)
				{
					delete[] pTagData;//debug й¶
					pTagData=NULL;//debug й¶
				}
			}
		}
	}
	CUniValue UniVar;
	UniVar.AddData(result);
	pOutPortObj->SetPortUniValue(UniVar);
	OutputResultToGlobalWS();
}
void CRTPointBlock::DrawShape(CDC* dc, CRect rect,int CutSize)
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
    dc->Ellipse(rcCenter);
	///////////////////////////////////////////////
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CRTPointBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
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
	else if(strPropName.Compare("QueryPar")==0)
	{
		if(!strItem1.IsEmpty())  {m_TagQueryTimeSpan=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_lTimeLength=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {m_iQueryType=atoi(strItem3);}
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CRTPointBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
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

	CPropertyDlgRTPoint dlg;
    //���Ի��������ֵ
	dlg.m_lOffset=m_lOffset;
	dlg.m_strTagName=m_strTagName;
	dlg.m_strTagDesp=m_strTagDesp;
	dlg.m_TagTimeOffsetOrInput=m_TagTimeOffsetOrInput;

	dlg.m_lTimeLength=m_lTimeLength;
	dlg.m_TagQueryTimeSpan=m_TagQueryTimeSpan;
	dlg.m_iQueryType=m_iQueryType;
	if(dlg.DoModal()==IDOK)
	{
		m_lOffset=dlg.m_lOffset;
		m_strTagName=dlg.m_strTagName;
		m_strTagDesp=dlg.m_strTagDesp;
		m_TagTimeOffsetOrInput=dlg.m_TagTimeOffsetOrInput;
		m_lTimeLength=dlg.m_lTimeLength;
		m_TagQueryTimeSpan=dlg.m_TagQueryTimeSpan;
		m_iQueryType=dlg.m_iQueryType;

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
void CRTPointBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
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
	strFieldValue="QueryPar";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_TagQueryTimeSpan); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_lTimeLength); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iQueryType); //������
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
	pRecord->Update();
}

//����ͼ��ʱ����Ҫ�������ԡ�
void CRTPointBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_TagTimeOffsetOrInput=((CRTPointBlock*)pBlock)->m_TagTimeOffsetOrInput;
	this->m_lOffset=((CRTPointBlock*)pBlock)->m_lOffset;
	this->m_strTagName=((CRTPointBlock*)pBlock)->m_strTagName;
	this->m_strTagDesp=((CRTPointBlock*)pBlock)->m_strTagDesp;
	this->m_TagQueryTimeSpan=((CRTPointBlock*)pBlock)->m_TagQueryTimeSpan;
	this->m_lTimeLength=((CRTPointBlock*)pBlock)->m_lTimeLength;
	this->m_iQueryType=((CRTPointBlock*)pBlock)->m_iQueryType;
}
//�����Է��ô���������
void CRTPointBlock::SetBlockPropertyToTransferData(
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
	strcpy(pData->m_cPropName,"QueryPar");	
	str.Format("%d",m_TagQueryTimeSpan);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_lTimeLength);
	strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_iQueryType);
	strcpy(pData->m_cValueItem3,str);
	dataArr.Add(pData);
}
