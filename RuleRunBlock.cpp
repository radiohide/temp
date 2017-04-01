// RuleRunBlock.cpp: implementation of the CRuleRunBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RuleRunBlock.h"

#include "DlgPropRuleRun.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
/* 本模块连接一个数据库的若干表，该组表描述规则，模型 根据规则进行预测输出。
输入参数：工况划分变量、前件变量
输出参数：后件变量
运行算法：
         前向推理：
         *工况变量读取后，进行离散化，然后找到该工况对应的集合的规则集合；
         *前件变量读取后，进入模块，进行离散化，构成矢量，然后和规则库表中的前件逐一进行比较，找到完全
         匹配或最接近匹配的前件，然后直接获得后件数据并输出。
		 * 当没有匹配度的时候，输出中应体现偏差。
//---关于离散化
        -- 模拟量数据进入后，和离散化端点比较
		-- 数字量数据进入后，离散值和数字量值一致。

// 
*/
//////////////////////////////////////////////////////////////////////

CRuleRunBlock::CRuleRunBlock()
{
	SetBlockType(_T("RuleRun") );
	ConstructBlockInputOutput();

	m_strDBConnect = _T("rule.mdb");
	m_strRuleDBName = _T("");
	m_iTagToFOrB=0;//默认为前向推理
	m_iTagWeightOrTime=0;//默认为权重最大优先
	m_bCon=false;
}

CRuleRunBlock::~CRuleRunBlock()
{
	CRuleData *pRuleData;
	for(int i=0;i<ruleDataArr.GetSize();i++)
	{
		pRuleData=ruleDataArr[i];
		if(pRuleData!=NULL)
		{
			delete pRuleData;
			pRuleData=NULL;
		}
	}
	ruleDataArr.RemoveAll();
}
void CRuleRunBlock::ConstructBlockInputOutput()
{
	//输入
    AddInputPort(_T("Enable"), VALUE_DOUBLE);
	AddInputPort(_T("Condition"), VALUE_RTVALUE_SERIES);
	AddOutputPort(_T("Diff"), VALUE_DOUBLE);
}
	//根据类型新建对象的函数
CCalcBlock* CRuleRunBlock::CreateFromString( const CString& strType )
{
	CRuleRunBlock* obj = new CRuleRunBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CRuleRunBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	//m_nInNums=((CRuleRunBlock*)pBlock)->m_nInNums;
}
//初始化计算的函数做特殊的初始化
void CRuleRunBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	//添加自身的计算前初始化代码
	//读取规则表、参数表等到内存中
	if(!m_bCon)
	{
		m_bCon=EstablishMDBConnectPtr(getABSDirRuleDB());
	}
	if(m_bCon)
	{
		//读取所有的参数
		readParData();
		//读取所有的规则
		readRuleData();
	}
}

