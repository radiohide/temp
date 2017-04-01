// CoalDataBlock.cpp: implementation of the CCoalDataBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "CoalDataBlock.h"
#include "PropertyDlgCoalData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCoalDataBlock::CCoalDataBlock()
{
	m_iDays=30;
	m_dThreshold=0.02;
	m_iNearTimeOrValue=0;

	m_strTableName="";
	m_strDBName="";
	
	m_lOffset=0;
	m_TagTimeOffsetOrInput=1;//采用内部时间

	SetBlockType(_T( "CoalData" ) );//设置类型
	ConstructBlockInputOutput ();
}

CCoalDataBlock::~CCoalDataBlock()
{

}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CCoalDataBlock::ConstructBlockInputOutput ()
{
	AddOutputPort("Valid", VALUE_DOUBLE);
}
//根据类型新建对象的函数
CCalcBlock* CCoalDataBlock::CreateFromString( const CString& strType )
{
	CCoalDataBlock* obj = new CCoalDataBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CCoalDataBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CCoalDataBlock::Calc() //进行计算的函数,需要重载
{
	int tagBeCon=0;
	CString strDBType="";
	_ConnectionPtr m_pConRDB = this->m_pModel->GetSourceRDBByDesp(m_strDBName,tagBeCon,strDBType);
	if(tagBeCon==0)//关系库未连接
		return;
	GetInputValueFromGlobalWS();
    //计算过程
	double timeflag=0;
	double dCoalK=0;
	if(m_TagTimeOffsetOrInput==1)//内部数据时刻+偏移
	{
		timeflag=this->m_pModel->m_lDataTime-this->m_lOffset;
		CCalcPort *pPortObj1 = GetInputPortObj(0);
		int ret = pPortObj1->GetPortUniValue().GetDoubleVal(&dCoalK);
	}
	else//外部时间
	{
		CCalcPort *pPortObj1 = GetInputPortObj(0);
		int ret = pPortObj1->GetPortUniValue().GetDoubleVal(&timeflag);
		CCalcPort *pPortObj2 = GetInputPortObj(1);
		ret = pPortObj2->GetPortUniValue().GetDoubleVal(&dCoalK);
	}
	if(timeflag<0)
		timeflag=0;
	//
	CStringArray strTagNameArr;
	CArray<long,long> lTimeArr;
	CArray<double,double> dCoalKArr;
	CArray<double,double> dCoalAllValue;
	CArray<double,double> dCoalVarValue;//具体的煤质变量的值，与匹配m_strCoalTagName一一对应
	for(int i=0;i<m_strCoalTagName.GetSize();i++)//初始化为0
	{
		dCoalVarValue.Add(0);
	}
	bool bValid=true;
	//if(getCurrentCoalK((long)timeflag,dCoalK))//得到当前煤质系数K
	{
		getAllNearCoalData(m_pConRDB,strDBType,(long)timeflag,strTagNameArr,lTimeArr,dCoalKArr,dCoalAllValue);//得到预估时间段内的所有煤质数据。
		calcMatchCoalData(dCoalK,strTagNameArr,lTimeArr,dCoalKArr);//得到满足阈值范围的煤质数据
		//进行策略计算
		//1:按照时间从小到大进行排序
		for(int i=0;i<lTimeArr.GetSize();i++)
		{
			for(int j=i+1;j<lTimeArr.GetSize();j++)
			{
				if(lTimeArr[j]<lTimeArr[i])
				{
					long lTimeTemp=lTimeArr[i];
					lTimeArr[i]=lTimeArr[j];
					lTimeArr[j]=lTimeTemp;

					CString strNameTemp=strTagNameArr[i];
					strTagNameArr[i]=strTagNameArr[j];
					strTagNameArr[j]=strNameTemp;

					double dTemp=dCoalKArr[i];
					dCoalKArr[i]=dCoalKArr[j];
					dCoalKArr[j]=dCoalKArr[i];

					double dValTemp=dCoalAllValue[i];
					dCoalAllValue[i]=dCoalAllValue[j];
					dCoalAllValue[j]=dValTemp;
				}
			}
		}
		if(m_iNearTimeOrValue)//0:时间优先
		{
			if(strTagNameArr.GetSize()<=0)
				bValid=false;
			for(int i=0;i<m_strCoalTagName.GetSize();i++)
			{
				CString strName=m_strCoalTagName[i];
				bool bFind=false;
				for(int j=0;j<strTagNameArr.GetSize();j++)
				{
					if((strTagNameArr[j]==strName)&&(lTimeArr[j]==lTimeArr[lTimeArr.GetSize()-1]))
					{
						dCoalVarValue[i]=dCoalAllValue[j];
						bFind=true;
						break;
					}
				}
				if(!bFind)//如果煤质中有一个点找不到匹配的变量则输出无效
					bValid=false;
			}
		}
		else//1值最接近优先
		{
			double dDif=0;
			int iIndexFind=-1;
			if(dCoalKArr.GetSize()>0)
			{
				dDif=fabs(dCoalK-dCoalKArr[0]);
				iIndexFind=0;
				for(int i=1;i<dCoalKArr.GetSize();i++)
				{
					if(fabs(dCoalK-dCoalKArr[i])<dDif)
					{
						iIndexFind=i;
						dDif=fabs(dCoalK-dCoalKArr[i]);
					}	
				}
			}
			if(iIndexFind!=-1)
			{
				for(int i=0;i<m_strCoalTagName.GetSize();i++)
				{
					CString strName=m_strCoalTagName[i];
					bool bFind=false;
					for(int j=0;j<strTagNameArr.GetSize();j++)
					{
						if((strTagNameArr[j]==strName)&&(dCoalKArr[j]==dCoalKArr[iIndexFind]))
						{
							dCoalVarValue[i]=dCoalAllValue[j];
							bFind=true;
							break;
						}
					}
					if(!bFind)//如果煤质中有一个点找不到匹配的变量则输出无效
						bValid=false;
				}
			}
			else
			{
				bValid=false;
			}
		}
		//进行策略计算
	}
	//else
	//	bValid=false;
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	CUniValue UniVar;
	UniVar.AddData((double)bValid);
	pOutPortObj->SetPortUniValue(UniVar);
	for(i=0;i<this->m_strCoalTagName.GetSize();i++)
	{
		pOutPortObj = GetOutputPortObj(i+1);
		CUniValue UniVar2;
		UniVar2.AddData(dCoalVarValue[i]);
		pOutPortObj->SetPortUniValue(UniVar2);			
	}
	OutputResultToGlobalWS();
}
void CCoalDataBlock::calcMatchCoalData(double dCoalK,CStringArray &strTagNameArr,
										CArray<long,long> &lTimeArr,
										CArray<double,double> &dCoalKArr)
{
	for(int i=strTagNameArr.GetSize()-1;i>=0;i--)
	{
		//超出阈值范围则删除
		if(!((dCoalKArr[i]>dCoalK*(1-this->m_dThreshold))&&(dCoalKArr[i]<dCoalK*(1+this->m_dThreshold))))
		{
			strTagNameArr.RemoveAt(i);
			lTimeArr.RemoveAt(i);
			dCoalKArr.RemoveAt(i);
		}
	}
}
void CCoalDataBlock::getAllNearCoalData(_ConnectionPtr &m_pConRDB,
										CString strDBType,
										long timeflag,
										CStringArray &strTagNameArr,
										CArray<long,long> &lTimeArr,
										CArray<double,double> &dCoalKArr,
										CArray<double,double> &dCoalAllValue)
{
	strTagNameArr.RemoveAll();
	lTimeArr.RemoveAll();
	dCoalKArr.RemoveAll();

	BOOL ret=FALSE;

	CTime tE(timeflag);
	CString strTE=tE.Format("%Y-%m-%d %H:%M:%S");
	CTimeSpan timeSpan(this->m_iDays,0,0,0);
	CTime tS(tE.GetTime()-timeSpan.GetTotalSeconds());
	CString strTS=tS.Format("%Y-%m-%d %H:%M:%S");

	_RecordsetPtr m_pRec;
	try   
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSql;
		strDBType.MakeUpper();
		if(strDBType=="MYSQL")
		{
			strSql.Format("select  * from %s  where DateTime>='%s' and DateTime<='%s';",
				m_strTableName,strTS,strTE);
		}
		else if(strDBType=="MDB")
		{
			strSql.Format("select top 1 * from %s  where DateTime>=#%s# and DateTime<=#%s#;",
				m_strTableName,strTS,strTE);
		}
		m_pRec->Open((char *)_bstr_t(strSql),_variant_t((IDispatch*)m_pConRDB,true),
			adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error  *e)   
	{   
		CString str=e->ErrorMessage();	//AfxMessageBox(e->ErrorMessage());   
	}  
	long lRetTime=0;
	while(!m_pRec->adoEOF)
	{
		CString strVarName =(char *)_bstr_t(m_pRec->GetCollect("var_name"));
		CString strDataTime =(char *)_bstr_t(m_pRec->GetCollect("DateTime"));
		CString strCoalK =(char *)_bstr_t(m_pRec->GetCollect("CoalK"));
		CString strCoalValue =(char *)_bstr_t(m_pRec->GetCollect("Value"));
		strTagNameArr.Add(strVarName);

		int   nYear,   nMonth,   nDate;
		int   nHour=0;
		int   nMin=0;
		int   nSec=0;   
		sscanf(strDataTime,"%d-%d-%d %d:%d:%d",&nYear,   &nMonth,   &nDate,   &nHour,   &nMin,   &nSec); 
		CTime   t(nYear,   nMonth,   nDate,   nHour,   nMin,   nSec);
		lTimeArr.Add(t.GetTime());
		dCoalKArr.Add(atof(strCoalK));
		dCoalAllValue.Add(atof(strCoalValue));
		m_pRec->MoveNext();
	}
}

//用于根据参数项和值进行属性的设置，当读取值要用
void CCoalDataBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("RDB")==0)
	{
		if(!strItem1.IsEmpty())  {m_strDBName=strItem1;}
		if(!strItem2.IsEmpty())  {m_strTableName=strItem2;}
	}
	else if(strPropName.Compare("TimeOffset")==0)
	{
		if(!strItem1.IsEmpty())  {m_TagTimeOffsetOrInput=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_lOffset=atoi(strItem2);}
	}
	else if(strPropName.Compare("Rule")==0)
	{
		if(!strItem1.IsEmpty())  {m_iDays=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {m_dThreshold=atof(strItem2);}
		if(!strItem3.IsEmpty())  {m_iNearTimeOrValue=atoi(strItem3);}
	}
	else if(strPropName.Left(3)=="Coa")//煤质变量
	{
		if(!strItem1.IsEmpty())  
			m_strCoalTagName.Add(strItem1);
		else
			return;
		if(!strItem2.IsEmpty())  
			m_strCoalTagDesp.Add(strItem2);
		else
			m_strCoalTagDesp.Add("");
		if(!strItem3.IsEmpty())  
			m_strCoalTagUnit.Add(strItem3);
		else
			m_strCoalTagUnit.Add("");
	}
}
//重载以显示不同的属性对象对话框
void CCoalDataBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgCoalData dlg;

	dlg.m_strTableName=m_strTableName;
	dlg.m_strDBName=m_strDBName;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);
	dlg.m_lOffset=m_lOffset;
	dlg.m_TagTimeOffsetOrInput=m_TagTimeOffsetOrInput;

	dlg.m_iDays=m_iDays;
	dlg.m_dThreshold=m_dThreshold;
	dlg.m_iNearTimeOrValue=m_iNearTimeOrValue;

	dlg.SetCoalTagData(m_strCoalTagName,
		m_strCoalTagDesp,
		m_strCoalTagUnit
		);

	if(IDOK==dlg.DoModal())
	{
		m_strTableName=dlg.m_strTableName;
		m_strDBName=dlg.m_strDBName;
		m_lOffset=dlg.m_lOffset;
		m_TagTimeOffsetOrInput=dlg.m_TagTimeOffsetOrInput;
		m_iDays=dlg.m_iDays;
		m_dThreshold=dlg.m_dThreshold;
		m_iNearTimeOrValue=dlg.m_iNearTimeOrValue;

		m_strCoalTagName.Copy(dlg.m_strCoalTagName);
		m_strCoalTagDesp.Copy(dlg.m_strCoalTagDesp);
		m_strCoalTagUnit.Copy(dlg.m_strCoalTagUnit);
		
		if(m_TagTimeOffsetOrInput==0)//外部
		{
			ClearInputPorts();
			AddInputPort(_T("Time"), VALUE_DOUBLE);
			AddInputPort(_T("CoalK"), VALUE_DOUBLE);

		}
		else
		{
			ClearInputPorts();
			AddInputPort(_T("CoalK"), VALUE_DOUBLE);
		}
		ClearOutputPorts();
		AddOutputPort("Valid", VALUE_DOUBLE);
		for(int i=0;i<m_strCoalTagName.GetSize();i++)
		{
			AddOutputPort(_T(m_strCoalTagName[i]), VALUE_DOUBLE);
		}
		parent->RedrawWindow();
	}
}
//重载以将本模块写入到数据库中
void CCoalDataBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue.Format("RDB");
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strDBName); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%s",m_strTableName); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue.Format("TimeOffset");
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_TagTimeOffsetOrInput); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_lOffset); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue.Format("Rule");
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iDays); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%f",m_dThreshold); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iNearTimeOrValue); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名
	
	for(int i=0;i<m_strCoalTagName.GetSize();i++)//表达式
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("CoalTag.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%s",m_strCoalTagName[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",m_strCoalTagDesp[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",m_strCoalTagUnit[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	}
	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CCoalDataBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_strDBName=((CCoalDataBlock*)pBlock)->m_strDBName;
	this->m_strTableName=((CCoalDataBlock*)pBlock)->m_strTableName;

	this->m_lOffset=((CCoalDataBlock*)pBlock)->m_lOffset;
	this->m_TagTimeOffsetOrInput=((CCoalDataBlock*)pBlock)->m_TagTimeOffsetOrInput;

	this->m_iDays=((CCoalDataBlock*)pBlock)->m_iDays;
	this->m_dThreshold=((CCoalDataBlock*)pBlock)->m_dThreshold;
	this->m_iNearTimeOrValue=((CCoalDataBlock*)pBlock)->m_iNearTimeOrValue;

	this->m_strCoalTagName.Copy(((CCoalDataBlock*)pBlock)->m_strCoalTagName);
	this->m_strCoalTagDesp.Copy(((CCoalDataBlock*)pBlock)->m_strCoalTagDesp);
	this->m_strCoalTagUnit.Copy(((CCoalDataBlock*)pBlock)->m_strCoalTagUnit);
}
//将属性放置传输数据中
void CCoalDataBlock::SetBlockPropertyToTransferData(
													CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	CString str;
	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"RDB");
	str.Format("%s",m_strDBName);strcpy(pData->m_cValueItem1,str);
	str.Format("%s",m_strTableName);strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TimeOffset");
	str.Format("%d",m_TagTimeOffsetOrInput);strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_lOffset);strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"TimeOffset");
	str.Format("%d",m_iDays);strcpy(pData->m_cValueItem1,str);
	str.Format("%f",m_dThreshold);strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_iNearTimeOrValue);strcpy(pData->m_cValueItem3,str);
	dataArr.Add(pData);

	CString strFieldValue;
	for(int i=0;i<m_strCoalTagName.GetSize();i++)//煤质变量
	{
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		strFieldValue.Format("CoalTag.%d",i);
		strcpy(pData->m_cPropName,strFieldValue);
		str.Format("%s",m_strCoalTagName[i]);
		strcpy(pData->m_cValueItem1,str);
		str.Format("%s",m_strCoalTagDesp[i]);
		strcpy(pData->m_cValueItem2,str);
		str.Format("%s",m_strCoalTagUnit[i]);
		strcpy(pData->m_cValueItem3,str);
		dataArr.Add(pData);
	}
}
