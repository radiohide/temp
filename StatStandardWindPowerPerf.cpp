// StatStandardWindPowerPerf.cpp: implementation of the CStatStandardWindPowerPerf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatStandardWindPowerPerf.h"
#include <vector>
#include "WindPowerTag.h"
#include <math.h>
#include "PropertyStandardWindPowerPerf.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatStandardWindPowerPerf::CStatStandardWindPowerPerf()
{
	m_iPowerPorTagMin=22;//并网标识
	m_iPowerPorTagMax=24;//并网标识
	m_dPowerLimit=3150000;//限电标识
	m_dMinParWindSpeed=3;//最小切入风速
	m_dMaxParWindSpeed=15;//风电机组85%额定功率输出时风速

	ConstructBlockInputOutput();
	SetBlockType(_T( "StatStandardWindPowerPerf" ) );
}

CStatStandardWindPowerPerf::~CStatStandardWindPowerPerf()
{

}
//根据类型新建对象的函数
CCalcBlock* CStatStandardWindPowerPerf::CreateFromString( const CString& strType )
{
	CStatStandardWindPowerPerf* obj = new CStatStandardWindPowerPerf();
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CStatStandardWindPowerPerf::ConstructBlockInputOutput ()
{
	AddInputPort("WindSpeed",VALUE_RTVALUE_SERIES);//风速
	AddInputPort("Power",VALUE_RTVALUE_SERIES);//功率
	AddInputPort("PowerProductionTag",VALUE_RTVALUE_SERIES);//并网标识
	//AddInputPort("PowerLimit",VALUE_RTVALUE_SERIES);//限电标志
}
//初始化计算的函数做特殊的初始化
void CStatStandardWindPowerPerf::InitCalc()
{
	CCalcBlock::InitCalc();
}
//返回属性的逗点字符串
CString CStatStandardWindPowerPerf::GetBlockStrPro()//to do
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CStatStandardWindPowerPerf::SetBlockStrPro(CString str)//设置计算块的字符串属性//to do
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
void CStatStandardWindPowerPerf::Calc()//进行计算的函数,需要重载
{
	//严格保持输入数据个数一致，在一个时间刻度上
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
		return;

	GetInputValueFromGlobalWS();

	CCalcPort *pPortObjWindSpeed = GetInputPortObj(0);	
	CUniValue  &CUVWindSpeed=pPortObjWindSpeed->GetPortUniValue();
	CCalcPort *pPortObjPower = GetInputPortObj(1);	
	CUniValue  &CUVPower=pPortObjPower->GetPortUniValue();

	
	CCalcPort *pPortObjProductionTag = GetInputPortObj(2);
	CUniValue  &CUVProductionTag=pPortObjProductionTag->GetPortUniValue();
	/*
	CCalcPort *pPortObjLimitTag = GetInputPortObj(3);
	CUniValue  &CUVLimitTag=pPortObjLimitTag->GetPortUniValue();
	*/

	if((CUVWindSpeed.GetDataSize()!=CUVPower.GetDataSize())||(CUVWindSpeed.GetDataSize()==0))
	{
		return;
	}
	int lTimeForYear=0;
	CArray<WindPowerTag,WindPowerTag&> WindPowerArr;
	for(int j=0; j<CUVPower.GetDataSize();j++)
	{
		double value=0;
		long lTime=0;
		CUVPower.GetDoubleValAt(j,&value);
		CUVPower.GetTimeValAt(j,&lTime);
		if(j==0)
		{
			lTimeForYear=lTime;
		}
		WindPowerTag oneTag;
		oneTag.dPower=value;
		oneTag.lTime=lTime;
		
		CUVWindSpeed.GetDoubleValAt(j,&value);
		oneTag.dWindSpeed=value;
		WindPowerArr.Add(oneTag);
	}
	//过滤数据，根据限电、并网、对风角度
	delTagWindPowerMinMaxArr(WindPowerArr,CUVProductionTag,m_iPowerPorTagMin,m_iPowerPorTagMax);//删除未并网数据
	//delTagWindPowerArr(WindPowerArr,CUVLimitTag,m_dPowerLimit,2);//删除限电数据
	delWindPowerArr(WindPowerArr);

	//根据均值和方差删除数据
	getAvgAndVar(WindPowerArr);//得到均值和方差
	delWindPowerArrByAvgVar(WindPowerArr);//删除均值和方差不满足的数据
	
	//bin分组
	double dMinWSpeed=m_dMinParWindSpeed-1;
	double dMaxBinWSpeed=m_dMaxParWindSpeed*1.5;
	double dMinBinWSpeed=(int(dMinWSpeed/0.5))*0.5;
	CArray<int,int> iDiffGroupNums;
	getBinGroup(WindPowerArr,dMinBinWSpeed,dMaxBinWSpeed,iDiffGroupNums);
	
	//bin分组内进行均值和方差删除数据
	getAvgAndVar(WindPowerArr,iDiffGroupNums);
	delWindPowerArrByAvgVar(WindPowerArr);//删除均值和方差不满足的数据

	//最后求均值
	getAvgAndVar(WindPowerArr,iDiffGroupNums);
	if(WindPowerArr.GetSize()==0)
		return;
	CString strSQL;
	CTime t(lTimeForYear);
	CString strTableName;
	strTableName.Format("%s_%d",m_strTableName,t.GetYear());
	
	_variant_t RecordsAffected;
	if(!gIsTableExist(m_pConRDB,strTableName))
	{
		strSQL.Format("CREATE TABLE `%s` (`turbine` varchar(64) NOT NULL,`time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,`wind_speed` int(11) NOT NULL,`power` double NOT NULL,`reactive_power` double unsigned zerofill DEFAULT NULL,`cosi` double unsigned zerofill DEFAULT NULL, `mark` enum('FALSE','TRUE') NOT NULL DEFAULT 'FALSE', PRIMARY KEY (`turbine`,`time`,`wind_speed`,`power`), KEY `turbine` (`turbine`,`time`)) ENGINE=MyISAM DEFAULT CHARSET=latin1",
			strTableName);
		m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText);
	}
	for(int i=0;i<iDiffGroupNums.GetSize();i++)
	{
		int iBinNum=iDiffGroupNums[i];
		int iFindIndex=-1;
		for(int j=0;j<WindPowerArr.GetSize();j++)
		{
			if(WindPowerArr[j].iBinNum==iBinNum)
			{
				iFindIndex=j;
				break;
			}
		}
		if(iFindIndex==-1)
			continue;
		WindPowerTag &oneTag=WindPowerArr[iFindIndex];
		try
		{
			writeDataToTable(m_strTurbine,strTableName,oneTag);
		}
		catch(...)
		{
			CString str;
			//str.Format("写入表'%s'失败!",strOutPutNameArr[i]);
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
	}
	//OutputResultToGlobalWS();
}
void CStatStandardWindPowerPerf::writeDataToTable(CString strTurbine,CString strTableName,WindPowerTag &oneTag)
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
		return;
	CString strSQL;
	_variant_t RecordsAffected;
	
	CTime time(oneTag.lTime);
	CString strTime;
	strTime=time.Format("%Y-%m-%d %H:%M:%S");
	strSQL.Format("insert into %s(turbine,time,wind_speed,power,mark) values(\'%s\',\'%s\',\'%d\',\'%f\',\'%s\')",
		strTableName,strTurbine,strTime,(int)(oneTag.dMidSpeed*10),
		oneTag.dPowerAvg,"FALSE");
	m_pConRDB->Execute((_bstr_t)(strSQL),&RecordsAffected,adCmdText);
}
void CStatStandardWindPowerPerf::getBinGroup(
									 CArray<WindPowerTag,WindPowerTag&> &WindPowerArr,
									 double dMinBinWSpeed,
									 double dMaxBinWSpeed,
									 CArray<int,int> &iDiffGroupNums)
{
	int iBinNum=1;
	double dStart=dMinBinWSpeed;
	double dEnd=dMinBinWSpeed+0.25;
	while(dEnd<dMaxBinWSpeed)
	{
		bool bValidNum=false;
		for(int i=0;i<WindPowerArr.GetSize();i++)
		{
			double dWindSpeed=WindPowerArr[i].dWindSpeed;
			if(iBinNum==1)
			{
				if((dWindSpeed>=dStart)&&(dWindSpeed<=dEnd))
				{
					WindPowerArr[i].iBinNum=iBinNum;
					WindPowerArr[i].dMidSpeed=dStart;
					bValidNum=true;
				}
			}
			else
			{
				if((dWindSpeed>dStart)&&(dWindSpeed<=dEnd))
				{
					WindPowerArr[i].iBinNum=iBinNum;
					WindPowerArr[i].dMidSpeed=dStart+0.25;
					bValidNum=true;
				}
			}
		}
		if(bValidNum)
		{
			iDiffGroupNums.Add(iBinNum);
		}
		dStart=dEnd;
		dEnd=dStart+0.5;
		iBinNum++;
	}
}
void CStatStandardWindPowerPerf::delWindPowerArrByAvgVar(
												 CArray<WindPowerTag,WindPowerTag&> &WindPowerArr)
{
	for(int i=WindPowerArr.GetSize()-1;i>=0;i--)
	{
		double dVar=WindPowerArr[i].dPowerVar;
		double dAvg=WindPowerArr[i].dPowerAvg;
		double dPower=WindPowerArr[i].dPower;
		if(((dPower-dAvg)*(dPower-dAvg))>(dVar*dVar))
		{
			WindPowerArr.RemoveAt(i);
		}
	}
}

