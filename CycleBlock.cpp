// CycleBlock.cpp: implementation of the CCycleBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "CycleBlock.h"
#include "PropertyDlgIter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCycleBlock::CCycleBlock()
{
	maxInPut=200;//����������
	maxOutPut=200;//����������
	maxFormula=400;//���ʽ����������м��������Ҳ�������
	tagConFOrB=1;//Ĭ���ڵ��������

	inPutArr.RemoveAll();
	for(int i=0;i<maxInPut;i++)
		inPutArr.Add("");
	inPutTypeArr.RemoveAll();
	for(i=0;i<maxInPut;i++)
		inPutTypeArr.Add(0);//Ĭ��Ϊ��ֵ��
	inPutCanBeEdit.RemoveAll();
	for(i=0;i<maxInPut;i++)
		inPutCanBeEdit.Add(1);//Ĭ��Ϊ�ɱ��༭

	outPutArr.RemoveAll();
	for(i=0;i<maxOutPut;i++)
		outPutArr.Add("");//Ĭ��Ϊ��ֵ��	
	outPutTypeArr.RemoveAll();
	for(i=0;i<maxOutPut;i++)
		outPutTypeArr.Add(0);
	outPutCanBeEdit.RemoveAll();
	for(i=0;i<maxOutPut;i++)
		outPutCanBeEdit.Add(1);//Ĭ��Ϊ�ɱ��༭

	IterFVarArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		IterFVarArr.Add("");
	IterFFormulaArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		IterFFormulaArr.Add("");
	IterFVarTypeArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		IterFVarTypeArr.Add(0);
	IterFCanBeEdit.RemoveAll();
	for(i=0;i<maxFormula;i++)
		IterFCanBeEdit.Add(1);
	IterFCanBeShow.RemoveAll();
	for(i=0;i<maxFormula;i++)
		IterFCanBeShow.Add(1);

	conVarArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		conVarArr.Add("");
	conFormulaArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		conFormulaArr.Add("");
	conVarTypeArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		conVarTypeArr.Add(0);
	conVarCanBeEdit.RemoveAll();
	for(i=0;i<maxFormula;i++)
		conVarCanBeEdit.Add(1);
	conVarCanBeShow.RemoveAll();
	for(i=0;i<maxFormula;i++)
		conVarCanBeShow.Add(1);

	IterVarArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		IterVarArr.Add("");
	IterFormulaArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		IterFormulaArr.Add("");
	IterVarTypeArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		IterVarTypeArr.Add(0);
	IterCanBeEdit.RemoveAll();
	for(i=0;i<maxFormula;i++)
		IterCanBeEdit.Add(1);
	IterCanBeShow.RemoveAll();
	for(i=0;i<maxFormula;i++)
		IterCanBeShow.Add(1);

	IterBVarArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		IterBVarArr.Add("");
	IterBFormulaArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		IterBFormulaArr.Add("");
	IterBVarTypeArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		IterBVarTypeArr.Add(0);
	IterBCanBeEdit.RemoveAll();
	for(i=0;i<maxFormula;i++)
		IterBCanBeEdit.Add(1);
	IterBCanBeShow.RemoveAll();
	for(i=0;i<maxFormula;i++)
		IterBCanBeShow.Add(1);

	inPutArr[0]="x";
	outPutArr[0]="y";
	SetBlockType(_T( "Cycle" ) );//��������
	ConstructBlockInputOutput ();
}

