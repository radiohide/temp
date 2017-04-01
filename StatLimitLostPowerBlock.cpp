// StatLimitLostPowerBlock.cpp: implementation of the CStatLimitLostPowerBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatLimitLostPowerBlock.h"
#include "PropertyDlgLimitLostPower.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatLimitLostPowerBlock::CStatLimitLostPowerBlock()
{
	dMaxPower=3150000;
	this->iTimeSelfOrInput=1;
	ConstructBlockInputOutput();
	SetBlockType(_T( "LimitLostPower" ) );
}

CStatLimitLostPowerBlock::~CStatLimitLostPowerBlock()
{

}

//���������½�����ĺ���
CCalcBlock* CStatLimitLostPowerBlock::CreateFromString( const CString& strType )
{
	CStatLimitLostPowerBlock* obj = new CStatLimitLostPowerBlock();
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CStatLimitLostPowerBlock::ConstructBlockInputOutput ()
{
	AddInputPort("StartTime",VALUE_DOUBLE);
	AddInputPort("EndTime",VALUE_DOUBLE);
	AddInputPort("WindSpeed",VALUE_RTVALUE_SERIES);
	AddInputPort("LimitPower",VALUE_RTVALUE_SERIES);

	AddOutputPort("OutPut",VALUE_STAT);
}
//��ʼ������ĺ���������ĳ�ʼ��
void CStatLimitLostPowerBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//�������ԵĶ����ַ���
CString CStatLimitLostPowerBlock::GetBlockStrPro()//to do
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CStatLimitLostPowerBlock::SetBlockStrPro(CString str)//���ü������ַ�������//to do
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
void CStatLimitLostPowerBlock::Calc()//���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	int lSTime=0;
	int lETime=0;
	int iInputDataStart=0;
	if(iTimeSelfOrInput==1)
	{
		double dInValue=0;
		CCalcPort *pInPortObj1 = GetInputPortObj(0);
		pInPortObj1->GetPortUniValue().GetDoubleVal(&dInValue);
		lSTime=(long)dInValue;

		CCalcPort *pInPortObj2 = GetInputPortObj(1);
		pInPortObj2->GetPortUniValue().GetDoubleVal(&dInValue);
		lETime=(long)dInValue;
		iInputDataStart=2;
	}
	long lRealSTime=0;
	long lRealETime=0;
	double dValue=0;
	long lTimeOld=0;

	CCalcPort *pPortObjWindSpeed = GetInputPortObj(iInputDataStart);	
	CUniValue  &CUVWindSpeed=pPortObjWindSpeed->GetPortUniValue();

	CCalcPort *pPortObjLimitPower = GetInputPortObj(iInputDataStart+1);	
	CUniValue  &CUVLimitPower=pPortObjLimitPower->GetPortUniValue();

	lTimeOld=0;
	for(int j=0; j<CUVWindSpeed.GetDataSize();j++)
	{
		double value=0;
		long lTime=0;
		CUVWindSpeed.GetDoubleValAt(j,&value);
		CUVWindSpeed.GetTimeValAt(j,&lTime);
		double dLimitPower=getLimitPowerByTime(CUVLimitPower,lTime);
		double dTheoryPower=getPowerFromWindSpeed(value);
		bool bValidLost=false;
		if(dLimitPower<dMaxPower)
		{
			bValidLost=true;
		}
		if(j==0)
		{
			lTimeOld=lTime;
			lRealSTime=lTime;
			lRealETime=lTime;
		}
		else
		{
			if(bValidLost)
			{
				double dDiff=dTheoryPower-dLimitPower/1000;//������ֵ�޸�Ϊkw
				if(dDiff<0)
					dDiff=0;
				dValue+=dDiff*(lTime-lTimeOld);
			}
		}
		if(j+1==CUVWindSpeed.GetDataSize())
		{
			lRealETime=lTime;
		}
		lTimeOld=lTime;
	}
	double dMin=0;
	double dMax=0;		
	if(iTimeSelfOrInput==1)//����Ϊ�ⲿʱ��Ρ�
	{
		lRealSTime=lSTime;
		lRealETime=lETime;
	}
	StatData oneStatData;
	oneStatData.lSTime=lRealSTime;
	oneStatData.lETime=lRealETime;
	oneStatData.lInsertTime=0;
	oneStatData.dMin=dMin;
	oneStatData.dMax=dMax;
	oneStatData.dValue=dValue/3600.0;//ת��Ϊǧ��ʱ
	oneStatData.iType=0;
	oneStatData.iCount=CUVWindSpeed.GetDataSize();
	CUniValue UniVarOut;
	CCalcPort *pPortObjOut = GetOutputPortObj(0);
	UniVarOut.AddStatData(oneStatData);
	pPortObjOut->SetPortUniValue(UniVarOut);
	//�������
	OutputResultToGlobalWS();
}
double CStatLimitLostPowerBlock::getLimitPowerByTime(CUniValue &CUV,long lParTime)
{
	double dPower=0;
	for(int j=CUV.GetDataSize()-1;j>=0;j--)
	{
		double value=0;
		long lTime=0;
		CUV.GetDoubleValAt(j,&value);
		CUV.GetTimeValAt(j,&lTime);
		if(lParTime>=lTime)
		{
			dPower=value;
			break;
		}
	}
	return dPower;
}
/*
bool CStatLimitLostPowerBlock::isValidLostByTime(CUniValue &CUV,long lParTime)
{
	bool bRet=false;
	for(int j=CUV.GetDataSize()-1;j>=0;j--)
	{
		double value=0;
		long lTime=0;
		CUV.GetDoubleValAt(j,&value);
		CUV.GetTimeValAt(j,&lTime);
		if(lParTime>=lTime)
		{
			if(((int)value)==iValid)
			{
				bRet=true;
				break;
			}
		}
	}
	return  bRet;
}
*/
double CStatLimitLostPowerBlock::getPowerFromWindSpeed(double dWindSpeed)
{
	double dPower=0;
	for(int i=dWindSpeedArr.GetSize()-1;i>=0;i--)
	{
		if(dWindSpeed>=dWindSpeedArr[i])
		{
			if(i==dWindSpeedArr.GetSize()-1)
			{
				dPower=dPowerArr[i];
				break;
			}
			else
			{
				dPower=dPowerArr[i]+(dPowerArr[i+1]-dPowerArr[i])*(dWindSpeed-dWindSpeedArr[i])/(dWindSpeedArr[i+1]-dWindSpeedArr[i]);
			}
		}
	}
	return dPower;
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CStatLimitLostPowerBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("SelfOrInput")==0)
	{
		if(!strItem1.IsEmpty())  {iTimeSelfOrInput=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {dMaxPower=atof(strItem2);}
	}
	else
	{
		int propType,propIndex;
		getPropTypeByName(strPropName,propType,propIndex);
		if(propType==2)
		{
			if(!strItem1.IsEmpty())  {dWindSpeedArr.SetAtGrow(propIndex,atof(strItem1));}
			if(!strItem2.IsEmpty())  {dPowerArr.SetAtGrow(propIndex,atof(strItem2));}
		}
	}
}
void CStatLimitLostPowerBlock::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
{
	int dIndex=strPropName.ReverseFind('.');
	CString strIndex=strPropName.Right(strPropName.GetLength()-dIndex-1);
	CString strName=strPropName.Left(dIndex);
	propIndex=atoi(strIndex);

	if(strName=="input")//����Ϊ1
	{
		propType=1;
	}
	else if(strName=="WindPower")//���Ϊ2
	{
		propType=2;
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CStatLimitLostPowerBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgLimitLostPower dlg;

	int portnumLinkBefore=this->GetInputNum();//����
	CStringArray  IDArrLeftOut;//�����ԭ�е�����˿�����
	CArray<int,int> IDArrLeftOutType;//�����ԭ�е�����˿�����
	CStringArray  IDArrLeftIn;//������ڲ�������˿�����
	CArray<int,int>  IDArrLeftInType;//������ڲ�������˿�����
	
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		int iPortType=this->GetInputPortObj(q)->GetPortValueType();
		IDArrLeftOut.Add(strPortID);
		IDArrLeftOutType.Add(iPortType);
	}

	dlg.dMaxPower=dMaxPower;
	dlg.dWindSpeedArr.Copy(dWindSpeedArr);
	dlg.dPowerArr.Copy(dPowerArr);
	dlg.iTimeSelfOrInput=iTimeSelfOrInput;

	if(dlg.DoModal()==IDOK)
	{
		dMaxPower=dlg.dMaxPower;	
		dWindSpeedArr.Copy(dlg.dWindSpeedArr);
		dPowerArr.Copy(dlg.dPowerArr);	
		iTimeSelfOrInput=dlg.iTimeSelfOrInput;

		//1��������˿ڵ��޸�
		if(iTimeSelfOrInput==1)
		{
			IDArrLeftIn.Add("STime");
			IDArrLeftInType.Add(VALUE_DOUBLE);
			IDArrLeftIn.Add("ETime");
			IDArrLeftInType.Add(VALUE_DOUBLE);
		}
		IDArrLeftIn.Add("WindSpeed");IDArrLeftInType.Add(VALUE_RTVALUE_SERIES);
		IDArrLeftIn.Add("LimitPower");IDArrLeftInType.Add(VALUE_RTVALUE_SERIES);
		
		for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)	//ɾ��ԭ�ж����õ�Port,����ɾ
		{
			int  runs=0;//��������
			for(int k=0; k<IDArrLeftIn.GetSize(); k++)
			{
				if((IDArrLeftIn[k]!=IDArrLeftOut[j])||(IDArrLeftInType[k]!=IDArrLeftOutType[j]))	
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
				if((IDArrLeftIn[m]!=IDArrLeftOut[n])||(IDArrLeftInType[m]!=IDArrLeftOutType[n]))
				{runs++;}		
			}
			if(runs==IDArrLeftOut.GetSize())
			{
				CString str=IDArrLeftIn[m];
				InSertInputPortBefore(m,str, IDArrLeftInType[m], _T(str));
			}
		}
		parent->RedrawWindow();
	}
}
//�����Խ���ģ��д�뵽���ݿ���
void CStatLimitLostPowerBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	for(int i=0;i<dWindSpeedArr.GetSize();i++)//
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("WindPower.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������

		strFieldValue.Format("%f",dWindSpeedArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%f",dPowerArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������
	}
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="SelfOrInput";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",iTimeSelfOrInput); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%f",dMaxPower); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
	
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CStatLimitLostPowerBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->iTimeSelfOrInput=((CStatLimitLostPowerBlock*)pBlock)->iTimeSelfOrInput;
	this->dMaxPower=((CStatLimitLostPowerBlock*)pBlock)->dMaxPower;
	this->dWindSpeedArr.Copy(((CStatLimitLostPowerBlock*)pBlock)->dWindSpeedArr);
	this->dPowerArr.Copy(((CStatLimitLostPowerBlock*)pBlock)->dPowerArr);
}
//�����Է��ô���������
void CStatLimitLostPowerBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}