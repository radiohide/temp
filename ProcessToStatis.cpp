// ProcessToStatisBlock.cpp: implementation of the CProcessToStatisBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "ProcessToStatis.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProcessToStatisBlock::CProcessToStatisBlock()
{
	m_nStatType = 1;//������ʼֵ
	SetBlockType(_T( "ProcessToStatis" ) );
	ConstructBlockInputOutput();
}

CProcessToStatisBlock::~CProcessToStatisBlock()
{

}
//�����ʼ�������������
void CProcessToStatisBlock::ConstructBlockInputOutput()
{
	AddInputPort("StartTime",VALUE_DOUBLE);
	AddInputPort("EndTime",VALUE_DOUBLE);

	//AddInputPort(_T("A1"), VALUE_DOUBLE);
	//AddOutputPort("out",VALUE_STAT,"out");
}
//ͬʱ��CCalcBlockControlFactory�У�Ҫ������Ӧ�Ĵ���
CCalcBlock* CProcessToStatisBlock::CreateFromString( const CString& strType )
{
	CProcessToStatisBlock* obj = new CProcessToStatisBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}

//��ʼ������ĺ���������ĳ�ʼ��
void CProcessToStatisBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}

//���㺯����ʵ�ֱ�ģ��ļ���
void CProcessToStatisBlock::Calc() 
{
	//�������˿ڵ�����
	GetInputValueFromGlobalWS();

	//��ȡ����

	//��ʼ��ֹʱ��ֵ
	int lSTime=0;
	int lETime=0;

	double dInValue=0;
	CCalcPort *pInPortObj1 = GetInputPortObj(0);
	pInPortObj1->GetPortUniValue().GetDoubleVal(&dInValue);
	lSTime=(long)dInValue;

	CCalcPort *pInPortObj2 = GetInputPortObj(1);
	pInPortObj2->GetPortUniValue().GetDoubleVal(&dInValue);
	lETime=(long)dInValue;

	//����ֵ��ȡ
	for(int i=0;i<strInputArr.GetSize();i++)
	{
		double inValue = 0;
		CCalcPort *pPortObj = GetInputPortObj(i+2);
		pPortObj->GetPortUniValue().GetDoubleVal(&inValue);
		
		//ת��
		StatData oneStatData;
		oneStatData.lSTime = lSTime;
		oneStatData.lETime = lETime;
		oneStatData.dValue = inValue;
		oneStatData.dMax = inValue;
		oneStatData.dMin = inValue;
		oneStatData.iCount = 1;
		oneStatData.iType = m_nStatType;
		CTime curTime=CTime::GetCurrentTime();
		oneStatData.lInsertTime=curTime.GetTime();
		
		
		//////////////////////////////////
		//�������
		CUniValue UniVarOut;
		CCalcPort *pPortObjOut = GetOutputPortObj(i);
		UniVarOut.AddStatData(oneStatData);
		pPortObjOut->SetPortUniValue(UniVarOut);
	}

	OutputResultToGlobalWS();
}

//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CProcessToStatisBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("StrInputCfg")==0)
	{
		if(!strItem1.IsEmpty())  {m_nStatType=atoi(strItem1);}
	}
	else
	{
		int propIndex;
		int dIndex=strPropName.ReverseFind('.');
		CString strIndex=strPropName.Right(strPropName.GetLength()-dIndex-1);//������ֵΪ�ַ��������ĸ���
		CString strName=strPropName.Left(dIndex);
	    propIndex=atoi(strIndex);

		if(!strItem1.IsEmpty())  {strInputArr.SetAtGrow(propIndex,strItem1);}
		if(!strItem2.IsEmpty())  {strOutputArr.SetAtGrow(propIndex,strItem2);}
		if(!strItem3.IsEmpty())  {strUnitArr.SetAtGrow(propIndex,strItem3);}
		if(!strItem4.IsEmpty())  {strTagDespArr.SetAtGrow(propIndex,strItem4);}
	}
}

//��������ʾ��ͬ�����Զ���Ի���
void CProcessToStatisBlock::ShowPropertyDialog( CWnd* parent, BOOL show)
{
	//��ʾ����
	CPropertyDlgProcToStatis dlg;

	//�����������ʱ�������Դ��ڶԽӿ����޸�ʱ��Ԥ����
	int portnumLinkBefore=this->GetInputNum();//����
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//�����ԭ�е�����˿�����
	CStringArray  IDArrRightIn;//������ڲ�������˿�����
	
	CStringArray  IDArrLeftOut;//�����ԭ�е�����˿�����
	CArray<int,int> IDArrLeftOutType;//�����ԭ�е�����˿�����
	CStringArray  IDArrLeftIn;//������ڲ�������˿�����
	CArray<int,int>  IDArrLeftInType;//������ڲ�������˿�����
	
	//���ʱ�������˿���
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		int iPortType=this->GetInputPortObj(q)->GetPortValueType();
		IDArrLeftOut.Add(strPortID);
		IDArrLeftOutType.Add(iPortType);
	}
	
	//���ʱ�������˿���
	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}

	//����������еı���ֵ���µ��������Ӧ�ı���
	dlg.strInputArr.Copy(strInputArr);//�������
	dlg.strOutputArr.Copy(strOutputArr);//�������
	dlg.strTagDespArr.Copy(strTagDespArr);//��������
	dlg.strUnitArr.Copy(strUnitArr);//��λ
	dlg.m_nStatType = m_nStatType;


	//������ҳ�����ݼ��뵱ǰ��������
	if(IDOK==dlg.DoModal())
	{
		//�����Դ����и��µ����������������Ӧ��������
		m_nStatType = dlg.m_nStatType;
		strInputArr.Copy(dlg.strInputArr);//�������
		strOutputArr.Copy(dlg.strOutputArr);//�������
		strTagDespArr.Copy(dlg.strTagDespArr);//��������
		strUnitArr.Copy(dlg.strUnitArr);//��λ

		//1��������˿ڵ��޸�
		IDArrLeftIn.Add("STime");
		IDArrLeftInType.Add(VALUE_DOUBLE);
		IDArrLeftIn.Add("ETime");
		IDArrLeftInType.Add(VALUE_DOUBLE);

		for(int i=0;i<strInputArr.GetSize();i++)
		{
			IDArrLeftIn.Add(strInputArr[i]);
			IDArrLeftInType.Add(VALUE_DOUBLE);//VALUE_RTVALUE_SERIES
		}
		
		//�������������ɾ�����Դ�����ɾ������
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
		//��������������������Դ��������ӵ���
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
void CProcessToStatisBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	//Ĭ�Ϲ̶���
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name,"block_name"Ϊprop���ֶ���
	strFieldValue="StrInputCfg";//������,ͬ�����ʹ��
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_nStatType); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	//��̬������
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
	
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CProcessToStatisBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_nStatType=((CProcessToStatisBlock*)pBlock)->m_nStatType;
}
//�����Է��ô���������
void CProcessToStatisBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	
}