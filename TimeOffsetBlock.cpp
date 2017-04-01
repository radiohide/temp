// TimeOffsetBlock.cpp: implementation of the CTimeOffsetBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "TimeOffsetBlock.h"
#include "PropertyDlgTimeOffset.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeOffsetBlock::CTimeOffsetBlock()
{
	CTime t(2012,1,1,0,0,0);

	m_iOffsetType= 0;//0:秒;1小时;2:日;3:周;4:月;5:年;
	m_iSecond=3600;//秒

	m_iHour=0;//小时
	m_HTime=t.GetTime();//小时-时间

	m_iDay=0;//日
	m_DTime=t.GetTime();//日-时间

	m_iWeek=0;//周
	m_iWeekDay=0;//周-日?
	m_WTime=t.GetTime();//周-时间

	
	m_iMonth=0;//月
	m_iMonthDay=1;//月-日?
	m_MTime=t.GetTime();//月-时间
	
	m_iYear=0;//年
	m_iYearMonth=1;//年-月?
	m_iYearDay=1;//年-日?
	m_YTime=t.GetTime();//年-时间

	SetBlockType(_T( "TimeOffset" ) );
	ConstructBlockInputOutput();
}

CTimeOffsetBlock::~CTimeOffsetBlock()
{

}
//根据类型新建对象的函数
CCalcBlock* CTimeOffsetBlock::CreateFromString( const CString& strType )
{
	CTimeOffsetBlock* obj = new CTimeOffsetBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CTimeOffsetBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("InTime"), VALUE_DOUBLE);
	AddOutputPort("OutTime",VALUE_DOUBLE,"out");
}
//初始化计算的函数做特殊的初始化
void CTimeOffsetBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CTimeOffsetBlock::Calc()//进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	CCalcPort *pInPortObj = GetInputPortObj(0);
	long lInTime;
	long lOutTime=0;
	double dValue=0;
	pInPortObj->GetPortUniValue().GetDoubleVal(&dValue);
	lInTime=(long)dValue;
	CTime tNoWrong(1990,1,1,0,0,0);
	if(lInTime<tNoWrong.GetTime())//输入时间不能小于1990,1,1,0,0,0
	{
		lInTime=tNoWrong.GetTime();
	}
	if(m_iOffsetType==0)//秒偏移
	{
		lOutTime=lInTime-m_iSecond;
	}
	else if(m_iOffsetType==1)//小时偏移
	{
		CTimeSpan ts(0,m_iHour,0,0);
		lInTime-=ts.GetTotalSeconds();
		CTime inTimeSubedTime(lInTime);
		CTime outTime(inTimeSubedTime.GetYear(),inTimeSubedTime.GetMonth(),
			inTimeSubedTime.GetDay(),inTimeSubedTime.GetHour(),
			m_HTime.GetMinute(),m_HTime.GetSecond());
		lOutTime=outTime.GetTime();
	}
	else if(m_iOffsetType==2)//日偏移
	{
		CTimeSpan ts(m_iDay,0,0,0);
		long lLength=ts.GetTotalSeconds();
		lInTime-=lLength;
		CTime inTimeSubedDay(lInTime);
		CTime outTime(inTimeSubedDay.GetYear(),inTimeSubedDay.GetMonth(),
			inTimeSubedDay.GetDay(),m_DTime.GetHour(),
			m_DTime.GetMinute(),m_DTime.GetSecond());
		lOutTime=outTime.GetTime();
	}
	else if(m_iOffsetType==3)//周偏移
	{
		CTimeSpan ts(m_iWeek*7,0,0,0);
		long lLength=ts.GetTotalSeconds();
		lInTime-=lLength;
		CTime inTimeSubed(lInTime);
		int currWeek=inTimeSubed.GetDayOfWeek();//周日=1,周1=2,...
		int setWeek=m_iWeekDay+1;
		if(setWeek>currWeek)
		{
			CTimeSpan ts2(setWeek-currWeek,0,0,0);//
			inTimeSubed+=ts2;
		}
		else if(setWeek<currWeek)
		{
			CTimeSpan ts2(currWeek-setWeek,0,0,0);//
			inTimeSubed-=ts2;
		}
		CTime outTime(inTimeSubed.GetYear(),inTimeSubed.GetMonth(),
			inTimeSubed.GetDay(),m_WTime.GetHour(),
			m_WTime.GetMinute(),m_WTime.GetSecond());
		lOutTime=outTime.GetTime();
	}
	else if(m_iOffsetType==4)//月偏移
	{
		CTime inTime(lInTime);
		int inYear=inTime.GetYear();
		int inMonth=inTime.GetMonth();
		if(inMonth<=m_iMonth)//
		{	
			inYear-=m_iMonth/12;
			int iLeftMonth=m_iMonth%12;
			if(inMonth<=iLeftMonth)
			{
				inYear-=1;
				inMonth=12-(iLeftMonth-inMonth);
			}
			else
			{
				inMonth-=iLeftMonth;
			}
		}
		else
		{
			inMonth-=m_iMonth;
		}
		int iDay=getDayOfTheMonth(inYear,inMonth,m_iMonthDay+1);

		CTime outTime(inYear,inMonth,iDay,
			m_MTime.GetHour(),m_MTime.GetMinute(),m_MTime.GetSecond());
		lOutTime=outTime.GetTime();
	}
	else if(m_iOffsetType==5)//年偏移
	{
		CTime inTime(lInTime);
		int inYear=inTime.GetYear();
		int iSubYear=inYear-m_iYear;
		int iDay=getDayOfTheMonth(iSubYear,m_iYearMonth,m_iYearDay+1);
		CTime outTime(iSubYear,m_iYearMonth+1,iDay,
			m_YTime.GetHour(),m_YTime.GetMinute(),m_YTime.GetSecond());
		lOutTime=outTime.GetTime();
	}
	CTime t(lOutTime);
	CString str;
	str=t.Format("%Y-%m-%d %H:%M:%S %W-%A"); 
	//AfxMessageBox(str);

	CUniValue UniVar;
	UniVar.AddData(lOutTime);
	pOutPortObj->SetPortUniValue(UniVar);
	OutputResultToGlobalWS();
}
int CTimeOffsetBlock::getDayOfTheMonth(int iYear,int iMonth,int iDayIn)
{
	int iDay=0;
	if((iMonth==1)||(iMonth==3)||(iMonth==5)||(iMonth==7)||(iMonth==8)||(iMonth==10)||(iMonth==12))
	{
		iDay=iDayIn;
	}
	else if((iMonth==4)||(iMonth==6)||(iMonth==9)||(iMonth==11))
	{
		if(iDayIn==31)
		{
			iDay=30;
		}
		else
		{
			iDay=iDayIn;
		}
	}
	else//2月
	{
		int iTagRunNian=iYear%4;
		if(iTagRunNian==0)//闰年
		{
			if(iDayIn<=29)
			{
				iDay=iDayIn;
			}
			else
			{
				iDay=29;
			}
		}
		else
		{
			if(iDayIn<=28)
			{
				iDay=iDayIn;
			}
			else
			{
				iDay=28;
			}
		}
	}
	return iDay;
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CTimeOffsetBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("Type")==0)
	{
		if(!strItem1.IsEmpty())  {m_iOffsetType=atoi(strItem1);}
	}
	else if(strPropName.Compare("SecondAndDay")==0)
	{
		if(!strItem1.IsEmpty())  {m_iSecond=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_iDay=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {m_DTime=CTime(atoi(strItem3));}
	}
	else if(strPropName.Compare("HourAndWeek")==0)
	{
		if(!strItem1.IsEmpty())  {m_iHour=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_HTime=CTime(atoi(strItem2));}

		if(!strItem3.IsEmpty())  {m_iWeek=atoi(strItem3);}
		if(!strItem4.IsEmpty())  {m_iWeekDay=atoi(strItem4);}
		if(!strItem5.IsEmpty())  {m_WTime=CTime(atoi(strItem5));}
	}
	else if(strPropName.Compare("Month")==0)
	{
		if(!strItem1.IsEmpty())  {m_iMonth=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_iMonthDay=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {m_MTime=CTime(atoi(strItem3));}
	}
	else if(strPropName.Compare("Year")==0)
	{
		if(!strItem1.IsEmpty())  {m_iYear=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_iYearDay=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {m_iYearMonth=atoi(strItem3);}
		if(!strItem4.IsEmpty())  {m_YTime=CTime(atoi(strItem4));}
	}
}
//重载以显示不同的属性对象对话框
void CTimeOffsetBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgTimeOffset dlg;
	dlg.m_iOffsetType=m_iOffsetType;//0:秒;1小时;2:日;3:周;4:月;5:年;
	dlg.m_iSecond=m_iSecond;//秒
	dlg.m_iHour=m_iHour;//小时
	dlg.m_HTime=m_HTime;//小时-时间
	dlg.m_iDay=m_iDay;//日
	dlg.m_DTime=m_DTime;//日-时间
	dlg.m_iWeek=m_iWeek;//周
	dlg.m_iWeekDay=m_iWeekDay;//周-日
	dlg.m_WTime=m_WTime;//周-时间	
	dlg.m_iMonth=m_iMonth;//月
	dlg.m_iMonthDay=m_iMonthDay;//月-日
	dlg.m_MTime=m_MTime;//月-时间	
	dlg.m_iYear=m_iYear;//年
	dlg.m_iYearDay=m_iYearDay;//年-日
	dlg.m_iYearMonth=m_iYearMonth;//年-月
	dlg.m_YTime=m_YTime;//年-时间	
	if(dlg.DoModal()==IDOK)
	{
		m_iOffsetType=dlg.m_iOffsetType;//0:秒;1小时;2:日;3:周;4:月;5:年;
		m_iSecond=dlg.m_iSecond;//秒
		m_iHour=dlg.m_iHour;//小时
		m_HTime=dlg.m_HTime;//小时-时间
		m_iDay=dlg.m_iDay;//日
		m_DTime=dlg.m_DTime;//日-时间
		m_iWeek=dlg.m_iWeek;//周
		m_iWeekDay=dlg.m_iWeekDay;//周-日
		m_WTime=dlg.m_WTime;//周-时间	
		m_iMonth=dlg.m_iMonth;//月
		m_iMonthDay=dlg.m_iMonthDay;//月-日
		m_MTime=dlg.m_MTime;//月-时间	
		m_iYear=dlg.m_iYear;//年
		m_iYearDay=dlg.m_iYearDay;//年-日
		m_iYearMonth=dlg.m_iYearMonth;//年-月
		m_YTime=dlg.m_YTime;//年-时间	

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
void CTimeOffsetBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="Type";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iOffsetType); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="SecondAndDay";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iSecond); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_iDay); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_DTime.GetTime()); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="HourAndWeek";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iHour); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_HTime.GetTime()); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iWeek); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iWeekDay); //属性名
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_WTime.GetTime()); //属性名
	pRecord->PutCollect(_T("value_item5"),_variant_t(strFieldValue) ); //属性名

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="Month";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iMonth); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_iMonthDay); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_MTime.GetTime()); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="Year";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iYear); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_iYearDay); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iYearMonth); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_YTime.GetTime()); //属性名
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名
	pRecord->Update();
}

