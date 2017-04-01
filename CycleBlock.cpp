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
	maxInPut=200;//最大输入个数
	maxOutPut=200;//最大输出个数
	maxFormula=400;//最大公式条数（最大中间变量个数也是这个）
	tagConFOrB=1;//默认在迭代后计算

	inPutArr.RemoveAll();
	for(int i=0;i<maxInPut;i++)
		inPutArr.Add("");
	inPutTypeArr.RemoveAll();
	for(i=0;i<maxInPut;i++)
		inPutTypeArr.Add(0);//默认为数值型
	inPutCanBeEdit.RemoveAll();
	for(i=0;i<maxInPut;i++)
		inPutCanBeEdit.Add(1);//默认为可被编辑

	outPutArr.RemoveAll();
	for(i=0;i<maxOutPut;i++)
		outPutArr.Add("");//默认为数值型	
	outPutTypeArr.RemoveAll();
	for(i=0;i<maxOutPut;i++)
		outPutTypeArr.Add(0);
	outPutCanBeEdit.RemoveAll();
	for(i=0;i<maxOutPut;i++)
		outPutCanBeEdit.Add(1);//默认为可被编辑

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
	SetBlockType(_T( "Cycle" ) );//设置类型
	ConstructBlockInputOutput ();
}

CCycleBlock::~CCycleBlock()
{
	DelInnerWorkSpace();
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CCycleBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("x"), VALUE_DOUBLE, _T("in1"), _T("in1"));//默认构造单输入单输出
	AddOutputPort("y",VALUE_DOUBLE,"out");
}
//根据类型新建对象的函数
CCalcBlock* CCycleBlock::CreateFromString( const CString& strType )
{
	CCycleBlock* obj = new CCycleBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CCycleBlock::DelInnerWorkSpace()//清空Map
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
	for(int i=0;i<this->inPutArr.GetSize();i++)//输入点，在两个Map中
	{
		strVarName=inPutArr[i];
		if(strVarName=="")
			break;
		pElement=new CElementInOut(strVarName,inPutTypeArr[i],this);
		block_WorkSpaceList.SetAt(strVarName,pElement);

		pElement=new CElementInOut(strVarName,inPutTypeArr[i],this);
		pElement->SetPtrMap(&blockIter_WorkSpaceList);//需设置MapPtr
		blockIter_WorkSpaceList.SetAt(strVarName,pElement);
	}
}
void CCycleBlock::BulidIterFEle()
{
	CString strVarName;
	CElementInOut *pElement =NULL;
	for(int i=0;i<this->IterFVarArr.GetSize();i++)//计算点，迭代前变量
	{
		strVarName=IterFVarArr[i];
		if(strVarName=="")
			break;
		pElement=new CElementInOut(strVarName,IterFVarTypeArr[i],IterFFormulaArr[i]+"@",this);
		pElement->SetPtrMap(&blockIter_WorkSpaceList);
		blockIter_WorkSpaceList.SetAt(strVarName,pElement);
		CElementInOut *pElementBlock;
		if(!block_WorkSpaceList.Lookup(IterFVarArr[i],pElementBlock))//若没有找到该变量，则加入block_WorkSpaceList
		{
			block_WorkSpaceList.SetAt(IterFVarArr[i],pElement);
		}
	}
}
void CCycleBlock::BulidConVarEle()
{
	CString strVarName;
	CElementInOut *pElement =NULL;
	for(int i=0;i<this->conVarArr.GetSize();i++)//计算点，条件变量
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
	for(int i=0;i<this->IterVarArr.GetSize();i++)//计算点，迭代变量
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
	for(int i=0;i<this->IterBVarArr.GetSize();i++)//计算点，迭代后变量
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
			if(pElement->PType==1)//计算点
				pElement->ParseFormula();//初始化公式二叉树
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
			if(pElement->PType==1)//计算点
				pElement->ParseFormula();//初始化公式二叉树
		}
	}
}

