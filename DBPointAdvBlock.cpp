// DBPointAdvBlock.cpp: implementation of the CDBPointAdvBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "DBPointAdvBlock.h"
#include "PropertyDlgDBPointAdv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBPointAdvBlock::CDBPointAdvBlock()
{
	//设置类型
    SetBlockType(_T( "DBPointAdv" ) );
	ConstructBlockInputOutput();
	CString	n_strTagDesp="";
	n_strTagDesp="";
	n_strTagName1="";
	n_strTagName2="";
	n_strTagName3="";
	n_strTagName4="";
	n_strTagName5="";
	n_iOutMethod=0;
	n_bChkLimit=TRUE;
	n_dHighLimit=300;
	n_dLowLimit=50.0;
	n_dConstWhenBad=100.0;
	n_bForceEnable=TRUE;
	n_dForceValue=200.0;

	m_iInOrOut=1;
	m_lOffsetTime=0;
	m_strTableName="point_data";
	m_iForwardOrBack=0;
}

CDBPointAdvBlock::~CDBPointAdvBlock()
{

}
CCalcBlock* CDBPointAdvBlock::CreateFromString( const CString& strType )
{
	CDBPointAdvBlock* obj = new CDBPointAdvBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}

void CDBPointAdvBlock::ConstructBlockInputOutput()
{
	//该模块无输入，当计算完毕后，输出为一个时间序列的数据，
	//可自由定义输入输出的变量名称。
	AddOutputPort("Value",VALUE_DOUBLE);

}
//默认该计算块的计算是可组态公式的基本计算
void CDBPointAdvBlock::Calc()
{
	CUniValue UniVar;
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = m_pModel->GetSourceRDBByDesp(m_strDBDesp,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
		return;
	double dRetValue=0;
	long lRetTime=0;
	int iRetState=0;
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	if(n_bForceEnable==TRUE)
	{
		UniVar.AddData(n_dForceValue);
		pOutPortObj->SetPortUniValue(UniVar);
	}
	else
	{
		GetInputValueFromGlobalWS();
		//计算过程
		CCalcPort *pOutPortObj = GetOutputPortObj(0);
		double result=0;
		
		long timeflag=0;
		if(m_iInOrOut==1)
		{
			timeflag=this->m_pModel->m_lDataTime-m_lOffsetTime;
		}	
		else
		{
			double inValue=0;
			CCalcPort *pPortObj = GetInputPortObj(0);
			pPortObj->GetPortUniValue().GetDoubleVal(&inValue);
			timeflag=(long)inValue;
		}
		if(timeflag<0)//处理时间不能为负数
				timeflag=0;
		
		CStringArray strTagNameArr;//配置的点名的序列
		if(n_strTagName1!="")strTagNameArr.Add(n_strTagName1);
		if(n_strTagName2!="")strTagNameArr.Add(n_strTagName2);
		if(n_strTagName3!="")strTagNameArr.Add(n_strTagName3);
		if(n_strTagName4!="")strTagNameArr.Add(n_strTagName4);
		if(n_strTagName5!="")strTagNameArr.Add(n_strTagName5);
		CArray<double,double> dValueArr;//读取到的所有点的值。
		for(int i=0;i<strTagNameArr.GetSize();i++)
		{
			BOOL bRet = getValueFromDB(m_pConRDB,strDBType,m_strTableName,strTagNameArr[i],timeflag,m_iForwardOrBack,
				dRetValue,lRetTime,iRetState);
			if(bRet==TRUE)
			{
				dValueArr.Add(dRetValue);
			}
		}
		if(n_bChkLimit)
		{
			for(i=dValueArr.GetSize()-1;i>=0;i--)//移出所有坏点
			{
				if((dValueArr[i]<n_dLowLimit)||(dValueArr[i]>n_dHighLimit))
				{
					dValueArr.RemoveAt(i);
				}
			}
		}
		if(dValueArr.GetSize()==0)
		{
			dValueArr.Add(n_dConstWhenBad);
		}	
		double min=0;
		double max=0;
		double sum=0;
		for(i=0;i<dValueArr.GetSize();i++)
		{
			if(0==i)
			{
				min=dValueArr[i];
				max=dValueArr[i];
			}
			else 
			{
				if(dValueArr[i]<min)
					min=dValueArr[i];
				if(dValueArr[i]>max)
					max=dValueArr[i];
			}
			sum+=dValueArr[i];
		}
		if(n_iOutMethod==0)//均值
		{
			if(dValueArr.GetSize()>0)
				result=sum/(dValueArr.GetSize()+0.0);
		}
		else if(n_iOutMethod==1)//中值
		{
			for(int i=0;i<dValueArr.GetSize();i++)//从小到大排序
			{
				for(int j=i+1;j<dValueArr.GetSize();j++)
				{
					if(dValueArr[i]>dValueArr[j])
					{
						double temp=dValueArr[i];
						dValueArr[i]=dValueArr[j];
						dValueArr[j]=temp;
					}
				}
			}
			if(dValueArr.GetSize()%2==1)//奇数个
			{
				result=dValueArr[(dValueArr.GetSize()/2)+1-1];
			}
			else 
			{	
				if(dValueArr.GetSize()>0)
					result=(dValueArr[(dValueArr.GetSize()/2)-1]+dValueArr[(dValueArr.GetSize()/2)])/2.0;
			}
		}
		else if(n_iOutMethod==2)//最大
		{
			result=max;
		}
		else if(n_iOutMethod==3)//最小
		{
			result=min;
		}
		UniVar.AddData(result);
		pOutPortObj->SetPortUniValue(UniVar);
	}
	OutputResultToGlobalWS();
}
void CDBPointAdvBlock::DrawShape(CDC* dc, CRect rect,int CutSize)
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
	dc->MoveTo(MidX, rc.top);
	dc->LineTo(MidX,rc.bottom);
	//dc->MoveTo(MidX+2, rc.top);
	//dc->LineTo(MidX+2,rc.bottom);
	//画中间的园表示实时
	CRect rcCenter;
	int MidY = rc.top+int(double(rc.Height())/2);
	int Dis =  int(double(rc.Height())*0.25);
	rcCenter.SetRect(MidX-Dis,MidY-Dis,MidX+Dis,MidY+Dis);
    dc->Rectangle(rcCenter);
	///////////////////////////////////////////////
}
BOOL CDBPointAdvBlock::getValueFromDB(_ConnectionPtr &m_pConRDB,
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
	
	try   
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		strDBType.MakeUpper();
		if(iForwardOrBack==0)
		{
			if(strDBType=="MYSQL")
			{
				strSql.Format("select  * from %s  where LongID='%s' and DateTime<='%s' order by DateTime desc limit 0,1;",
					strTableName,strTagName,timestr1);
			}
			else if(strDBType=="MDB")
			{
				strSql.Format("select top 1 * from %s  where LongID='%s' and DateTime<=#%s# order by DateTime desc;",
					strTableName,strTagName,timestr1);
			}
		}
		else if(iForwardOrBack==1)
		{
			if(strDBType=="MYSQL")
			{
				strSql.Format("select  * from %s  where LongID='%s' and DateTime>='%s' order by DateTime asc limit 0,1;",
					strTableName,strTagName,timestr1);
			}
			else if(strDBType=="MDB")
			{
				strSql.Format("select top 1 * from %s  where LongID='%s' and DateTime>=#%s# order by DateTime asc;",
					strTableName,strTagName,timestr1);
			}
		}
		
		m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pConRDB,true),
			adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error  *e)   
	{   
		CString str;
		str.Format("CDBPointAdvBlock->getValueFromDB(...)->%s",e->ErrorMessage());
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

	while(!m_pRec->adoEOF)
	{
		CString U_Input_Date =(char *)_bstr_t(m_pRec->GetCollect("DateTime"));
		CString U_Input_Value =(char *)_bstr_t(m_pRec->GetCollect("Value"));
		CString U_Input_State =(char *)_bstr_t(m_pRec->GetCollect("State"));
		
		dRetValue=atof(U_Input_Value);
		iRetState=atoi(U_Input_State);
		
		int   nYear,   nMonth,   nDate;
		int   nHour=0;
		int   nMin=0;
		int   nSec=0;    
		sscanf(U_Input_Date, "%d-%d-%d   %d:%d:%d",   &nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec);   
		CTime   t(nYear,nMonth,nDate,nHour,nMin,nSec);	
		lRetTime=t.GetTime();

		bRet=TRUE;
		break; 
	}
	return bRet;	
}
/////////////////////////////////////////////////////////////////////////
void CDBPointAdvBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(strPropName.Compare("tagnames")==0)
	{
		if(!strItem1.IsEmpty())  n_strTagName1=strItem1;
		if(!strItem2.IsEmpty())  n_strTagName2=strItem2;
		if(!strItem3.IsEmpty())  n_strTagName3=strItem3;
		if(!strItem4.IsEmpty())  n_strTagName4=strItem4;
		if(!strItem5.IsEmpty())  n_strTagName5=strItem5;
	}
	else if(strPropName.Compare("outmethod")==0)
	{
		if(!strItem1.IsEmpty()) 
		{
			n_iOutMethod = atoi(strItem1);
		}
	}
	else if(strPropName.Compare("limit")==0)
	{
		if(!strItem1.IsEmpty()) 
		{
			if(strItem1.Compare("0")==0) n_bChkLimit =FALSE;
			else n_bChkLimit =TRUE;
		}
		if(!strItem2.IsEmpty())  n_dHighLimit = atof(strItem2);
		if(!strItem3.IsEmpty())  n_dLowLimit = atof(strItem3);
	}
	else if(strPropName.Compare("ConstWhenBad")==0)
	{
		if(!strItem1.IsEmpty())  n_dConstWhenBad = atof(strItem1);
	}
	else if(strPropName.Compare("ForceOutput")==0)
	{
		if(!strItem1.IsEmpty()) 
		{
			if(strItem1.Compare("0")==0) n_bForceEnable =FALSE;
			else n_bForceEnable =TRUE;
		}
		if(!strItem2.IsEmpty())  n_dForceValue = atof(strItem2);
	}
	else if(strPropName.Compare("InOrOut")==0)
	{
		if(!strItem1.IsEmpty())  m_iInOrOut = atoi(strItem1);
	}
	else if(strPropName.Compare("OffsetTime")==0)
	{
		if(!strItem1.IsEmpty())  m_lOffsetTime = atoi(strItem1);
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
}
////////////////////////////////////////////////////////////////////////