CCycleBlock::~CCycleBlock()
{
	DelInnerWorkSpace();
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CCycleBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("x"), VALUE_DOUBLE, _T("in1"), _T("in1"));//Ĭ�Ϲ��쵥���뵥���
	AddOutputPort("y",VALUE_DOUBLE,"out");
}
//���������½�����ĺ���
CCalcBlock* CCycleBlock::CreateFromString( const CString& strType )
{
	CCycleBlock* obj = new CCycleBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CCycleBlock::DelInnerWorkSpace()//���Map
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
		if(blockIter_WorkSpaceList.Lookup(strKey,pElement))
		{
			blockIter_WorkSpaceList[strKey]=NULL;
		}
	}
	block_WorkSpaceList.RemoveAll();

	pos = this->blockIter_WorkSpaceList.GetStartPosition();
	while(pos)
    {
		CString strKey;
		CElementInOut *pElement;
		blockIter_WorkSpaceList.GetNextAssoc(pos, strKey, pElement);
		if(pElement!=NULL)
		{
			delete pElement;
			pElement=NULL;
		}
	}
	blockIter_WorkSpaceList.RemoveAll();
}
void CCycleBlock::BulidInPutEle()
{
	CString strVarName;
	CElementInOut *pElement =NULL;
	for(int i=0;i<this->inPutArr.GetSize();i++)//����㣬������Map��
	{
		strVarName=inPutArr[i];
		if(strVarName=="")
			break;
		pElement=new CElementInOut(strVarName,inPutTypeArr[i],this);
		block_WorkSpaceList.SetAt(strVarName,pElement);

		pElement=new CElementInOut(strVarName,inPutTypeArr[i],this);
		pElement->SetPtrMap(&blockIter_WorkSpaceList);//������MapPtr
		blockIter_WorkSpaceList.SetAt(strVarName,pElement);
	}
}
void CCycleBlock::BulidIterFEle()
{
	CString strVarName;
	CElementInOut *pElement =NULL;
	for(int i=0;i<this->IterFVarArr.GetSize();i++)//����㣬����ǰ����
	{
		strVarName=IterFVarArr[i];
		if(strVarName=="")
			break;
		pElement=new CElementInOut(strVarName,IterFVarTypeArr[i],IterFFormulaArr[i]+"@",this);
		pElement->SetPtrMap(&blockIter_WorkSpaceList);
		blockIter_WorkSpaceList.SetAt(strVarName,pElement);
		CElementInOut *pElementBlock;
		if(!block_WorkSpaceList.Lookup(IterFVarArr[i],pElementBlock))//��û���ҵ��ñ����������block_WorkSpaceList
		{
			block_WorkSpaceList.SetAt(IterFVarArr[i],pElement);
		}
	}
}
void CCycleBlock::BulidConVarEle()
{
	CString strVarName;
	CElementInOut *pElement =NULL;
	for(int i=0;i<this->conVarArr.GetSize();i++)//����㣬��������
	{
		strVarName=conVarArr[i];
		if(strVarName=="")
			break;
		pElement=new CElementInOut(strVarName,conVarTypeArr[i],conFormulaArr[i]+"@",this);
		block_WorkSpaceList.SetAt(strVarName,pElement);
	}
}
void CCycleBlock::BulidIterEle()
{
	CString strVarName;
	CElementInOut *pElement =NULL;
	for(int i=0;i<this->IterVarArr.GetSize();i++)//����㣬��������
	{
		strVarName=IterVarArr[i];
		if(strVarName=="")
			break;
		pElement=new CElementInOut(strVarName,IterVarTypeArr[i],IterFormulaArr[i]+"@",this);
		block_WorkSpaceList.SetAt(strVarName,pElement);
	}
}
void CCycleBlock::BulidIterBEle()
{
	CString strVarName;
	CElementInOut *pElement =NULL;
	for(int i=0;i<this->IterBVarArr.GetSize();i++)//����㣬���������
	{
		strVarName=IterBVarArr[i];
		if(strVarName=="")
			break;
		pElement=new CElementInOut(strVarName,IterBVarTypeArr[i],IterBFormulaArr[i]+"@",this);
		block_WorkSpaceList.SetAt(strVarName,pElement);
	}
}
void CCycleBlock::ElementParseFormula()
{
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
	pos = this->blockIter_WorkSpaceList.GetStartPosition();
	while(pos)
    {
		CString strKey;
		CElementInOut *pElement;
		blockIter_WorkSpaceList.GetNextAssoc(pos, strKey, pElement);
		if(pElement!=NULL)
		{
			if(pElement->PType==1)//�����
				pElement->ParseFormula();//��ʼ����ʽ������
		}
	}
}

