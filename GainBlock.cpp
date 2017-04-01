// GainBlock.cpp: implementation of the CGainBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "GainBlock.h"
#include "PropertyDlgGain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGainBlock::CGainBlock()
{
	m_dGainValue=1;
	SetBlockType(_T( "Gain" ) );
	ConstructBlockInputOutput();

	m_bShowDesp=TRUE;//�Ƿ���ʾDESP
	m_bShowInPortText=FALSE; //�Ƿ���ʾ����˿ڵ���������
	m_bShowOutPortText=FALSE; //�Ƿ���ʾ����˿ڵ���������
}

CGainBlock::~CGainBlock()
{

}
//���������½�����ĺ���
CCalcBlock* CGainBlock::CreateFromString( const CString& strType )
{
	CGainBlock* obj = new CGainBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CGainBlock::ConstructBlockInputOutput ()
{
	AddInputPort("In",VALUE_DOUBLE);
	AddOutputPort("Out",VALUE_DOUBLE);
}
//��ʼ������ĺ���������ĳ�ʼ��
void CGainBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CGainBlock::Calc()//���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	int inNum = GetInputNum();
	double inValue=0;
	CCalcPort *pInPortObj = GetInputPortObj(0);
	pInPortObj->GetPortUniValue().GetDoubleVal(&inValue);

	CUniValue UniVar;
	UniVar.AddData(inValue*m_dGainValue);
	pOutPortObj->SetPortUniValue(UniVar);
	OutputResultToGlobalWS();
}
//������ͼ��
void CGainBlock::DrawShape(CDC* dc, CRect rect,int CutSize)
{
	//rect Ϊ
	//dc->Rectangle(rect);
	CCalcBlock::DrawShape(dc,rect,CutSize);
	CString str;
	if((double)((int)m_dGainValue)==m_dGainValue)
	{
              str.Format("%d", (int)m_dGainValue);
	}
	else
	{
		 str.Format(_T("%f"), m_dGainValue);
	 	 str.TrimRight(_T("0")); 
	}
	CRect rcText(rect);
	rcText.top = rect.top + rect.Height()/2 - CutSize;
	rcText.bottom = rcText.top + 2*CutSize;
	//
	CPoint midPt;
	midPt.x = rect.right ;
	midPt.y = rect.top + rect.Height()/2;
	dc->MoveTo(rect.left, rect.top+2*CutSize);
	dc->LineTo(midPt);
	dc->LineTo(rect.left,rect.bottom - 2*CutSize);
	dc->DrawText(str,rcText,DT_NOPREFIX | DT_WORDBREAK | DT_CENTER );

}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CGainBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("GainValue")==0)
	{
		if(!strItem1.IsEmpty())  {m_dGainValue=atof(strItem1);}
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CGainBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgGain dlg;
    //���Ի��������ֵ
	dlg.m_dGainValue=m_dGainValue;
	if(dlg.DoModal()==IDOK)
	{
		m_dGainValue=dlg.m_dGainValue;
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
void CGainBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="GainValue";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%f",m_dGainValue); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	pRecord->Update();
}

//����ͼ��ʱ����Ҫ�������ԡ�
void CGainBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_dGainValue=((CGainBlock*)pBlock)->m_dGainValue;
}
//�����Է��ô���������
void CGainBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"GainValue");
	
	CString str;
	str.Format("%f",m_dGainValue);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
