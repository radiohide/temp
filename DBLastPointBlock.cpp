// DBLastPointBlock.cpp: implementation of the CDBLastPointBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "DBLastPointBlock.h"
#include "PropertyDlgDBLastPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDBLastPointBlock::CDBLastPointBlock()
{
	m_bConDB=false;
	m_TagTimeOffsetOrInput=1;//1:采用系统时间+偏移。
	m_lOffset=0;
	m_strTagName="";
	m_strTagDesp="";
	m_iForwardOrBack=0;
	m_strTableName="point_data";
	SetBlockType(_T( "DBLastPoint" ) );

	//默认字段名字
	m_strFieldVarName="var_name";
	m_strFieldDateTime="DateTime";
	m_strFieldState="State";
	m_strFieldValue="Value";
	ConstructBlockInputOutput();
}

CDBLastPointBlock::~CDBLastPointBlock()
{
	
}
//根据类型新建对象的函数
CCalcBlock* CDBLastPointBlock::CreateFromString( const CString& strType )
{
	CDBLastPointBlock* obj = new CDBLastPointBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CDBLastPointBlock::ConstructBlockInputOutput ()
{
	AddOutputPort("Out",VALUE_DOUBLE);
}
//初始化计算的函数做特殊的初始化
void CDBLastPointBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//返回属性的逗点字符串
CString CDBLastPointBlock::GetBlockStrPro()
{
	CString str;
	str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CDBLastPointBlock::SetBlockStrPro(CString str)//设置计算块的字符串属性
{
	m_strTagName=str.Left(str.Find('@'));
	m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
//计算函数，实现本模块的计算
void CDBLastPointBlock::Calc()//进行计算的函数,需要重载
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = m_pModel->GetSourceRDBByDesp(m_strDBDesp,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
		return;

	double dRetValue=0;
	long lRetTime=0;
	int iRetState=192;
	GetInputValueFromGlobalWS();
    //计算过程
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	double result=0;
	
	long timeflag=0;
	if(m_TagTimeOffsetOrInput==1)
	{
		timeflag=this->m_pModel->m_lDataTime-m_lOffset;
	}
	else
	{
		double inValue=0;
		CCalcPort *pPortObj = GetInputPortObj(0);
		pPortObj->GetPortUniValue().GetDoubleVal(&inValue);
		timeflag=(long)inValue;
	}
	if(timeflag<0)//时间不能为负数
		 timeflag=0;

	CTime t(timeflag);
	CString csTableName="";
	csTableName.Format("%s_%d",m_strTableName,t.GetYear());

	if(m_strTagName!="")
	{
		getValueFromDB(m_pConRDB,strDBType,csTableName,m_strTagName,timeflag,m_iForwardOrBack,
			dRetValue,lRetTime,iRetState);
	}
	CUniValue UniVar;
	UniVar.AddData(dRetValue,timeflag,iRetState);
	pOutPortObj->SetPortUniValue(UniVar);
	OutputResultToGlobalWS();
}
void CDBLastPointBlock::DrawShape(CDC* dc, CRect rect,int CutSize)
{
	//dc->Rectangle(rect);
	CPoint roundness;
	////////////////////////////////////////////////
	CRect rc(rect);
	rc.top = rect.top + 2*CutSize;
	rc.bottom = rect.bottom - 2*CutSize; 
    roundness.x = int(double(rc.Width())*0.6);
	roundness.y = int(double(rc.Height())*0.6);
	dc->RoundRect(rc,roundness);
 
	//画中线
	int MidX = rc.left+int(double(rc.Width())/2);
	//dc->MoveTo(MidX, rc.top);
	//dc->LineTo(MidX,rc.bottom);
	//画中间的园表示实时
	CRect rcCenter;
	int MidY = rc.top+int(double(rc.Height())/2);
	int Dis =  int(double(rc.Height())*0.25);
	rcCenter.SetRect(MidX-Dis,MidY-Dis,MidX+Dis,MidY+Dis);
    dc->Rectangle(rcCenter);
	///////////////////////////////////////////////
}
BOOL CDBLastPointBlock::getValueFromDB(_ConnectionPtr &m_pConRDB,
								   CString strDBType,
								   CString strTableName,
								   CString strTagName,
								   long timeflag,
								   int iForwardOrBack,
								   double &dRetValue,
								   long &lRetTime,
								   int &iRetState)
{
	CTime  t1(timeflag);
	CString timestr1;
	timestr1.Format("%d-%.2d-%.2d %.2d:%.2d:%.2d",t1.GetYear(),t1.GetMonth(),t1.GetDay(),t1.GetHour(),t1.GetMinute(),t1.GetSecond());
	
	_RecordsetPtr m_pRec;

	//表不存在，认为该表还未创建，累加值输出0
	if(!gIsTableExist(m_pConRDB,strTableName))
	{
		dRetValue = 0.0;
		iRetState = 192;//计算累计值写回vestore要求192
		return TRUE;
	}
	
	try   
	{
		strDBType.MakeUpper();
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		if(iForwardOrBack==0)
		{
			if(strDBType=="MYSQL")
			{
				strSql.Format("select  * from %s  where %s='%s' and %s<='%s' order by DateTime desc limit 0,1;",
					strTableName,m_strFieldVarName,strTagName,m_strFieldDateTime,timestr1);
			}
			else if(strDBType=="MDB")
			{
				strSql.Format("select top 1 * from %s  where %s='%s' and %s<=#%s# order by DateTime desc;",
					strTableName,m_strFieldVarName,strTagName,m_strFieldDateTime,timestr1);
			}
		}
		else if(iForwardOrBack==1)
		{
			if(strDBType=="MYSQL")
			{
				strSql.Format("select  * from %s  where %s='%s' and %s>='%s' order by DateTime asc limit 0,1;",
					strTableName,m_strFieldVarName,strTagName,m_strFieldDateTime,timestr1);
			}
			else if(strDBType=="MDB")
			{
				strSql.Format("select top 1 * from %s  where %s='%s' and %s>=#%s# order by DateTime asc;",
					strTableName,m_strFieldVarName,strTagName,m_strFieldDateTime,timestr1);
			}
		}
		
		m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pConRDB,true),
			adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error  *e)   
	{   
		CString str;
		str.Format("CDBPointBlock->getValueFromDB(...)->%s",e->ErrorMessage());
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
	BOOL  bRet=FALSE;

	if (m_pRec->adoEOF)
	{
		dRetValue = 0.0;
		iRetState = 192;
		bRet = TRUE;
		return bRet;
	}

	while(!m_pRec->adoEOF)
	{
		// CString strConFieldDateTime=this->m_strFieldDateTime;
		CString U_Input_Date =(char *)_bstr_t(m_pRec->GetCollect((_variant_t)m_strFieldDateTime));
		CString U_Input_Value =(char *)_bstr_t(m_pRec->GetCollect((_variant_t)m_strFieldValue));
		CString U_Input_State =(char *)_bstr_t(m_pRec->GetCollect((_variant_t)m_strFieldState));
		
		dRetValue=atof(U_Input_Value);
		iRetState=atoi(U_Input_State);

		//
		if (iRetState == 0)
		{
			iRetState = 192;//纠正前面错误数据
		}

		
		int   nYear,   nMonth,   nDate;
		int nHour=0;
		int nMin=0;
		int nSec=0;   

		//注释掉，时间戳应打上当前模型计算时间戳，不应打数据库中的时间戳
		/*if(U_Input_Date.Find('/')>=0)
		{
			sscanf(U_Input_Date, "%d/%d/%d   %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);
		}
		else
		{
			sscanf(U_Input_Date, "%d-%d-%d   %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);
		}
		//sscanf(U_Input_Date, "%d-%d-%d   %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);   
		CTime   t(nYear,nMonth,nDate,nHour,nMin,nSec);	
		lRetTime=t.GetTime();*/

		bRet=TRUE;
		break; 
	}
	return bRet;	
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CDBLastPointBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("TimeOffsetOrInput")==0)
	{
		if(!strItem1.IsEmpty())  {m_TagTimeOffsetOrInput=atoi(strItem1);}
	}
	else if(strPropName.Compare("lOffset")==0)
	{
		if(!strItem1.IsEmpty())  {m_lOffset=atol(strItem1);}
	}
	else if(strPropName.Compare("TagName")==0)
	{
		if(!strItem1.IsEmpty())  {m_strTagName=strItem1;}
	}
	else if(strPropName.Compare("TagDesp")==0)
	{
		if(!strItem1.IsEmpty())  {m_strTagDesp=strItem1;}
	}
	else if(strPropName.Compare("ForwardOrBack")==0)
	{
		if(!strItem1.IsEmpty())  {m_iForwardOrBack=atoi(strItem1);}
	}
	else if(strPropName.Compare("TableName")==0)
	{
		if(!strItem1.IsEmpty())  {m_strTableName=strItem1;}
	}
	else if(strPropName.Compare("DBDesp")==0)
	{
		if(!strItem1.IsEmpty())  {m_strDBDesp=strItem1;}
	}
	else if(strPropName.Compare("StrField")==0)
	{
		if(!strItem1.IsEmpty())  {m_strFieldVarName=strItem1;}
		if(!strItem2.IsEmpty())  {m_strFieldDateTime=strItem2;}
		if(!strItem3.IsEmpty())  {m_strFieldState=strItem3;}
		if(!strItem4.IsEmpty())  {m_strFieldValue=strItem4;}
	}
}
//重载以显示不同的属性对象对话框
void CDBLastPointBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBefore=this->GetInputNum();//新增
	CStringArray  IDArrLeftOut;//计算块原有的输入端口描述
	IDArrLeftOut.RemoveAll();
	
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		IDArrLeftOut.Add(strPortID);
	}

	CPropertyDlgDBLastPoint dlg;
    //给对话框变量赋值
	dlg.m_lOffset=m_lOffset;
	dlg.m_strTagName=m_strTagName;
	dlg.m_strTagDesp=m_strTagDesp;
	dlg.m_TagTimeOffsetOrInput=m_TagTimeOffsetOrInput;
	dlg.m_iForwardOrBack=m_iForwardOrBack;
	dlg.m_strTableName=m_strTableName;
	dlg.m_strDBDesp=m_strDBDesp;
	dlg.m_strFieldVarName=m_strFieldVarName;
	dlg.m_strFieldDateTime=m_strFieldDateTime;
	dlg.m_strFieldState=m_strFieldState;
	dlg.m_strFieldValue=m_strFieldValue;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);
	if(dlg.DoModal()==IDOK)
	{
		m_lOffset=dlg.m_lOffset;
		m_strTagName=dlg.m_strTagName;
		m_strTagDesp=dlg.m_strTagDesp;
		m_TagTimeOffsetOrInput=dlg.m_TagTimeOffsetOrInput;
		m_iForwardOrBack=dlg.m_iForwardOrBack;
		m_strTableName=dlg.m_strTableName;
		m_strDBDesp=dlg.m_strDBDesp;
		m_strFieldVarName=dlg.m_strFieldVarName;
		m_strFieldDateTime=dlg.m_strFieldDateTime;
		m_strFieldState=dlg.m_strFieldState;
		m_strFieldValue=dlg.m_strFieldValue;
		
		if(m_TagTimeOffsetOrInput==0)//外部
		{
			if(IDArrLeftOut.GetSize()==1){}
			else
			{
				AddInputPort(_T("Offset"), VALUE_DOUBLE, _T("in1"), _T("in1"));
			}
		}
		else
			ClearInputPorts();
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
void CDBLastPointBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="TimeOffsetOrInput";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_TagTimeOffsetOrInput); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="lOffset";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_lOffset); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="TagName";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strTagName); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="TagDesp";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strTagDesp); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="ForwardOrBack";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iForwardOrBack); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="TableName";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strTableName); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="DBDesp";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strDBDesp); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="StrField";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strFieldVarName); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_strFieldDateTime); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_strFieldState); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_strFieldValue); //属性名
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名	

	pRecord->Update();
}

