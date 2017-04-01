// RuleREPoint.cpp: implementation of the RuleREPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RuleREPoint.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RuleREPoint::RuleREPoint(CString name,CString expr,CMap<CString,const char*,RuleREPoint*,RuleREPoint*&> *pRuleREPointMap)//构造计算点
{
	PSourceValue = new TAGINFO[1000];
	memset(PSourceValue,0,sizeof(TAGINFO)*1000);//清0

	pListPoteDll=NULL;
	m_BTree=NULL;
	b_Calced = false;//初始化该点没有被计算过

	pG = new TAGINFO;

	this->LongID=name;
	this->PFormula=expr;
	this->pRuleREPointMap=pRuleREPointMap;
	this->PType=1;//计算点+数据点
}
RuleREPoint::RuleREPoint(CString name,CMap<CString,const char*,RuleREPoint*,RuleREPoint*&> *pRuleREPointMap)//构造数据点
{
	PSourceValue = new TAGINFO[1000];
	memset(PSourceValue,0,sizeof(TAGINFO)*1000);//清0

	pListPoteDll=NULL;
	m_BTree=NULL;
	b_Calced = false;//初始化该点没有被计算过

	pG = new TAGINFO;

	this->LongID=name;
	this->pRuleREPointMap=pRuleREPointMap;
	this->PType=0;//数据点
}
RuleREPoint::~RuleREPoint()
{
	if (m_BTree) 
		delete m_BTree;

	if (pG)
		delete pG;
	
	if (PSourceValue)
		delete[] PSourceValue;
}
void RuleREPoint::SetRDBInf(CCalcModel *pModel,CString strDBName,int iUnit,int iSet)
{
	m_pModel=pModel;
	m_strDBName=strDBName;
	m_iUnit=iUnit;
	m_iSet=iSet;
}
void RuleREPoint::SetDllMap(CPtrArray *pListPoteDll)
{
	this->pListPoteDll=pListPoteDll;
}
void RuleREPoint::SetRuleFBPtr(RuleFB* pRuleFB)
{
	this->pRuleFB=pRuleFB;
}
void RuleREPoint::SetCGMark(CString cGMark)//设置复合组分类结果
{
	CGMark=cGMark;
}
void RuleREPoint::ReadEValue()//读取特征值
{
	CString gIDstr=LongID.Left(LongID.Find('.'));
	CString varStr=LongID.Right(LongID.GetLength()-LongID.Find('.')-1);
	
	CString gTableName,cGTableName;
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pCon=m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);;
	_RecordsetPtr m_pRec;
	CString tableName;
	gTableName.Format("t_u%ds%d_group%s_class_hist",m_iUnit,m_iSet,gIDstr);
	cGTableName.Format("t_u%ds%d_combgroup%d_class_hist",m_iUnit,m_iSet,pRuleFB->BID);
	try   
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		strSql.Format("SELECT %s FROM `%s` WHERE class_mark=(SELECT SubGroup%s_ClassMark FROM %s WHERE class_mark='%s')",varStr,gTableName,gIDstr,cGTableName,CGMark);
        m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pCon,true),
			adOpenStatic,adLockOptimistic,adCmdText);
		//AfxMessageBox(strSql);
	}   
	catch(_com_error  *e)
	{   
		AfxMessageBox(e->ErrorMessage());
		return;
	}
	while(!m_pRec->adoEOF)
	{
		const CString constVarStr= varStr;
		PValue=::atof((char *)_bstr_t(m_pRec->GetCollect((_variant_t)constVarStr)));
		m_pRec->MoveNext();
	}
}
double RuleREPoint::GetValue()
{
	if(PType == 1)	//计算点
	{
		CalcFormula();
	}
	return PValue;
}
int RuleREPoint::ParseFormula()
{
	//如果不是计算点 
	if(PType != 1)
		return -1;
	//解析公式，返回解析状态
	if (PFormula=="") return -1;

	try
	{
		m_BTree = NULL;
		m_Position = 0;
		m_BTree = pmOP();//递归解析公式生成二叉树

		//判断是否到了表达式末尾
		if ((PFormula[m_Position] != '@') || (m_BTree == NULL))
		{
			m_BTree = NULL;
			IsFormula = false;
			return -1;
		}
		else
		{
			IsFormula = true;
			return 0;
		}
	}
	catch(CException *e)
	{
		/*
		Console::ForegroundColor = ConsoleColor::DarkRed;
		str=String::Concat(ThreadHandle::strModuleName,S":\t",S"解析公式出错！测点名为'",LongID,S"'错误原因：ParseFormula->",e->Message);
		StringToChar(str,cLog1,LOG_MAX);//转换日志	
		ThreadHandle::log->fatal(cLog1);
		*/
		e->Delete();
		IsFormula=false;
		return -1;
	}
}
//计算点值(计算点)
int RuleREPoint::CalcFormula()
{
	if (!m_BTree) return -1;
	if (IsFormula == false) return -1;

	if(b_Calced)	//如果已经计算
		return 0;
	b_Calced = true;
	try
	{
		PValue = ReportValue(m_BTree);	
		return 0;
	}
	catch(CException *e)
	{
		/*
		Console::ForegroundColor = ConsoleColor::DarkRed;
		str=String::Concat(ThreadHandle::strModuleName,S":\t",S"计算点值出错！测点名：'",LongID,S"'错误原因：CalcFormula->",e->Message);
		StringToChar(str,cLog1,LOG_MAX);//转换日志	
		ThreadHandle::log->fatal(cLog1);
		*/
		e->Delete();
		return -1;
	}
}