//初始化计算的函数做特殊的初始化
void CCycleBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	DelInnerWorkSpace();
	BulidInPutEle();
	BulidConVarEle();
	BulidIterEle();
	BulidIterBEle();
	BulidIterFEle();//要在最后创建，其要给block_WorkSpaceList补充元素
	ElementParseFormula();
}
void CCycleBlock::setDataToElem(CUniValue &uniValue,CElementInOut *pElement,int type)//type:0,单值点；1,序列值
{
	long dateSize=uniValue.GetDataSize();
	if(dateSize>0)//有数值空间
	{
		if(type==0)//单值
		{
			uniValue.GetDoubleVal(&(pElement->PValue));
		}
		else if(type==1)//序列值
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
void CCycleBlock::setDataToUniValue(CElementInOut *pElement,CUniValue &uniValue,int type)//type:0,单值点；1,序列值
{
	if(type==0)//单值
	{
		uniValue.AddData(pElement->PValue);
	}
	else if(type==1)//序列值
	{
		for(long i=0;i<pElement->NumData;i++)
		{
			uniValue.AddData(pElement->pDataAll[i].dValue);
		}
	}
}

bool CCycleBlock::calcCon() //进行条件
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
			pElement->ReSet();//复位，使之未计算
			pElement->CalcFormula();
			if(pElement->PValue==0)
			{
				result=false;
			}
		}
	}
	return result;
}
void CCycleBlock::setBlokcElemBeCalc() //设置所有元素被计算过
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
void CCycleBlock::setAllElemReSet() //设置所有元素未被计算过
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
void CCycleBlock::CalcIter() //进行迭代
{
	for(int i=0;i<IterVarArr.GetSize();i++)
	{
		if(IterVarArr[i]=="")
			break;
		CElementInOut *pElement=block_WorkSpaceList[IterVarArr[i]];
		if(pElement!=NULL)
		{
			pElement->ReSet();//复位，使之未计算
			pElement->CalcFormula();
		}
	}
}
//计算函数，实现本模块的计算
void CCycleBlock::Calc() //进行计算的函数,需要重载
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
		setDataToElem(uniValue,pElement,pPortObj->GetPortValueType());//给第二个Map中的输入变量也赋值
	}
	
	for(int i=0;i<IterFVarArr.GetSize();i++)//计算并给block_WorkSpaceList中同名变量赋值
	{
		if(IterFVarArr[i]=="")
			break;
		CElementInOut *pElement=blockIter_WorkSpaceList[IterFVarArr[i]];
		if(pElement!=NULL)
		{
			pElement->CalcFormula();
			CElementInOut *pElementBlock;
			if(block_WorkSpaceList.Lookup(IterFVarArr[i],pElementBlock))//找到后赋值
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
	if(tagConFOrB==0)//迭代前计算条件，既while模式
	{
		while(calcCon())
		{
			CalcIter();
			count++;
			if(count==1000000)
			{
				CString str;
				str.Format("迭代计算块--%s   迭代循环次数已达到1000000次并中断退出，请检查迭代条件",GetBlockName());
				if(m_iDebugOn)//调试模式
					AfxMessageBox(str);
				else if(m_iTraceOn)//运行模式,且追踪信息
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
	else if(tagConFOrB==1)//迭代后计算条件，既do-while模式
	{
		do
		{
			CalcIter();
			if(count==1000000)
			{
				CString str;
				str.Format("迭代计算块--%s   迭代循环次数已达到1000000次并中断退出，请检查迭代条件",GetBlockName());
				if(m_iDebugOn)//调试模式
					AfxMessageBox(str);
				else if(m_iTraceOn)//运行模式,且追踪信息
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
	//输出到工作空间中
	OutputResultToGlobalWS();
}
//用于根据参数项和值进行属性的设置，当读取值要用
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

	if(strName=="input")//输入为1
	{
		propType=1;
	}
	else if(strName=="output")//输出为2
	{
		propType=2;
	}
	else if(strName=="IterF")//迭代前为3
	{
		propType=3;
	}
	else if(strName=="con")//条件为4
	{
		propType=4;
	}
	else if(strName=="Iter")//迭代为5
	{
		propType=5;
	}
	else if(strName=="IterB")//迭代后为6
	{
		propType=6;
	}
}
//重载以显示不同的属性对象对话框
void CCycleBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgIter dlg;

	int portnumLinkBefore=this->GetInputNum();//D???
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrLeftOut;//?????é?-óDμ?ê?è????ú?èê?
	CStringArray  IDArrRightOut;//?????é?-óDμ?ê?3????ú?èê?
	IDArrLeftOut.RemoveAll();
	IDArrRightOut.RemoveAll();

	CStringArray  IDArrLeftIn;//?????é?ú2?μ?ê?è????ú?èê?
	CStringArray  IDArrRightIn;//?????é?ú2?μ?ê?3????ú?èê?
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

    //给对话框变量赋值
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

        //1、对输入端口的修改
		for(int i=0;i<inPutArr.GetSize();i++)
		{
			if(inPutArr[i]=="")
				break;
			IDArrLeftIn.Add(inPutArr[i]);
		}	
		for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)	//删除原有而不用的Port,倒着删
		{
			int  runs=0;//遍历次数
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
		for(int m=0; m<IDArrLeftIn.GetSize(); m++)	//添加新的Port
		{
			int  runs=0;//遍历次数
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

        //2、对输出端口的修改
		for(i=0;i<outPutArr.GetSize();i++)
		{
			if(outPutArr[i]=="")
				break;
			IDArrRightIn.Add(outPutArr[i]);
		}		
		for(int jj=IDArrRightOut.GetSize()-1; jj>-1; jj--)//删除原有而不用的Port,倒着删
		{
			int  runs=0;//遍历次数
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
		for(int mm=0; mm<IDArrRightIn.GetSize(); mm++)//添加新的Port
		{
			int  runs=0;//遍历次数
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
		if((m_pModel!=NULL)&&(this->m_pModel->m_iTagModeLocalOrRemote==1))//远程
		{
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> dataArr;
			SetBlockPropertyToTransferData(dataArr);//获取要传输的属性到传送数组中
			SendBlockPropertyTransferData(dataArr);// 传送数组中数据
			ClearPropertyTransferData(dataArr);//释放数据
		}
		parent->RedrawWindow();
	}
}
//重载以将本模块写入到数据库中
void CCycleBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue.Format("tagConFOrB");
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	
	strFieldValue.Format("%d",tagConFOrB); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	for(int i=0;i<inPutArr.GetSize();i++)//输入
	{
		CString strInPut=inPutArr[i];
		if(strInPut=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("input.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%s",inPutArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%d",inPutTypeArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%d",inPutCanBeEdit[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	}
	for(i=0;i<outPutArr.GetSize();i++)//输出
	{
		CString strOutPut=outPutArr[i];
		if(strOutPut=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("output.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%s",outPutArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%d",outPutTypeArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%d",outPutCanBeEdit[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	}
	for(i=0;i<IterFVarArr.GetSize();i++)//迭代前
	{
		CString strVar=IterFVarArr[i];
		if(strVar=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("IterF.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%s",IterFVarArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		
		strFieldValue.Format("%d",IterFVarTypeArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	

		strFieldValue.Format("%s",IterFFormulaArr[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	

		strFieldValue.Format("%d",IterFCanBeEdit[i]); //属性名
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名	

		strFieldValue.Format("%d",IterFCanBeShow[i]); //属性名
		pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //属性名	

	}
	for(i=0;i<conVarArr.GetSize();i++)//条件变量
	{
		CString strVar=conVarArr[i];
		if(strVar=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("con.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%s",conVarArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		
		strFieldValue.Format("%d",conVarTypeArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	

		strFieldValue.Format("%s",conFormulaArr[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	

		strFieldValue.Format("%d",conVarCanBeEdit[i]); //属性名
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名	

		strFieldValue.Format("%d",conVarCanBeShow[i]); //属性名
		pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //属性名
	}
	for(i=0;i<IterVarArr.GetSize();i++)//迭代变量
	{
		CString strVar=IterVarArr[i];
		if(strVar=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("Iter.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%s",IterVarArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		
		strFieldValue.Format("%d",IterVarTypeArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	

		strFieldValue.Format("%s",IterFormulaArr[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	

		strFieldValue.Format("%d",IterCanBeEdit[i]); //属性名
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名	

		strFieldValue.Format("%d",IterCanBeShow[i]); //属性名
		pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //属性名
	}
	for(i=0;i<IterBVarArr.GetSize();i++)//迭代后变量
	{
		CString strVar=IterBVarArr[i];
		if(strVar=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("IterB.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%s",IterBVarArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		
		strFieldValue.Format("%d",IterBVarTypeArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	

		strFieldValue.Format("%s",IterBFormulaArr[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	

		strFieldValue.Format("%d",IterBCanBeEdit[i]); //属性名
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名	

		strFieldValue.Format("%d",IterBCanBeShow[i]); //属性名
		pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //属性名
	}
	pRecord->Update();
}
//复制图形时，需要拷贝属性。
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
//将属性放置传输数据中
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
	for(int i=0;i<inPutArr.GetSize();i++)//输入
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
	for(i=0;i<outPutArr.GetSize();i++)//输出
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
	for(i=0;i<IterFVarArr.GetSize();i++)//迭代前
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
	for(i=0;i<IterFVarArr.GetSize();i++)//迭代前
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
	for(i=0;i<IterVarArr.GetSize();i++)//迭代变量
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
	for(i=0;i<IterBVarArr.GetSize();i++)//迭代后变量
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