//复制图形时，需要拷贝属性。
void CTimeOffsetBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_iOffsetType=((CTimeOffsetBlock*)pBlock)->m_iOffsetType;//0:秒;1小时;2:日;3:周;4:月;5:年;
	m_iSecond=((CTimeOffsetBlock*)pBlock)->m_iSecond;//秒
	m_iHour=((CTimeOffsetBlock*)pBlock)->m_iHour;//小时
	m_HTime=((CTimeOffsetBlock*)pBlock)->m_HTime;//小时-时间
	m_iDay=((CTimeOffsetBlock*)pBlock)->m_iDay;//日
	m_DTime=((CTimeOffsetBlock*)pBlock)->m_DTime;//日-时间
	m_iWeek=((CTimeOffsetBlock*)pBlock)->m_iWeek;//周
	m_iWeekDay=((CTimeOffsetBlock*)pBlock)->m_iWeekDay;//周-日
	m_WTime=((CTimeOffsetBlock*)pBlock)->m_WTime;//周-时间	
	m_iMonth=((CTimeOffsetBlock*)pBlock)->m_iMonth;//月
	m_iMonthDay=((CTimeOffsetBlock*)pBlock)->m_iMonthDay;//月-日
	m_MTime=((CTimeOffsetBlock*)pBlock)->m_MTime;//月-时间	
	m_iYear=((CTimeOffsetBlock*)pBlock)->m_iYear;//年
	m_iYearDay=((CTimeOffsetBlock*)pBlock)->m_iYearDay;//年-日
	m_iYearMonth=((CTimeOffsetBlock*)pBlock)->m_iYearMonth;//年-月
	m_YTime=((CTimeOffsetBlock*)pBlock)->m_YTime;//年-时间	
}
//将属性放置传输数据中
void CTimeOffsetBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Type");
	
	CString str;
	str.Format("%d",m_iOffsetType);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"SecondAndDay");
	
	str.Format("%d",m_iSecond);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iDay);
	strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_DTime.GetTime());
	strcpy(pData->m_cValueItem3,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"HourAndWeek");
	
	str.Format("%d",m_iHour);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_HTime.GetTime());
	strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_iWeek);
	strcpy(pData->m_cValueItem3,str);
	str.Format("%d",m_iWeekDay);
	strcpy(pData->m_cValueItem4,str);
	str.Format("%d",m_WTime.GetTime());
	strcpy(pData->m_cValueItem5,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Month");
	
	str.Format("%d",m_iMonth);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iMonthDay);
	strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_MTime.GetTime());
	strcpy(pData->m_cValueItem3,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"Year");
	
	str.Format("%d",m_iYear);
	strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iYearDay);
	strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_iYearMonth);
	strcpy(pData->m_cValueItem3,str);
	str.Format("%d",m_YTime.GetTime());
	strcpy(pData->m_cValueItem4,str);
	dataArr.Add(pData);
}