//��ʼ������ĺ���������ĳ�ʼ��
void CCycleBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	DelInnerWorkSpace();
	BulidInPutEle();
	BulidConVarEle();
	BulidIterEle();
	BulidIterBEle();
	BulidIterFEle();//Ҫ����󴴽�����Ҫ��block_WorkSpaceList����Ԫ��
	ElementParseFormula();
}
void CCycleBlock::setDataToElem(CUniValue &uniValue,CElementInOut *pElement,int type)//type:0,��ֵ�㣻1,����ֵ
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
void CCycleBlock::setDataToUniValue(CElementInOut *pElement,CUniValue &uniValue,int type)//type:0,��ֵ�㣻1,����ֵ
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

bool CCycleBlock::calcCon() //��������
{
	bool result=true;
	if(conVarArr.GetSize()==0)
		result=false;
	for(int i=0;i<conVarArr.GetSize();i++)
	{
		if(conVarArr[i]=="")
			break;
		CElementInOut *pElement=block_WorkSpaceList[conVarArr[i]];
		if(pElement!=NULL)
		{
			pElement->ReSet();//��λ��ʹ֮δ����
			pElement->CalcFormula();
			if(pElement->PValue==0)
			{
				result=false;
			}
		}
	}
	return result;
}
void CCycleBlock::setBlokcElemBeCalc() //��������Ԫ�ر������
{
	POSITION pos = this->block_WorkSpaceList.GetStartPosition();
	while(pos)
    {
		CString strKey;
		CElementInOut *pElement;
		block_WorkSpaceList.GetNextAssoc(pos, strKey, pElement);
		if(pElement!=NULL)
		{
			pElement->SetBeCalc();
		}
	}
}
void CCycleBlock::setAllElemReSet() //��������Ԫ��δ�������
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
	pos = this->blockIter_WorkSpaceList.GetStartPosition();
	while(pos)
    {
		CString strKey;
		CElementInOut *pElement;
		blockIter_WorkSpaceList.GetNextAssoc(pos, strKey, pElement);
		if(pElement!=NULL)
		{
			pElement->ReSet();
		}
	}
}
void CCycleBlock::CalcIter() //���е���
{
	for(int i=0;i<IterVarArr.GetSize();i++)
	{
		if(IterVarArr[i]=="")
			break;
		CElementInOut *pElement=block_WorkSpaceList[IterVarArr[i]];
		if(pElement!=NULL)
		{
			pElement->ReSet();//��λ��ʹ֮δ����
			pElement->CalcFormula();
		}
	}
}
//���㺯����ʵ�ֱ�ģ��ļ���
void CCycleBlock::Calc() //���м���ĺ���,��Ҫ����
{
	setAllElemReSet();
	GetInputValueFromGlobalWS();
	
	int inNum = GetInputNum();
	for(int k=0;k<inNum;k++)
	{
		CCalcPort *pPortObj = GetInputPortObj(k);
		CUniValue  uniValue=pPortObj->GetPortUniValue();
		CElementInOut *pElement=block_WorkSpaceList[pPortObj->GetPortID()];
		setDataToElem(uniValue,pElement,pPortObj->GetPortValueType());
		pElement=blockIter_WorkSpaceList[pPortObj->GetPortID()];
		setDataToElem(uniValue,pElement,pPortObj->GetPortValueType());//���ڶ���Map�е��������Ҳ��ֵ
	}
	
	for(int i=0;i<IterFVarArr.GetSize();i++)//���㲢��block_WorkSpaceList��ͬ��������ֵ
	{
		if(IterFVarArr[i]=="")
			break;
		CElementInOut *pElement=blockIter_WorkSpaceList[IterFVarArr[i]];
		if(pElement!=NULL)
		{
			pElement->CalcFormula();
			CElementInOut *pElementBlock;
			if(block_WorkSpaceList.Lookup(IterFVarArr[i],pElementBlock))//�ҵ���ֵ
			{
				pElementBlock->PValue=pElement->PValue;
				pElementBlock->PState=pElement->PState;
				pElementBlock->PTime=pElement->PTime;
			}
			else
			{
				block_WorkSpaceList.SetAt(IterFVarArr[i],pElement);
			}
		}
	}
	setBlokcElemBeCalc();
	int count=0;
	if(tagConFOrB==0)//����ǰ������������whileģʽ
	{
		while(calcCon())
		{
			CalcIter();
			count++;
			if(count==1000000)
			{
				CString str;
				str.Format("���������--%s   ����ѭ�������Ѵﵽ1000000�β��ж��˳��������������",GetBlockName());
				if(m_iDebugOn)//����ģʽ
					AfxMessageBox(str);
				else if(m_iTraceOn)//����ģʽ,��׷����Ϣ
				{
					if(m_pWnd!=NULL)
						m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
				}
				if(m_iLogOnOrOff)
				{
					if(m_pLog!=NULL)
						m_pLog->WriteFileData(str);
				}
				break;
			}
		}
	}
	else if(tagConFOrB==1)//�����������������do-whileģʽ
	{
		do
		{
			CalcIter();
			if(count==1000000)
			{
				CString str;
				str.Format("���������--%s   ����ѭ�������Ѵﵽ1000000�β��ж��˳��������������",GetBlockName());
				if(m_iDebugOn)//����ģʽ
					AfxMessageBox(str);
				else if(m_iTraceOn)//����ģʽ,��׷����Ϣ
				{
					if(m_pWnd!=NULL)
						m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
				}
				if(m_iLogOnOrOff)
				{
					if(m_pLog!=NULL)
						m_pLog->WriteFileData(str);
				}
				break;
			}
		}
		while(calcCon());
	}
	for(i=0;i<IterBVarArr.GetSize();i++)
	{
		CElementInOut *pElement=block_WorkSpaceList[IterBVarArr[i]];
		if(pElement!=NULL)
		{
			pElement->ReSet();
			pElement->CalcFormula();
		}
	}
	inNum = GetOutputNum();
	for(i=0;i<inNum;i++)
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
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CCycleBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName=="tagConFOrB")
	{
		if(!strItem1.IsEmpty())  {tagConFOrB=atoi(strItem1);}
	}
	int propType,propIndex;
	getPropTypeByName(strPropName,propType,propIndex);
	if(propType==1)
	{
		if(!strItem1.IsEmpty())  {inPutArr[propIndex]=strItem1;}
		if(!strItem2.IsEmpty())  {inPutTypeArr[propIndex]=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {inPutCanBeEdit[propIndex]=atoi(strItem3);}
	}
	else if(propType==2)
	{
		if(!strItem1.IsEmpty())  {outPutArr[propIndex]=strItem1;}
		if(!strItem2.IsEmpty())  {outPutTypeArr[propIndex]=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {outPutCanBeEdit[propIndex]=atoi(strItem3);}
	}
	else if(propType==3)
	{
		if(!strItem1.IsEmpty())  {IterFVarArr[propIndex]=strItem1;}
		if(!strItem2.IsEmpty())  {IterFVarTypeArr[propIndex]=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {IterFFormulaArr[propIndex]=strItem3;}
		if(!strItem4.IsEmpty())  {IterFCanBeEdit[propIndex]=atoi(strItem4);}
		if(!strItem5.IsEmpty())  {IterFCanBeShow[propIndex]=atoi(strItem5);}
	}
	else if(propType==4)
	{
		if(!strItem1.IsEmpty())  {conVarArr[propIndex]=strItem1;}
		if(!strItem2.IsEmpty())  {conVarTypeArr[propIndex]=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {conFormulaArr[propIndex]=strItem3;}
		if(!strItem4.IsEmpty())  {conVarCanBeEdit[propIndex]=atoi(strItem4);}
		if(!strItem5.IsEmpty())  {conVarCanBeShow[propIndex]=atoi(strItem5);}
	}
	else if(propType==5)
	{
		if(!strItem1.IsEmpty())  {IterVarArr[propIndex]=strItem1;}
		if(!strItem2.IsEmpty())  {IterVarTypeArr[propIndex]=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {IterFormulaArr[propIndex]=strItem3;}
		if(!strItem4.IsEmpty())  {IterCanBeEdit[propIndex]=atoi(strItem4);}
		if(!strItem5.IsEmpty())  {IterCanBeShow[propIndex]=atoi(strItem5);}
	}
	else if(propType==6)
	{
		if(!strItem1.IsEmpty())  {IterBVarArr[propIndex]=strItem1;}
		if(!strItem2.IsEmpty())  {IterBVarTypeArr[propIndex]=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {IterBFormulaArr[propIndex]=strItem3;}
		if(!strItem4.IsEmpty())  {IterBCanBeEdit[propIndex]=atoi(strItem4);}
		if(!strItem5.IsEmpty())  {IterBCanBeShow[propIndex]=atoi(strItem5);}
	}
}
void CCycleBlock::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
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
	else if(strName=="IterF")//����ǰΪ3
	{
		propType=3;
	}
	else if(strName=="con")//����Ϊ4
	{
		propType=4;
	}
	else if(strName=="Iter")//����Ϊ5
	{
		propType=5;
	}
	else if(strName=="IterB")//������Ϊ6
	{
		propType=6;
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CCycleBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgIter dlg;

	int portnumLinkBefore=this->GetInputNum();//D???
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrLeftOut;//?????��?-��D��?��?��????��?����?
	CStringArray  IDArrRightOut;//?????��?-��D��?��?3????��?����?
	IDArrLeftOut.RemoveAll();
	IDArrRightOut.RemoveAll();

	CStringArray  IDArrLeftIn;//?????��?��2?��?��?��????��?����?
	CStringArray  IDArrRightIn;//?????��?��2?��?��?3????��?����?
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

    //���Ի��������ֵ
	///////////////////////////////////////////////////
	dlg.SetPropData(inPutArr,inPutTypeArr,outPutArr,outPutTypeArr,
		IterFVarArr,IterFVarTypeArr,IterFFormulaArr,
		conVarArr,conVarTypeArr,conFormulaArr,
		IterVarArr,IterVarTypeArr,IterFormulaArr,
		IterBVarArr,IterBVarTypeArr,IterBFormulaArr);
	dlg.SetTagConFOrB(this->tagConFOrB);
	dlg.SetCanBeEditCanBeShow(
		inPutCanBeEdit,outPutCanBeEdit,
		IterFCanBeEdit,IterFCanBeShow,
		conVarCanBeEdit,conVarCanBeShow,
		IterCanBeEdit,IterCanBeShow,
		IterBCanBeEdit,IterBCanBeShow);
	if(dlg.DoModal()==IDOK)
	{
		tagConFOrB=dlg.tagConFOrB;
		inPutArr.Copy(dlg.inPutArr);
		inPutTypeArr.Copy(dlg.inPutTypeArr);
		inPutCanBeEdit.Copy(dlg.inPutCanBeEdit);

		outPutArr.Copy(dlg.outPutArr);
		outPutTypeArr.Copy(dlg.outPutTypeArr);
		outPutCanBeEdit.Copy(dlg.outPutCanBeEdit);

		IterFVarArr.Copy(dlg.IterFVarArr);
		IterFVarTypeArr.Copy(dlg.IterFVarTypeArr);
		IterFFormulaArr.Copy(dlg.IterFFormulaArr);
		IterFCanBeEdit.Copy(dlg.IterFCanBeEdit);
		IterFCanBeShow.Copy(dlg.IterFCanBeShow);

		conVarArr.Copy(dlg.conVarArr);
		conVarTypeArr.Copy(dlg.conVarTypeArr);
		conFormulaArr.Copy(dlg.conFormulaArr);
		conVarCanBeEdit.Copy(dlg.conVarCanBeEdit);
		conVarCanBeShow.Copy(dlg.conVarCanBeShow);
		
		IterVarArr.Copy(dlg.IterVarArr);
		IterVarTypeArr.Copy(dlg.IterVarTypeArr);
		IterFormulaArr.Copy(dlg.IterFormulaArr);
		IterCanBeEdit.Copy(dlg.IterCanBeEdit);
		IterCanBeShow.Copy(dlg.IterCanBeShow);

		IterBVarArr.Copy(dlg.IterBVarArr);
		IterBVarTypeArr.Copy(dlg.IterBVarTypeArr);
		IterBFormulaArr.Copy(dlg.IterBFormulaArr);
		IterBCanBeEdit.Copy(dlg.IterBCanBeEdit);
		IterBCanBeShow.Copy(dlg.IterBCanBeShow);

        //1��������˿ڵ��޸�
		for(int i=0;i<inPutArr.GetSize();i++)
		{
			if(inPutArr[i]=="")
				break;
			IDArrLeftIn.Add(inPutArr[i]);
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
		for(i=0;i<outPutArr.GetSize();i++)
		{
			if(outPutArr[i]=="")
				break;
			IDArrRightIn.Add(outPutArr[i]);
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
		parent->RedrawWindow();
		if((m_pModel!=NULL)&&(this->m_pModel->m_iTagModeLocalOrRemote==1))//Զ��
		{
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> dataArr;
			SetBlockPropertyToTransferData(dataArr);//��ȡҪ��������Ե�����������
			SendBlockPropertyTransferData(dataArr);// ��������������
			ClearPropertyTransferData(dataArr);//�ͷ�����
		}
		parent->RedrawWindow();
	}
}
//�����Խ���ģ��д�뵽���ݿ���
void CCycleBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue.Format("tagConFOrB");
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	
	strFieldValue.Format("%d",tagConFOrB); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	

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
		strFieldValue.Format("%d",inPutCanBeEdit[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
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
		strFieldValue.Format("%d",outPutCanBeEdit[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
	}
	for(i=0;i<IterFVarArr.GetSize();i++)//����ǰ
	{
		CString strVar=IterFVarArr[i];
		if(strVar=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("IterF.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������

		strFieldValue.Format("%s",IterFVarArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		
		strFieldValue.Format("%d",IterFVarTypeArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	

		strFieldValue.Format("%s",IterFFormulaArr[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	

		strFieldValue.Format("%d",IterFCanBeEdit[i]); //������
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������	

		strFieldValue.Format("%d",IterFCanBeShow[i]); //������
		pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //������	

	}
	for(i=0;i<conVarArr.GetSize();i++)//��������
	{
		CString strVar=conVarArr[i];
		if(strVar=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("con.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������

		strFieldValue.Format("%s",conVarArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		
		strFieldValue.Format("%d",conVarTypeArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	

		strFieldValue.Format("%s",conFormulaArr[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	

		strFieldValue.Format("%d",conVarCanBeEdit[i]); //������
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������	

		strFieldValue.Format("%d",conVarCanBeShow[i]); //������
		pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //������
	}
	for(i=0;i<IterVarArr.GetSize();i++)//��������
	{
		CString strVar=IterVarArr[i];
		if(strVar=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("Iter.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������

		strFieldValue.Format("%s",IterVarArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		
		strFieldValue.Format("%d",IterVarTypeArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	

		strFieldValue.Format("%s",IterFormulaArr[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	

		strFieldValue.Format("%d",IterCanBeEdit[i]); //������
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������	

		strFieldValue.Format("%d",IterCanBeShow[i]); //������
		pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //������
	}
	for(i=0;i<IterBVarArr.GetSize();i++)//���������
	{
		CString strVar=IterBVarArr[i];
		if(strVar=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("IterB.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������

		strFieldValue.Format("%s",IterBVarArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		
		strFieldValue.Format("%d",IterBVarTypeArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	

		strFieldValue.Format("%s",IterBFormulaArr[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	

		strFieldValue.Format("%d",IterBCanBeEdit[i]); //������
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������	

		strFieldValue.Format("%d",IterBCanBeShow[i]); //������
		pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //������
	}
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CCycleBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	maxInPut=((CCycleBlock*)pBlock)->maxInPut;
	maxOutPut=((CCycleBlock*)pBlock)->maxOutPut;
	maxFormula=((CCycleBlock*)pBlock)->maxFormula;
	tagConFOrB=((CCycleBlock*)pBlock)->tagConFOrB;

	inPutArr.Copy(((CCycleBlock*)pBlock)->inPutArr);
	inPutTypeArr.Copy(((CCycleBlock*)pBlock)->inPutTypeArr);
	inPutCanBeEdit.Copy(((CCycleBlock*)pBlock)->inPutCanBeEdit);

	outPutArr.Copy(((CCycleBlock*)pBlock)->outPutArr);
	outPutTypeArr.Copy(((CCycleBlock*)pBlock)->outPutTypeArr);
	outPutCanBeEdit.Copy(((CCycleBlock*)pBlock)->outPutCanBeEdit);

	IterFVarArr.Copy(((CCycleBlock*)pBlock)->IterFVarArr);
	IterFFormulaArr.Copy(((CCycleBlock*)pBlock)->IterFFormulaArr);
	IterFVarTypeArr.Copy(((CCycleBlock*)pBlock)->IterFVarTypeArr);
	IterFCanBeEdit.Copy(((CCycleBlock*)pBlock)->IterFCanBeEdit);
	IterFCanBeShow.Copy(((CCycleBlock*)pBlock)->IterFCanBeShow);
	
	conVarArr.Copy(((CCycleBlock*)pBlock)->conVarArr);
	conFormulaArr.Copy(((CCycleBlock*)pBlock)->conFormulaArr);
	conVarTypeArr.Copy(((CCycleBlock*)pBlock)->conVarTypeArr);
	conVarCanBeEdit.Copy(((CCycleBlock*)pBlock)->conVarCanBeEdit);
	conVarCanBeShow.Copy(((CCycleBlock*)pBlock)->conVarCanBeShow);

	IterVarArr.Copy(((CCycleBlock*)pBlock)->IterVarArr);
	IterFormulaArr.Copy(((CCycleBlock*)pBlock)->IterFormulaArr);
	IterVarTypeArr.Copy(((CCycleBlock*)pBlock)->IterVarTypeArr);
	IterCanBeEdit.Copy(((CCycleBlock*)pBlock)->IterCanBeEdit);
	IterCanBeShow.Copy(((CCycleBlock*)pBlock)->IterCanBeShow);

	IterBVarArr.Copy(((CCycleBlock*)pBlock)->IterBVarArr);
	IterBFormulaArr.Copy(((CCycleBlock*)pBlock)->IterBFormulaArr);
	IterBVarTypeArr.Copy(((CCycleBlock*)pBlock)->IterBVarTypeArr);
	IterBCanBeEdit.Copy(((CCycleBlock*)pBlock)->IterBCanBeEdit);
	IterBCanBeShow.Copy(((CCycleBlock*)pBlock)->IterBCanBeShow);
}
//�����Է��ô���������
void CCycleBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"tagConFOrB");
	CString str;
	str.Format("%d",tagConFOrB);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	CString strFieldValue;
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
		str.Format("%d",inPutCanBeEdit[i]);
		strcpy(pData->m_cValueItem3,str);
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
		str.Format("%d",outPutCanBeEdit[i]);
		strcpy(pData->m_cValueItem3,str);
		dataArr.Add(pData);
	}
	for(i=0;i<IterFVarArr.GetSize();i++)//����ǰ
	{
		CString strVar=IterFVarArr[i];
		if(strVar=="")
			break;
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		strFieldValue.Format("IterF.%d",i);
		strcpy(pData->m_cPropName,strFieldValue);
		str.Format("%s",IterFVarArr[i]);
		strcpy(pData->m_cValueItem1,str);
		str.Format("%d",IterFVarTypeArr[i]);
		strcpy(pData->m_cValueItem2,str);
		str.Format("%s",IterFFormulaArr[i]);
		strcpy(pData->m_cValueItem3,str);
		str.Format("%d",IterFCanBeEdit[i]);
		strcpy(pData->m_cValueItem4,str);
		str.Format("%d",IterFCanBeShow[i]);
		strcpy(pData->m_cValueItem5,str);
		dataArr.Add(pData);
	}
	for(i=0;i<IterFVarArr.GetSize();i++)//����ǰ
	{
		CString strVar=IterFVarArr[i];
		if(strVar=="")
			break;
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		strFieldValue.Format("con.%d",i);
		strcpy(pData->m_cPropName,strFieldValue);
		str.Format("%s",conVarArr[i]);
		strcpy(pData->m_cValueItem1,str);
		str.Format("%d",conVarTypeArr[i]);
		strcpy(pData->m_cValueItem2,str);
		str.Format("%s",conFormulaArr[i]);
		strcpy(pData->m_cValueItem3,str);
		str.Format("%d",conVarCanBeEdit[i]);
		strcpy(pData->m_cValueItem4,str);
		str.Format("%d",conVarCanBeShow[i]);
		strcpy(pData->m_cValueItem5,str);
		dataArr.Add(pData);
	}
	for(i=0;i<IterVarArr.GetSize();i++)//��������
	{
		CString strVar=IterVarArr[i];
		if(strVar=="")
			break;
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		strFieldValue.Format("Iter.%d",i);
		strcpy(pData->m_cPropName,strFieldValue);
		str.Format("%s",IterVarArr[i]);
		strcpy(pData->m_cValueItem1,str);
		str.Format("%d",IterVarTypeArr[i]);
		strcpy(pData->m_cValueItem2,str);
		str.Format("%s",IterFormulaArr[i]);
		strcpy(pData->m_cValueItem3,str);
		str.Format("%d",IterCanBeEdit[i]);
		strcpy(pData->m_cValueItem4,str);
		str.Format("%d",IterCanBeShow[i]);
		strcpy(pData->m_cValueItem5,str);
		dataArr.Add(pData);
	}
	for(i=0;i<IterBVarArr.GetSize();i++)//���������
	{
		CString strVar=IterBVarArr[i];
		if(strVar=="")
			break;
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		strFieldValue.Format("IterB.%d",i);
		strcpy(pData->m_cPropName,strFieldValue);
		str.Format("%s",IterBVarArr[i]);
		strcpy(pData->m_cValueItem1,str);
		str.Format("%d",IterBVarTypeArr[i]);
		strcpy(pData->m_cValueItem2,str);
		str.Format("%s",IterBFormulaArr[i]);
		strcpy(pData->m_cValueItem3,str);
		str.Format("%d",IterBCanBeEdit[i]);
		strcpy(pData->m_cValueItem4,str);
		str.Format("%d",IterBCanBeShow[i]);
		strcpy(pData->m_cValueItem5,str);
		dataArr.Add(pData);
	}
}
