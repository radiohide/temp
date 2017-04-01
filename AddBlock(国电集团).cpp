// AddBlock.cpp: implementation of the CAddBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "AddBlock.h"
#include "PropertyDlgAdd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAddBlock::CAddBlock()
{
	m_strInputEdit="++";
	SetBlockType(_T( "Add" ) );
	ConstructBlockInputOutput();
}

CAddBlock::~CAddBlock()
{
	
}
//���������½�����ĺ���
CCalcBlock* CAddBlock::CreateFromString( const CString& strType )
{
	CAddBlock* obj = new CAddBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CAddBlock::ConstructBlockInputOutput ()
{
//	for (int i=0;i<150;i++)
	{
		AddInputPort(_T("A1"), VALUE_DOUBLE);
	    AddInputPort(_T("A2"), VALUE_DOUBLE);
	}


	AddOutputPort("out",VALUE_DOUBLE,"out");
}
//��ʼ������ĺ���������ĳ�ʼ��
void CAddBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CAddBlock::Calc()//���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	int inNum = GetInputNum();
	double sum=0;
	double inValue=0;
	long inTime=0;
	short inState=0;

	long lResultTime=0;
	short lResultState=0;

	for(int k=inNum-1;k>=0;k--)
	{
		CCalcPort *pPortObj = GetInputPortObj(k);
		pPortObj->GetPortUniValue().GetDoubleVal(&inValue);
		pPortObj->GetPortUniValue().GetTimeVal(&inTime);
		pPortObj->GetPortUniValue().GetStatusVal(&inState);
		if(inState<0)
			continue;

		if('+'==m_strInputEdit.GetAt(k))
		{
			sum+=inValue;
		}
		else if('-'==m_strInputEdit.GetAt(k))
		{
			sum-=inValue;
		}
		if(k==inNum-1)
		{
			lResultTime=inTime;//ȡ��һ����Чʱ��
		    lResultState=inState;//ȡ��һ����Ч״̬
		}
	}
	CUniValue UniVar;
	UniVar.AddData(sum,lResultTime,lResultState);
	pOutPortObj->SetPortUniValue(UniVar);
	OutputResultToGlobalWS();
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CAddBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("StrInputCfg")==0)
	{
		if(!strItem1.IsEmpty())  {m_strInputEdit=strItem1;}
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CAddBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgAdd dlg;
    //���Ի��������ֵ
	dlg.m_strInputEdit=m_strInputEdit;
    
	int portnumLinkBefore=this->GetInputNum();//����
	CStringArray  IDArrLeftOut;//�����ԭ�е�����˿�����
    IDArrLeftOut.RemoveAll();

    for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		IDArrLeftOut.Add(strPortID);
	}
	if(dlg.DoModal()==IDOK)
	{
		m_strInputEdit=dlg.m_strInputEdit;

		buildInputPort(IDArrLeftOut);
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
void CAddBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="StrInputCfg";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strInputEdit); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	pRecord->Update();
}

void CAddBlock::buildInputPort(CStringArray &IDArrLeftOut)
{
	CStringArray  IDArrLeftIn;//������ڲ������Ըı䣬��ʹ�ⲿҪ���µ�����˿�����
	IDArrLeftIn.RemoveAll();

	CString str;
	int aCount=0;
	int dCount=0;
	for(int i=0;i<m_strInputEdit.GetLength();i++)
	{
		if('+'==m_strInputEdit.GetAt(i))
		{
			str.Format("A%d",++aCount);
			IDArrLeftIn.Add(str);
		}
		else if('-'==m_strInputEdit.GetAt(i))
		{
			str.Format("S%d",++dCount);
			IDArrLeftIn.Add(str);			
		}
	}
    //ɾ��ԭ�ж����õ�Port,����ɾ
	for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)
	{
		int  runs=0;//��������
		for(int k=0; k<IDArrLeftIn.GetSize(); k++)
		{
			if(IDArrLeftIn[k]!=IDArrLeftOut[j])	
			{runs++;}
		}
		if(runs==IDArrLeftIn.GetSize())
		{
			this->ClearInputPortByIndex(j);
		}
	}
	//����µ�Port
	for(int m=0; m<IDArrLeftIn.GetSize(); m++)
	{
        int  runs=0;//��������
		for(int n=0; n<IDArrLeftOut.GetSize(); n++)
		{
			if(IDArrLeftIn[m]!=IDArrLeftOut[n])
			{runs++;}		
		}
		if(runs==IDArrLeftOut.GetSize())
		{
			str=IDArrLeftIn[m];
			InSertInputPortBefore(m,str, VALUE_DOUBLE, _T(str));
		}
	}
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CAddBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_strInputEdit=((CAddBlock*)pBlock)->m_strInputEdit;
}
//�����Է��ô���������
void CAddBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"StrInputCfg");
	
	CString str;
	str.Format("%s",m_strInputEdit);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
