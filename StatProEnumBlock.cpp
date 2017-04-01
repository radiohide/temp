// StatProEnumBlock.cpp: implementation of the CStatProEnumBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatProEnumBlock.h"
#include "PropertyDlgStatProEnum.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatProEnumBlock::CStatProEnumBlock(int iTimeSelfOrInput)
{
	m_dLeft=0.5;//�����󣨱����䣩
	m_dRight=1.5;//�����ң������䣩
	m_iStatType=1;//״ֵ̬
	m_iFunType=0;//ͳ�Ʒ���������ͳ��
	m_dGain=1;//1��=0.0166666666666667Сʱ

	this->iTimeSelfOrInput=iTimeSelfOrInput;
	if(iTimeSelfOrInput==1)
	{
		ConstructBlockInputOutput();
	}
	SetBlockType(_T( "StatProEnum" ) );
}

CStatProEnumBlock::~CStatProEnumBlock()
{

}
//���������½�����ĺ���
CCalcBlock* CStatProEnumBlock::CreateFromString( const CString& strType )
{
	CStatProEnumBlock* obj = new CStatProEnumBlock(1);
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CStatProEnumBlock::ConstructBlockInputOutput ()
{
	//��ʼ��ʱ���ⲿ��ȡʱ����ʼ�����Գ�ʼʱ��������˿�
	AddInputPort("StartTime",VALUE_DOUBLE);
	AddInputPort("EndTime",VALUE_DOUBLE);
}
//��ʼ������ĺ���������ĳ�ʼ��
void CStatProEnumBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//�������ԵĶ����ַ���
CString CStatProEnumBlock::GetBlockStrPro()//to do
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CStatProEnumBlock::SetBlockStrPro(CString str)//���ü������ַ�������//to do
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
void CStatProEnumBlock::Calc()//���м���ĺ���,��Ҫ����
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

	//����ⲿ�������������ʽ��ӣ�������vestore�ṩ��ԭ���ý������÷�ʽ���ò���Ϊ��߲����任ʱ����Ӧ��

	for(int i=0;i<strInputArr.GetSize();i++)
	{
		CCalcPort *pPortObjIn3 = GetInputPortObj(iInputDataStart+i);	
		CUniValue  &CUV=pPortObjIn3->GetPortUniValue();
		
		double dValue=0;
		double dMin=0;
		double dMax=0;	
		long lRealSTime=0;
		long lRealETime=0;
		int iCount=0; 
		if(m_iFunType==0)//ͳ�ƴ���
		{
			statDataCount(CUV,dValue,dMin,dMax,lRealSTime,lRealETime,iCount);
		}
		else//ͳ��ʱ��
		{
			statDataTimeLength(CUV,dValue,dMin,dMax,lRealSTime,lRealETime,iCount);
		}
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
		oneStatData.dValue=dValue*m_dGain;//�Ŵ�m_dGain����
		oneStatData.iType=m_iStatType;//�趨״ֵ̬
		oneStatData.iCount=iCount;

		CUniValue UniVarOut;
		CCalcPort *pPortObjOut = GetOutputPortObj(i);
		UniVarOut.AddStatData(oneStatData);
		pPortObjOut->SetPortUniValue(UniVarOut);
	}
	//�������
	OutputResultToGlobalWS();
}
void CStatProEnumBlock::statDataTimeLength(CUniValue  &CUV,//ͳ��ʱ��
							 double &dValue,
							 double &dMin,
							 double &dMax,
							 long &lRealSTime,
							 long &lRealETime,
							 int &iCount)
{
	dValue=0;
	dMin=0;
	dMax=0;
	lRealSTime=0;
	lRealETime=0;
	iCount=CUV.GetDataSize();
	double dOldValue=0;
	long lOldTime=0;
	for(int j=0; j<CUV.GetDataSize();j++)
	{
		double value=0;
		long lTime=0;
		CUV.GetDoubleValAt(j,&value);
		CUV.GetTimeValAt(j,&lTime);
		if(j==0)
		{
			dMin=value;
			dMax=dMin;
			lRealSTime=lTime;
			lRealETime=lTime;
			dOldValue=value;
			lOldTime=lTime;
		}
		else
		{
			//ǰ���������ݵ��������������Ϊ�������ڳ���
			if((isInRange(dOldValue,m_dLeft,m_dRight))&&(isInRange(value,m_dLeft,m_dRight)))
			{
				dValue+=(double)(lTime-lOldTime);//ʱ���ۼ�
			}
			if(value<dMin)
			{
				dMin=value;
			}
			if(value>dMax)
			{
				dMax=value;
			}
		}
		if(j+1==CUV.GetDataSize())
		{
			lRealETime=lTime;
		}
		dOldValue=value;
		lOldTime=lTime;
	}
}
void CStatProEnumBlock::statDataCount(CUniValue  &CUV,
							 double &dValue,
							 double &dMin,
							 double &dMax,
							 long &lRealSTime,
							 long &lRealETime,
							 int &iCount)
{
	dValue=0;
	dMin=0;
	dMax=0;
	lRealSTime=0;
	lRealETime=0;
	iCount=CUV.GetDataSize();
	double dOldValue=0;
	for(int j=0; j<CUV.GetDataSize();j++)
	{
		double value=0;
		long lTime=0;
		CUV.GetDoubleValAt(j,&value);
		CUV.GetTimeValAt(j,&lTime);
		if(j==0)
		{
			dMin=value;
			dMax=dMin;
			lRealSTime=lTime;
			lRealETime=lTime;
			dOldValue=value;
		}
		else
		{
			//ǰһ���㲻�ڸ÷�Χ�ڣ���ǰ���ڸ÷�Χ�ڣ�����Ϊ����һ�Σ�����һ�Σ�
			if((!isInRange(dOldValue,m_dLeft,m_dRight))&&(isInRange(value,m_dLeft,m_dRight)))
			{
				dValue+=1;//������1
			}
			if(value<dMin)
			{
				dMin=value;
			}
			if(value>dMax)
			{
				dMax=value;
			}
		}
		if(j+1==CUV.GetDataSize())
		{
			lRealETime=lTime;
		}
		dOldValue=value;
	}
}
bool CStatProEnumBlock::isInRange(const double &dValue,const double &dLeft,const double &dRight)
{
	bool bRet=false;
	if((dValue>=dLeft)&&(dValue<=dRight))
	{
		bRet=true;
	}
	return bRet;
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CStatProEnumBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("SelfOrInput")==0)
	{
		if(!strItem1.IsEmpty())  {iTimeSelfOrInput=atoi(strItem1);}
	}
	else if(strPropName.Compare("StatFun")==0)
	{
		if(!strItem1.IsEmpty())  {m_dLeft=atof(strItem1);}
		if(!strItem2.IsEmpty())  {m_dRight=atof(strItem2);}
		if(!strItem3.IsEmpty())  {m_iStatType=atoi(strItem3);}
		if(!strItem4.IsEmpty())  {m_iFunType=atoi(strItem4);}
		if(!strItem5.IsEmpty())  {m_dGain=atof(strItem5);}
	}
	else
	{
		int propType,propIndex;
		getPropTypeByName(strPropName,propType,propIndex);
		if(propType==2)
		{
			if(!strItem1.IsEmpty())  {strInputArr.SetAtGrow(propIndex,strItem1);}
			if(!strItem2.IsEmpty())  {strOutputArr.SetAtGrow(propIndex,strItem2);}
			if(!strItem3.IsEmpty())  {strUnitArr.SetAtGrow(propIndex,strItem3);}
			if(!strItem4.IsEmpty())  {strTagDespArr.SetAtGrow(propIndex,strItem4);}
		}
	}
}
void CStatProEnumBlock::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
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
void CStatProEnumBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStatProEnum dlg;

	int portnumLinkBefore=this->GetInputNum();//����
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//�����ԭ�е�����˿�����
	CStringArray  IDArrRightIn;//������ڲ�������˿�����
	
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

	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}

	dlg.strInputArr.Copy(strInputArr);//�������
	dlg.strOutputArr.Copy(strOutputArr);//�������
	dlg.strTagDespArr.Copy(strTagDespArr);//��������
	dlg.strUnitArr.Copy(strUnitArr);//��λ
	dlg.iTimeSelfOrInput=iTimeSelfOrInput;//��עʱ��Σ�0���ݱ���1�ⲿ����
	dlg.m_dLeft=m_dLeft;//�����󣨱����䣩
	dlg.m_dRight=m_dRight;//�����ң������䣩
	dlg.m_iStatType=m_iStatType;//״ֵ̬
	dlg.m_iFunType=m_iFunType;//ͳ�Ʒ���
	dlg.m_dGain=m_dGain;//�Ŵ���
	if(dlg.DoModal()==IDOK)
	{
		strInputArr.Copy(dlg.strInputArr);//�������
		strOutputArr.Copy(dlg.strOutputArr);//�������
		strTagDespArr.Copy(dlg.strTagDespArr);//��������
		strUnitArr.Copy(dlg.strUnitArr);//��λ
		iTimeSelfOrInput=dlg.iTimeSelfOrInput;//��עʱ��Σ�0���ݱ���1�ⲿ����

		m_dLeft=dlg.m_dLeft;//�����󣨱����䣩
		m_dRight=dlg.m_dRight;//�����ң������䣩
		m_iStatType=dlg.m_iStatType;//״ֵ̬
		m_iFunType=dlg.m_iFunType;//ͳ�Ʒ���
		m_dGain=dlg.m_dGain;//�Ŵ���
		//1��������˿ڵ��޸�
		if(iTimeSelfOrInput==1)
		{
			IDArrLeftIn.Add("STime");
			IDArrLeftInType.Add(VALUE_DOUBLE);
			IDArrLeftIn.Add("ETime");
			IDArrLeftInType.Add(VALUE_DOUBLE);
		}
		for(int i=0;i<strInputArr.GetSize();i++)
		{
			IDArrLeftIn.Add(strInputArr[i]);
			IDArrLeftInType.Add(VALUE_RTVALUE_SERIES);
		}
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

		//������˿ڵ��޸�
		for(i=0;i<strOutputArr.GetSize();i++)
		{
			IDArrRightIn.Add(strOutputArr[i]);
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
				InSertOutputPortBefore(mm,str, VALUE_STAT, _T(str));
			}
		}
		parent->RedrawWindow();
	}
}
//�����Խ���ģ��д�뵽���ݿ���
void CStatProEnumBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	for(int i=0;i<strInputArr.GetSize();i++)//���
	{
		CString strInPut=strInputArr[i];
		if(strInPut=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("output.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������

		strFieldValue.Format("%s",strInputArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",strOutputArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",strUnitArr[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",strTagDespArr[i]); //������
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������
	}
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="SelfOrInput";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",iTimeSelfOrInput); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="StatFun";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%f",m_dLeft); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%f",m_dRight); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%i",m_iStatType); //������
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%i",m_iFunType); //������
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%f",m_dGain); //������
	pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //������
	
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CStatProEnumBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->strInputArr.Copy(((CStatProEnumBlock*)pBlock)->strInputArr);
	this->strOutputArr.Copy(((CStatProEnumBlock*)pBlock)->strOutputArr);
	this->strTagDespArr.Copy(((CStatProEnumBlock*)pBlock)->strTagDespArr);
	this->strUnitArr.Copy(((CStatProEnumBlock*)pBlock)->strUnitArr);
	this->iTimeSelfOrInput=((CStatProEnumBlock*)pBlock)->iTimeSelfOrInput;

	this->m_dLeft=((CStatProEnumBlock*)pBlock)->m_dLeft;
	this->m_dRight=((CStatProEnumBlock*)pBlock)->m_dRight;
	this->m_iStatType=((CStatProEnumBlock*)pBlock)->m_iStatType;
	this->m_iFunType=((CStatProEnumBlock*)pBlock)->m_iFunType;
	this->m_dGain=((CStatProEnumBlock*)pBlock)->m_dGain;
}
//�����Է��ô���������
void CStatProEnumBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}
