// OptimalRuleBlock.cpp: implementation of the COptimalRuleBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "OptimalRuleBlock.h"
#include "PropertyDlgOptimalRule.h"
#include "RuleRecommendListArr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptimalRuleBlock::COptimalRuleBlock()
{
	m_RuleRecommendListArr=NULL;
	m_strDBName="";
	m_iUnit=-1;
	m_iSet=-1;
	m_iRuleID=-1; 

	SetBlockType(_T( "OptimalRule" ) );
	ConstructBlockInputOutput();
}

COptimalRuleBlock::~COptimalRuleBlock()
{
	if(m_RuleRecommendListArr!=NULL)
	{
		delete m_RuleRecommendListArr;
		m_RuleRecommendListArr=NULL;
	}
}
CCalcBlock* COptimalRuleBlock::CreateFromString( const CString& strType )
{
	COptimalRuleBlock* obj = new COptimalRuleBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void COptimalRuleBlock::ConstructBlockInputOutput()
{
	AddInputPort(_T("Condition"), VALUE_RTVALUE_SERIES);
	AddInputPort(_T("IsStable"), VALUE_DOUBLE);
}
//默认该计算块的计算是可组态公式的基本计算
void COptimalRuleBlock::Calc()
{
	GetInputValueFromGlobalWS();
    //计算过程
	CCalcPort *pInputPortObj1 = GetInputPortObj(0);
	CCalcPort *pInputPortObj2 = GetInputPortObj(1);
	double beStable=0;
	pInputPortObj2->GetPortUniValue().GetDoubleVal(&beStable);
//	if(!beStable)//如果不稳定则退出该计算块。
//		return;
	CUniValue &input1UnitValue=pInputPortObj1->GetPortUniValue();
	CString strConCode="";
	for(long i=0;i<input1UnitValue.GetDataSize();i++)
	{
		double dChar=0;
		input1UnitValue.GetDoubleValAt(i,&dChar);
		strConCode+=(char)dChar;
	}
	//m_RuleRecommendListArr->SetConCode("L220E22M3XXV4");
	m_RuleRecommendListArr->SetConCode(strConCode);
	m_RuleRecommendListArr->ReInitRuleArr();//重新初始化
	if(m_RuleRecommendListArr->RuleArr.GetSize()>0)
	{
		RuleRecommendList *pRuleRecom=m_RuleRecommendListArr->RuleArr[0];
		pRuleRecom->RunRecom();
		for(int i=0;i<this->m_strVarNameArr.GetSize();i++)
		{
			CString subGMark=pRuleRecom->GetSubGMarkBySGID(m_iVarCGArr[i]);
			double EValue=0;
			if(getEValueBySGIDMarkVar(m_iVarCGArr[i],subGMark,m_strVarNameArr[i],EValue))
			{
				CCalcPort *pOutPortObj = GetOutputPortObj(i);
				CUniValue UniVar;
				UniVar.AddData(EValue);
				pOutPortObj->SetPortUniValue(UniVar);
			}			
		}
		//pRuleRecom->SaveRunRecom();
	}
	OutputResultToGlobalWS();
}

BOOL COptimalRuleBlock::getEValueBySGIDMarkVar(int iSubGID,CString SubGMark,CString varName,double &EValue)
{
	EValue=0;
	if(SubGMark=="")
		return FALSE;
	BOOL ret=FALSE;
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	_RecordsetPtr m_pRec;
	CString tableName;
	tableName.Format("t_u%ds%d_group%d_class_hist",m_iUnit,m_iSet,iSubGID);

	try   
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		//CString strSql("select * from ");
		//strSql+=tableName;
		CString strSql;
		//strSql.Format("SELECT * FROM `%s` Where class_mark='%s'",tableName,Mark);
		strSql.Format("SELECT * FROM `%s` Where class_mark='%s'",tableName,SubGMark);
        m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);				
	}   
	catch(_com_error  *e)   
	{   
		AfxMessageBox(e->ErrorMessage());
		ret= FALSE;
	}
	if(!m_pRec->adoEOF)
	{
		const CString conFieldTag=varName;
		CString strValue=(char *)_bstr_t(m_pRec->GetCollect((_variant_t)conFieldTag));
		EValue=atof(strValue);
		ret=TRUE;
	}
	return ret;
}
/////////////////////////////////////////////////////////////////////////
void COptimalRuleBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(strPropName.Compare("DBName")==0)
	{
		if(!strItem1.IsEmpty())  {m_strDBName=strItem1;}
	}
	else if(strPropName.Compare("UnitSet")==0)
	{
		if(!strItem1.IsEmpty())  {m_iUnit=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_iSet=atoi(strItem2);}
	}
	else if(strPropName.Compare("RuleID")==0)
	{
		if(!strItem1.IsEmpty())  {m_iRuleID=atoi(strItem1);}
	}
	else
	{
		int propType,propIndex;
		getPropTypeByName(strPropName,propType,propIndex);
		if(propType==1)
		{
			if(!strItem1.IsEmpty())  {m_iCGArr.Add(atoi(strItem1));}
			if(!strItem2.IsEmpty())  {m_strFunExpArr.Add(strItem2);}
			if(!strItem3.IsEmpty())  {m_strFunDepArr.Add(strItem3);}
		}
		else if(propType==2)
		{
			if(!strItem1.IsEmpty())  {m_iVarCGArr.Add(atoi(strItem1));}
			if(!strItem2.IsEmpty())  {m_strVarNameArr.Add(strItem2);}
			if(!strItem3.IsEmpty())  {m_strVarDespArr.Add(strItem3);}
			if(!strItem4.IsEmpty())  
			{
				m_StrVarUnit.Add(strItem4);
			}
			else
			{
				m_StrVarUnit.Add("");
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////


void COptimalRuleBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	if(m_RuleRecommendListArr==NULL)
	{
		m_RuleRecommendListArr=new RuleRecommendListArr;
		m_RuleRecommendListArr->SetRDBInf(this->m_pModel,m_strDBName,m_iUnit,m_iSet);
		m_RuleRecommendListArr->SetRuleListID(m_iRuleID);
		m_RuleRecommendListArr->SetFunExpData(&m_iCGArr,&m_strFunExpArr,&m_strFunDepArr);
		m_RuleRecommendListArr->InitRuleRecommendListArr();
	}
}
void COptimalRuleBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//计算块原有的输出端口描述
	IDArrRightOut.RemoveAll();

	CStringArray  IDArrRightIn;//计算块内部的输出端口描述
    IDArrRightIn.RemoveAll();

	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}

	CPropertyDlgOptimalRule dlg;
	dlg.SetPtrModel(this->m_pModel);
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);

	dlg.m_strDBName=m_strDBName;
	dlg.m_iUnit=m_iUnit;
	dlg.m_iSet=m_iSet;
	dlg.m_iRuleID=m_iRuleID;

	dlg.m_iCGArr.Copy(m_iCGArr);
	dlg.m_strFunExpArr.Copy(m_strFunExpArr);
	dlg.m_strFunDepArr.Copy(m_strFunDepArr);
	
	dlg.m_iVarCGArr.Copy(m_iVarCGArr);
	dlg.m_strVarNameArr.Copy(m_strVarNameArr);
	dlg.m_strVarDespArr.Copy(m_strVarDespArr);
	dlg.m_StrVarUnit.Copy(m_StrVarUnit);

	if(IDOK==dlg.DoModal())
	{
		m_strDBName=dlg.m_strDBName;
		m_iUnit=dlg.m_iUnit;
		m_iSet=dlg.m_iSet;
		m_iRuleID=dlg.m_iRuleID;
		
		m_iCGArr.Copy(dlg.m_iCGArr);
		m_strFunExpArr.Copy(dlg.m_strFunExpArr);
		m_strFunDepArr.Copy(dlg.m_strFunDepArr);
		
		m_iVarCGArr.Copy(dlg.m_iVarCGArr);
		m_strVarNameArr.Copy(dlg.m_strVarNameArr);
		m_strVarDespArr.Copy(dlg.m_strVarDespArr);
		m_StrVarUnit.Copy(dlg.m_StrVarUnit);

		//对输出端口的修改
		for(int i=0;i<m_strVarNameArr.GetSize();i++)
		{
			IDArrRightIn.Add(m_strVarNameArr[i]);
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
	}
}
//将属性写到记录的当前位置
void COptimalRuleBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue.Format("DBName");
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strDBName); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue.Format("UnitSet");
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iUnit); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_iSet); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名
	
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue.Format("RuleID");
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iRuleID); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名
	
	for(int i=0;i<m_strFunExpArr.GetSize();i++)//表达式
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("FunExp.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%d",m_iCGArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",m_strFunExpArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",m_strFunDepArr[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	}
	for(i=0;i<m_strVarNameArr.GetSize();i++)//输出变量
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("Var.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%d",m_iVarCGArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",m_strVarNameArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",m_strVarDespArr[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",m_StrVarUnit[i]); //属性名
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名	
	}
	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void COptimalRuleBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_strDBName=((COptimalRuleBlock*)pBlock)->m_strDBName;
	this->m_iUnit=((COptimalRuleBlock*)pBlock)->m_iUnit;
	this->m_iSet=((COptimalRuleBlock*)pBlock)->m_iSet;
	this->m_iRuleID=((COptimalRuleBlock*)pBlock)->m_iRuleID;

	this->m_iCGArr.Copy(((COptimalRuleBlock*)pBlock)->m_iCGArr);
	this->m_strFunExpArr.Copy(((COptimalRuleBlock*)pBlock)->m_strFunExpArr);
	this->m_strFunDepArr.Copy(((COptimalRuleBlock*)pBlock)->m_strFunDepArr);
	
	this->m_iVarCGArr.Copy(((COptimalRuleBlock*)pBlock)->m_iVarCGArr);
	this->m_strVarNameArr.Copy(((COptimalRuleBlock*)pBlock)->m_strVarNameArr);
	this->m_strVarDespArr.Copy(((COptimalRuleBlock*)pBlock)->m_strVarDespArr);
	this->m_StrVarUnit.Copy(((COptimalRuleBlock*)pBlock)->m_StrVarUnit);
}
//将属性放置传输数据中
void COptimalRuleBlock::SetBlockPropertyToTransferData(
									CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"DBName");	
	CString str;
	str.Format("%s",m_strDBName);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"UnitSet");	
	str.Format("%d",m_iUnit);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iSet);
	strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"RuleID");
	str.Format("%d",m_iRuleID);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	CString strFieldValue;
	for(int i=0;i<m_strFunExpArr.GetSize();i++)//表达式
	{
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		strFieldValue.Format("FunExp.%d",i);
		strcpy(pData->m_cPropName,strFieldValue);
		str.Format("%d",m_iCGArr[i]);
		strcpy(pData->m_cValueItem1,str);
		str.Format("%s",m_strFunExpArr[i]);
		strcpy(pData->m_cValueItem2,str);
		str.Format("%s",m_strFunDepArr[i]);
		strcpy(pData->m_cValueItem3,str);
		dataArr.Add(pData);
	}
	for(i=0;i<m_strVarNameArr.GetSize();i++)//输出变量
	{
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		strFieldValue.Format("Var.%d",i);
		strcpy(pData->m_cPropName,strFieldValue);
		str.Format("%d",m_iVarCGArr[i]);
		strcpy(pData->m_cValueItem1,str);
		str.Format("%s",m_strVarNameArr[i]);
		strcpy(pData->m_cValueItem2,str);
		str.Format("%s",m_strVarDespArr[i]);
		strcpy(pData->m_cValueItem3,str);
		str.Format("%s",m_StrVarUnit[i]);
		strcpy(pData->m_cValueItem3,str);
		dataArr.Add(pData);
	}
}
void COptimalRuleBlock::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
{
	int dIndex=strPropName.ReverseFind('.');
	CString strIndex=strPropName.Right(strPropName.GetLength()-dIndex-1);
	CString strName=strPropName.Left(dIndex);
	propIndex=atoi(strIndex);

	if(strName=="FunExp")//表达式
	{
		propType=1;
	}
	else if(strName=="Var")//变量
	{
		propType=2;
	}
}
