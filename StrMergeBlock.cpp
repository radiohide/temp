// StrMergeBlock.cpp: implementation of the CStrMergeBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StrMergeBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStrMergeBlock::CStrMergeBlock()
{
	m_strInputNum=0;

	//SYSTME=====��������======
	//��������
	SetBlockType(_T( "StrMerge" ) );
	ConstructBlockInputOutput();
	//SYSTME=====��ʼ����ģ�������===

}

CStrMergeBlock::~CStrMergeBlock()
{

}
//�����ʼ�������������
void CStrMergeBlock::ConstructBlockInputOutput()
{	
	AddOutputPort("Out",VALUE_RTVALUE_SERIES, _T("out1"), _T("out1"));	
}
//���������½�����ĺ���
//ͬʱ��CCalcBlockControlFactory�У�Ҫ������Ӧ�Ĵ���
CCalcBlock* CStrMergeBlock::CreateFromString( const CString& strType )
{
	CStrMergeBlock* obj = new CStrMergeBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CStrMergeBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CStrMergeBlock::Calc() 
{
	//�������˿ڵ�����
	GetInputValueFromGlobalWS();
	//
	CUniValue UniVar;//�������
	UniVar.SetValueType(1);//����ΪVALUE_RTVALUE_SERIES����
	//���м���
	int inNum = GetInputNum();
    for(int i=0; i<inNum; i++)
	{
		CCalcPort *pPortObjIn = GetInputPortObj(i);		
		
		CUniValue  &CUV=pPortObjIn->GetPortUniValue();
		for(int i=0; i<CUV.GetDataSize();i++)
		{
			double value;
			CUV.GetDoubleValAt(i,&value);
	        UniVar.AddData(value);	
		}
	}
	///////////////////////////////////
	CCalcPort *pPortObjOut = GetOutputPortObj(0);
	pPortObjOut->SetPortUniValue(UniVar);
	//�������
	OutputResultToGlobalWS();

}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CStrMergeBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("StrMerge")==0)
	{
		if(!strItem1.IsEmpty())
			m_strInputNum=atoi(strItem1);	
	}	
}
//��������ʾ��ͬ�����Զ���Ի���
void CStrMergeBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBefore=this->GetInputNum();//D???
	CStringArray  IDArrLeftOut;//?????��?-��D��?��?��????��?����?
	IDArrLeftOut.RemoveAll();
	
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		IDArrLeftOut.Add(strPortID);
	}
	
	CPropertyDlgStrMerge  dlg;
	dlg.m_strInputNum=m_strInputNum;
	if(dlg.DoModal()==IDOK)
	{
		CStringArray  IDArrLeftIn;//������ڲ������Ըı䣬��ʹ�ⲿҪ���µ�����˿�����
		IDArrLeftIn.RemoveAll();
		
		m_strInputNum=dlg.m_strInputNum;
		for(int i=0; i<m_strInputNum; i++)
		{
			CString str;
			str.Format("In%d",i);
			IDArrLeftIn.Add(str);
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
				CString str=IDArrLeftIn[m];
				InSertInputPortBefore(m,str, VALUE_RTVALUE_SERIES, _T(str));
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
void CStrMergeBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	//����1-4�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("StrMerge"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
	strFieldValue.Format("%d",m_strInputNum);
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(strFieldValue) );

	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CStrMergeBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strInputNum=((CStrMergeBlock*)pBlock)->m_strInputNum;
}
//�����Է��ô���������
void CStrMergeBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

	CString str;
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"StrMerge");
	str.Format("%f",m_strInputNum);strcpy(pData->m_cValueItem1,str);

	dataArr.Add(pData);

}