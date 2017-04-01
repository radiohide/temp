// LogicalBlock.cpp: implementation of the CLogicalBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "LogicalBlock.h"
#include "PropertyDlgMathFun.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogicalBlock::CLogicalBlock()
{
	SetBlockType(_T( "Logical" ) );//��������
	m_strFuncName="&&";
	m_strBlockDisp="�߼�����顣��ѡ���߼����㺯�����м��㡣";
	m_strPropertityTableName="�߼������";
	m_iInputCount=2;
	
	m_strArrFunName.Add("&&");m_strArrFunDisp.Add("��");
	m_strArrFunName.Add("||");m_strArrFunDisp.Add("��");	
	m_strArrFunName.Add("!");m_strArrFunDisp.Add("��");
	m_strArrFunName.Add("~&");m_strArrFunDisp.Add("����");
	m_strArrFunName.Add("~|");m_strArrFunDisp.Add("�ǻ�");
	m_strArrFunName.Add("^");m_strArrFunDisp.Add("���");
	ConstructBlockInputOutput();
}

CLogicalBlock::~CLogicalBlock()
{
	
}

//���������½�����ĺ���
CCalcBlock* CLogicalBlock::CreateFromString( const CString& strType )
{
	CLogicalBlock* obj = new CLogicalBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CLogicalBlock::ConstructBlockInputOutput ()
{
	ClearInputPorts();
	ClearOutputPorts();
	CString str;
	for(UINT i=0;i<m_iInputCount;i++)
	{
		str.Format("In%d",i+1);
		AddInputPort(str, VALUE_DOUBLE);
	}
	AddOutputPort("Out", VALUE_DOUBLE);
}
//��ʼ������ĺ���������ĳ�ʼ��
void CLogicalBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CLogicalBlock::Calc()//���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	int result=0;
	double inValue;
	if(m_strFuncName=="&&")
	{
		result=1;
		for(UINT i=0;i<m_iInputCount;i++)
		{
			CCalcPort *pInputObj=GetInputPortObj(i);
			pInputObj->GetPortUniValue().GetDoubleVal(&inValue);
			result=result&&inValue;
		}
	}
	else if(m_strFuncName=="||")
	{
		result=0;
		for(UINT i=0;i<m_iInputCount;i++)
		{
			CCalcPort *pInputObj=GetInputPortObj(i);
			pInputObj->GetPortUniValue().GetDoubleVal(&inValue);
			result=result||inValue;
		}
	}
	else if(m_strFuncName=="!")
	{
		CCalcPort *pInputObj=GetInputPortObj(0);
		pInputObj->GetPortUniValue().GetDoubleVal(&inValue);
		result=!inValue;
	}
	else if(m_strFuncName=="~&")
	{
		result=1;
		for(UINT i=0;i<m_iInputCount;i++)
		{
			CCalcPort *pInputObj=GetInputPortObj(i);
			pInputObj->GetPortUniValue().GetDoubleVal(&inValue);
			result=result&&inValue;
		}
		result=!result;
	}
	else if(m_strFuncName=="~|")
	{
		result=0;
		for(UINT i=0;i<m_iInputCount;i++)
		{
			CCalcPort *pInputObj=GetInputPortObj(i);
			pInputObj->GetPortUniValue().GetDoubleVal(&inValue);
			result=result||inValue;
		}
		result=!result;
	}
	else if(m_strFuncName=="^")
	{
		for(UINT i=0;i<m_iInputCount;i++)
		{
			CCalcPort *pInputObj=GetInputPortObj(i);
			pInputObj->GetPortUniValue().GetDoubleVal(&inValue);
			if(i==0)
			{
				result=inValue>0;
			}
			else
				result=(result>0)!=(inValue>0);
		}
	}
	
	CUniValue UniVar;
	UniVar.AddData(result);
	pOutPortObj->SetPortUniValue(UniVar);
	//����������ռ���
	OutputResultToGlobalWS();
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CLogicalBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("strFuncName")==0)
	{
		if(!strItem1.IsEmpty())  m_strFuncName=strItem1;
	}
	if(strPropName.Compare("InputCount")==0)
	{
		if(!strItem1.IsEmpty())  m_iInputCount=atoi(strItem1);
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CLogicalBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBefore=this->GetInputNum();//����
	CStringArray  IDArrLeftOut;//�����ԭ�е�����˿�����
	IDArrLeftOut.RemoveAll();
	
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		IDArrLeftOut.Add(strPortID);
	}

	CPropertyDlgMathFun dlg;
	dlg.m_pStrArrFunDisp=&m_strArrFunDisp;
	dlg.m_pStrArrFunName=&m_strArrFunName;
	dlg.m_strBlockDesp=this->m_strBlockDisp;
	dlg.m_strFunName=this->m_strFuncName;
	dlg.m_strPropertityTableName=m_strPropertityTableName;
	dlg.m_iInputCount=m_iInputCount;
	if(dlg.DoModal()==IDOK)
	{
		m_iInputCount=dlg.m_iInputCount;
		m_strFuncName=dlg.m_strFunName;
		if(m_strFuncName=="!")
			m_iInputCount=1;
	

		CStringArray  IDArrLeftIn;//������ڲ������Ըı䣬��ʹ�ⲿҪ���µ�����˿�����
		IDArrLeftIn.RemoveAll();		

		CString str;
		for(UINT i=0;i<m_iInputCount;i++)
		{
			str.Format("In%d",i+1);
			IDArrLeftIn.Add(str);
			//	AddInputPort(str, VALUE_DOUBLE);
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
void CLogicalBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	//����1�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="strFuncName";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strFuncName); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="InputCount";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iInputCount); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	pRecord->Update();
}

//����ͼ��ʱ����Ҫ�������ԡ�
void CLogicalBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strFuncName=((CLogicalBlock*)pBlock)->m_strFuncName;
	m_iInputCount=((CLogicalBlock*)pBlock)->m_iInputCount;
}
//�����Է��ô���������
void CLogicalBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"strFuncName");
	
	CString str;
	str.Format("%s",m_strFuncName);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"InputCount");
	
	str.Format("%d",m_iInputCount);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
