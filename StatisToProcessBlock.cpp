// StatisToProcessBlock.cpp: implementation of the CStatisToProcessBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatisToProcessBlock.h"
#include "PropertyDlgStatToProc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatisToProcessBlock::CStatisToProcessBlock()
{
	iTimeSelect = 1;
	m_nProcType = 0;
	SetBlockType(_T( "StatisToProcess" ) );
	ConstructBlockInputOutput();
}

CStatisToProcessBlock::~CStatisToProcessBlock()
{

}

//�����ʼ�������������
void CStatisToProcessBlock::ConstructBlockInputOutput()
{
	//AddInputPort(_T("A1"), VALUE_STAT);
	//AddOutputPort("out",VALUE_DOUBLE,"out");
}
//ͬʱ��CCalcBlockControlFactory�У�Ҫ������Ӧ�Ĵ���
CCalcBlock* CStatisToProcessBlock::CreateFromString( const CString& strType )
{
	CStatisToProcessBlock* obj = new CStatisToProcessBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}

//��ʼ������ĺ���������ĳ�ʼ��
void CStatisToProcessBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}

//���㺯����ʵ�ֱ�ģ��ļ���
void CStatisToProcessBlock::Calc() 
{
	//�������˿ڵ�����
	GetInputValueFromGlobalWS();

	//��ȡ����ֵ
	for(int i=0;i<strInputArr.GetSize();i++)
	{
	StatData statData;
	CCalcPort *pPortObj = GetInputPortObj(i);
	pPortObj->GetPortUniValue().GetStatData(&statData);
	long lCurTime = 0; 
	switch(iTimeSelect)
	{
	case 0:
		{
			lCurTime = statData.lSTime;
		}
		break;
		
	case 1:
		{
			lCurTime = statData.lETime;
		}
		break;

	case 2:
		{
			lCurTime = statData.lInsertTime;
		}
		break;

	default:
		break;
	}

	//ת��
	CUniValue UniVar;
	UniVar.AddData(statData.dValue,lCurTime,m_nProcType);

	//���
    CCalcPort *pOutPortObj = GetOutputPortObj(i);
	pOutPortObj->SetPortUniValue(UniVar);
	}

	//////////////////////////////////
	//�������
	OutputResultToGlobalWS();
}

//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CStatisToProcessBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("StrInputCfg")==0)
	{
		if(!strItem1.IsEmpty())  {m_nProcType=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {iTimeSelect=atoi(strItem2);}
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
void CStatisToProcessBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStatToProc dlg;

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

	//��������
	dlg.m_nProcType = m_nProcType;
	dlg.iTimeSelect = iTimeSelect;
	dlg.strInputArr.Copy(strInputArr);//�������
	dlg.strOutputArr.Copy(strOutputArr);//�������
	dlg.strTagDespArr.Copy(strTagDespArr);//��������
	dlg.strUnitArr.Copy(strUnitArr);//��λ

	if(IDOK==dlg.DoModal())
	{
		//�����Դ������õ�ֵ������ǰ��Ӧ������Ϊ����׼��
		m_nProcType = dlg.m_nProcType;
		iTimeSelect = dlg.iTimeSelect;
		strInputArr.Copy(dlg.strInputArr);//�������
		strOutputArr.Copy(dlg.strOutputArr);//�������
		strTagDespArr.Copy(dlg.strTagDespArr);//��������
		strUnitArr.Copy(dlg.strUnitArr);//��λ

		//1��������˿ڵ��޸�		
		for(int i=0;i<strInputArr.GetSize();i++)
		{
			IDArrLeftIn.Add(strInputArr[i]);
			IDArrLeftInType.Add(VALUE_STAT);//VALUE_RTVALUE_SERIES
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
				InSertOutputPortBefore(mm,str, VALUE_DOUBLE, _T(str));
			}
		}
		parent->RedrawWindow();
	}
}
//�����Խ���ģ��д�뵽���ݿ���
void CStatisToProcessBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	//Ĭ�Ϲ̶���
	CString strFieldValue;
	CString strFieldValue1;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name,"block_name"Ϊprop���ֶ���
	strFieldValue="StrInputCfg";//������,ͬ�����ʹ��
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_nProcType); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue1.Format("%d",iTimeSelect); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue1) ); //������
	
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
void CStatisToProcessBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->strInputArr.Copy(((CStatisToProcessBlock*)pBlock)->strInputArr);
	this->strOutputArr.Copy(((CStatisToProcessBlock*)pBlock)->strOutputArr);
	this->strTagDespArr.Copy(((CStatisToProcessBlock*)pBlock)->strTagDespArr);
	this->strUnitArr.Copy(((CStatisToProcessBlock*)pBlock)->strUnitArr);

	this->m_nProcType=((CStatisToProcessBlock*)pBlock)->m_nProcType;
	this->iTimeSelect=((CStatisToProcessBlock*)pBlock)->iTimeSelect;
}

//�����Է��ô���������
void CStatisToProcessBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	
}
