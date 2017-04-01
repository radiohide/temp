// WASPCalcBlock.cpp: implementation of the CWASPCalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WASPCalcBlock.h"
#include "PropertyDlgWASP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPtrArray ListPoteDll;
CWASPCalcBlock::CWASPCalcBlock()
{
	m_strFuncName="";
	PSourceValue=new TAGINFO[2];
	pG = new TAGINFO;
}

CWASPCalcBlock::~CWASPCalcBlock()
{
	if (PSourceValue)
	{
		delete[] PSourceValue;
		PSourceValue=NULL;
	}
	if (pG)
	{
		delete pG;
		pG=NULL;
	}
}

//���㺯����ʵ�ֱ�ģ��ļ���
void CWASPCalcBlock::Calc() //���м���ĺ���,��Ҫ����
{
	rtFunc = NULL;
	for (int i=0; i<(ListPoteDll).GetSize(); i++)
	{
		CPoteDll* PoteDll = (CPoteDll*)((ListPoteDll)[i]);
		for (int flag=0; flag<PoteDll->lstPoteFuncName.GetSize();flag++)
		{
			if (PoteDll->lstPoteFuncName[flag] == m_strFuncName)
			{
				rtFunc = (RTFUNC)PoteDll->lstPoteFuncAddr[flag];
				if (rtFunc)	break;
			}
		}
		if (rtFunc)	break;
	}
	if(rtFunc)
	{
		GetInputValueFromGlobalWS();
		//�������
		CCalcPort *pOutPortObj = GetOutputPortObj(0);
		CCalcPort *pInput1Obj=GetInputPortObj(0);
		CCalcPort *pInput2Obj=GetInputPortObj(1);
		
		double inValue1,inValue2;
		pInput1Obj->GetPortUniValue().GetDoubleVal(&inValue1);
		pInput2Obj->GetPortUniValue().GetDoubleVal(&inValue2);
		
		PSourceValue[0].dValue = inValue1;
		PSourceValue[0].wState = 1;
		PSourceValue[0].tTime=CTime::GetCurrentTime().GetTime();
		
		PSourceValue[1].dValue = inValue2;
		PSourceValue[1].wState = 1;
		PSourceValue[1].tTime=CTime::GetCurrentTime().GetTime();
		
		*pG = rtFunc(PSourceValue, 2);
		CUniValue UniVar;
		UniVar.AddData(pG->dValue);
		pOutPortObj->SetPortUniValue(UniVar);
		//����������ռ���
		OutputResultToGlobalWS();
	}
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CWASPCalcBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("strFuncName")==0)
	{
		if(!strItem1.IsEmpty())  m_strFuncName=strItem1;
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CWASPCalcBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//�����ԭ�е�����˿�����
	IDArrRightOut.RemoveAll();

	CStringArray  IDArrRightIn;//������ڲ�������˿�����
    IDArrRightIn.RemoveAll();

	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}

	CPropertyDlgWASP dlg;
	dlg.m_pStrArrFunDisp=&m_strArrFunDisp;
	dlg.m_pStrArrFunName=&m_strArrFunName;
	dlg.m_strBlockDesp=this->m_strBlockDisp;
	dlg.m_strFunName=this->m_strFuncName;
	if(dlg.DoModal()==IDOK)
	{
		m_strFuncName=dlg.m_strFunName;
		int index=-1;
		for(int i=0;i<m_strArrFunName.GetSize();i++)
		{
			if(m_strFuncName==m_strArrFunName[i])
			{
				index=i;
				break;
			}
		}

	    //������˿ڵ��޸�
		if(index!=-1)
			IDArrRightIn.Add(m_strArrOutPortName[index]);
		else
			IDArrRightIn.Add("out");
		
		for(int jj=IDArrRightOut.GetSize()-1; jj>-1; jj--)//ɾ��ԭ�ж����õ�Port,����ɾ
		{
			int  runs=0;//��������
			for(int kk=0; kk<IDArrRightIn.GetSize(); kk++)
			{
				if(IDArrRightIn[kk]!=IDArrRightOut[jj])	
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
				if(IDArrRightIn[mm]!=IDArrRightOut[nn])
				{runs++;}		
			}
			if(runs==IDArrRightOut.GetSize())
			{
				CString str=IDArrRightIn[mm];
				InSertOutputPortBefore(mm,str, VALUE_DOUBLE, _T(str));
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
void CWASPCalcBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
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
void CWASPCalcBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strFuncName=((CWASPCalcBlock*)pBlock)->m_strFuncName;
}
//�����Է��ô���������
void CWASPCalcBlock::SetBlockPropertyToTransferData(
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