double RuleREPoint::ReportValue (CBTree* pnod)
{
	CString FuncName;
	CPoteDll* PoteDll;
	CBTree* pNodRef;	//用于记录来源点

	int iError=0;
	double v =0;
	if (pnod->oprt==NULL) return v;
	switch(pnod->oprt) 
	{
		//对于数值：
	case VALD : return (pnod->valueD);
		//对于四则运算：
	case ADD : return( ReportValue((CBTree*)(pnod->Leaf[0]))+ReportValue((CBTree*)(pnod->Leaf[1])) );
	case MIN : return( ReportValue((CBTree*)(pnod->Leaf[0]))-ReportValue((CBTree*)(pnod->Leaf[1])) );
	case MUL : return( ReportValue((CBTree*)(pnod->Leaf[0]))*ReportValue((CBTree*)(pnod->Leaf[1])) );
	case DEV : v=ReportValue((CBTree*)(pnod->Leaf[1])) ;
		if (v==0)	return -ReportValue((CBTree*)(pnod->Leaf[0]))/0.001;
		else		return(ReportValue((CBTree*)(pnod->Leaf[0]))/v);
	case IV  : 
		return(pow(ReportValue((CBTree*)(pnod->Leaf[0])),ReportValue((CBTree*)(pnod->Leaf[1]))));
		
		//对于算术运算：
	case NOT : return(!ReportValue((CBTree*)(pnod->Leaf[0]))) ;
	case SIN : return(sin(ReportValue((CBTree*)(pnod->Leaf[0]))));
	case COS : return(cos(ReportValue((CBTree*)(pnod->Leaf[0]))));
	case EXP : return(exp(ReportValue((CBTree*)(pnod->Leaf[0]))));
	case SQRT: v=ReportValue((CBTree*)(pnod->Leaf[0])) ;
		if (v<0) return 0;
		else return(sqrt(v));
	case LOG : v=ReportValue((CBTree*)(pnod->Leaf[0])) ;
		if (v<=0) return 0;
		else return(log(v));
	case TAN : return (tan (ReportValue((CBTree*)(pnod->Leaf[0]))));
	case CTAN: return (1 / tan (ReportValue((CBTree*)(pnod->Leaf[0]))));
	case ASIN: return (asin (ReportValue((CBTree*)(pnod->Leaf[0]))));
	case ACOS: return (acos (ReportValue((CBTree*)(pnod->Leaf[0]))));
	case ATAN: return (atan (ReportValue((CBTree*)(pnod->Leaf[0]))));
	case ABS : return (fabs(ReportValue((CBTree*)(pnod->Leaf[0]))));
		
		//对于逻辑运算:
	case AND : return(ReportValue((CBTree*)(pnod->Leaf[0])) && ReportValue((CBTree*)(pnod->Leaf[1])));
	case OR : return(ReportValue((CBTree*)(pnod->Leaf[0])) || ReportValue((CBTree*)(pnod->Leaf[1])));
		
		//对于关系运算:
	case EQ : return(ReportValue((CBTree*)(pnod->Leaf[0])) == ReportValue((CBTree*)(pnod->Leaf[1])));
	case NE : return(ReportValue((CBTree*)(pnod->Leaf[0])) != ReportValue((CBTree*)(pnod->Leaf[1])));
	case SE : return(ReportValue((CBTree*)(pnod->Leaf[0])) <= ReportValue((CBTree*)(pnod->Leaf[1])));
	case LE : return(ReportValue((CBTree*)(pnod->Leaf[0])) >= ReportValue((CBTree*)(pnod->Leaf[1])));
	case LA : return(ReportValue((CBTree*)(pnod->Leaf[0])) > ReportValue((CBTree*)(pnod->Leaf[1])));
	case SM : return(ReportValue((CBTree*)(pnod->Leaf[0])) < ReportValue((CBTree*)(pnod->Leaf[1])));
		
		//对于测点变量:
	case PAR : 
		{
			if(!pnod->pRuleREPoint->b_Calced)
				pnod->pRuleREPoint->CalcFormula();
			double value = pnod->pRuleREPoint->GetValue();
			PTime = pnod->pRuleREPoint->PTime;
			if(pnod->pRuleREPoint->PState != 1)	//点状态不正确
				PState = 0;
			return value;
		}
	case IF : 
		if(ReportValue(static_cast<CBTree*>(pnod->Leaf[0])))
		{
			return ReportValue(static_cast<CBTree*>(pnod->Leaf[1]));
		}
		else
		{	
			return ReportValue(static_cast<CBTree*>(pnod->Leaf[2]));
		}
	case FUNC :
		rtFunc = NULL;
		for (int i=0; i<(*pListPoteDll).GetSize(); i++)
		{
			PoteDll = (CPoteDll*)((*pListPoteDll)[i]);
			FuncName = pnod->oprtName;
			for (int flag=0; flag<PoteDll->lstPoteFuncName.GetSize();flag++)
			{
				if (PoteDll->lstPoteFuncName[flag] == FuncName)
				{
					rtFunc = (RTFUNC)PoteDll->lstPoteFuncAddr[flag];
					if (rtFunc)	break;
				}
			}
			if (rtFunc)	break;
		}
		//找到函数指针后，进行调用计算过程：
		if(rtFunc)
		{
			if(0==pnod->type)//0计算点,计算单点数据
			{
				if (pnod->Leaf.GetSize()==0)
				{
					//Console::WriteLine(String::Concat("调用实时函数时有没有测点数据！测点名：",LongID));
					PState = -1;
					return 0;
				}
				for (int i=0; i<pnod->Leaf.GetSize(); i++)
				{
					pNodRef = (CBTree*)(pnod->Leaf[i]);
					if (pNodRef == NULL)
					{
						PState = -1;
						return 0;
					}
					PSourceValue[i].dValue = ReportValue(pNodRef);
					if (pNodRef->pRuleREPoint->PState == 1)
						PSourceValue[i].wState = 1;
					else 
						PSourceValue[i].wState = 0;
					PSourceValue[i].tTime = pNodRef->pRuleREPoint->PTime;
				}
				*pG = rtFunc(PSourceValue, pnod->Leaf.GetSize());
			}
			/*
			else if(1==pnod->type)//1统计点;计算统计数据
			{
				int leafNum=pnod->Leaf.GetSize();
				if(leafNum<1)//参数至少一个
				{
					PState = -1;
					return 0;
				}
				pNodRef = (CBTree*)(pnod->Leaf[0]);
				if (pNodRef == NULL)
				{
					PState = -1;
					return 0;
				}
				//判断调用统计函数时有没有测点数据
				pNodRef = (CBTree*)(pnod->Leaf[leafNum-1]);//获取最后一个参数（统计点）结点
				if (pNodRef->RtPt->NumData==0) 
				{
					PState = -1;
					return 0;
				}
				for (int i=0; i<pnod->Leaf.GetSize()-1; i++)//获取统计函数除最后一个，之前的所有参数
				{
					pNodRef = (CBTree*)(pnod->Leaf[i]);
					if (pNodRef == NULL)
					{
						PState = -1;
						return 0;
					}
					PSourceValue[i].dValue = ReportValue(pNodRef);
					if (pNodRef->RtPt->PState == 1)
						PSourceValue[i].wState = 1;
					else 
						PSourceValue[i].wState = 0;
					PSourceValue[i].tTime = pNodRef->RtPt->PTime;
				}
				pNodRef = (CBTree*)(pnod->Leaf[leafNum-1]);//获取最后一个参数（统计点）结点
				for(i=0;i<pNodRef->RtPt->NumData;i++)//获取统计点的数据
				{
					double vv=pNodRef->RtPt->pDataAll[i].dValue;//for debug

					PSourceValue[i+leafNum-1].dValue=pNodRef->RtPt->pDataAll[i].dValue;
					if (pNodRef->RtPt->PState == 1)
						PSourceValue[i+leafNum-1].wState=1;
					else
						PSourceValue[i+leafNum-1].wState=0;
					PSourceValue[i+leafNum-1].tTime=pNodRef->RtPt->pDataAll[i].tTime;
				}
				*pG = rtFunc(PSourceValue, leafNum+pNodRef->RtPt->NumData-1);
				PTime = pNodRef->RtPt->PTime;
			}
			*/
		}
		if (pG->wState == 1)//在外部dll中，点状态为1，表示点值正确
			return pG->dValue;
		else
			return 0;
	}
	return 0;
}
//消除空格
void RuleREPoint::SkipSpaces()
{
	while (PFormula[m_Position]==' ' && PFormula[m_Position]!='@')
		m_Position ++;
}
CBTree* RuleREPoint::pmOP()
{
	CBTree* nod=NULL;
	CBTree* arb1=NULL;
	CBTree* arb2=NULL;
	
	arb1 = mdOP();
	if  (arb1 == NULL)	return NULL;
	
	while ((PFormula[m_Position]=='-') || (PFormula[m_Position]=='+')) 
	{
		nod=new CBTree;
//		//nod->Leaf = new ArrayList();
		nod->Leaf.Add(arb1);

		if(PFormula[m_Position]=='-')
		{
			nod->oprt=MIN;
			m_Position++;
		}
		else if(PFormula[m_Position]=='+')
		{
			nod->oprt=ADD;
			m_Position++;
		}
		arb2 = mdOP();
		nod->Leaf.Add(arb2);
		if  (arb2 == NULL) 
		{
			delete nod;
			return NULL;
		}
		arb1 = nod;
	}
	return arb1;

}

