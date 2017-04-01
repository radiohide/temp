// LinkFromProBat.cpp: implementation of the CLinkFromProBat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "LinkFromProBat.h"
#include "PropertyDlgLinkFromProBat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


extern CMap<CString,LPCTSTR,CCalcPort*,CCalcPort*&> g_WorkSpace;

CLinkFromProBat::CLinkFromProBat()
{
	SetBlockType(_T( "LinkFromProBat" ) );//��������
	ConstructBlockInputOutput ();
}

CLinkFromProBat::~CLinkFromProBat()
{

}

//�����ʼ�������������
void CLinkFromProBat::ConstructBlockInputOutput()
{
}
//ͬʱ��CCalcBlockControlFactory�У�Ҫ������Ӧ�Ĵ���
CCalcBlock* CLinkFromProBat::CreateFromString( const CString& strType )
{
	CLinkFromProBat* obj = new CLinkFromProBat;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}

//��ʼ������ĺ���������ĳ�ʼ��
void CLinkFromProBat::InitCalc()
{
	CCalcBlock::InitCalc();
}

//���㺯����ʵ�ֱ�ģ��ļ���
void CLinkFromProBat::Calc() 
{
	//�������˿ڵ�����
	GetInputValueFromGlobalWS();

	//�������
	int outNum = GetOutputNum();
	for(int k=0;k<outNum;k++)
	{
		CCalcPort *pPortObj = GetOutputPortObj(k);
		CCalcPort *pPortInObj = getRefFromWSByID(pPortObj->GetPortID());
		if(pPortInObj!=NULL)
			pPortObj->SetPortUniValue(pPortInObj->GetPortUniValue());
	}

	//��ȡ��
	OutputResultToGlobalWS();
}

CCalcPort* CLinkFromProBat::getRefFromWSByID(CString csID)
{
	CString str;
	CCalcPort *pPortObj=NULL;
	
	if(g_WorkSpace.Lookup(csID,pPortObj))
	{}
	return pPortObj;
}

//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CLinkFromProBat::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString 

strItem4,CString strItem5)
{
		int propIndex;
		int dIndex=strPropName.ReverseFind('.');
		CString strIndex=strPropName.Right(strPropName.GetLength()-dIndex-1);//������ֵΪ�ַ��������ĸ���
		CString strName=strPropName.Left(dIndex);
	    propIndex=atoi(strIndex);

		//if(!strItem1.IsEmpty())  {strInputArr.SetAtGrow(propIndex,strItem1);}
		if(!strItem1.IsEmpty())  {strOutputArr.SetAtGrow(propIndex,strItem1);}
		//if(!strItem3.IsEmpty())  {strUnitArr.SetAtGrow(propIndex,strItem3);}
		if(!strItem2.IsEmpty())  {strTagDespArr.SetAtGrow(propIndex,strItem2);}
}

//��������ʾ��ͬ�����Զ���Ի���
void CLinkFromProBat::ShowPropertyDialog( CWnd* parent, BOOL show)
{
	//��ʾ����
	CPropertyDlgLinkFromProBat dlg;

	//�����������ʱ�������Դ��ڶԽӿ����޸�ʱ��Ԥ����
	//int portnumLinkBefore=this->GetInputNum();//����
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//�����ԭ�е�����˿�����
	CStringArray  IDArrRightIn;//������ڲ�������˿�����
	
	//CStringArray  IDArrLeftOut;//�����ԭ�е�����˿�����
	//CArray<int,int> IDArrLeftOutType;//�����ԭ�е�����˿�����
	//CStringArray  IDArrLeftIn;//������ڲ�������˿�����
	//CArray<int,int>  IDArrLeftInType;//������ڲ�������˿�����
	
	//���ʱ�������˿���
	/*for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		int iPortType=this->GetInputPortObj(q)->GetPortValueType();
		IDArrLeftOut.Add(strPortID);
		IDArrLeftOutType.Add(iPortType);
	}*/
	
	//���ʱ�������˿���
	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}

	//����������еı���ֵ���µ��������Ӧ�ı���
	//dlg.strInputArr.Copy(strInputArr);//�������
	dlg.strOutputArr.Copy(strOutputArr);//�������
	dlg.strTagDespArr.Copy(strTagDespArr);//��������
	//dlg.strUnitArr.Copy(strUnitArr);//��λ
	//dlg.m_nStatType = m_nStatType;
	dlg.m_nWindowsType = 1;


	//������ҳ�����ݼ��뵱ǰ��������
	if(IDOK==dlg.DoModal())
	{
		//�����Դ����и��µ����������������Ӧ��������
		//m_nStatType = dlg.m_nStatType;
		//strInputArr.Copy(dlg.strInputArr);//�������
		strOutputArr.Copy(dlg.strOutputArr);//�������
		strTagDespArr.Copy(dlg.strTagDespArr);//��������
		//strUnitArr.Copy(dlg.strUnitArr);//��λ


		/*for(int i=0;i<strInputArr.GetSize();i++)
		{
			IDArrLeftIn.Add(strInputArr[i]);
			IDArrLeftInType.Add(VALUE_DOUBLE);//VALUE_RTVALUE_SERIES
		}*/
		
		//�������������ɾ�����Դ�����ɾ������
		/*for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)	//ɾ��ԭ�ж����õ�Port,����ɾ
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
		}*/
		
		//������˿ڵ��޸�
		for(int i=0;i<strOutputArr.GetSize();i++)
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
void CLinkFromProBat::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	//Ĭ�Ϲ̶���
	CString strFieldValue;
	CString strField;
	/*pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name,"block_name"Ϊprop���ֶ���
	strFieldValue="StrInputCfg";//������,ͬ�����ʹ��
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_nStatType); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	*/

	//��̬������
	for(int i=0;i<strOutputArr.GetSize();i++)//���
	{
		CString strOutPut=strOutputArr[i];
		if(strOutPut=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("output.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
		
		//strFieldValue.Format("%s",strInputArr[i]); //������
		//pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",strOutputArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		//strFieldValue.Format("%s",strUnitArr[i]); //������
		//pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",strTagDespArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������
	}
	
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CLinkFromProBat::CopyPropertyData(CCalcBlock* pBlock)
{
	//this->m_nStatType=((CProcessToStatisBlock*)pBlock)->m_nStatType;
}
//�����Է��ô���������
void CLinkFromProBat::SetBlockPropertyToTransferData(CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	
}
