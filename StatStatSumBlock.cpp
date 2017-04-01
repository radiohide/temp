// StatStatSumBlock.cpp: implementation of the CStatStatSumBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatStatSumBlock.h"
#include "PropertyDlgStatProAvg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatStatSumBlock::CStatStatSumBlock(int iTimeSelfOrInput):CStatProAvgBlock(iTimeSelfOrInput)
{
	SetBlockType(_T( "StatStatSum" ) );
	this->iTimeSelfOrInput=iTimeSelfOrInput;//Ĭ�ϲ�ȡ�ⲿʱ��
}

CStatStatSumBlock::~CStatStatSumBlock()
{

}
//���������½�����ĺ���
CCalcBlock* CStatStatSumBlock::CreateFromString( const CString& strType )
{
	CStatStatSumBlock* obj = new CStatStatSumBlock(0);
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CStatStatSumBlock::Calc()//���м���ĺ���,��Ҫ����
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
	for(int i=0;i<strInputArr.GetSize();i++)
	{
		CCalcPort *pPortObjIn3 = GetInputPortObj(iInputDataStart+i);	
		CUniValue  &CUV=pPortObjIn3->GetPortUniValue();
		
		double dSum=0;
		double dMin=0;
		double dMax=0;	
		long lRealSTime=0;
		long lRealETime=0;
		int iCount=0; 
		statData(CUV,dSum,dMin,dMax,lRealSTime,lRealETime,iCount);
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
		oneStatData.dValue=dSum;
		oneStatData.iType=0;
		oneStatData.iCount=iCount;

		CUniValue UniVarOut;
		CCalcPort *pPortObjOut = GetOutputPortObj(i);
		UniVarOut.AddStatData(oneStatData);
		pPortObjOut->SetPortUniValue(UniVarOut);
	}
	//�������
	OutputResultToGlobalWS();
}
void CStatStatSumBlock::statData(CUniValue  &CUV,
							 double &dSum,
							 double &dMin,
							 double &dMax,
							 long &lRealSTime,
							 long &lRealETime,
							 int &iCount)
{
	dSum=0;
	dMin=0;
	dMax=0;
	lRealSTime=0;
	lRealETime=0;
	iCount=CUV.GetStatDataSize();
	for(int j=0; j<CUV.GetStatDataSize();j++)
	{
		double value=0;
		StatData statData;
		CUV.GetStatDataAt(j,&statData);
		if(j==0)
		{
			dMin=statData.dMin;
			dMax=statData.dMax;
			lRealSTime=statData.lSTime;
			lRealETime=statData.lETime;
		}
		else
		{
			if(statData.dMin<dMin)
			{
				dMin=statData.dMin;
			}
			if(statData.dMax>dMax)
			{
				dMax=statData.dMax;
			}
		}
		dSum+=statData.dValue;
		if(j+1==CUV.GetStatDataSize())
		{
			lRealETime=statData.lETime;
		}
	}
}
void CStatStatSumBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStatProAvg dlg;
	dlg.m_iProOrStatOrSum=2;

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
	if(dlg.DoModal()==IDOK)
	{
		strInputArr.Copy(dlg.strInputArr);//�������
		strOutputArr.Copy(dlg.strOutputArr);//�������
		strTagDespArr.Copy(dlg.strTagDespArr);//��������
		strUnitArr.Copy(dlg.strUnitArr);//��λ
		iTimeSelfOrInput=dlg.iTimeSelfOrInput;//��עʱ��Σ�0���ݱ���1�ⲿ����


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
			IDArrLeftInType.Add(VALUE_STAT_SERIES);
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
