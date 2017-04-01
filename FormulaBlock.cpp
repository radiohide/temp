// FormulaBlock.cpp: implementation of the CFormulaBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "FormulaBlock.h"
#include "PropertyDlgFormula.h"
#include "FlowchartEditor/DiagramEditor/Tokenizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFormulaBlock::CFormulaBlock()
{
	maxInPut=20;//����������
	maxOutPut=10;//����������
	maxFormula=20;//���ʽ����������м��������Ҳ�������

	inPutArr.RemoveAll();
	for(int i=0;i<maxInPut;i++)
		inPutArr.Add("");

	inPutTypeArr.RemoveAll();
	for(i=0;i<maxInPut;i++)
		inPutTypeArr.Add(0);//Ĭ��Ϊ��ֵ��

	outPutArr.RemoveAll();
	for(i=0;i<maxOutPut;i++)
		outPutArr.Add("");//Ĭ��Ϊ��ֵ��
	
	outPutTypeArr.RemoveAll();
	for(i=0;i<maxOutPut;i++)
		outPutTypeArr.Add(0);

	varArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		varArr.Add("");

	formulaArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		formulaArr.Add("");

	varTypeArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		varTypeArr.Add(0);

	inPutArr[0]="x";
	outPutArr[0]="y";
	SetBlockType(_T( "Formula" ) );//��������
	ConstructBlockInputOutput ();
}

