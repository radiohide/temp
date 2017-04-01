// WaveIntervalBlock.cpp: implementation of the CWaveIntervalBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WaveIntervalBlock.h"
#include "PropertyDlgWaveInterval.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWaveIntervalBlock::CWaveIntervalBlock()
{
	m_iSmoothWidth=15;//�������(��)
	m_iSmoothStep=1;//��������(��)
	m_bBeFirstCalc=true;
	m_dSpanIntPercent=50;//�����ٽ����䳬���߽�İٷֱ�
	m_iSpanKeepTime=10;//�����趨���䲢������ʱ�䳤��
	SetBlockType(_T( "WaveInterval" ) );
	ConstructBlockInputOutput ();
}

CWaveIntervalBlock::~CWaveIntervalBlock()
{

}

//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CWaveIntervalBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("In"), VALUE_RTVALUE_SERIES, _T("in1"), _T("in1"));	
	AddOutputPort("Out",VALUE_DOUBLE, _T("out"), _T("out"));
}
//���������½�����ĺ���
CCalcBlock* CWaveIntervalBlock::CreateFromString( const CString& strType )
{
	CWaveIntervalBlock* obj = new CWaveIntervalBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CWaveIntervalBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CWaveIntervalBlock::Calc() //���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
	CCalcPort *pPortObjIn = GetInputPortObj(0);	
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	CUniValue  &CUV=pPortObjIn->GetPortUniValue();
	CArray<double,double> dOrigValueArr;
	CArray<long,long> dOrigTimeArr;
	CArray<short,short> dOrigStatusArr;

	m_dSmoothValueArr.RemoveAll();
	m_dSmoothTimeArr.RemoveAll();
	m_dSmoothStatusArr.RemoveAll();
	double value;
	long time;
	short status;
	for(int i=0; i<CUV.GetDataSize();i++)
	{
		CUV.GetDoubleValAt(i,&value);
		CUV.GetTimeValAt(i,&time);
		CUV.GetStatusValAt(i,&status);
		dOrigValueArr.Add(value);
		dOrigTimeArr.Add(time);
		dOrigStatusArr.Add(status);
	}
	int ret=getSmoothData(
		dOrigValueArr,
		dOrigTimeArr,
		dOrigStatusArr,
		m_iSmoothWidth,
		m_iSmoothStep,
		m_dSmoothValueArr,
		m_dSmoothTimeArr,
		m_dSmoothStatusArr
		);
	if(!ret)
	{
		CString str;
		str.Format("%s--%s--%s:�˲�ʧ�ܣ������˲��������ò�������������������̫�٣�",this->m_pModel->m_strModelName,this->m_pModule->GetModuleName(),this->GetBlockName());
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
		CUniValue UniVar;
		UniVar.AddData(-1);//����ʧ��
		pOutPortObj->SetPortUniValue(UniVar);
		OutputResultToGlobalWS();
		return;
	}
	if(m_bBeFirstCalc)
	{
		//��һ�μ���ʱ�������ʱ��������ǰ����m_iSpanKeepTime���ʱ��Σ�
		//�����ڳ���m_iSpanKeepTimeʱ��Σ����е������ͬһ��ɢ�����ڣ���õ���ɢֵ��
		//���������û�ҵ���ȡ���е�ľ�ֵ���õ���ɢֵ��
		m_dDisValue=getFirstCalcDisValue();
		m_bBeFirstCalc=false;
	}
	else
	{
		//�ж����m_iSpanKeepTime���Ƿ�����������ķ�Χ�ڣ����ȶ���ĳ��������ڣ���õ���ɢֵ��
		//������Ϊm_iSpanKeepTime���п������𵴣���ά����ɢֵ���䡣
		double dNextDisValue=-1;
		int iRet=getDisValue(dNextDisValue);
		if(iRet)
		{
			m_dDisValue=dNextDisValue;
		}
	}
	CUniValue UniVar;
	UniVar.AddData(m_dDisValue);
	pOutPortObj->SetPortUniValue(UniVar);
	OutputResultToGlobalWS();
}
int CWaveIntervalBlock::getDisValue(double &disValue)
{
	long lTimeS,lTimeE;
	bool bFindSETime=false;
	CArray<double,double> dArrNewDisValue;
	CArray<double,double> dArrNewIntStart;
	CArray<double,double> dArrNewIntEnd;	
	getNewDisDataArr(dArrNewDisValue,dArrNewIntStart,dArrNewIntEnd);
	if(m_dSmoothValueArr.GetSize()>1)
	{
		lTimeE=m_dSmoothTimeArr[m_dSmoothValueArr.GetSize()-1];
		lTimeS=lTimeE-m_iSpanKeepTime;
		bFindSETime=true;
	}
	CArray<double,double> tempValueArr;
	if(bFindSETime)
	{
		for(int j=0;j<m_dSmoothValueArr.GetSize();j++)
		{
			if(m_dSmoothTimeArr[j]<lTimeS)
				continue;
			if(m_dSmoothTimeArr[j]>lTimeE)
				break;
			tempValueArr.Add(m_dSmoothValueArr[j]);
		}
	}
	int iRet=getDisValueByValueArr(tempValueArr,dArrNewDisValue,dArrNewIntStart,dArrNewIntEnd,disValue);
	return iRet;
}

