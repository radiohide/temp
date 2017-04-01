// MathFunBlock.cpp: implementation of the CMathFunBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "MathFunBlock.h"
#include "PropertyDlgMathFun.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMathFunBlock::CMathFunBlock()
{
	SetBlockType(_T( "MathFun" ) );//��������
	m_strFuncName="Power";
	m_strBlockDisp="��ѧ��������顣��ѡ����ѧ���㺯�����м��㡣";
	m_strPropertityTableName="��ѧ���������";
	
	m_strArrFunName.Add("Power");m_strArrFunDisp.Add("�˷�");
	m_strArrInPortName.Add("Base");m_iArrInFunIndex.Add(0);
	m_strArrInPortName.Add("Exponent");m_iArrInFunIndex.Add(0);
	m_strArrOutPortName.Add("Power");m_iArrOutFunIndex.Add(0);

	m_strArrFunName.Add("Log");m_strArrFunDisp.Add("����");
	m_strArrInPortName.Add("Base");m_iArrInFunIndex.Add(1);
	m_strArrInPortName.Add("Real");m_iArrInFunIndex.Add(1);
	m_strArrOutPortName.Add("Log");m_iArrOutFunIndex.Add(1);
	
	m_strArrFunName.Add("Abs");m_strArrFunDisp.Add("����ֵ");
	m_strArrInPortName.Add("In");m_iArrInFunIndex.Add(2);
	m_strArrOutPortName.Add("Out");m_iArrOutFunIndex.Add(2);

	ConstructBlockInputOutput();
}

CMathFunBlock::~CMathFunBlock()
{
	
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CMathFunBlock::ConstructBlockInputOutput ()
{
	ClearInputPorts();
	ClearOutputPorts();
	int index=-1;
	for(int i=0;i<m_strArrFunName.GetSize();i++)
	{
		if(m_strFuncName==m_strArrFunName[i])
		{
			index=i;
			break;
		}
	}
	if(index!=-1)//��ѡȡ����
	{
		for(int i=0;i<m_iArrInFunIndex.GetSize();i++)
		{
			if(m_iArrInFunIndex[i]==index)
			{
				AddInputPort(m_strArrInPortName[i], VALUE_DOUBLE);
			}
		}
		for(i=0;i<m_iArrOutFunIndex.GetSize();i++)
		{
			if(m_iArrOutFunIndex[i]==index)
			{
				AddOutputPort(m_strArrOutPortName[i], VALUE_DOUBLE);
			}
		}
	}
}
//���������½�����ĺ���
CCalcBlock* CMathFunBlock::CreateFromString( const CString& strType )
{
	CMathFunBlock* obj = new CMathFunBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CMathFunBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CMathFunBlock::Calc() //���м���ĺ���,��Ҫ����
{
	GetInputValueFromGlobalWS();
    //�������
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	double inValue1,inValue2; 
	CUniValue UniVar;
	if("Power"==m_strFuncName)
	{
		CCalcPort *pInPortObj1 = GetInputPortObj(0);
		CCalcPort *pInPortObj2 = GetInputPortObj(1);
		pInPortObj1->GetPortUniValue().GetDoubleVal(&inValue1);
		pInPortObj2->GetPortUniValue().GetDoubleVal(&inValue2);
		if(inValue1<0)
			UniVar.AddData(-1*pow(-1*inValue1,inValue2));
		else
			UniVar.AddData(pow(inValue1,inValue2));
	}
	else if("Log"==m_strFuncName)
	{
		CCalcPort *pInPortObj1 = GetInputPortObj(0);
		CCalcPort *pInPortObj2 = GetInputPortObj(1);
		pInPortObj1->GetPortUniValue().GetDoubleVal(&inValue1);
		pInPortObj2->GetPortUniValue().GetDoubleVal(&inValue2);
		UniVar.AddData(log(inValue2>=0?inValue2:(-1)*inValue2)/log(inValue1>=0?inValue1:(-1)*inValue1));
	}
	else if("Abs"==m_strFuncName)
	{
		CCalcPort *pInPortObj1 = GetInputPortObj(0);
		pInPortObj1->GetPortUniValue().GetDoubleVal(&inValue1);
		UniVar.AddData(inValue1>=0?inValue1:(-1)*inValue1);
	}
	pOutPortObj->SetPortUniValue(UniVar);
	OutputResultToGlobalWS();	
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CMathFunBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("strFuncName")==0)
	{
		if(!strItem1.IsEmpty())  m_strFuncName=strItem1;
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CMathFunBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgMathFun dlg;
	dlg.m_pStrArrFunDisp=&m_strArrFunDisp;
	dlg.m_pStrArrFunName=&m_strArrFunName;
	dlg.m_strBlockDesp=this->m_strBlockDisp;
	dlg.m_strFunName=this->m_strFuncName;
	dlg.m_strPropertityTableName=m_strPropertityTableName;
	
	int portnumLinkBefore=this->GetInputNum();//����
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrLeftOut;//�����ԭ�е�����˿�����
	CStringArray  IDArrRightOut;//�����ԭ�е�����˿�����
	IDArrLeftOut.RemoveAll();
	IDArrRightOut.RemoveAll();
	
	CStringArray  IDArrLeftIn;//������ڲ�������˿�����
	CStringArray  IDArrRightIn;//������ڲ�������˿�����
	IDArrLeftIn.RemoveAll();
    IDArrRightIn.RemoveAll();
	
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		IDArrLeftOut.Add(strPortID);
	}
	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}
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
		if(index!=-1)//��ѡȡ����
		{
			//1��������˿ڵ��޸�
			for(int i=0;i<m_iArrInFunIndex.GetSize();i++)
			{
				if(m_iArrInFunIndex[i]==index)
				{
					//AddInputPort(m_strArrInPortName[i], VALUE_DOUBLE);
					IDArrLeftIn.Add(m_strArrInPortName[i]);
				}
			}
			for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)	//ɾ��ԭ�ж����õ�Port,����ɾ
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
			for(int m=0; m<IDArrLeftIn.GetSize(); m++)	//����µ�Port
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
					InSertInputPortBefore(m,str, VALUE_DOUBLE, _T(str));
				}
			}
			//2��������˿ڵ��޸�
			for(i=0;i<m_iArrOutFunIndex.GetSize();i++)
			{
				if(m_iArrOutFunIndex[i]==index)
				{
					//AddOutputPort(m_strArrOutPortName[i], VALUE_DOUBLE);
					IDArrRightIn.Add(m_strArrOutPortName[i]);
				}
			}
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
void CMathFunBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
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
void CMathFunBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strFuncName=((CMathFunBlock*)pBlock)->m_strFuncName;
}
//�����Է��ô���������
void CMathFunBlock::SetBlockPropertyToTransferData(
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
