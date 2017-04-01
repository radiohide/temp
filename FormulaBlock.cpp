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
	maxInPut=20;//最大输入个数
	maxOutPut=10;//最大输出个数
	maxFormula=20;//最大公式条数（最大中间变量个数也是这个）

	inPutArr.RemoveAll();
	for(int i=0;i<maxInPut;i++)
		inPutArr.Add("");

	inPutTypeArr.RemoveAll();
	for(i=0;i<maxInPut;i++)
		inPutTypeArr.Add(0);//默认为数值型

	outPutArr.RemoveAll();
	for(i=0;i<maxOutPut;i++)
		outPutArr.Add("");//默认为数值型
	
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
	SetBlockType(_T( "Formula" ) );//设置类型
	ConstructBlockInputOutput ();
}

CFormulaBlock::~CFormulaBlock()
{
	delInnerWorkSpace();
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CFormulaBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("x"), VALUE_DOUBLE);//默认构造单输入单输出
	AddOutputPort("y",VALUE_DOUBLE);
} 
//根据类型新建对象的函数
CCalcBlock* CFormulaBlock::CreateFromString( const CString& strType )
{
	CFormulaBlock* obj = new CFormulaBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CFormulaBlock::establishInnerWorkSpace()
{
	delInnerWorkSpace();
	CString strVarName;
	CElementInOut *pElement =NULL;
	for(int i=0;i<this->inPutArr.GetSize();i++)//输入点
	{
		strVarName=inPutArr[i];
		if(strVarName=="")
			break;
		pElement=new CElementInOut(strVarName,inPutTypeArr[i],this);
		block_WorkSpaceList.SetAt(strVarName,pElement);
	}
	for(i=0;i<this->varArr.GetSize();i++)//计算点，需要进行公式计算的点
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
			if(pElement->PType==1)//计算点
				pElement->ParseFormula();//初始化公式二叉树
		}	
	}
}
void CFormulaBlock::delInnerWorkSpace()//清空Map
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
void CFormulaBlock::setDataToElem(CUniValue &uniValue,CElementInOut *pElement,int type)//type:0,单值点；1,序列值
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
void CFormulaBlock::setDataToUniValue(CElementInOut *pElement,CUniValue &uniValue,int type)//type:0,单值点；1,序列值
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

void CFormulaBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	establishInnerWorkSpace();//初始化Map
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
//计算函数，实现本模块的计算
void CFormulaBlock::Calc() //进行计算的函数,需要重载
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
	//输出到工作空间中
	OutputResultToGlobalWS();
}
//返回属性的逗点字符串
CString CFormulaBlock::GetBlockStrPro()
{
	CString str;

	for(int i=0;i<maxInPut;i++)//输入
		str+=inPutArr[i]+"@";

	for(i=0;i<maxOutPut;i++)//输出
		str+=outPutArr[i]+"@";

	for(i=0;i<maxFormula;i++)//变量
		str+=varArr[i]+"@";

	for(i=0;i<maxFormula;i++)//公式
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
void CFormulaBlock::SetBlockStrPro(CString str)//设置计算块的字符串属性
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
//用于根据参数项和值进行属性的设置，当读取值要用
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

	if(strName=="input")//输入为1
	{
		propType=1;
	}
	else if(strName=="output")//输出为2
	{
		propType=2;
	}
	else if(strName=="formula")//公式为3
	{
		propType=3;
	}
}
//重载以显示不同的属性对象对话框
void CFormulaBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgFormula dlg;
	
	int portnumLinkBefore=this->GetInputNum();//新增
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrLeftOut;//计算块原有的输入端口描述
	CArray<int,int> IDArrLeftOutType;//计算块原有的输入端口类型
	CStringArray  IDArrRightOut;//计算块原有的输出端口描述
	CArray<int,int> IDArrRightOutType;//计算块原有的输入端口类型
	
	CStringArray  IDArrLeftIn;//计算块内部的输入端口描述
	CArray<int,int>  IDArrLeftInType;//计算块内部的输入端口类型
	CStringArray  IDArrRightIn;//计算块内部的输出端口描述
	CArray<int,int>  IDArrRightInType;//计算块内部的输出端口类型
	

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

    //给对话框变量赋值
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

        //1、对输入端口的修改
		for(int i=0;i<inPutArr.GetSize();i++)
		{
			if(inPutArr[i]=="")
				break;
			IDArrLeftIn.Add(inPutArr[i]);
			IDArrLeftInType.Add(inPutTypeArr[i]);
		}	
		for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)	//删除原有而不用的Port,倒着删
		{
			int  runs=0;//遍历次数
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
		for(int m=0; m<IDArrLeftIn.GetSize(); m++)	//添加新的Port
		{
			int  runs=0;//遍历次数
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

        //2、对输出端口的修改
		for(i=0;i<outPutArr.GetSize();i++)
		{
			if(outPutArr[i]=="")
				break;
			IDArrRightIn.Add(outPutArr[i]);
			IDArrRightInType.Add(outPutTypeArr[i]);
		}		
		for(int jj=IDArrRightOut.GetSize()-1; jj>-1; jj--)//删除原有而不用的Port,倒着删
		{
			int  runs=0;//遍历次数
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
		for(int mm=0; mm<IDArrRightIn.GetSize(); mm++)//添加新的Port
		{
			int  runs=0;//遍历次数
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
		if((m_pModel!=NULL)&&(this->m_pModel->m_iTagModeLocalOrRemote==1))//远程
		{
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> dataArr;
			SetBlockPropertyToTransferData(dataArr);//获取要传输的属性到传送数组中
			SendBlockPropertyTransferData(dataArr);// 传送数组中数据
			ClearPropertyTransferData(dataArr);//释放数据
		}
	}
}
//重载以将本模块写入到数据库中
void CFormulaBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
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
	}
	for(i=0;i<varArr.GetSize();i++)//公式
	{
		CString strVar=varArr[i];
		if(strVar=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("formula.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%s",varArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		
		strFieldValue.Format("%d",varTypeArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	

		strFieldValue.Format("%s",formulaArr[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	}
	pRecord->Update();
}
//复制图形时，需要拷贝属性。
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
//将属性放置传输数据中
void CFormulaBlock::SetBlockPropertyToTransferData(
								CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=NULL;
	CString strFieldValue,str;
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
		dataArr.Add(pData);
	}
	for(i=0;i<varArr.GetSize();i++)//公式
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


