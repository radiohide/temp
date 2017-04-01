// RelationalBlock.cpp: implementation of the CRelationalBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RelationalBlock.h"
#include "PropertyDlgMathFun.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRelationalBlock::CRelationalBlock()
{
	SetBlockType(_T( "Relational" ) );//��������
	m_strFuncName=">";
	m_strBlockDisp="��ϵ����顣��ѡ���ϵ���㺯�����м��㡣";
	m_strPropertityTableName="��ϵ�����";
	
	m_strArrFunName.Add(">");m_strArrFunDisp.Add("����");
	m_strArrFunName.Add(">=");m_strArrFunDisp.Add("���ڵ���");	
	m_strArrFunName.Add("<");m_strArrFunDisp.Add("С��");
	m_strArrFunName.Add("<=");m_strArrFunDisp.Add("С�ڵ���");
	m_strArrFunName.Add("==");m_strArrFunDisp.Add("����");
	m_strArrFunName.Add("!=");m_strArrFunDisp.Add("������");
	ConstructBlockInputOutput();
}

CRelationalBlock::~CRelationalBlock()
{

}
//���������½�����ĺ���
CCalcBlock* CRelationalBlock::CreateFromString( const CString& strType )
{
	CRelationalBlock* obj = new CRelationalBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CRelationalBlock::ConstructBlockInputOutput ()
{
	AddInputPort("In1", VALUE_DOUBLE);
	AddInputPort("In2", VALUE_DOUBLE);
	AddOutputPort("Out", VALUE_DOUBLE);
}
//��ʼ������ĺ���������ĳ�ʼ��
void CRelationalBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CRelationalBlock::Calc()//���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	CCalcPort *pInput1Obj=GetInputPortObj(0);
	CCalcPort *pInput2Obj=GetInputPortObj(1);
	
	double inValue1,inValue2;
	pInput1Obj->GetPortUniValue().GetDoubleVal(&inValue1);
	pInput2Obj->GetPortUniValue().GetDoubleVal(&inValue2);
	
	CUniValue UniVar;
	if(m_strFuncName==">")
	{
		UniVar.AddData(inValue1>inValue2);
	}
	else if(m_strFuncName==">=")
	{
		UniVar.AddData(inValue1>=inValue2);
	}
	else if(m_strFuncName=="<")
	{
		UniVar.AddData(inValue1<inValue2);
	}
	else if(m_strFuncName=="<=")
	{
		UniVar.AddData(inValue1<=inValue2);
	}
	else if(m_strFuncName=="==")
	{
		UniVar.AddData(inValue1==inValue2);
	}
	else if(m_strFuncName=="!=")
	{
		UniVar.AddData(inValue1!=inValue2);
	}
	pOutPortObj->SetPortUniValue(UniVar);
	//����������ռ���
	OutputResultToGlobalWS();
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CRelationalBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("strFuncName")==0)
	{
		if(!strItem1.IsEmpty())  m_strFuncName=strItem1;
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CRelationalBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgMathFun dlg;
	dlg.m_pStrArrFunDisp=&m_strArrFunDisp;
	dlg.m_pStrArrFunName=&m_strArrFunName;
	dlg.m_strBlockDesp=this->m_strBlockDisp;
	dlg.m_strFunName=this->m_strFuncName;
	dlg.m_strPropertityTableName=m_strPropertityTableName;
	if(dlg.DoModal()==IDOK)
	{
		m_strFuncName=dlg.m_strFunName;
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
void CRelationalBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
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
	pRecord->Update();
}

//����ͼ��ʱ����Ҫ�������ԡ�
void CRelationalBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strFuncName=((CRelationalBlock*)pBlock)->m_strFuncName;
}
//�����Է��ô���������
void CRelationalBlock::SetBlockPropertyToTransferData(
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
}