void CRuleRunBlock::readParData()//读取变量信息
{
	nameArrDB.RemoveAll();
	typeFBArrDB.RemoveAll();
	disMethodArrDB.RemoveAll();
	disRefValueArrDB.RemoveAll();
	disStepArrDB.RemoveAll();
	
	CString strTb_para;
	strTb_para.Format("%s_para",m_strRuleDBName);
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
	VARIANT_BOOL bBOF ;
	pRecord.CreateInstance(__uuidof(Recordset));
	
	CString strSQL;
	strSQL.Format("select * from %s ",strTb_para);
	try
	{
		pRecord->Open((const char*)strSQL,m_pConRule.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)
	{
		CString str="打开参数表出错!";
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
		return;
	}	
	bBOF = pRecord->GetBOF();
	if(!bBOF)  pRecord->MoveFirst();
	while(!pRecord->adoEOF )
	{
		try
		{
			nameArrDB.Add((LPCSTR)_bstr_t(pRecord->GetCollect("var_name")));
			typeFBArrDB.Add((LPCSTR)_bstr_t(pRecord->GetCollect("func_type")));
			disMethodArrDB.Add((LPCSTR)_bstr_t(pRecord->GetCollect("dis_method")));
			disRefValueArrDB.Add(atof((LPCSTR)_bstr_t(pRecord->GetCollect("dis_refvalue"))));
			disStepArrDB.Add(atof((LPCSTR)_bstr_t(pRecord->GetCollect("dis_step"))));
		}
		catch(_com_error e)
		{
			//AfxMessageBox(e.Description());
		}
		pRecord->MoveNext();
	}
}
void CRuleRunBlock::readRuleData()//读取规则信息
{
	CRuleData *pRuleData;
	for(int i=0;i<ruleDataArr.GetSize();i++)
	{
		pRuleData=ruleDataArr[i];
		if(pRuleData!=NULL)
		{
			delete pRuleData;
			pRuleData=NULL;
		}
	}
	ruleDataArr.RemoveAll();
	CString strTb_rule;
	strTb_rule.Format("%s_rule",m_strRuleDBName);
	_RecordsetPtr        pRecord;
	_variant_t           Tempvalue ;
	VARIANT_BOOL bBOF ;
	pRecord.CreateInstance(__uuidof(Recordset));
	
	CString strSQL;
	strSQL.Format("select * from %s t",strTb_rule);
	try
	{
		pRecord->Open((const char*)strSQL,m_pConRule.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)
	{
		CString str="打开规则表出错!";
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
		return;
	}	
	bBOF = pRecord->GetBOF();
	if(!bBOF)  pRecord->MoveFirst();
	while(!pRecord->adoEOF )
	{
		pRuleData=new CRuleData;
		pRuleData->m_pStrNameArrDB=&nameArrDB;
		pRuleData->m_ptypeFBArrDB=&typeFBArrDB;
		pRuleData->m_piTagToFOrB=&m_iTagToFOrB;
		pRuleData->m_piDisValueFArr=&iDisValueFArr;
		pRuleData->m_piDisValueBArr=&iDisValueBArr;
		pRuleData->m_NameFArr=&nameFArr;
		pRuleData->m_pTagBeInFArr=&tagBeInFArr;
		pRuleData->m_pTagBeInBArr=&tagBeInBArr;
		pRuleData->m_NameBArr=&nameBArr;
		pRuleData->m_strConCode=(LPCSTR)_bstr_t(pRecord->GetCollect("concode"));
		pRuleData->m_iWeight=atoi((LPCSTR)_bstr_t(pRecord->GetCollect("weight")));
		pRuleData->m_learRID=atoi((LPCSTR)_bstr_t(pRecord->GetCollect("ear_RID")));
		pRuleData->m_llateRID=atoi((LPCSTR)_bstr_t(pRecord->GetCollect("late_RID")));

		for(int i=0;i<nameArrDB.GetSize();i++)
		{
			const CString strField=nameArrDB[i];
			pRuleData->m_iDisValue.Add(atoi((LPCSTR)_bstr_t(pRecord->GetCollect((_variant_t)strField))));
		}
		ruleDataArr.Add(pRuleData);
		pRecord->MoveNext();
	}
}
BOOL CRuleRunBlock::EstablishMDBConnectPtr(CString strFileName)
{
BOOL bResult = FALSE;
	try
	{
		HRESULT hr;
		hr =m_pConRule.CreateInstance("ADODB.Connection");
		if(SUCCEEDED(hr))
		{
			CString strSqlConn;
			strSqlConn = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")+strFileName;
			hr =m_pConRule->Open(_bstr_t(strSqlConn), "", "", adModeUnknown);
			bResult = TRUE;
		}
	}
	catch(_com_error e)
	{
		CString str;
		str.Format("连接MDB数据库错误:%s",e.ErrorMessage());
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
	}
	return bResult;
}
CString CRuleRunBlock::getABSDirRuleDB()
{
	CString strRet;
	if(m_strDBConnect.Find(':')!=-1)//绝对路径
	{
		strRet = m_strDBConnect;
	}
	else
	{
		strRet= gGetExeFileDir() + "model\\"+m_strDBConnect;
	}
	return strRet;
}
//计算函数，实现本模块的计算
void CRuleRunBlock::Calc() //进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程-------------------------
    if(!m_bCon)
	{
		m_bCon=EstablishMDBConnectPtr(getABSDirRuleDB());
		if(m_bCon)
		{
			//读取所有的参数
			readParData();
			//读取所有的规则
			readRuleData();
		}
	}
	if(!m_bCon)
	{
		CCalcPort *pOutputPortObj1 = GetOutputPortObj(0);//差异程度
		CUniValue  UniVar;
		UniVar.AddData(-1);
		pOutputPortObj1->SetPortUniValue(UniVar);
		return;
	}
	//计算过程
	CCalcPort *pInputPortObj1 = GetInputPortObj(0);//使能端
	CCalcPort *pInputPortObj2 = GetInputPortObj(1);//工况端
	double beAble=0;
	pInputPortObj1->GetPortUniValue().GetDoubleVal(&beAble);
	if(!beAble)//使能为关闭
	{
		CCalcPort *pOutputPortObj1 = GetOutputPortObj(0);//差异程度
		CUniValue  UniVar;
		UniVar.AddData(-1);
		pOutputPortObj1->SetPortUniValue(UniVar);
		return;
	}
	resetBeforeCalc();
	CUniValue &input2UnitValue=pInputPortObj2->GetPortUniValue();
	CString strConCode="";
	for(long i=0;i<input2UnitValue.GetDataSize();i++)
	{
		double dChar=0;
		input2UnitValue.GetDoubleValAt(i,&dChar);
		strConCode+=(char)dChar;
	}
	int inNum = GetInputNum();
	for(int k=2;k<inNum;k++)
	{
		CCalcPort *pPortObj = GetInputPortObj(k);
		CString portName=pPortObj->GetPortID();
		CUniValue  uniValue=pPortObj->GetPortUniValue();
		double dValue;
		uniValue.GetDoubleVal(&dValue);
		if(0==this->m_iTagToFOrB)//前向推理
		{
			setFDValueByName(portName,dValue);
		}
		else
		{
			setBDValueByName(portName,dValue);
		}
	}
	setConToRuleData(&strConCode);//设置工况数据
	//匹配结果,得到规则记录
	calcDiffValue();
	CRuleData *pRuleData=getMinDiffRuleData();
	//从规则记录中拿到输出变量的值
	CCalcPort *pOutputPortObj1 = GetOutputPortObj(0);//差异程度
	CUniValue  UniVar;
	if(pRuleData!=NULL)
		UniVar.AddData(pRuleData->m_iDiffValue);
	else
		UniVar.AddData(-1);
	pOutputPortObj1->SetPortUniValue(UniVar);

	inNum = GetOutputNum();
	for(i=1;i<inNum;i++)
	{
		CCalcPort *pPortObj = GetOutputPortObj(i);
		CUniValue  UniVar;
		CString portName=pPortObj->GetPortID();
		if(pRuleData!=NULL)
		{
			int disValue=pRuleData->GetDisValueByName(portName);
			double dValue=getDValueByDis(portName,disValue);
			UniVar.AddData(dValue);
		}
		else
			UniVar.AddData(0);
		pPortObj->SetPortUniValue(UniVar);
	}
    //结果输出，工作空间中
	OutputResultToGlobalWS();
}
double CRuleRunBlock::getDValueByDis(CString strVarName,int disValue)
{
	if(disValue==0)
		return 0;
	double dRet=0;
	CString strDisMethod;
	double dDisRefValue,dDisStep;
	bool befind=false;
	for(int i=0;i<nameArrDB.GetSize();i++)
	{
		if(strVarName==nameArrDB[i])
		{
			strDisMethod=disMethodArrDB[i];
			dDisRefValue=disRefValueArrDB[i];
			dDisStep=disStepArrDB[i];
			befind=true;
			break;
		}
	}
	if(!befind)
	{
		return dRet;
	}
	CArray<int,int> disValueArr;
	CArray<double,double> sValueArr;
	CArray<double,double> eValueArr;
	if(disValue>0)
	{
		GetDisInterval(strDisMethod,dDisRefValue,dDisStep,0,disValue,disValueArr,sValueArr,eValueArr);
		if((sValueArr.GetSize()>0)&&(eValueArr.GetSize()))
			dRet=(sValueArr[sValueArr.GetSize()-1]+eValueArr[eValueArr.GetSize()-1])/2;
	}
	else
	{
		GetDisInterval(strDisMethod,dDisRefValue,dDisStep,-disValue,0,disValueArr,sValueArr,eValueArr);
		if((sValueArr.GetSize()>0)&&(eValueArr.GetSize()))
			dRet=(sValueArr[0]+eValueArr[0])/2;
	}
	return dRet;
	
}
void CRuleRunBlock::setConToRuleData(CString *pStrConCode)
{
	CRuleData *pRuleData=NULL;
	for(int i=0;i<ruleDataArr.GetSize();i++)
	{
		pRuleData=ruleDataArr[i];
		if(pRuleData!=NULL)
		{
			pRuleData->m_pStrInPutConCode=pStrConCode;
		}
	}
}
void CRuleRunBlock::calcDiffValue()
{
	CRuleData *pRuleData=NULL;
	for(int i=0;i<ruleDataArr.GetSize();i++)
	{
		pRuleData=ruleDataArr[i];
		if(pRuleData!=NULL)
		{
			pRuleData->m_iDiffValue=-1;
			pRuleData->CalcDiffValue();
		}
	}
}
CRuleData* CRuleRunBlock::getMinDiffRuleData()
{
	//第一步,得到所有差异度不为-1的规则，即工况不匹配的
	CArray<CRuleData *,CRuleData *&> ruleDataTempArr;
	CArray<CRuleData *,CRuleData *&> ruleDataTemp0Arr;
	CRuleData *pRuleData=NULL;
	for(int i=0;i<ruleDataArr.GetSize();i++)
	{
		pRuleData=ruleDataArr[i];
		if(pRuleData->m_iDiffValue!=-1)
		{
			ruleDataTempArr.Add(pRuleData);
		}
		if(pRuleData->m_iDiffValue==0)
		{
			ruleDataTemp0Arr.Add(pRuleData);
		}
	}
	//第二部，对差异度为0的规则进行排序
	for(i=0;i<ruleDataTemp0Arr.GetSize();i++)//策略排序
	{
		if(this->m_iTagWeightOrTime==0)//权重最大
		{
			for(int j=i+1;j<ruleDataTemp0Arr.GetSize();j++)
			{
				if(ruleDataTemp0Arr[i]->m_iWeight<ruleDataTemp0Arr[j]->m_iWeight)
				{
					CRuleData *pRuleDataTemp=ruleDataTemp0Arr[j];
					ruleDataTemp0Arr[j]=ruleDataTemp0Arr[i];
					ruleDataTemp0Arr[i]=pRuleDataTemp;
				}
			}
		}
		else//时间最晚
		{
			for(int j=i+1;j<ruleDataTemp0Arr.GetSize();j++)
			{
				if(ruleDataTemp0Arr[i]->m_llateRID<ruleDataTemp0Arr[j]->m_llateRID)
				{
					CRuleData *pRuleDataTemp=ruleDataTemp0Arr[j];
					ruleDataTemp0Arr[j]=ruleDataTemp0Arr[i];
					ruleDataTemp0Arr[i]=pRuleDataTemp;
				}
			}
		}
	}
	if(ruleDataTemp0Arr.GetSize()>0)//首要选择匹配完全相同中的
	{
		pRuleData=ruleDataTemp0Arr[0];
		return pRuleData;
	}
	//第三步，对差异度进行排序。
	for(i=0;i<ruleDataTempArr.GetSize();i++)
	{
		for(int j=i+1;j<ruleDataTempArr.GetSize();j++)
		{
			if(ruleDataTempArr[j]->m_iDiffValue<ruleDataTempArr[i]->m_iDiffValue)
			{
				CRuleData *pRuleDataTemp=ruleDataTempArr[j];
				ruleDataTempArr[j]=ruleDataTempArr[i];
				ruleDataTempArr[i]=pRuleDataTemp;
			}
		}
	}
	if(ruleDataTempArr.GetSize()>0)
	{
		pRuleData=ruleDataTempArr[0];
	}
	else
	{
		pRuleData=NULL;
	}
	return pRuleData;
}
void CRuleRunBlock::setFDValueByName(CString strVarName,double dValue)
{
	for(int i=0;i<nameFArr.GetSize();i++)
	{
		if(strVarName==nameFArr[i])
		{
			dValueFArr[i]=dValue;
			iDisValueFArr[i]=getDisValueByName(strVarName,dValue);
			break;
		}
	}
}
void CRuleRunBlock::setBDValueByName(CString strVarName,double dValue)
{
	for(int i=0;i<nameBArr.GetSize();i++)
	{
		if(strVarName==nameBArr[i])
		{
			dValueBArr[i]=dValue;
			iDisValueBArr[i]=getDisValueByName(strVarName,dValue);
			break;
		}
	}
}
int CRuleRunBlock::getDisValueByName(CString strVarName,double dValue)
{
	int iRet=0;
	CString strDisMethod;
	double dDisRefValue;
	double dDisStep;
	bool befind=false;
	for(int i=0;i<nameArrDB.GetSize();i++)
	{
		if(strVarName==nameArrDB[i])
		{
			strDisMethod=disMethodArrDB[i];
			dDisRefValue=disRefValueArrDB[i];
			dDisStep=disStepArrDB[i];
			befind=true;
			break;
		}
	}
	if(!befind)
	{
		return iRet;
	}
	CArray<int,int> disValueArr;
	CArray<double,double> sValueArr;
	CArray<double,double> eValueArr;

	GetDisInterval(strDisMethod,dDisRefValue,dDisStep,dValue,dValue,disValueArr,sValueArr,eValueArr);
	if(disValueArr.GetSize()!=1)
	{
		return iRet;//错误结果。
	}
	else
		return disValueArr[0];
}
void CRuleRunBlock::resetBeforeCalc()//在计算前进行复位
{
	//对输入输出数据进行清空
	dValueFArr.RemoveAll();
	iDisValueFArr.RemoveAll();
	for(int i=0;i<nameFArr.GetSize();i++)
	{
		dValueFArr.Add(0);
		iDisValueFArr.Add(0);
	}
	dValueBArr.RemoveAll();
	iDisValueBArr.RemoveAll();
	for(i=0;i<nameBArr.GetSize();i++)
	{
		dValueBArr.Add(0);
		iDisValueBArr.Add(0);
	}
}
//画特征图形
void CRuleRunBlock::DrawShape(CDC* dc, CRect rect,int CutSize)
{
	//rect 为block的外矩形框,在中部写常数值
	CCalcBlock::DrawShape(dc,rect,CutSize);
}
void CRuleRunBlock::SetBlockPropertyToTransferData(
				CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)//将属性放置传输数据中
{
	/*
	*/
}
    //用于根据参数项和值进行属性的设置，当读取值要用
void CRuleRunBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(strPropName.Compare("DBCon")==0)
	{
		if(!strItem1.IsEmpty()) {m_strDBConnect=strItem1;}
		if(!strItem2.IsEmpty()) {m_strRuleDBName=strItem2;}
		if(!strItem3.IsEmpty()) {m_strRuleDBDesp=strItem3;}
		return;
	}
	if(strPropName.Compare("Strategy")==0)
	{
		if(!strItem1.IsEmpty()) {m_iTagToFOrB=atoi(strItem1);}
		if(!strItem2.IsEmpty()) {m_iTagWeightOrTime=atoi(strItem2);}
		return;
	}
	int propType,propIndex;
	getPropTypeByName(strPropName,propType,propIndex);
	if(propType==1)//前件
	{
		if(!strItem1.IsEmpty())  {nameFArr.SetAtGrow(propIndex,strItem1);}
		if(!strItem2.IsEmpty())  {despFArr.SetAtGrow(propIndex,strItem2);}
		if(!strItem3.IsEmpty())  {tagBeInFArr.SetAtGrow(propIndex,atoi(strItem3));}
	}
	else if(propType==2)//后件
	{
		if(!strItem1.IsEmpty())  {nameBArr.SetAtGrow(propIndex,strItem1);}
		if(!strItem2.IsEmpty())  {despBArr.SetAtGrow(propIndex,strItem2);}
		if(!strItem3.IsEmpty())  {tagBeInBArr.SetAtGrow(propIndex,atoi(strItem3));}
	}
}
void CRuleRunBlock::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
{
	int dIndex=strPropName.ReverseFind('.');
	CString strIndex=strPropName.Right(strPropName.GetLength()-dIndex-1);
	CString strName=strPropName.Left(dIndex);
	propIndex=atoi(strIndex);

	if(strName=="FVar")//前件1
	{
		propType=1;
	}
	else if(strName=="BVar")//后件为2
	{
		propType=2;
	}
}
	//重载以显示不同的属性对象对话框
