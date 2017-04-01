// LinkToProBatBlock.cpp: implementation of the CLinkToProBatBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "LinkToProBatBlock.h"
#include "PropertyDlgLinkToProBat.h"
#include "FlowchartEditor/DiagramEditor/Tokenizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CMap<CString,LPCTSTR,CCalcPort*,CCalcPort*&> g_WorkSpace;

CLinkToProBatBlock::CLinkToProBatBlock()
{
	SetBlockType(_T( "LinkToProBat" ) );//��������
	ConstructBlockInputOutput ();
}

CLinkToProBatBlock::~CLinkToProBatBlock()
{
}

//�����ʼ�������������
void CLinkToProBatBlock::ConstructBlockInputOutput()
{
	AddInputPort("Enable",VALUE_DOUBLE);
}
//ͬʱ��CCalcBlockControlFactory�У�Ҫ������Ӧ�Ĵ���
CCalcBlock* CLinkToProBatBlock::CreateFromString( const CString& strType )
{
	CLinkToProBatBlock* obj = new CLinkToProBatBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
}

//��ʼ������ĺ���������ĳ�ʼ��
void CLinkToProBatBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}

CString CLinkToProBatBlock::getRefToWSTagName(int index)
{
	CString csCurInputName = strInputArr[index];
	return csCurInputName;
}

//���㺯����ʵ�ֱ�ģ��ļ���
void CLinkToProBatBlock::Calc() 
{
	//�������˿ڵ�����
	GetInputValueFromGlobalWS();

	//ʹ���ж�
	double inValue=0;
	double fzero = 0.00001;
	CCalcPort *pPortObj = GetInputPortObj(0);
	pPortObj->GetPortUniValue().GetDoubleVal(&inValue);
	if (((1-fzero)<inValue) && (inValue<(1+fzero)))
	{
	int inNum = GetInputNum();
	for(int k=1;k<inNum;k++)
	{
		CCalcPort *pPortObj = GetInputPortObj(k);
		CString strVarName = getRefToWSTagName(k-1);
		CCalcPort *pPortObjFind;
		if(g_WorkSpace.Lookup(strVarName,pPortObjFind))
		{
			g_WorkSpace.RemoveKey(strVarName);
		}
		g_WorkSpace.SetAt(strVarName,pPortObj);
	}
	}
}

//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CLinkToProBatBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	/*if(strPropName.Compare("StrInputCfg")==0)
	{
		if(!strItem1.IsEmpty())  {m_nStatType=atoi(strItem1);}
	}
	else
	{*/
		int propIndex;
		int dIndex=strPropName.ReverseFind('.');
		CString strIndex=strPropName.Right(strPropName.GetLength()-dIndex-1);//������ֵΪ�ַ��������ĸ���
		CString strName=strPropName.Left(dIndex);
	    propIndex=atoi(strIndex);

		if(!strItem1.IsEmpty())  {strInputArr.SetAtGrow(propIndex,strItem1);}
		//if(!strItem2.IsEmpty())  {strOutputArr.SetAtGrow(propIndex,strItem2);}
		//if(!strItem3.IsEmpty())  {strUnitArr.SetAtGrow(propIndex,strItem3);}
		if(!strItem2.IsEmpty())  {strTagDespArr.SetAtGrow(propIndex,strItem2);}
	//}
}

//��������ʾ��ͬ�����Զ���Ի���
void CLinkToProBatBlock::ShowPropertyDialog( CWnd* parent, BOOL show)
{
	//��ʾ����
	CPropertyDlgLinkToProBat dlg;

	//�����������ʱ�������Դ��ڶԽӿ����޸�ʱ��Ԥ����
	int portnumLinkBefore=this->GetInputNum();//����
	//int portnumLinkBehind=this->GetOutputNum();
	//CStringArray  IDArrRightOut;//�����ԭ�е�����˿�����
	//CStringArray  IDArrRightIn;//������ڲ�������˿�����
	
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
	/*for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}*/

	//����������еı���ֵ���µ��������Ӧ�ı���
	dlg.strInputArr.Copy(strInputArr);//�������
	//dlg.strOutputArr.Copy(strOutputArr);//�������
	dlg.strTagDespArr.Copy(strTagDespArr);//��������
	//dlg.strUnitArr.Copy(strUnitArr);//��λ
	//dlg.m_nStatType = m_nStatType;
	dlg.m_nWindowsType = 1;


	//������ҳ�����ݼ��뵱ǰ��������
	if(IDOK==dlg.DoModal())
	{
		//�����Դ����и��µ����������������Ӧ��������
		//m_nStatType = dlg.m_nStatType;
		strInputArr.Copy(dlg.strInputArr);//�������
		//strOutputArr.Copy(dlg.strOutputArr);//�������
		strTagDespArr.Copy(dlg.strTagDespArr);//��������
		//strUnitArr.Copy(dlg.strUnitArr);//��λ

		//1��������˿ڵ��޸�
		IDArrLeftIn.Add("Enable");
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
		/*for(i=0;i<strOutputArr.GetSize();i++)
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
		}*/
		parent->RedrawWindow();
	}

}
//�����Խ���ģ��д�뵽���ݿ���
void CLinkToProBatBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	//Ĭ�Ϲ̶���
	CString strFieldValue;
	CString strField;
	//pRecord->AddNew();
	//pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name,"block_name"Ϊprop���ֶ���
	//strFieldValue="StrInputCfg";//������,ͬ�����ʹ��
	//pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//strFieldValue.Format("%d",m_nStatType); //������
	//pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

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
		//strFieldValue.Format("%s",strOutputArr[i]); //������
		//pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
		//strFieldValue.Format("%s",strUnitArr[i]); //������
		//pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",strTagDespArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������
	}
	
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CLinkToProBatBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	//this->m_nStatType=((CLinkToProBatBlock*)pBlock)->m_nStatType;
}
//�����Է��ô���������
void CLinkToProBatBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	
}