void CDBPointAdvBlock::InitCalc()
{

	CCalcBlock::InitCalc();
	//
	//
}
void CDBPointAdvBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
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

	CPropertyDlgDBPointAdv dlg;
	dlg.m_strTagDesp = GetBlockDesp();
	///////////////////////////////////////////////////	
	dlg.m_strTag1 = n_strTagName1;	
	dlg.m_strTag2 = n_strTagName2;	
	dlg.m_strTag3 = n_strTagName3;	
	dlg.m_strTag4 = n_strTagName4;	
	dlg.m_strTag5 = n_strTagName5;
	//////////////////////////////////////////////////
	dlg.m_iOutMethod = n_iOutMethod;
	dlg.m_bChkLimit =	n_bChkLimit  ;
	dlg.m_dHighLimit =	n_dHighLimit  ;
	dlg.m_dLowLimit =	n_dLowLimit  ;
	dlg.m_dConstWhenBad  = n_dConstWhenBad;
	dlg.m_bForceEnable = n_bForceEnable;
	dlg.m_dForceValue = n_dForceValue;
	dlg.m_iInOrOut=this->m_iInOrOut;
	dlg.m_lOffset=this->m_lOffsetTime;
	dlg.m_strTableName=m_strTableName;
	dlg.m_strDBDesp=m_strDBDesp;
	dlg.m_iForwardOrBack=m_iForwardOrBack;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);
	////////////////////////////////////////////////////////////////////
	if(dlg.DoModal()==IDOK)
	{
		//1.
		SetBlockDesp(dlg.m_strTagDesp);
		n_strTagName1 = dlg.m_strTag1;
		n_strTagName2 = dlg.m_strTag2;
		n_strTagName3 = dlg.m_strTag3;	
		n_strTagName4 = dlg.m_strTag4;
		n_strTagName5 = dlg.m_strTag5;	
		//3.
		n_iOutMethod = dlg.m_iOutMethod ;
		//4.
		n_bChkLimit = dlg.m_bChkLimit ;
		n_dHighLimit = dlg.m_dHighLimit ;
		n_dLowLimit = dlg.m_dLowLimit;
		//5.
		n_dConstWhenBad = dlg.m_dConstWhenBad ;
       //6.
		n_bForceEnable = dlg.m_bForceEnable ;
		n_dForceValue = dlg.m_dForceValue ;
		this->m_iInOrOut=dlg.m_iInOrOut;
		this->m_lOffsetTime=dlg.m_lOffset;
		m_strTableName=dlg.m_strTableName;
		m_strDBDesp=dlg.m_strDBDesp;
		m_iForwardOrBack=dlg.m_iForwardOrBack;

	    if(m_iInOrOut==0)//外部
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
	//////////////////////////////////////////////////
}
//将属性写到记录的当前位置
void CDBPointAdvBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("tagnames"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
    pRecord->PutCollect(_variant_t("value_item1"),_variant_t(n_strTagName1) );
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(n_strTagName2) );
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(n_strTagName3) );
	pRecord->PutCollect(_variant_t("value_item4"),_variant_t(n_strTagName4) );
	pRecord->PutCollect(_variant_t("value_item5"),_variant_t(n_strTagName5) );
	//属性2：输出方法////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("outmethod"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//
	strFieldValue.Format("%d",n_iOutMethod);
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名
	//属性3：检测上下限////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("limit"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//是否有limit检查
	strFieldValue = (n_bChkLimit == TRUE)? _T("1") : _T("0");
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); 

	strFieldValue.Format("%f",n_dHighLimit);
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); 

	strFieldValue.Format("%f",n_dLowLimit);
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); 
	//属性4：全坏后替代常值////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("ConstWhenBad"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%f",n_dConstWhenBad);
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); 
	//属性5：强制输出////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("ForceOutput"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue = (n_bForceEnable == TRUE)? _T("1") : _T("0");
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); 

	strFieldValue.Format("%f",n_dForceValue);
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); 
	//
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("InOrOut"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iInOrOut);
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名
	
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("OffsetTime"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_lOffsetTime);
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

	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CDBPointAdvBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	n_strTagDesp=((CDBPointAdvBlock*)pBlock)->n_strTagDesp;
	n_strTagName1=((CDBPointAdvBlock*)pBlock)->n_strTagName1;
	n_strTagName2=((CDBPointAdvBlock*)pBlock)->n_strTagName2;
	n_strTagName3=((CDBPointAdvBlock*)pBlock)->n_strTagName3;
	n_strTagName4=((CDBPointAdvBlock*)pBlock)->n_strTagName4;
	n_strTagName5=((CDBPointAdvBlock*)pBlock)->n_strTagName5;

	n_iOutMethod=((CDBPointAdvBlock*)pBlock)->n_iOutMethod;
	n_bChkLimit=((CDBPointAdvBlock*)pBlock)->n_bChkLimit;
	n_dHighLimit=((CDBPointAdvBlock*)pBlock)->n_dHighLimit;
	n_dLowLimit=((CDBPointAdvBlock*)pBlock)->n_dLowLimit;

	n_dConstWhenBad=((CDBPointAdvBlock*)pBlock)->n_dConstWhenBad;
	n_bForceEnable=((CDBPointAdvBlock*)pBlock)->n_bForceEnable;
	n_dForceValue=((CDBPointAdvBlock*)pBlock)->n_dForceValue;

	m_iInOrOut=((CDBPointAdvBlock*)pBlock)->m_iInOrOut;
	m_lOffsetTime=((CDBPointAdvBlock*)pBlock)->m_lOffsetTime;
	m_iForwardOrBack=((CDBPointAdvBlock*)pBlock)->m_iForwardOrBack;
	m_strTableName=((CDBPointAdvBlock*)pBlock)->m_strTableName;
	m_strDBDesp=((CDBPointAdvBlock*)pBlock)->m_strDBDesp;
}
//将属性放置传输数据中
void CDBPointAdvBlock::SetBlockPropertyToTransferData(
				CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CString str;
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"tagnames");	
	str.Format("%s",n_strTagName1);strcpy(pData->m_cValueItem1,str);
	str.Format("%s",n_strTagName2);strcpy(pData->m_cValueItem2,str);
	str.Format("%s",n_strTagName3);strcpy(pData->m_cValueItem3,str);
	str.Format("%s",n_strTagName4);strcpy(pData->m_cValueItem4,str);
	str.Format("%s",n_strTagName5);strcpy(pData->m_cValueItem5,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"outmethod");	
	str.Format("%d",n_iOutMethod);strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"limit");	
	str=(n_bChkLimit == TRUE)? _T("1") : _T("0");
	strcpy(pData->m_cValueItem1,str);
	str.Format("%f",n_dHighLimit);strcpy(pData->m_cValueItem2,str);
	str.Format("%f",n_dLowLimit);strcpy(pData->m_cValueItem3,str);
	dataArr.Add(pData);
	
	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"ConstWhenBad");		
	str.Format("%f",n_dConstWhenBad);strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"ForceOutput");		
	str = (n_bForceEnable == TRUE)? _T("1") : _T("0");
	strcpy(pData->m_cValueItem1,str);
	str.Format("%f",n_dForceValue);
	strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"InOrOut");		
	str.Format("%d",m_iInOrOut);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
	
	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"OffsetTime");		
	str.Format("%d",m_lOffsetTime);
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
}