void CStatStandardWindPowerPerf::getAvgAndVar(
									  CArray<WindPowerTag,WindPowerTag&> &WindPowerArr,
									  CArray<int,int> &iDiffGroupNums)
{
	for(int i=0;i<iDiffGroupNums.GetSize();i++)
	{
		int iBinNum=iDiffGroupNums[i];
		double dSum=0;
		double dAvg=0;
		int iCount=0;
		double dVar=0;//方差
		for(int j=0;j<WindPowerArr.GetSize();j++)
		{
			if(WindPowerArr[j].iBinNum==iBinNum)
			{
				dSum+=WindPowerArr[j].dPower;
				iCount++;
			}
		}
		if(iCount>0)
		{
			dAvg=dSum/iCount;
		}
		double dSum2=0;
		for(j=0;j<WindPowerArr.GetSize();j++)
		{
			if(WindPowerArr[j].iBinNum==iBinNum)
			{
				WindPowerArr[j].dPowerAvg=dAvg;
				dSum2+=(WindPowerArr[j].dPower-dAvg)*(WindPowerArr[j].dPower-dAvg);
			}
		}
		if(iCount>1)
		{
			dVar=sqrt(double(dSum2/(iCount-1)));
		}
		for(j=0;j<WindPowerArr.GetSize();j++)
		{
			if(WindPowerArr[j].iBinNum==iBinNum)
			{
				WindPowerArr[j].dPowerVar=dVar;//赋方差
			}
		}
	}
}
void CStatStandardWindPowerPerf::getAvgAndVar(
									  CArray<WindPowerTag,WindPowerTag&> &WindPowerArr)
{
	double dSum=0;
	double dAvg=0;
	double dVar=0;//方差
	for(int i=0;i<WindPowerArr.GetSize();i++)
	{
		dSum+=WindPowerArr[i].dPower;
	}
	if(WindPowerArr.GetSize()>0)
	{
		dAvg=dSum/WindPowerArr.GetSize();
	}
	double dSum2=0;
	for(i=0;i<WindPowerArr.GetSize();i++)
	{
		WindPowerArr[i].dPowerAvg=dAvg;//赋均值
		dSum2+=(WindPowerArr[i].dPower-dAvg)*(WindPowerArr[i].dPower-dAvg);
	}
	if(WindPowerArr.GetSize()>1)
	{
		dVar=sqrt(double(dSum2/(WindPowerArr.GetSize()-1)));
	}
	for(i=0;i<WindPowerArr.GetSize();i++)
	{
		WindPowerArr[i].dPowerVar=dVar;//赋方差
	}
}
void CStatStandardWindPowerPerf::delWindPowerArr(
										 CArray<WindPowerTag,WindPowerTag&> &WindPowerArr)
{
	for(int i=WindPowerArr.GetSize()-1;i>=0;i--)
	{
		if(WindPowerArr[i].bDel)
		{
			WindPowerArr.RemoveAt(i);
		}
	}
}
void CStatStandardWindPowerPerf::delTagWindPowerMinMaxArr(
												  CArray<WindPowerTag,WindPowerTag&> &WindPowerArr,
												  CUniValue  &CUV,
												  int iMin,
												  int iMax											
												  )
{
	for(int i=CUV.GetDataSize()-1;i>=0;i--)
	{
		double value=0;
		CUV.GetDoubleValAt(i,&value);
		if((value<iMin)||(value>iMax))
		{
			WindPowerArr[i].bDel=true;
		}
	}
}
void CStatStandardWindPowerPerf::delTagWindPowerArr(
											CArray<WindPowerTag,WindPowerTag&> &WindPowerArr,
											CUniValue  &CUV,
											double iDelTag,//删除值的标识
											int iMaxMinTag//是否过滤大于设定值
											)
{
	for(int i=CUV.GetDataSize()-1;i>=0;i--)
	{
		double value=0;
		CUV.GetDoubleValAt(i,&value);
		if(iMaxMinTag==0)//删除相等值
		{
			if(value==iDelTag)
			{
				WindPowerArr[i].bDel=true;
			}
		}
		else if(iMaxMinTag==1)//删除大于值
		{
			if(value>iDelTag)
			{
				WindPowerArr[i].bDel=true;
			}
		}
		else//2删除小于值
		{
			if(value<iDelTag)
			{
				WindPowerArr[i].bDel=true;
			}
		}
	}
}

