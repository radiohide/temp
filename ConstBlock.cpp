// ConstBlock.cpp: implementation of the CConstBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "ConstBlock.h"
#include "PropertyDlgConst.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConstBlock::CConstBlock()
{
	SetBlockType(_T("Const") );
	ConstructBlockInputOutput();
	m_dConstValue=1;

	m_bShowDesp=TRUE;//�Ƿ���ʾDESP
	m_bShowInPortText=FALSE; //�Ƿ���ʾ����˿ڵ���������
	m_bShowOutPortText=FALSE; //�Ƿ���ʾ����˿ڵ���������
}

CConstBlock::~CConstBlock()
{

}
void CConstBlock::ConstructBlockInputOutput ()
{
	//�����ɶ�����������ı������ơ�
    AddOutputPort("out", VALUE_DOUBLE);
}
	//���������½�����ĺ���
CCalcBlock* CConstBlock::CreateFromString( const CString& strType )
{
	CConstBlock* obj = new CConstBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CConstBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_dConstValue=((CConstBlock*)pBlock)->m_dConstValue;
}
	//��ʼ������ĺ���������ĳ�ʼ��
void CConstBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
	//���㺯����ʵ�ֱ�ģ��ļ���
void CConstBlock::Calc() //���м���ĺ���,��Ҫ����
{
	CUniValue UniVar;
	CCalcPort *pOutPortObj=GetOutputPortObj(0);
	if(pOutPortObj)
	{	
		UniVar.AddData(m_dConstValue,this->m_pModel->m_lDataTime);
		//UniVar.AddData(m_dConstValue);
		pOutPortObj->SetPortUniValue(UniVar);
	}
	////////////////////////////////////////////////////////////////////////
   //����������ռ���
	OutputResultToGlobalWS();
}
//������ͼ��
void CConstBlock::DrawShape(CDC* dc, CRect rect,int CutSize)
{
	//rect Ϊblock������ο�,���в�д����ֵ
	//dc->Rectangle(rect);
	CCalcBlock::DrawShape(dc,rect,CutSize);
	CString str;
	if((double)((int)m_dConstValue)==m_dConstValue)
	{
         str.Format("%d", (int)m_dConstValue);
	}
	else
	{
		 str.Format(_T("%f"), m_dConstValue);
	 	 str.TrimRight(_T("0")); 
	}
	CRect rcText(rect);
	rcText.top = rect.top + rect.Height()/2 - CutSize;
	rcText.bottom = rcText.top + 2*CutSize;
	
	dc->DrawText(str,rcText,DT_NOPREFIX | DT_WORDBREAK | DT_CENTER );

}
void CConstBlock::SetBlockPropertyToTransferData(
				CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)//�����Է��ô���������
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"ConstValue");
	
	CString str;
	str.Format("%f",m_dConstValue);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
    //���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CConstBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(strPropName.Compare("ConstValue")==0)
	{
		if(!strItem1.IsEmpty())
			m_dConstValue=atof(strItem1);
	}	
}
	//��������ʾ��ͬ�����Զ���Ի���
void CConstBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgConst dlg;
    //���Ի��������ֵ
	///////////////////////////////////////////////////
	//int tagnum = m_listIn.GetCount();
	dlg.m_dConstValue=m_dConstValue;
	////////////////////////////////////////////////////////////////////
	if(dlg.DoModal()==IDOK)
	{
		m_dConstValue = dlg.m_dConstValue;
		if((m_pModel!=NULL)&&(this->m_pModel->m_iTagModeLocalOrRemote==1))//Զ��
		{
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> dataArr;
			SetBlockPropertyToTransferData(dataArr);//��ȡҪ��������Ե�����������
			SendBlockPropertyTransferData(dataArr);// ��������������
			ClearPropertyTransferData(dataArr);//�ͷ�����
		}
	}
	//////////////////////////////////////////////////
}
	//�����Խ���ģ��д�뵽���ݿ���
void CConstBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//ÿ������Ϊһ����¼
	//����1�����ݲ���ǩ��////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue=_T("ConstValue"); //������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	//�Բ�����Խ���д��
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(m_dConstValue) );
	pRecord->Update();
}