CFormulaBlock::~CFormulaBlock()
{
	delInnerWorkSpace();
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CFormulaBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("x"), VALUE_DOUBLE);//Ĭ�Ϲ��쵥���뵥���
	AddOutputPort("y",VALUE_DOUBLE);
} 
//���������½�����ĺ���
CCalcBlock* CFormulaBlock::CreateFromString( const CString& strType )
{
	CFormulaBlock* obj = new CFormulaBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//��ʼ������ĺ���������ĳ�ʼ��
void CFormulaBlock::establishInnerWorkSpace()
{
	delInnerWorkSpace();
	CString strVarName;
	CElementInOut *pElement =NULL;
	for(int i=0;i<this->inPutArr.GetSize();i++)//�����
	{
		strVarName=inPutArr[i];
		if(strVarName=="")
			break;
		pElement=new CElementInOut(strVarName,inPutTypeArr[i],this);
		block_WorkSpaceList.SetAt(strVarName,pElement);
	}
	for(i=0;i<this->varArr.GetSize();i++)//����㣬��Ҫ���й�ʽ����ĵ�
	{
		strVarName=varArr[i];
		if(strVarName=="")
			break;
		pElement=new CElementInOut(strVarName,varTypeArr[i],formulaArr[i]+"@",this);
		block_WorkSpaceList.SetAt(strVarName,pElement);
	}
	POSITION pos = this->block_WorkSpaceList.GetStartPosition();
	while(pos)
    {
		CString strKey;
		CElementInOut *pElement;
		block_WorkSpaceList.GetNextAssoc(pos, strKey, pElement);
		if(pElement!=NULL)
		{
			if(pElement->PType==1)//�����
				pElement->ParseFormula();//��ʼ����ʽ������
		}	
	}
}
void CFormulaBlock::delInnerWorkSpace()//���Map
{
	POSITION pos = this->block_WorkSpaceList.GetStartPosition();
	while(pos)
    {
		CString strKey;
		CElementInOut *pElement;
		block_WorkSpaceList.GetNextAssoc(pos, strKey, pElement);
		if(pElement!=NULL)
		{
			delete pElement;
			pElement=NULL;
		}
	}
	block_WorkSpaceList.RemoveAll();
}
void CFormulaBlock::setDataToElem(CUniValue &uniValue,CElementInOut *pElement,int type)//type:0,��ֵ�㣻1,����ֵ
{
	long dateSize=uniValue.GetDataSize();
	if(dateSize>0)//����ֵ�ռ�
	{
		if(type==0)//��ֵ
		{
			uniValue.GetDoubleVal(&(pElement->PValue));
		}
		else if(type==1)//����ֵ
		{
			pElement->NumData=dateSize;
			pElement->pDataAll = new TAGINFO[dateSize];
			memset(pElement->pDataAll,0,sizeof(TAGINFO)*dateSize);	
			
			for(long i=0;i<dateSize;i++)
			{
				uniValue.GetDoubleValAt(i,&(pElement->pDataAll[i].dValue));
			}
		}
	}
}
void CFormulaBlock::setDataToUniValue(CElementInOut *pElement,CUniValue &uniValue,int type)//type:0,��ֵ�㣻1,����ֵ
{
	if(type==0)//��ֵ
	{
		uniValue.AddData(pElement->PValue);
	}
	else if(type==1)//����ֵ
	{
		for(long i=0;i<pElement->NumData;i++)
		{
			uniValue.AddData(pElement->pDataAll[i].dValue);
		}
	}
}

void CFormulaBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	establishInnerWorkSpace();//��ʼ��Map
}
void CFormulaBlock::resetAllElement()
{
	POSITION pos = this->block_WorkSpaceList.GetStartPosition();
	while(pos)
    {
		CString strKey;
		CElementInOut *pElement;
		block_WorkSpaceList.GetNextAssoc(pos, strKey, pElement);
		if(pElement!=NULL)
		{
			pElement->ReSet();
		}
	}
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CFormulaBlock::Calc() //���м���ĺ���,��Ҫ����
{
	resetAllElement();
	GetInputValueFromGlobalWS();
	
	int inNum = GetInputNum();
	for(int k=0;k<inNum;k++)
	{
		CCalcPort *pPortObj = GetInputPortObj(k);
		CUniValue  uniValue=pPortObj->GetPortUniValue();
		CElementInOut *pElement=block_WorkSpaceList[pPortObj->GetPortID()];
		setDataToElem(uniValue,pElement,pPortObj->GetPortValueType());
	}
	POSITION pos = this->block_WorkSpaceList.GetStartPosition();
	while(pos)
    {
		CString strKey;
		CElementInOut *pElement;
		block_WorkSpaceList.GetNextAssoc(pos, strKey, pElement);
		if(pElement!=NULL)
		{
			if(pElement->PType==1)
				pElement->CalcFormula();
		}
	}
	inNum = GetOutputNum();
	for(int i=0;i<inNum;i++)
	{
		CCalcPort *pPortObj = GetOutputPortObj(i);
		CUniValue  UniVar;
		CElementInOut *pElement=block_WorkSpaceList[pPortObj->GetPortID()];
		setDataToUniValue(pElement,UniVar,pPortObj->GetPortValueType());
		pPortObj->SetPortUniValue(UniVar);
	}
	//����������ռ���
	OutputResultToGlobalWS();
}
//�������ԵĶ����ַ���
CString CFormulaBlock::GetBlockStrPro()
{
	CString str;

	for(int i=0;i<maxInPut;i++)//����
		str+=inPutArr[i]+"@";

	for(i=0;i<maxOutPut;i++)//���
		str+=outPutArr[i]+"@";

	for(i=0;i<maxFormula;i++)//����
		str+=varArr[i]+"@";

	for(i=0;i<maxFormula;i++)//��ʽ
	{
		if(i+1==maxFormula)
		{
			str+=formulaArr[i];
		}
		else
		{
			str+=formulaArr[i]+"@";
		}
	}
	return str;

}
void CFormulaBlock::SetBlockStrPro(CString str)//���ü������ַ�������
{
	CTokenizer tok(str,_T('@'));
	CString strPut;
	for(int i=0;i<tok.GetSize();i++)
	{
		tok.GetAt(i,strPut);
		if((i>=0)&&(i<maxInPut))
		{
			if(inPutArr[i]!=strPut)
			{
				ChangeInPortName(inPutArr[i],strPut);
				inPutArr[i]=strPut;
			}
		}
		else if((i>=maxInPut)&&(i<maxInPut+maxOutPut))
		{
			if(outPutArr[i-maxInPut]!=strPut)
			{
				ChangeOutPortName(outPutArr[i-maxInPut],strPut);
				outPutArr[i-maxInPut]=strPut;
			}
		}
		else if((i>=maxInPut+maxOutPut)&&(i<maxInPut+maxOutPut+maxFormula))
		{
			if(varArr[i-maxInPut-maxOutPut]!=strPut)
			{
				varArr[i-maxInPut-maxOutPut]=strPut;
			}
		}
		else if((i>=maxInPut+maxOutPut+maxFormula)&&(i<maxInPut+maxOutPut+maxFormula*2))
		{	
			if(formulaArr[i-maxInPut-maxOutPut-maxFormula]!=strPut)
			{
				formulaArr[i-maxInPut-maxOutPut-maxFormula]=strPut;
			}
		}
	}
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CFormulaBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	int propType,propIndex;
	getPropTypeByName(strPropName,propType,propIndex);
	if(propType==1)
	{
		if(!strItem1.IsEmpty())  {inPutArr[propIndex]=strItem1;}
		if(!strItem2.IsEmpty())  {inPutTypeArr[propIndex]=atoi(strItem2);}
	}
	else if(propType==2)
	{
		if(!strItem1.IsEmpty())  {outPutArr[propIndex]=strItem1;}
		if(!strItem2.IsEmpty())  {outPutTypeArr[propIndex]=atoi(strItem2);}
	}
	else if(propType==3)
	{
		if(!strItem1.IsEmpty())  {varArr[propIndex]=strItem1;}
		if(!strItem2.IsEmpty())  {varTypeArr[propIndex]=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {formulaArr[propIndex]=strItem3;}
	}
}
void CFormulaBlock::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
{
	int dIndex=strPropName.ReverseFind('.');
	CString strIndex=strPropName.Right(strPropName.GetLength()-dIndex-1);
	CString strName=strPropName.Left(dIndex);
	propIndex=atoi(strIndex);

	if(strName=="input")//����Ϊ1
	{
		propType=1;
	}
	else if(strName=="output")//���Ϊ2
	{
		propType=2;
	}
	else if(strName=="formula")//��ʽΪ3
	{
		propType=3;
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CFormulaBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgFormula dlg;
	
	int portnumLinkBefore=this->GetInputNum();//����
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrLeftOut;//�����ԭ�е�����˿�����
	CArray<int,int> IDArrLeftOutType;//�����ԭ�е�����˿�����
	CStringArray  IDArrRightOut;//�����ԭ�е�����˿�����
	CArray<int,int> IDArrRightOutType;//�����ԭ�е�����˿�����
	
	CStringArray  IDArrLeftIn;//������ڲ�������˿�����
	CArray<int,int>  IDArrLeftInType;//������ڲ�������˿�����
	CStringArray  IDArrRightIn;//������ڲ�������˿�����
	CArray<int,int>  IDArrRightInType;//������ڲ�������˿�����
	

	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		int iPortType=this->GetInputPortObj(q)->GetPortValueType();
		IDArrLeftOut.Add(strPortID);
		IDArrLeftOutType.Add(iPortType);
	}
	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		int iPortType=this->GetOutputPortObj(r)->GetPortValueType();
		IDArrRightOut.Add(strPortID);
		IDArrRightOutType.Add(iPortType);
	}

    //���Ի��������ֵ
	///////////////////////////////////////////////////
	dlg.SetPropData(
		inPutArr,inPutTypeArr,outPutArr,outPutTypeArr,
		varArr,varTypeArr,formulaArr);
	if(dlg.DoModal()==IDOK)
	{
		inPutArr.Copy(dlg.inPutArr);
		inPutTypeArr.Copy(dlg.inPutTypeArr);
		outPutArr.Copy(dlg.outPutArr);
		outPutTypeArr.Copy(dlg.outPutTypeArr);
		varArr.Copy(dlg.varArr);
			
		formulaArr.Copy(dlg.formulaArr);

        //1��������˿ڵ��޸�
		for(int i=0;i<inPutArr.GetSize();i++)
		{
			if(inPutArr[i]=="")
				break;
			IDArrLeftIn.Add(inPutArr[i]);
			IDArrLeftInType.Add(inPutTypeArr[i]);
		}	
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

        //2��������˿ڵ��޸�
		for(i=0;i<outPutArr.GetSize();i++)
		{
			if(outPutArr[i]=="")
				break;
			IDArrRightIn.Add(outPutArr[i]);
			IDArrRightInType.Add(outPutTypeArr[i]);
		}		
		for(int jj=IDArrRightOut.GetSize()-1; jj>-1; jj--)//ɾ��ԭ�ж����õ�Port,����ɾ
		{
			int  runs=0;//��������
			for(int kk=0; kk<IDArrRightIn.GetSize(); kk++)
			{
				if((IDArrRightIn[kk]!=IDArrRightOut[jj])||(IDArrRightInType[kk]!=IDArrRightOutType[jj]))	
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
				if((IDArrRightIn[mm]!=IDArrRightOut[nn])||(IDArrRightInType[mm]!=IDArrRightOutType[nn]))
				{runs++;}		
			}
			if(runs==IDArrRightOut.GetSize())
			{
				CString str=IDArrRightIn[mm];
				InSertOutputPortBefore(mm,str, outPutTypeArr[mm], _T(str));
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
void CFormulaBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	for(int i=0;i<inPutArr.GetSize();i++)//����
	{
		CString strInPut=inPutArr[i];
		if(strInPut=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("input.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������

		strFieldValue.Format("%s",inPutArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%d",inPutTypeArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
	}
	for(i=0;i<outPutArr.GetSize();i++)//���
	{
		CString strOutPut=outPutArr[i];
		if(strOutPut=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("output.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������

		strFieldValue.Format("%s",outPutArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%d",outPutTypeArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
	}
	for(i=0;i<varArr.GetSize();i++)//��ʽ
	{
		CString strVar=varArr[i];
		if(strVar=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("formula.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������

		strFieldValue.Format("%s",varArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		
		strFieldValue.Format("%d",varTypeArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	

		strFieldValue.Format("%s",formulaArr[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
	}
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CFormulaBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	maxInPut=((CFormulaBlock*)pBlock)->maxInPut;
	maxOutPut=((CFormulaBlock*)pBlock)->maxOutPut;
	maxFormula=((CFormulaBlock*)pBlock)->maxFormula;

	inPutArr.Copy(((CFormulaBlock*)pBlock)->inPutArr);
	inPutTypeArr.Copy(((CFormulaBlock*)pBlock)->inPutTypeArr);

	outPutArr.Copy(((CFormulaBlock*)pBlock)->outPutArr);
	outPutTypeArr.Copy(((CFormulaBlock*)pBlock)->outPutTypeArr);

	varArr.Copy(((CFormulaBlock*)pBlock)->varArr);
	formulaArr.Copy(((CFormulaBlock*)pBlock)->formulaArr);
	varTypeArr.Copy(((CFormulaBlock*)pBlock)->varTypeArr);
}
//�����Է��ô���������
void CFormulaBlock::SetBlockPropertyToTransferData(
								CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=NULL;
	CString strFieldValue,str;
	for(int i=0;i<inPutArr.GetSize();i++)//����
	{
		CString strInPut=inPutArr[i];
		if(strInPut=="")
			break;
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		strFieldValue.Format("input.%d",i);
		strcpy(pData->m_cPropName,strFieldValue);
		str.Format("%s",inPutArr[i]);
		strcpy(pData->m_cValueItem1,str);
		str.Format("%d",inPutTypeArr[i]);
		strcpy(pData->m_cValueItem2,str);
		dataArr.Add(pData);
	}
	for(i=0;i<outPutArr.GetSize();i++)//���
	{
		CString strOutPut=outPutArr[i];
		if(strOutPut=="")
			break;
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		strFieldValue.Format("output.%d",i);
		strcpy(pData->m_cPropName,strFieldValue);
		str.Format("%s",outPutArr[i]);
		strcpy(pData->m_cValueItem1,str);
		str.Format("%d",outPutTypeArr[i]);
		strcpy(pData->m_cValueItem2,str);
		dataArr.Add(pData);
	}
	for(i=0;i<varArr.GetSize();i++)//��ʽ
	{
		CString strVar=varArr[i];
		if(strVar=="")
			break;
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		strFieldValue.Format("formula.%d",i);
		strcpy(pData->m_cPropName,strFieldValue);
		str.Format("%s",varArr[i]);
		strcpy(pData->m_cValueItem1,str);
		str.Format("%d",varTypeArr[i]);
		strcpy(pData->m_cValueItem2,str);
		str.Format("%d",formulaArr[i]);
		strcpy(pData->m_cValueItem2,str);
		dataArr.Add(pData);
	}
}