//////////////////////////////////////////////////////////////////////
//处理“乘、除”
CBTree* RuleREPoint::mdOP()
{
	CBTree* nod=NULL;
	CBTree* arb1=NULL;
	CBTree* arb2=NULL;

	arb1 = involOP();
	if  (arb1 == NULL) return NULL;
	
	while ((PFormula[m_Position]=='*') || (PFormula[m_Position]=='/')) 
	{
		nod=new CBTree;
//		//nod->Leaf = new ArrayList();
		nod->Leaf.Add(arb1);

		if (PFormula[m_Position]=='*')
		{
			nod->oprt=MUL;
			m_Position++;
		}
		else if (PFormula[m_Position]=='/')
		{
			nod->oprt=DEV;
			m_Position++;
		}
		arb2 = involOP();
		nod->Leaf.Add(arb2);
		if  (arb2 == NULL) 
		{
			delete nod;
			return NULL;
		}
		arb1 = nod;
	}
	return arb1;

}

//////////////////////////////////////////////////////////////////////
//处理乘方
CBTree* RuleREPoint::involOP()
{
	CBTree* nod = NULL;
	CBTree* arb1=NULL;
	CBTree* arb2=NULL;

	arb1 = logicOP();
	if  (arb1 == NULL) return NULL;
	while (PFormula[m_Position]=='^') 
	{
		nod=new CBTree;
		//nod->Leaf = new ArrayList();
		nod->Leaf.Add(arb1);
		nod->oprt=IV;
		m_Position++;
		arb2 = logicOP();
		nod->Leaf.Add(arb2);
		if  (arb2 == NULL) 
		{
			delete nod;
			return NULL;
		}
		arb1 = nod;
	}
	return arb1;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//处理逻辑运算符：
CBTree* RuleREPoint::logicOP()
{
	CBTree* nod=NULL;
	CBTree* arb1=NULL;
	CBTree* arb2=NULL;

	arb1 = logsecOP();
	if  (arb1 == NULL) return NULL;
	while ((PFormula[m_Position]=='&') || (PFormula[m_Position]=='|')) 
	{
		nod=new CBTree;
		//nod->Leaf = new ArrayList();
		nod->Leaf.Add(arb1);
		if (PFormula[m_Position]=='&')
		{
			nod->oprt=AND;
			m_Position++;
		}
		else if (PFormula[m_Position]=='|')
		{
			nod->oprt=OR;
			m_Position++;
		}
		arb2 = logsecOP();
		nod->Leaf.Add(arb2);
		if  (arb2 == NULL) 
		{
			delete nod;
			return NULL;
		}
		arb1 = nod;
	}
	return arb1;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//处理关系运算符：
CBTree* RuleREPoint::logsecOP()
{
	CBTree* nod =NULL;
	CBTree* arb1=NULL;
	CBTree* arb2=NULL;

	arb1 = factorOP();
	if  (arb1 == NULL) return NULL;
	
	while ( (PFormula[m_Position]=='=') || (PFormula[m_Position]=='<') ||
		(PFormula[m_Position]=='>') || (PFormula[m_Position]=='!') ) 
	{
		nod=new CBTree;
		//nod->Leaf = new ArrayList();
		nod->Leaf.Add(arb1);
		if ((PFormula[m_Position]=='!')&&(PFormula[m_Position+1]=='='))
		{
			nod->oprt=NE;
			m_Position+=2;
		}
		else if ((PFormula[m_Position]=='<')&&(PFormula[m_Position+1]=='='))
		{
			nod->oprt=SE;
			m_Position+=2;
		}
		else if ((PFormula[m_Position]=='>')&&(PFormula[m_Position+1]=='='))
		{
			nod->oprt=LE;
			m_Position+=2;
		}
		else if ((PFormula[m_Position]=='=')&&(PFormula[m_Position+1]=='='))
		{
			nod->oprt=EQ;
			m_Position+=2;
		}
		else if (PFormula[m_Position]=='>')
		{
			nod->oprt=LA;
			m_Position++;
		}
		else if (PFormula[m_Position]=='<')
		{
			nod->oprt=SM;
			m_Position++;
		}
		arb2 = factorOP();
		nod->Leaf.Add(arb2);
		if  (arb2 == NULL) 
		{
			delete nod;
			return NULL;
		}
		arb1 = nod;
	}
	return arb1;
}

//////////////////////////////////////////////////////////////////////
//处理括号
CBTree* RuleREPoint::factorOP()
{
	CBTree* nod = NULL;
	CBTree* left = NULL;
	CBTree* right = NULL;
	
	SkipSpaces();
	if (PFormula[m_Position]=='(')
	{
		m_Position++;
		nod = pmOP();
		if (nod == NULL) return NULL;
		if (PFormula[m_Position]!=')')
		{
			delete nod;
			return NULL;
		}
		m_Position++;
		return nod;
	}
	//当为负号"-"
	else if (PFormula[m_Position]=='-')
	{
		left = new CBTree;
		//left->Leaf[1]=NULL;
		//left->Leaf[0]=NULL;
		left->oprt=VALD;
		left->valueD=-1;

		nod = new CBTree;
		//nod->Leaf = new ArrayList();
		nod->Leaf.Add(left);
		nod->oprt=MUL;

		m_Position++;
		right = factorOP();
		if (right == NULL) return NULL;
		nod->Leaf.Add(right);

		return nod;   
	}
	else return identiOP();
}

//////////////////////////////////////////////////////////////////////
//处理数字、点号和各类函数
CBTree* RuleREPoint::identiOP()
{
	CBTree* nod = NULL;
	int poz=m_Position;
	SkipSpaces();
	if (PFormula[m_Position]=='|') return NULL;

	
////////////////////////////////////////////////////////////////jch+

	int pozNull=m_Position;//用来标记dbnull临时标志。
	int m_PositionNull=m_Position;//用来标记dbnull临时标志。
	/*
//解析dbnull，如果字符串不是DBNULL则恢复全局标志，否则更新标志位
	if (PFormula[m_PositionNull] != 0)
	{
		//--检查函数
		while (PFormula[m_PositionNull] != 0)
			m_PositionNull++;
		//CString ID = PFormula->Substring(pozNull,m_PositionNull-pozNull);
		CString ID = Substring(PFormula,pozNull,m_PositionNull-pozNull);
		ID.MakeUpper();	

		if (ID == "DBNULL")		// 当配置为DBNULL时，在程序中赋值为123456789
		{
			//Console::WriteLine("DBNULLaaaaaaaaaaaaa");//jch
			nod=new CBTree;
			nod->oprt=VALD;
			nod->valueD=123456789;
            m_Position=m_PositionNull;
            poz=m_Position;
			return nod;
		}
	}
	*/
//////////////////////////////////////////////////////jch+

	//处理数字：
	if (IsDigit(PFormula[m_Position]))
	{
		while  ( IsDigit(PFormula[m_Position]) || (PFormula[m_Position]=='.') )
			m_Position++;
		nod = new CBTree;
		nod->oprt = VALD;

		CString strNum;
		strNum = Substring(PFormula,poz,m_Position-poz);

		nod->valueD = ::atof(strNum.GetBuffer(strNum.GetLength()));
		strNum.ReleaseBuffer();
		//nod->valueD =  System::Convert::ToDouble(strNum);
		return nod;
	}
	//处理测点变量,测点变量用方括号“[]”扩起来：
	else if (PFormula[m_Position]=='[')
	{
		m_Position++;
		poz=m_Position;
		while (IsDigit(PFormula[m_Position]) 
			|| IsLetter(PFormula[m_Position]) 
			|| (PFormula[m_Position]=='.') 
			|| (PFormula[m_Position]=='_') 
			|| (PFormula[m_Position]==':') 
			|| (PFormula[m_Position]=='-')
			|| (PFormula[m_Position]=='@')) 
			m_Position++;
		if (PFormula[m_Position]!=']')
			return NULL;

		nod = new CBTree;
		nod->oprt = PAR;

		CString ID = Substring(PFormula,poz,m_Position-poz);
		m_Position++;

		nod->pRuleREPoint =	(*pRuleREPointMap)[ID.GetBuffer(ID.GetLength())];
		ID.ReleaseBuffer();

		if(nod->pRuleREPoint == NULL)
		{
			//CString strLog;	strLog.Format("\"%s\"模块的\"%s\"的公式配置有错误,\"%s\"不存在",sCalcSource,LongID,ID);
			//g_pLogfile.WriteFileData(strLog);
			//AfxMessageBox(strLog);
			delete nod; 
			nod=NULL;
			return NULL;
		}
		return nod;
	}	

	//对于算术函数及预定义函数
	else if (IsLetter(PFormula[m_Position]))
	{
		//--检查函数
		while (IsLetter(PFormula[m_Position]))
			m_Position++;

		if (PFormula[m_Position]!='(') return NULL;
		CString ID = Substring(PFormula,poz,m_Position-poz);
		ID.MakeUpper();	
		m_Position++;

		if (ID == "DBNULL")		// 当配置为DBNULL时，在程序中赋值为123456789
		{
			//Console::WriteLine("DBNULLaaaaaaaaaaaaa");//jch
			nod=new CBTree;
			nod->oprt=VALD;
			nod->valueD=123456789;
			return nod;
		}

		CBTree* nod1;
		if (ID == "SIN")		// Functia sinus CString
		{
			nod1 = factorOP();
			if (PFormula[m_Position]!=')')
			{
				delete nod1;
				return NULL;
			}
			m_Position++;
			nod = new CBTree;
			nod->oprt=SIN;
			//nod->Leaf = new ArrayList();
			nod->Leaf.Add(nod1);
			SkipSpaces();
			return nod;
		}
		if (ID == "COS")		// Functia sinus CString
		{
			nod1 = factorOP();
			if (PFormula[m_Position]!=')')
			{
				delete nod1;
				return NULL;
			}
			m_Position++;
			nod = new CBTree;
			nod->oprt=COS;
			//nod->Leaf = new ArrayList();
			nod->Leaf.Add(nod1);
			SkipSpaces();
			return nod;
		}
		if (ID == "EXP")	// Functia sinus CString
		{
			nod1 = factorOP();
			if (PFormula[m_Position]!=')')
			{
				delete nod1;
				return NULL;
			}
			m_Position++;
			nod = new CBTree;
			nod->oprt=EXP;
			//nod->Leaf = new ArrayList();
			nod->Leaf.Add(nod1);
			SkipSpaces();
			return nod;
		}
		if (ID = "SQRT") 		// Functia sinus CString
		{
			nod1 = factorOP();
			if (PFormula[m_Position]!=')')
			{
				delete nod1;
				return NULL;
			}
			m_Position++;
			nod = new CBTree;
			nod->oprt=SQRT;
			//nod->Leaf = new ArrayList();
			nod->Leaf.Add(nod1);
			SkipSpaces();
			return nod;
		}
		if (ID == "LOG")		// Functia sinus CString
		{
			nod1 = factorOP();
			if (PFormula[m_Position]!=')')
			{
				delete nod1;
				return NULL;
			}
			m_Position++;
			nod = new CBTree;
			nod->oprt=LOG;
			//nod->Leaf = new ArrayList();
			nod->Leaf.Add(nod1);
			SkipSpaces();
			return nod;
		}
		if (ID == "ABS")	// Functia sinus CString
		{
			nod1 = factorOP();
			if (PFormula[m_Position]!=')')
			{
				delete nod1;
				return NULL;
			}
			m_Position++;
			nod = new CBTree;
			nod->oprt=ABS;
			//nod->Leaf = new ArrayList();
			nod->Leaf.Add(nod1);
			SkipSpaces();
			return nod;
		}
		if (ID == "TAN")		// Functia sinus CString
		{
			nod1 = factorOP();
			if (PFormula[m_Position]!=')')
			{
				delete nod1;
				return NULL;
			}
			m_Position++;
			nod = new CBTree;
			nod->oprt=TAN;
			//nod->Leaf = new ArrayList();
			nod->Leaf.Add(nod1);
			SkipSpaces();
			return nod;
		}
		if (ID == "CTAN")		// Functia sinus CString
		{
			nod1 = factorOP();
			if (PFormula[m_Position]!=')')
			{
				delete nod1;
				return NULL;
			}
			m_Position++;
			nod = new CBTree;
			nod->oprt=CTAN;
			//nod->Leaf = new ArrayList();
			nod->Leaf.Add(nod1);
			SkipSpaces();
			return nod;
		}
		if (ID=="ASIN")		// Functia sinus CString
		{
			nod1 = factorOP();
			if (PFormula[m_Position]!=')')
			{
				delete nod1;
				return NULL;
			}
			m_Position++;
			nod = new CBTree;
			nod->oprt=ASIN;
			//nod->Leaf = new ArrayList();
			nod->Leaf.Add(nod1);
			SkipSpaces();
			return nod;
		}
		if (ID == "ACOS")		// Functia sinus CString
		{
			nod1 = factorOP();
			if (PFormula[m_Position]!=')')
			{
				delete nod1;
				return NULL;
			}
			m_Position++;
			nod = new CBTree;
			nod->oprt=ACOS;
			//nod->Leaf = new ArrayList();
			nod->Leaf.Add(nod1);
			SkipSpaces();
			return nod;
		}
		if (ID == "ATAN")		// Functia sinus CString
		{
			nod1 = factorOP();
			if (PFormula[m_Position]!=')')
			{
				delete nod1;
				return NULL;
			}
			m_Position++;
			nod = new CBTree;
			nod->oprt=ATAN;
			//nod->Leaf = new ArrayList();
			nod->Leaf.Add(nod1);
			SkipSpaces();
			return nod;
		}
	}

	//处理自定义函数：
	else if (PFormula[m_Position]=='$')//处理计算函数
	{
		m_Position++;
		poz=m_Position;
		while (IsDigit(PFormula[m_Position]) 
			|| IsLetter(PFormula[m_Position]) 
			|| (PFormula[m_Position]=='_') 
			|| (PFormula[m_Position]=='-'))
			m_Position++;
		if (PFormula[m_Position]!='(') return NULL;
		CString ID = Substring(PFormula,poz,m_Position-poz);

		m_Position++;

		ID.MakeUpper();
		nod = new CBTree;
		//nod->Leaf = new ArrayList();

		//判断语句：
		if (ID == "IF")
		{
			nod->oprt = IF;
		}
		else
		{
			nod->oprt = FUNC;
			nod->oprtName = ID;
		}

		//定义函数指向参数列表的指针：
		CBTree* nod1;
		nod1 = pmOP();	//第一个参数，默认至少有一个
		nod->Leaf.Add(nod1);

		SkipSpaces();
		//根据函数参数的个数定义参数二叉树链表，左、右节点指向1、2个参数
		int i = 1;
		while (PFormula[m_Position]==',')
		{
			m_Position++;
			nod1 = pmOP();
			nod->Leaf.Add(nod1);
			SkipSpaces();
		}

		if (PFormula[m_Position]!=')')
		{
			delete nod1;
			return NULL;
		}
		m_Position++;
		//返回的函数指针：
		return nod;
	}
	else if (PFormula[m_Position]=='#')//处理统计函数
	{
		m_Position++;
		poz=m_Position;
		while (IsDigit(PFormula[m_Position]) 
			|| IsLetter(PFormula[m_Position]) 
			|| (PFormula[m_Position]=='_') 
			|| (PFormula[m_Position]=='-'))
			m_Position++;
		if (PFormula[m_Position]!='(') return NULL;
		CString ID = Substring(PFormula,poz,m_Position-poz);

		m_Position++;
		
		ID.MakeUpper();
		nod = new CBTree;
		//nod->Leaf = new ArrayList();
		
		nod->type=1;//标记为统计点
		nod->oprt = FUNC;
		nod->oprtName = ID;
		
		//定义函数指向参数列表的指针：
		CBTree* nod1;
		nod1 = pmOP();	//第一个参数，默认至少有一个
		nod->Leaf.Add(nod1);

		SkipSpaces();
		//根据函数参数的个数定义参数二叉树链表，左、右节点指向1、2个参数
		int i = 1;
		while (PFormula[m_Position]==',')
		{
			m_Position++;
			nod1 = pmOP();
			nod->Leaf.Add(nod1);
			SkipSpaces();
		}
		char chaTest=PFormula[m_Position];
		if (PFormula[m_Position]!=')')
		{
			delete nod1;
			return NULL;
		}
		m_Position++;
		//返回的函数指针：
		return nod;
	}
	return NULL;
}
bool RuleREPoint::IsDigit(char byte)
{
	if((byte <= '9')&&(byte >='0'))
		return true;
	else
		return false;
}
CString RuleREPoint::Substring(CString str,int pos1,int count)	//取出字符串
{
	CString strTemp = str.Right(str.GetLength()-pos1);
	CString strTemp2 = strTemp.Left(count);
	return strTemp2;
}
bool RuleREPoint::IsLetter(char byte)
{
	if(((byte >= 0x0041)&&(byte <= 0x005A))||((byte >= 0x0061)&&(byte <= 0x007A))||((byte >= 0x002B0)&&(byte <= 0x02C1))||((byte >= 0x05D0)&&(byte <= 0x05EA)))
		return true;
	else
		return false;
}