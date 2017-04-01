// StableJudgeBlock.cpp: implementation of the CStableJudgeBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StableJudgeBlock.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStableJudgeBlock::CStableJudgeBlock()
{
	m_iCalcNoStable=0;
	m_dDown2 = 90.0;
	m_dDown1 = 95.0;
	m_dFazhi1 = 5.0;
	m_dFazhi2 = 2.0;
	m_iRadio = 0;
	//SYSTME=====��������======
	//��������
	SetBlockType(_T( "StableJudge" ) );
	ConstructBlockInputOutput();
	//SYSTME=====��ʼ����ģ�������===
}

CStableJudgeBlock::~CStableJudgeBlock()
{

}
//�����ʼ�������������
void CStableJudgeBlock::ConstructBlockInputOutput()
{	
	AddInputPort(_T("In"), VALUE_RTVALUE_SERIES, _T("in1"), _T("in1"));	
	AddOutputPort("Out",VALUE_DOUBLE, _T("out"), _T("out"));
}
//���������½�����ĺ���
//ͬʱ��CCalcBlockControlFactory�У�Ҫ������Ӧ�Ĵ���
CCalcBlock* CStableJudgeBlock::CreateFromString( const CString& strType )
{
	CStableJudgeBlock* obj = new CStableJudgeBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CStableJudgeBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CStableJudgeBlock::Calc() 
{
	//�������˿ڵ�����
	//1��̬;2��;3��;4BR,0δ֪
	GetInputValueFromGlobalWS();
	double dOutput1=0;//�����ֵ1��2�����㣬���1���������0
	double dOutput2=0;//��ֵ1��ʵ�ʴ���ٷ���
	double dOutput3=0;//��ֵ2��ʵ�ʴ���ٷ���
	
	CCalcPort *pPortObjIn = GetInputPortObj(0);	
	CUniValue  &CUV=pPortObjIn->GetPortUniValue();
	if(CUV.GetDataSize()>=3)//�����ݸ������ڵ���������ʱ��ʼ���������������Ϊ0
	{
		//�õ���ֵ
		double sum=0;
		double avg=0;
		CArray<double,double> dOrigValueArr;
		for(int i=0; i<CUV.GetDataSize();i++)
		{
			double value;
			CUV.GetDoubleValAt(i,&value);
			dOrigValueArr.Add(value);
			sum+=value;
		}
		avg=sum/CUV.GetDataSize();
		//�����ֵһ
		double dlow1=0;
		double dhigh1=0;
		int icont=0;
		dlow1=avg*(1-m_dFazhi1/100);
		dhigh1=avg*(1+m_dFazhi1/100);
		for(int m=0; m<CUV.GetDataSize(); m++)
		{
			double value;
			CUV.GetDoubleValAt(m,&value);
			if((dlow1<value) && (value<dhigh1))
			{
				icont++;
			}
		}
		dOutput2=((double)icont)/((double)CUV.GetDataSize());
		//�����ֵ��
		double dlow2=0;
		double dhigh2=0;
		icont=0;
		dlow2=avg*(1-m_dFazhi2/100);
		dhigh2=avg*(1+m_dFazhi2/100);
		for(int n=0; n<CUV.GetDataSize(); n++)
		{
			double value;
			CUV.GetDoubleValAt(n,&value);
			if((dlow2<value) && (value<dhigh2))
			{
				icont++;
			}
		}
		dOutput3=((double)icont)/((double)CUV.GetDataSize());
		//���
		if((dOutput2*100>=m_dDown1)&&(dOutput3*100>=m_dDown2))
		{
			dOutput1=1.0;
		}
		else
		{
			if(m_iCalcNoStable)
			{
				//1��̬;2��;3��;4BR,0δ֪
				CArray<double,double> dSmoothArr;
				int iSmoothMinSize=10;//ƽ������̵����鳤��
				int iSmoothStep=10;//ƽ����ÿ�����ֵ�ĸ���
				if(getSmoothArr(dOrigValueArr,iSmoothStep,iSmoothMinSize,dSmoothArr))//�õ�ƽ������
				{
					double dFBDataPart=0.4;//ȡƽ������ǰ��ٷ�֮�������ж��ھ�ֵ���µı���
					double dFDownPer,dFUpPer;//ǰ�Σ�λ�ھ�ֵ���µİٷֱ�
					double dBDownPer,dBUpPer;//��ˣ�λ�ھ�ֵ���µİٷֱ�
					getDisValue(dSmoothArr,dFBDataPart,dFDownPer,dFUpPer,dBDownPer,dBUpPer);//�ж�ǰ����ھ�ֵ���µİٷֱ�
					double dFBPartBeVauePer=0.7;
					int iUpOrDown=getUpOrDown(dFBPartBeVauePer,dFDownPer,dFUpPer,dBDownPer,dBUpPer);
					if(0==iUpOrDown)//��Ϊδ֪������ж���BRģʽ������������״̬��
					{
						double dBRVariance=6;//BR���������,BR������ͳ��PI�и��ɵ���ʵ���ݣ�������>10
						double dRealVariance=getVariance(dOrigValueArr);
						if(dRealVariance>dBRVariance)
						{
							//dOutput1=4;//BRģʽ
							CArray<double,double> dRealVarArr;
							int iPartNum=3;//�ֶ��󷽲�����ķֶ�����
							getVariance(dOrigValueArr,iPartNum,dRealVarArr);//�ֶ��󷽲�жϸ��η����Ƿ������BR������ߡ�
							if(dRealVarArr.GetSize()!=iPartNum)//���ֽ������ԭʼ��̫��
							{
								dOutput1=0;//δ֪ģʽ
							}
							else
							{
								bool bAllBR=true;
								for(int i=0;i<dRealVarArr.GetSize();i++)
								{
									if(dRealVarArr[i]<dBRVariance)
									{
										bAllBR=false;
										break;
									}
								}
								if(bAllBR)
								{
									dOutput1=4;//BRģʽ
								}
								else
								{
									dOutput1=0;//δ֪ģʽ
								}
							}
						}
						else
						{
							dOutput1=0;//δ֪ģʽ
						}
					}
					else
					{
						dOutput1=iUpOrDown;
					}
				}
				else//������̫�٣������������
				{
					dOutput1=0.0;
				}
			}
			else
			{
				dOutput1=0.0;
			}
		}
	}
	if(m_iRadio==0)
	{
		CUniValue UniVar1;
		CCalcPort *pPortObjOut1 = GetOutputPortObj(0);
		UniVar1.AddData(dOutput1);
		pPortObjOut1->SetPortUniValue(UniVar1);	
	}
	else if(m_iRadio==1)
	{
		CUniValue UniVar1;
		CCalcPort *pPortObjOut1 = GetOutputPortObj(0);
		UniVar1.AddData(dOutput1);
		pPortObjOut1->SetPortUniValue(UniVar1);
		
		CUniValue UniVar2;
		CCalcPort *pPortObjOut2 = GetOutputPortObj(1);
		UniVar2.AddData(dOutput2);
		pPortObjOut2->SetPortUniValue(UniVar2);
		
		CUniValue UniVar3;
		CCalcPort *pPortObjOut3 = GetOutputPortObj(2);
		UniVar3.AddData(dOutput3);
		pPortObjOut3->SetPortUniValue(UniVar3);
	}
	//�������
	OutputResultToGlobalWS();
}
void CStableJudgeBlock::getVariance(const CArray<double,double> &dOrigValueArr,int iPartNum,CArray<double,double> &dRealVarArr)
{
	dRealVarArr.RemoveAll();
	if(dOrigValueArr.GetSize()<iPartNum+1)//ԭʼ��̫��
	{
		return;
	}
	CArray<double,double> dTempArr;
	int partCount=(int)floor(dOrigValueArr.GetSize()/iPartNum);
	for(int i=0;i<iPartNum;i++)
	{
		dTempArr.RemoveAll();
		for(int j=i*partCount;j<i*(partCount+1);j++)
		{
			dTempArr.Add(dOrigValueArr[j]);
		}
		dRealVarArr.Add(getVariance(dTempArr));
	}
}
double CStableJudgeBlock::getVariance(const CArray<double,double> &dOrigValueArr)
{
	double dSum=0;
	for(int i=0;i<dOrigValueArr.GetSize();i++)
	{
		dSum+=dOrigValueArr[i];
	}
	double dAvg=dSum/dOrigValueArr.GetSize();
	dSum=0;
	for(i=0;i<dOrigValueArr.GetSize();i++)
	{
		dSum+=(dOrigValueArr[i]-dAvg)*(dOrigValueArr[i]-dAvg);
	}
	double dAvg2=dSum/dOrigValueArr.GetSize();
	return sqrt(dAvg2);
}
int CStableJudgeBlock::getUpOrDown(double dFBPartBeVauePer,
									double dFDownPer,
									double dFUpPer,
									double dBDownPer,
									double dBUpPer)
{
	int iState=0;
	if((dFDownPer>dFBPartBeVauePer)&&(dBUpPer>dFBPartBeVauePer))
	{
		iState=2;//������
	}
	else if((dFUpPer>dFBPartBeVauePer)&&(dBDownPer>dFBPartBeVauePer))
	{
		iState=3;//������
	}
	return iState;
}
void CStableJudgeBlock::getDisValue(const CArray<double,double> &dSmoothArr,
									double dFBDataPart,
									double &dFDownPer,
									double &dFUpPer,
									double &dBDownPer,
									double &dBUpPer)
{
	dFDownPer=0;
	dFUpPer=0;
	dBDownPer=0;
	dBUpPer=0;
	if(dFBDataPart>1)
		return;
	double dSum=0;
	for(int i=0;i<dSmoothArr.GetSize();i++)
	{
		dSum+=dSmoothArr[i];
	}
	int iSmoothCount=dSmoothArr.GetSize();
	double dAvg=dSum/iSmoothCount;
	int iPartCount=(int)floor(iSmoothCount*dFBDataPart);
	int iFDownCount=0;
	int iFUpCount=0;
	int iBDownCount=0;
	int iBUpCount=0;
	for(i=0;i<iPartCount;i++)
	{
		if(dSmoothArr[i]>dAvg)
		{
			iFUpCount++;
		}
		else if(dSmoothArr[i]<dAvg)
		{
			iFDownCount++;
		}
		if(dSmoothArr[iSmoothCount-1-i]>dAvg)
		{
			iBUpCount++;
		}
		else if(dSmoothArr[iSmoothCount-1-i]<dAvg)
		{
			iBDownCount++;
		}
	}
	dFDownPer=(double)iFDownCount/(double)iPartCount;
	dFUpPer=(double)iFUpCount/(double)iPartCount;
	dBDownPer=(double)iBDownCount/(double)iPartCount;
	dBUpPer=(double)iBUpCount/(double)iPartCount;
}
BOOL CStableJudgeBlock::getSmoothArr(const CArray<double,double> &dOrigValueArr,
									 int iSmoothStep,
									 int iSmoothMinSize,
									 CArray<double,double> &dSmoothArr)
{
	dSmoothArr.RemoveAll();
	BOOL bRet=TRUE;
	if(dOrigValueArr.GetSize()-iSmoothStep+1<iSmoothMinSize)//�ܹ�����ƽ������Ҫ��������С����
	{
		bRet=FALSE;
		return bRet;
	}
	double dSumTemp=0;
	for(int i=0;i<dOrigValueArr.GetSize()-iSmoothStep;i++)
	{
		if(i==0)
		{
			for(int j=0;j<iSmoothStep;j++)
			{
				dSumTemp+=dOrigValueArr[j];
			}
			dSmoothArr.Add(dSumTemp);
		}
		else
		{
			dSumTemp-=dOrigValueArr[i-1];
			dSumTemp+=dOrigValueArr[i+iSmoothStep-1];
			dSmoothArr.Add(dSumTemp);
		}
	}
	return bRet;
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CStableJudgeBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("StableJudge")==0)
	{
		if(!strItem1.IsEmpty())
			m_dFazhi1=atof(strItem1);
		if(!strItem2.IsEmpty())
			m_dDown1=atof(strItem2);
		if(!strItem3.IsEmpty())
			m_dFazhi2=atof(strItem3);
		if(!strItem4.IsEmpty())
			m_dDown2=atof(strItem4);
		if(!strItem5.IsEmpty())
			m_iRadio=atoi(strItem5);
	}
	if(strPropName.Compare("CalcNoStable")==0)
	{
		if(!strItem1.IsEmpty())
			m_iCalcNoStable=atoi(strItem1);
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CStableJudgeBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//�����ԭ�е�����˿�����
	IDArrRightOut.RemoveAll();

	CStringArray  IDArrRightIn;//������ڲ�������˿�����
    IDArrRightIn.RemoveAll();

	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}

	CPropertyDlgStableJudge dlg;
    dlg.m_dFazhi1=m_dFazhi1;
	dlg.m_dFazhi2=m_dFazhi2;
	dlg.m_dDown1=m_dDown1;
	dlg.m_dDown2=m_dDown2;
	dlg.m_iRadio=m_iRadio;
	dlg.m_BCalcNoStable=m_iCalcNoStable;

	if(dlg.DoModal()==IDOK)
	{
		m_dFazhi1=dlg.m_dFazhi1;
		m_dFazhi2=dlg.m_dFazhi2;
		m_dDown1=dlg.m_dDown1;
		m_dDown2=dlg.m_dDown2;
		m_iRadio=dlg.m_iRadio;	
		m_iCalcNoStable=dlg.m_BCalcNoStable;
		
		if(m_iRadio==0)
		{
			IDArrRightIn.Add("Out");
		}
		else if(m_iRadio==1)
		{
			IDArrRightIn.Add("Out");
			IDArrRightIn.Add("T1");
			IDArrRightIn.Add("T2");
		}
		
		for(int jj=IDArrRightOut.GetSize()-1; jj>-1; jj--)//ɾ��ԭ�ж����õ�Port,����ɾ
		{
			int  runs=0;//��������
			for(int kk=0; kk<IDArrRightIn.GetSize(); kk++)
			{
				if(IDArrRightIn[kk]!=IDArrRightOut[jj])	
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
				if(IDArrRightIn[mm]!=IDArrRightOut[nn])
				{runs++;}		
			}
			if(runs==IDArrRightOut.GetSize())
			{
				CString str=IDArrRightIn[mm];
				InSertOutputPortBefore(mm,str, VALUE_DOUBLE, _T(str));
			}
		}
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
void CStableJudgeBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	//����1-5�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("StableJudge"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
	//strFieldValue.Format("%d",m_strInputNum);
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(m_dFazhi1) );
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(m_dDown1) );
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(m_dFazhi2) );
	pRecord->PutCollect(_variant_t("value_item4"),_variant_t(m_dDown2) );
	strFieldValue.Format("%d",m_iRadio);
	pRecord->PutCollect(_variant_t("value_item5"),_variant_t(strFieldValue) );
	
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("CalcNoStable"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iCalcNoStable);
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(strFieldValue) );

	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CStableJudgeBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_dFazhi1=((CStableJudgeBlock*)pBlock)->m_dFazhi1;
	m_dFazhi2=((CStableJudgeBlock*)pBlock)->m_dFazhi2;
	m_dDown1=((CStableJudgeBlock*)pBlock)->m_dDown1;
	m_dDown2=((CStableJudgeBlock*)pBlock)->m_dDown2;
	m_iRadio=((CStableJudgeBlock*)pBlock)->m_iRadio;
	m_iCalcNoStable=((CStableJudgeBlock*)pBlock)->m_iCalcNoStable;			
}
//�����Է��ô���������
void CStableJudgeBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

	CString str;
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"StableJudge");

	str.Format("%f",m_dFazhi1);strcpy(pData->m_cValueItem1,str);
	str.Format("%f",m_dFazhi2);strcpy(pData->m_cValueItem2,str);
	str.Format("%f",m_dDown1);strcpy(pData->m_cValueItem3,str);
	str.Format("%f",m_dDown2);strcpy(pData->m_cValueItem4,str);
	str.Format("%d",m_iRadio);strcpy(pData->m_cValueItem5,str);

	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"CalcNoStable");
	str.Format("%d",m_iCalcNoStable);strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