void CWaveIntervalBlock::getNewDisDataArr(
										  CArray<double,double> &dArrNewDisValue,
										  CArray<double,double> &dArrNewIntStart,
										  CArray<double,double> &dArrNewIntEnd)//�õ����ǲ�����Χ����ɢ������
										  //�����˵�ǰ��������ɢ����
{
	int iFindIndex=-1;
	for(int i=0;i<m_dArrDisValue.GetSize();i++)
	{
		if(m_dArrDisValue[i]==m_dDisValue)
		{
			iFindIndex=i;
			break;
		}
	}
	int iTagArrAsc=1;//Ĭ�����������
	if(m_dArrIntStart.GetSize()>1)
	{
		if(m_dArrIntStart[0]>m_dArrIntStart[1])
		{	
			iTagArrAsc=0;//�ݼ����䡣
		}
	}
	double dBeforAbs=0;//ǰһ�����䲨����ȵľ���ֵ
	double dAfterAbs=0;//��һ�����䲨����ȵľ���ֵ
	for(i=0;i<m_dArrDisValue.GetSize();i++)
	{
		double disV=m_dArrDisValue[i];
		double intS=m_dArrIntStart[i];
		double intE=m_dArrIntEnd[i];
		if(iFindIndex!=-1)
		{
			if(iTagArrAsc)//�������
			{
				if(iFindIndex-1==i)
				{
					dBeforAbs=(intE-intS)*(m_dSpanIntPercent/100);
					intE-=dBeforAbs;
				}
				else if(iFindIndex==i)
				{
					intS-=dBeforAbs;
				}
				else if(iFindIndex+1==i)
				{
					dAfterAbs=(intE-intS)*(m_dSpanIntPercent/100);
					intS+=dAfterAbs;
				}
			}
			else
			{
				if(iFindIndex-1==i)
				{
					dBeforAbs=(intE-intS)*(m_dSpanIntPercent/100);
					intS+=dBeforAbs;
				}
				else if(iFindIndex==i)
				{
					intE+=dBeforAbs;
				}
				else if(iFindIndex+1==i)
				{
					dAfterAbs=(intE-intS)*(m_dSpanIntPercent/100);
					intE-=dAfterAbs;
				}
			}
		}
		dArrNewDisValue.Add(disV);
		dArrNewIntStart.Add(intS);
		dArrNewIntEnd.Add(intE);
	}
	if(iFindIndex!=-1)//����
	{
		if(iTagArrAsc)//�������
		{
			dArrNewIntEnd[iFindIndex]+=dAfterAbs;
		}
		else
		{
			dArrNewIntStart[iFindIndex]-=dAfterAbs;
		}
	}
}