BOOL CStatStandardWindPowerPerf::gIsTableExist(_ConnectionPtr &pConnection, CString strTableName)
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	pRecord.CreateInstance(__uuidof(Recordset));
	pRecord->CursorLocation = adUseClient;
	
	strSQL.Format("select * from %s limit 0,1",strTableName);
	try
	{
		pRecord->Open((const char*)strSQL,pConnection.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error e)
	{
		//AfxMessageBox(e.Description());
		return FALSE;
	} 
	return TRUE;
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CStatStandardWindPowerPerf::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("DBInf")==0)
	{
		if(!strItem1.IsEmpty())  {m_strDBName=(strItem1);}
		if(!strItem2.IsEmpty())  {m_strTableName=(strItem2);}
	}
	else if(strPropName.Compare("CalcInfo")==0)
	{
		if(!strItem1.IsEmpty())  {m_strTurbine=(strItem1);}
		if(!strItem2.IsEmpty())  {m_dPowerLimit=atof(strItem2);}
		if(!strItem3.IsEmpty())  {m_dMinParWindSpeed=atof(strItem3);}
		if(!strItem4.IsEmpty())  {m_dMaxParWindSpeed=atof(strItem4);}
	}
	else if(strPropName.Compare("PowerPro")==0)
	{
		if(!strItem1.IsEmpty())  {m_iPowerPorTagMin=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_iPowerPorTagMax=atoi(strItem2);}
	}
}
//重载以显示不同的属性对象对话框
void CStatStandardWindPowerPerf::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyStandardWindPowerPerf dlg;
	dlg.m_strDBName=m_strDBName;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);
	dlg.m_strTableName=m_strTableName;
	dlg.m_strTurbine=m_strTurbine;
	dlg.m_iPowerPorTagMin=m_iPowerPorTagMin;//并网标识最小值
	dlg.m_iPowerPorTagMax=m_iPowerPorTagMax;//并网标识最小值