void CRuleRunBlock::ShowPropertyDialog(CWnd* parent, BOOL show )
{
	CDlgPropRuleRun dlg;
    dlg.m_strDBConnect = m_strDBConnect;
	dlg.m_strRuleDBName = m_strRuleDBName;
	dlg.m_strRuleDBDesp = m_strRuleDBDesp;
	dlg.m_iTagWeightOrTime=m_iTagWeightOrTime;
	dlg.m_iTagToFOrB=m_iTagToFOrB;
	dlg.nameFArr.Copy(nameFArr);
	dlg.despFArr.Copy(despFArr);
	dlg.tagBeInFArr.Copy(tagBeInFArr);
	dlg.nameBArr.Copy(nameBArr);
	dlg.despBArr.Copy(despBArr);
	dlg.tagBeInBArr.Copy(tagBeInBArr);

	if(dlg.DoModal()==IDOK)
	{
		int portnumLinkBefore=this->GetInputNum();//新增
		int portnumLinkBehind=this->GetOutputNum();
		CStringArray  IDArrLeftOut;//计算块原有的输入端口描述
		CStringArray  IDArrRightOut;//计算块原有的输出端口描述
		CStringArray  IDArrLeftIn;//计算块内部的输入端口描述
		CStringArray  IDArrRightIn;//计算块内部的输出端口描述
		
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

		m_strDBConnect = dlg.m_strDBConnect;
		m_strRuleDBName = dlg.m_strRuleDBName;
		m_strRuleDBDesp = dlg.m_strRuleDBDesp;
		m_iTagToFOrB=dlg.m_iTagToFOrB;
		m_iTagWeightOrTime=dlg.m_iTagWeightOrTime;
		nameFArr.Copy(dlg.nameFArr);
		despFArr.Copy(dlg.despFArr);
		tagBeInFArr.Copy(dlg.tagBeInFArr);
		nameBArr.Copy(dlg.nameBArr);
		despBArr.Copy(dlg.despBArr);
		tagBeInBArr.Copy(dlg.tagBeInBArr);
		//进行端口的整理。
		
		//1、对输入端口的修改
		IDArrLeftIn.Add("Enable");
		IDArrLeftIn.Add("Condition");
		if(m_iTagToFOrB==0)//前向
		{
			for(int i=0;i<nameFArr.GetSize();i++)
			{
				if(nameFArr[i]=="")
					continue;
				if(tagBeInFArr[i]==0)
					continue;
				IDArrLeftIn.Add(nameFArr[i]);
			}
		}
		else
		{
			for(int i=0;i<nameBArr.GetSize();i++)
			{
				if(nameBArr[i]=="")
					continue;
				if(tagBeInBArr[i]==0)
					continue;
				IDArrLeftIn.Add(nameBArr[i]);
			}
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
		IDArrRightIn.Add("Diff");
		if(m_iTagToFOrB==0)//前向
		{
			for(int i=0;i<nameBArr.GetSize();i++)
			{
				if(nameBArr[i]=="")
					continue;
				if(tagBeInBArr[i]==0)
					continue;
				IDArrRightIn.Add(nameBArr[i]);
			}
		}
		else
		{
			for(int i=0;i<nameFArr.GetSize();i++)
			{
				if(nameFArr[i]=="")
					continue;
				if(tagBeInFArr[i]==0)
					continue;
				IDArrRightIn.Add(nameFArr[i]);
			}
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
	}
}
	//重载以将本模块写入到数据库中
void CRuleRunBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1：数据库连接信息
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName())); //放Name
	pRecord->PutCollect(_T("prop_name"),_variant_t("DBCon"));
	pRecord->PutCollect(_T("value_item1"),_variant_t(m_strDBConnect)); 
	pRecord->PutCollect(_T("value_item2"),_variant_t(m_strRuleDBName)); 
	pRecord->PutCollect(_T("value_item3"),_variant_t(m_strRuleDBDesp)); 

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName())); //放Name
	pRecord->PutCollect(_T("prop_name"),_variant_t("Strategy"));
	strFieldValue.Format("%d",m_iTagToFOrB);
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue));
	strFieldValue.Format("%d",m_iTagWeightOrTime);
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue)); 
	
	for(int i=0;i<nameFArr.GetSize();i++)//前件（默认正向）
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strField.Format("FVar.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strField) ); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(nameFArr[i]));
		pRecord->PutCollect(_T("value_item2"),_variant_t(despFArr[i]));
		strFieldValue.Format("%d",tagBeInFArr[i]);
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue));
	}
	for(i=0;i<nameBArr.GetSize();i++)//前件（默认正向）
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strField.Format("BVar.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strField) ); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(nameBArr[i]));
		pRecord->PutCollect(_T("value_item2"),_variant_t(despFArr[i]));
		strFieldValue.Format("%d",tagBeInBArr[i]);
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue));
	}
	pRecord->Update();
}
//---------其他自有函数---------------------------------------------------------------------------------------
int CRuleRunBlock::GetDisInterval(	CString strComFun, 
								  double dPointValue,
								  double dStepValue,	
								  double dSValue,//离散化范围的开始
								  double dEValue,//离散化范围的结束
								  CArray<int,int> &disValueArr,
								  CArray<double,double> &sValueArr,
								  CArray<double,double> &eValueArr)
{
	int inDivMinN,iDivMaxN;
	int tagType=-1;
	if(dSValue>dEValue)//如果起点大于终点则交换
	{
		double dtemp=dSValue;
		dSValue=dEValue;
		dEValue=dtemp;
	}
	if(strComFun=="TABLE")//TABLE:采用数据库表
	{
		return 0;
	}
	else if(strComFun=="ABS_STEP")//ABS_STEP:绝对间隔
	{
		if(dSValue>=dPointValue)//区间均大于端点
		{
			inDivMinN=(int)floor((dSValue-dPointValue)/dStepValue+1);
			iDivMaxN=(int)floor((dEValue-dPointValue)/dStepValue+1);
			tagType=2;//均大于
		}
		else if((dSValue<dPointValue)&&(dEValue>=dPointValue))//介于两者之间
		{
			inDivMinN=(int)((-1)*ceil((dPointValue-dSValue)/dStepValue));
			iDivMaxN=(int)floor((dEValue-dPointValue)/dStepValue+1);
			tagType=1;//介于两者之间
		}
		else if(dEValue<dPointValue)//区间均小于端点
		{
			inDivMinN=(int)((-1)*ceil((dPointValue-dSValue)/dStepValue));
			iDivMaxN=(int)((-1)*ceil((dPointValue-dEValue)/dStepValue));
			tagType=0;//区间均小于端点
		}
		if(iDivMaxN-inDivMinN>1000)
			return -1;//个数大于1000;
		if(tagType==2)
		{
			for(int i=inDivMinN;i<=iDivMaxN;i++)
			{
				//[a1+(N-1)*d，a1+N*d)
				int N=i;
				double sValue=dPointValue+(N-1)*dStepValue;
				double eValue=dPointValue+N*dStepValue;
				disValueArr.Add(N);
				sValueArr.Add(sValue);
				eValueArr.Add(eValue);
			}
		}
		else if(tagType==1)
		{
			for(int i=inDivMinN;i<=-1;i++)
			{
				//[a1+(-N)*d，a1+(-N+1)*d);
				int N=i;
				double sValue=dPointValue+(N)*dStepValue;
				double eValue=dPointValue+(N+1)*dStepValue;
				disValueArr.Add(N);
				sValueArr.Add(sValue);
				eValueArr.Add(eValue);
			}
			for(i=1;i<=iDivMaxN;i++)
			{
				//[a1+(N-1)*d，a1+N*d)
				int N=i;
				double sValue=dPointValue+(N-1)*dStepValue;
				double eValue=dPointValue+N*dStepValue;
				disValueArr.Add(N);
				sValueArr.Add(sValue);
				eValueArr.Add(eValue);
			}
		}
		else if(tagType==0)
		{
			for(int i=inDivMinN;i<=iDivMaxN;i++)
			{
				//[a1+(-N)*d，a1+(-N+1)*d);
				int N=i;
				double sValue=dPointValue+(N)*dStepValue;
				double eValue=dPointValue+(N+1)*dStepValue;
				disValueArr.Add(N);
				sValueArr.Add(sValue);
				eValueArr.Add(eValue);
			}
		}
	}
	else if(strComFun=="REL_STEP")//REL_STEP:相对间隔
	{
		if(dSValue>=dPointValue)//区间均大于端点
		{
			inDivMinN=(int)floor(1+log(dEValue/dPointValue)/log(1+dStepValue));
			iDivMaxN=(int)floor(1+log(dSValue/dPointValue)/log(1+dStepValue));
			tagType=2;//均大于
		}
		else if((dSValue<dPointValue)&&(dEValue>=dPointValue))//介于两者之间
		{
			inDivMinN=(int)ceil((-1)*(log(2-dSValue/dPointValue)/log(1+dStepValue))-1);
			iDivMaxN=(int)floor(1+log(dEValue/dPointValue)/log(1+dStepValue));
			tagType=1;//介于两者之间
		}
		else if(dEValue<dPointValue)//区间均小于端点
		{
			inDivMinN=(int)ceil((-1)*(log(2-dSValue/dPointValue)/log(1+dStepValue))-1);
			iDivMaxN=(int)ceil((-1)*(log(2-dEValue/dPointValue)/log(1+dStepValue))-1);
			tagType=0;//区间均小于端点
		}
		if(iDivMaxN-inDivMinN>1000)
			return -1;//个数大于1000;
		if(tagType==2)
		{	
			for(int i=inDivMinN;i<=iDivMaxN;i++)
			{
				int N=i;
				double sValue=dPointValue*pow(1+dStepValue,N-1);
				double eValue=dPointValue*pow(1+dStepValue,N);
				disValueArr.Add(N);
				sValueArr.Add(sValue);
				eValueArr.Add(eValue);
			}
		}
		else if(tagType==1)
		{
			for(int i=inDivMinN;i<=-1;i++)
			{
				int N=i;
				double sValue=dPointValue*(2-pow(1+dStepValue,(-1)*N));
				double eValue=dPointValue*(2-pow(1+dStepValue,(-1)*(N+1)));
				disValueArr.Add(N);
				sValueArr.Add(sValue);
				eValueArr.Add(eValue);
			}
			for(i=1;i<=iDivMaxN;i++)
			{
				int N=i;
				double sValue=dPointValue*pow(1+dStepValue,N-1);
				double eValue=dPointValue*pow(1+dStepValue,N);
				disValueArr.Add(N);
				sValueArr.Add(sValue);
				eValueArr.Add(eValue);
			}
		}
		else if(tagType==0)
		{
			for(int i=inDivMinN;i<=iDivMaxN;i++)
			{
				int N=i;
				double sValue=dPointValue*(2-pow(1+dStepValue,(-1)*N));
				double eValue=dPointValue*(2-pow(1+dStepValue,(-1)*(N+1)));
				disValueArr.Add(N);
				sValueArr.Add(sValue);
				eValueArr.Add(eValue);
			}
		}
	}
	return 1;
}
int CRuleRunBlock::GetDisInterval(	CString strComFun, 
						   double dPointValue,
						   double dStepValue,	
						   int iLeftNum,
						   int iRightNum,
						   CArray<int,int> &disValueArr,
						   CArray<double,double> &sValueArr,
						   CArray<double,double> &eValueArr)
{
	if(strComFun=="TABLE")//TABLE:采用数据库表
	{
		return 0;
	}
	else if(strComFun=="ABS_STEP")//ABS_STEP:绝对间隔
	{
		for(int i=-iLeftNum;i<=-1;i++)
		{
			//[a1+(-N)*d，a1+(-N+1)*d);
			int N=i;
			double sValue=dPointValue+(N)*dStepValue;
			double eValue=dPointValue+(N+1)*dStepValue;
			disValueArr.Add(N);
			sValueArr.Add(sValue);
			eValueArr.Add(eValue);
		}
		for(i=1;i<=iRightNum;i++)
		{
			//[a1+(N-1)*d，a1+N*d)
			int N=i;
			double sValue=dPointValue+(N-1)*dStepValue;
			double eValue=dPointValue+N*dStepValue;
			disValueArr.Add(N);
			sValueArr.Add(sValue);
			eValueArr.Add(eValue);
		}
	}
	else if(strComFun=="REL_STEP")//REL_STEP:相对间隔
	{
		for(int i=-iLeftNum;i<=-1;i++)
		{
			int N=i;
			double sValue=dPointValue*(2-pow(1+dStepValue,(-1)*N));
			double eValue=dPointValue*(2-pow(1+dStepValue,(-1)*(N+1)));
			disValueArr.Add(N);
			sValueArr.Add(sValue);
			eValueArr.Add(eValue);
		}
		for(i=1;i<=iRightNum;i++)
		{
			int N=i;
			double sValue=dPointValue*pow(1+dStepValue,N-1);
			double eValue=dPointValue*pow(1+dStepValue,N);
			disValueArr.Add(N);
			sValueArr.Add(sValue);
			eValueArr.Add(eValue);
		}
	}
	return 1;
}