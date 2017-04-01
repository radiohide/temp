// StatAddBlock.cpp: implementation of the CStatAddBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatAddBlock.h"
#include "PropertyDlgStatAdd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatAddBlock::CStatAddBlock()
{
	m_strInputEdit="++";
	SetBlockType(_T( "StatAdd" ) );
	ConstructBlockInputOutput();
}

CStatAddBlock::~CStatAddBlock()
{

}
//���������½�����ĺ���
CCalcBlock* CStatAddBlock::CreateFromString( const CString& strType )
{
	CStatAddBlock* obj = new CStatAddBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CStatAddBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("A1"), VALUE_STAT);
	AddInputPort(_T("A2"), VALUE_STAT);

	AddOutputPort("out",VALUE_STAT,"out");
}
//��ʼ������ĺ���������ĳ�ʼ��
void CStatAddBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CStatAddBlock::Calc()//���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();

    //�������
	int inNum = GetInputNum();
	double sum=0;
	StatData oneStatData;
	for(int k=0;k<inNum;k++)
	{
		StatData statData;
		CCalcPort *pPortObj = GetInputPortObj(k);
		pPortObj->GetPortUniValue().GetStatData(&statData);
		if(k==0)
		{
			oneStatData=statData;
		}

		if('+'==m_strInputEdit.GetAt(k))
		{
			sum+=statData.dValue;
		}
		else if('-'==m_strInputEdit.GetAt(k))
		{
			sum-=statData.dValue;
		}
	}

	oneStatData.dValue=sum;
	CUniValue UniVarOut;

	CCalcPort *pPortObjOut = GetOutputPortObj(0);
	UniVarOut.AddStatData(oneStatData);
	pPortObjOut->SetPortUniValue(UniVarOut);
	OutputResultToGlobalWS();
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CStatAddBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("StrInputCfg")==0)
	{
		if(!strItem1.IsEmpty())  {m_strInputEdit=strItem1;}
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CStatAddBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStatAdd dlg;
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
	}
}
//�����Խ���ģ��д�뵽���ݿ���
void CStatAddBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
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

void CStatAddBlock::buildInputPort(CStringArray &IDArrLeftOut)
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
			InSertInputPortBefore(m,str, VALUE_STAT, _T(str));
		}
	}
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CStatAddBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_strInputEdit=((CStatAddBlock*)pBlock)->m_strInputEdit;
}
//�����Է��ô���������
void CStatAddBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}