//	dlg.m_dPowerLimit=m_dPowerLimit;//限电标识
	dlg.m_dMinParWindSpeed=m_dMinParWindSpeed;//最小切入风速
	dlg.m_dMaxParWindSpeed=m_dMaxParWindSpeed;//风电机组85%额定功率输出时风速
	if(dlg.DoModal()==IDOK)
	{
		m_strDBName=dlg.m_strDBName;
		m_strTableName=dlg.m_strTableName;
		m_strTurbine=dlg.m_strTurbine;
		m_iPowerPorTagMin=dlg.m_iPowerPorTagMin;//并网标识最小值
		m_iPowerPorTagMax=dlg.m_iPowerPorTagMax;//并网标识最小值
//		m_dPowerLimit=dlg.m_dPowerLimit;//限电标识
		m_dMinParWindSpeed=dlg.m_dMinParWindSpeed;//最小切入风速
		m_dMaxParWindSpeed=dlg.m_dMaxParWindSpeed;//风电机组85%额定功率输出时风速
	}
}
//重载以将本模块写入到数据库中
void CStatStandardWindPowerPerf::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="DBInf";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strDBName); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strTableName); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="CalcInfo";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strTurbine); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%f",m_dPowerLimit); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%f",m_dMinParWindSpeed); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%f",m_dMaxParWindSpeed); //属性名
	pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名
	
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="PowerPro";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iPowerPorTagMin); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iPowerPorTagMax); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名
	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CStatStandardWindPowerPerf::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_strDBName=((CStatStandardWindPowerPerf*)pBlock)->m_strDBName;//数据库名称
	this->m_strTableName=((CStatStandardWindPowerPerf*)pBlock)->m_strTableName;
	this->m_strTurbine=((CStatStandardWindPowerPerf*)pBlock)->m_strTurbine;
	this->m_iPowerPorTagMin=((CStatStandardWindPowerPerf*)pBlock)->m_iPowerPorTagMin;
	this->m_iPowerPorTagMax=((CStatStandardWindPowerPerf*)pBlock)->m_iPowerPorTagMax;
	this->m_dPowerLimit=((CStatStandardWindPowerPerf*)pBlock)->m_dPowerLimit;
	this->m_dMinParWindSpeed=((CStatStandardWindPowerPerf*)pBlock)->m_dMinParWindSpeed;
	this->m_dMaxParWindSpeed=((CStatStandardWindPowerPerf*)pBlock)->m_dMaxParWindSpeed;
}
//将属性放置传输数据中
void CStatStandardWindPowerPerf::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}