double CWaveIntervalBlock::getFirstCalcDisValue()
{
	double disValue;
	long lTimeS,lTimeE;
	bool bLast=false;
	BOOL bAllInOne=FALSE;
	for(int i=m_dSmoothValueArr.GetSize()-1;i>=0;i--)
	{
		if(i==m_dSmoothValueArr.GetSize()-1)
		{
			lTimeE=m_dSmoothTimeArr[i];
			lTimeS=lTimeE-m_iSpanKeepTime;
		}
		else
		{
			lTimeE-=1;//��ǰƽ������1��
			lTimeS-=1;//��ǰƽ������1��
		}
		CArray<double,double> tempValueArr;
		for(int j=0;j<m_dSmoothValueArr.GetSize();j++)
		{
			if(m_dSmoothTimeArr[j]<lTimeS)
				continue;
			if(m_dSmoothTimeArr[j]>lTimeE)
				break;
			tempValueArr.Add(m_dSmoothValueArr[j]);
		}
		int iRet=getDisValueByValueArr(tempValueArr,m_dArrDisValue,m_dArrIntStart,m_dArrIntEnd,disValue);
		if(iRet)//����ҵ�ƥ������
		{
			bAllInOne=TRUE;//�������ҵ�ƥ������䡣
			break;
		}
	}
	if(!bAllInOne)//������û���ҵ�ƥ������䡣
	{
		double sum=0;
		for(int i=m_dSmoothValueArr.GetSize()-1;i>=0;i--)
		{
			sum+=m_dSmoothValueArr[i];
		}
		double dAvg=sum/m_dSmoothValueArr.GetSize();
		getDisValueByValue(dAvg,m_dArrDisValue,m_dArrIntStart,m_dArrIntEnd,disValue);
	}
	return disValue;
}
void CWaveIntervalBlock::getDisValueByValue(
											  double dAvg,
											  const CArray<double,double> &dArrDisValue,
											  const CArray<double,double> &dArrIntStart,
											  const CArray<double,double> &dArrIntEnd,
											  double &disValue)
{
	disValue=-1;
	for(int i=0;i<dArrDisValue.GetSize();i++)
	{
		if((dAvg>=dArrIntStart[i])&&(dAvg<dArrIntEnd[i]))
		{
			disValue=dArrDisValue[i];
			break;
		}
	}
}
int CWaveIntervalBlock::getDisValueByValueArr(
											  const CArray<double,double> &tempValueArr,
											  const CArray<double,double> &dArrDisValue,
											  const CArray<double,double> &dArrIntStart,
											  const CArray<double,double> &dArrIntEnd,
											  double &disValue)//�����Ƿ��ȶ�������ĳ��ɢ���䡣
{
	BOOL ret=FALSE;
	if(tempValueArr.GetSize()==0)
		return ret;
	for(int i=0;i<dArrDisValue.GetSize();i++)
	{
		bool bAllInOne=true;
		for(int j=0;j<tempValueArr.GetSize();j++)
		{
			if(!((tempValueArr[j]>=dArrIntStart[i])&&(tempValueArr[j]<dArrIntEnd[i])))
			{
				bAllInOne=false;
				break;
			}
		}
		if(bAllInOne)
		{
			disValue=dArrDisValue[i];
			ret=TRUE;
			break;
		}
	}
	return ret;
}
BOOL CWaveIntervalBlock::getSmoothData(
									   const CArray<double,double> &dOrigValueArr,
									   const CArray<long,long> &dOrigTimeArr,
									   const CArray<short,short> &dOrigStatusArr,
									   int iSmoothWidth,
									   int iSmoothStep,
									   CArray<double,double> &dSmoothValueArr,
									   CArray<long,long> &dSmoothTimeArr,
									   CArray<short,short> &dSmoothStatusArr
									   )
{
	BOOL ret=TRUE;
	if(iSmoothStep==0)
	{
		return FALSE;
	}
	dSmoothValueArr.RemoveAll();
	dSmoothTimeArr.RemoveAll();
	dSmoothStatusArr.RemoveAll();
	
	long lTimeS,lTimeE;
	bool bLast=false;
	for(int i=0;i<dOrigValueArr.GetSize();i++)
	{
		if(i==0)
		{
			lTimeS=dOrigTimeArr[i];
			lTimeE=lTimeS+iSmoothWidth;
		}
		else
		{
			lTimeS+=iSmoothStep;
			lTimeE+=iSmoothStep;
			if(lTimeE>=dOrigTimeArr[dOrigTimeArr.GetSize()-1])//����β���ڽ�βʱ���ʱ��ȡ���ʱ�䡣
			{
				lTimeE=dOrigTimeArr[dOrigTimeArr.GetSize()-1];
				bLast=true;
			}
		}
		double dSum=0;
		int iCounter=0;
		int iStatus=0;
		for(int j=0;j<dOrigValueArr.GetSize();j++)
		{
			if(dOrigTimeArr[j]<lTimeS)
				continue;
			if(dOrigTimeArr[j]>lTimeE)
				break;
			dSum+=dOrigValueArr[i];
			iStatus=dOrigStatusArr[i];
			iCounter++;
		}
		dSmoothValueArr.Add(dSum/iCounter);
		dSmoothTimeArr.Add(lTimeE);//ÿ��ƽ��ȥȡƽ�����ĩ��ֵ����һ������ʵ���ʱ�䡣
		dSmoothStatusArr.Add(iStatus);//״̬ȥ�����������һ�����״̬��
		if(bLast)
		{
			break;
		}
	}
	if(dSmoothValueArr.GetSize()<3)//�˲������̫��
	{
		ret=FALSE;
	}
	return ret;
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CWaveIntervalBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("SpanIntPercent")==0)
	{
		if(!strItem1.IsEmpty())  {m_dSpanIntPercent=atof(strItem1);}
	}
	else if(strPropName.Compare("SpanKeepTime")==0)
	{
		if(!strItem1.IsEmpty())  {m_iSpanKeepTime=atoi(strItem1);}
	}
	else if(strPropName.Compare("SmoothWidth")==0)
	{
		if(!strItem1.IsEmpty())  {m_iSmoothWidth=atoi(strItem1);}
	}
	else if(strPropName.Compare("SmoothStep")==0)
	{
		if(!strItem1.IsEmpty())  {m_iSmoothStep=atoi(strItem1);}
	}
	else //��ʼ
	{
		int index=strPropName.ReverseFind('.');
		CString strIndex=strPropName.Right(strPropName.GetLength()-index-1);
		CString strName=strPropName.Left(index);
		int propIndex=atoi(strIndex);
		if(strName.Compare("DisData")==0)
		{
			m_dArrDisValue.SetAtGrow(propIndex,atof(strItem1));
			m_dArrIntStart.SetAtGrow(propIndex,atof(strItem2));
			m_dArrIntEnd.SetAtGrow(propIndex,atof(strItem3));
		}
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CWaveIntervalBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgWaveInterval dlg;
	dlg.m_dSpanIntPercent=m_dSpanIntPercent;
	dlg.m_iSpanKeepTime=m_iSpanKeepTime;
	dlg.m_iSmoothWidth=m_iSmoothWidth;
	dlg.m_iSmoothStep=m_iSmoothStep;
	dlg.m_dArrDisValue.Copy(m_dArrDisValue);
	dlg.m_dArrIntStart.Copy(m_dArrIntStart);
	dlg.m_dArrIntEnd.Copy(m_dArrIntEnd);
	if(IDOK==dlg.DoModal())
	{
		m_iSmoothWidth=dlg.m_iSmoothWidth;
		m_iSmoothStep=dlg.m_iSmoothStep;
		m_dSpanIntPercent=dlg.m_dSpanIntPercent;
		m_iSpanKeepTime=dlg.m_iSpanKeepTime;
		m_dArrDisValue.Copy(dlg.m_dArrDisValue);
		m_dArrIntStart.Copy(dlg.m_dArrIntStart);
		m_dArrIntEnd.Copy(dlg.m_dArrIntEnd);
	}
}
//�����Խ���ģ��д�뵽���ݿ���
void CWaveIntervalBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="SpanIntPercent";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%f",m_dSpanIntPercent); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="SpanKeepTime";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iSpanKeepTime); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="SmoothWidth";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iSmoothWidth); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="SmoothStep";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iSmoothStep); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	for(int i=0;i<m_dArrDisValue.GetSize();i++)
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("DisData.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
		strFieldValue.Format("%f",m_dArrDisValue[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%f",m_dArrIntStart[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%f",m_dArrIntEnd[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
	}
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CWaveIntervalBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_dSpanIntPercent=((CWaveIntervalBlock*)pBlock)->m_dSpanIntPercent;
	m_iSpanKeepTime=((CWaveIntervalBlock*)pBlock)->m_iSpanKeepTime;
	m_iSmoothWidth=((CWaveIntervalBlock*)pBlock)->m_iSmoothWidth;
	m_iSmoothStep=((CWaveIntervalBlock*)pBlock)->m_iSmoothStep;
	m_dArrDisValue.Copy(((CWaveIntervalBlock*)pBlock)->m_dArrDisValue);
	m_dArrIntStart.Copy(((CWaveIntervalBlock*)pBlock)->m_dArrIntStart);
	m_dArrIntEnd.Copy(((CWaveIntervalBlock*)pBlock)->m_dArrIntEnd);
	
}
//�����Է��ô���������
void CWaveIntervalBlock::SetBlockPropertyToTransferData(
								CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"SpanIntPercent");
	
	CString str;
	str.Format("%f",m_dSpanIntPercent);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"SpanKeepTime");
	
	str.Format("%d",m_iSpanKeepTime);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"SmoothWidth");
	
	str.Format("%d",m_iSmoothWidth);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"SmoothStep");
	
	str.Format("%d",m_iSmoothStep);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	for(int i=0;i<m_dArrDisValue.GetSize();i++)
	{
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		CString str;
		str.Format("DisData.%d",i);
		strcpy(pData->m_cPropName,str);
		
		str.Format("%f",m_dArrDisValue[i]);
		strcpy(pData->m_cValueItem1,str);
		str.Format("%f",m_dArrIntStart[i]);
		strcpy(pData->m_cValueItem2,str);
		str.Format("%f",m_dArrIntEnd[i]);
		strcpy(pData->m_cValueItem3,str);
		dataArr.Add(pData);
	}
}