//复制图形时，需要拷贝属性。
void CDBLastPointBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_TagTimeOffsetOrInput=((CDBLastPointBlock*)pBlock)->m_TagTimeOffsetOrInput;
	this->m_lOffset=((CDBLastPointBlock*)pBlock)->m_lOffset;
	this->m_strTagName=((CDBLastPointBlock*)pBlock)->m_strTagName;
	this->m_strTagDesp=((CDBLastPointBlock*)pBlock)->m_strTagDesp;
	this->m_iForwardOrBack=((CDBLastPointBlock*)pBlock)->m_iForwardOrBack;
	this->m_strTableName=((CDBLastPointBlock*)pBlock)->m_strTableName;
	this->m_strDBDesp=((CDBLastPointBlock*)pBlock)->m_strDBDesp;\

	this->m_strFieldVarName=((CDBLastPointBlock*)pBlock)->m_strFieldVarName;
	this->m_strFieldDateTime=((CDBLastPointBlock*)pBlock)->m_strFieldDateTime;
	this->m_strFieldState=((CDBLastPointBlock*)pBlock)->m_strFieldState;
	this->m_strFieldValue=((CDBLastPointBlock*)pBlock)->m_strFieldValue;
}
//将属性放置传输数据中
void CDBLastPointBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TimeOffsetOrInput");	
	CString str;
	str.Format("%d",m_TagTimeOffsetOrInput);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"lOffset");	
	str.Format("%d",m_lOffset);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TagName");	
	str.Format("%s",m_strTagName);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TagDesp");	
	str.Format("%s",m_strTagDesp);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"ForwardOrBack");	
	str.Format("%d",m_iForwardOrBack);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TableName");	
	str.Format("%s",m_strTableName);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"DBDesp");	
	str.Format("%s",m_strDBDesp);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"StrField");
	str.Format("%s",m_strFieldVarName);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%s",m_strFieldDateTime);
	strcpy(pData->m_cValueItem2,str);
	str.Format("%s",m_strFieldState);
	strcpy(pData->m_cValueItem3,str);
	str.Format("%s",m_strFieldValue);
	strcpy(pData->m_cValueItem4,str);
	dataArr.Add(pData);
}