// WaveIntervalBlock.cpp: implementation of the CWaveIntervalBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WaveIntervalBlock.h"
#include "PropertyDlgWaveInterval.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWaveIntervalBlock::CWaveIntervalBlock()
{
	m_iSmoothWidth=15;//滑动宽度(秒)
	m_iSmoothStep=1;//滑动步长(秒)
	m_bBeFirstCalc=true;
	m_dSpanIntPercent=50;//跨至临近区间超过边界的百分比
	m_iSpanKeepTime=10;//跨至设定区间并持续的时间长度
	SetBlockType(_T( "WaveInterval" ) );
	ConstructBlockInputOutput ();
}

CWaveIntervalBlock::~CWaveIntervalBlock()
{

}

//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CWaveIntervalBlock::ConstructBlockInputOutput ()
{
	AddInputPort(_T("In"), VALUE_RTVALUE_SERIES, _T("in1"), _T("in1"));	
	AddOutputPort("Out",VALUE_DOUBLE, _T("out"), _T("out"));
}
//根据类型新建对象的函数
CCalcBlock* CWaveIntervalBlock::CreateFromString( const CString& strType )
{
	CWaveIntervalBlock* obj = new CWaveIntervalBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CWaveIntervalBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CWaveIntervalBlock::Calc() //进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
	CCalcPort *pPortObjIn = GetInputPortObj(0);	
	CCalcPort *pOutPortObj = GetOutputPortObj(0);
	CUniValue  &CUV=pPortObjIn->GetPortUniValue();
	CArray<double,double> dOrigValueArr;
	CArray<long,long> dOrigTimeArr;
	CArray<short,short> dOrigStatusArr;

	m_dSmoothValueArr.RemoveAll();
	m_dSmoothTimeArr.RemoveAll();
	m_dSmoothStatusArr.RemoveAll();
	double value;
	long time;
	short status;
	for(int i=0; i<CUV.GetDataSize();i++)
	{
		CUV.GetDoubleValAt(i,&value);
		CUV.GetTimeValAt(i,&time);
		CUV.GetStatusValAt(i,&status);
		dOrigValueArr.Add(value);
		dOrigTimeArr.Add(time);
		dOrigStatusArr.Add(status);
	}
	int ret=getSmoothData(
		dOrigValueArr,
		dOrigTimeArr,
		dOrigStatusArr,
		m_iSmoothWidth,
		m_iSmoothStep,
		m_dSmoothValueArr,
		m_dSmoothTimeArr,
		m_dSmoothStatusArr
		);
	if(!ret)
	{
		CString str;
		str.Format("%s--%s--%s:滤波失败，可能滤波参数设置不合理，或者输入数据量太少！",this->m_pModel->m_strModelName,this->m_pModule->GetModuleName(),this->GetBlockName());
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
		CUniValue UniVar;
		UniVar.AddData(-1);//计算失败
		pOutPortObj->SetPortUniValue(UniVar);
		OutputResultToGlobalWS();
		return;
	}
	if(m_bBeFirstCalc)
	{
		//第一次计算时，从最近时间依次向前搜索m_iSpanKeepTime秒的时间段，
		//若存在持续m_iSpanKeepTime时间段，所有点均落在同一离散区间内，则得到离散值。
		//若搜索完后没找到则，取所有点的均值，得到离散值。
		m_dDisValue=getFirstCalcDisValue();
		m_bBeFirstCalc=false;
	}
	else
	{
		//判断最近m_iSpanKeepTime秒是否运行至所跨的范围内，或稳定在某个区间段内，则得到离散值。
		//否则认为m_iSpanKeepTime内有跨区间震荡，则维持离散值不变。
		double dNextDisValue=-1;
		int iRet=getDisValue(dNextDisValue);
		if(iRet)
		{
			m_dDisValue=dNextDisValue;
		}
	}
	CUniValue UniVar;
	UniVar.AddData(m_dDisValue);
	pOutPortObj->SetPortUniValue(UniVar);
	OutputResultToGlobalWS();
}
int CWaveIntervalBlock::getDisValue(double &disValue)
{
	long lTimeS,lTimeE;
	bool bFindSETime=false;
	CArray<double,double> dArrNewDisValue;
	CArray<double,double> dArrNewIntStart;
	CArray<double,double> dArrNewIntEnd;	
	getNewDisDataArr(dArrNewDisValue,dArrNewIntStart,dArrNewIntEnd);
	if(m_dSmoothValueArr.GetSize()>1)
	{
		lTimeE=m_dSmoothTimeArr[m_dSmoothValueArr.GetSize()-1];
		lTimeS=lTimeE-m_iSpanKeepTime;
		bFindSETime=true;
	}
	CArray<double,double> tempValueArr;
	if(bFindSETime)
	{
		for(int j=0;j<m_dSmoothValueArr.GetSize();j++)
		{
			if(m_dSmoothTimeArr[j]<lTimeS)
				continue;
			if(m_dSmoothTimeArr[j]>lTimeE)
				break;
			tempValueArr.Add(m_dSmoothValueArr[j]);
		}
	}
	int iRet=getDisValueByValueArr(tempValueArr,dArrNewDisValue,dArrNewIntStart,dArrNewIntEnd,disValue);
	return iRet;
}

void CWaveIntervalBlock::getNewDisDataArr(
										  CArray<double,double> &dArrNewDisValue,
										  CArray<double,double> &dArrNewIntStart,
										  CArray<double,double> &dArrNewIntEnd)//得到考虑波动范围的离散化区间
										  //扩宽了当前所处的离散区间
{
	int iFindIndex=-1;
	for(int i=0;i<m_dArrDisValue.GetSize();i++)
	{
		if(m_dArrDisValue[i]==m_dDisValue)
		{
			iFindIndex=i;
			break;
		}
	}
	int iTagArrAsc=1;//默认是区间递增
	if(m_dArrIntStart.GetSize()>1)
	{
		if(m_dArrIntStart[0]>m_dArrIntStart[1])
		{	
			iTagArrAsc=0;//递减区间。
		}
	}
	double dBeforAbs=0;//前一个区间波动跨度的绝对值
	double dAfterAbs=0;//后一个区间波动跨度的绝对值
	for(i=0;i<m_dArrDisValue.GetSize();i++)
	{
		double disV=m_dArrDisValue[i];
		double intS=m_dArrIntStart[i];
		double intE=m_dArrIntEnd[i];
		if(iFindIndex!=-1)
		{
			if(iTagArrAsc)//区间递增
			{
				if(iFindIndex-1==i)
				{
					dBeforAbs=(intE-intS)*(m_dSpanIntPercent/100);
					intE-=dBeforAbs;
				}
				else if(iFindIndex==i)
				{
					intS-=dBeforAbs;
				}
				else if(iFindIndex+1==i)
				{
					dAfterAbs=(intE-intS)*(m_dSpanIntPercent/100);
					intS+=dAfterAbs;
				}
			}
			else
			{
				if(iFindIndex-1==i)
				{
					dBeforAbs=(intE-intS)*(m_dSpanIntPercent/100);
					intS+=dBeforAbs;
				}
				else if(iFindIndex==i)
				{
					intE+=dBeforAbs;
				}
				else if(iFindIndex+1==i)
				{
					dAfterAbs=(intE-intS)*(m_dSpanIntPercent/100);
					intE-=dAfterAbs;
				}
			}
		}
		dArrNewDisValue.Add(disV);
		dArrNewIntStart.Add(intS);
		dArrNewIntEnd.Add(intE);
	}
	if(iFindIndex!=-1)//修正
	{
		if(iTagArrAsc)//区间递增
		{
			dArrNewIntEnd[iFindIndex]+=dAfterAbs;
		}
		else
		{
			dArrNewIntStart[iFindIndex]-=dAfterAbs;
		}
	}
}

double CWaveIntervalBlock::getFirstCalcDisValue()
{
	double disValue;
	long lTimeS,lTimeE;
	bool bLast=false;
	BOOL bAllInOne=FALSE;
	for(int i=m_dSmoothValueArr.GetSize()-1;i>=0;i--)
	{
		if(i==m_dSmoothValueArr.GetSize()-1)
		{
			lTimeE=m_dSmoothTimeArr[i];
			lTimeS=lTimeE-m_iSpanKeepTime;
		}
		else
		{
			lTimeE-=1;//向前平滑推移1秒
			lTimeS-=1;//向前平滑推移1秒
		}
		CArray<double,double> tempValueArr;
		for(int j=0;j<m_dSmoothValueArr.GetSize();j++)
		{
			if(m_dSmoothTimeArr[j]<lTimeS)
				continue;
			if(m_dSmoothTimeArr[j]>lTimeE)
				break;
			tempValueArr.Add(m_dSmoothValueArr[j]);
		}
		int iRet=getDisValueByValueArr(tempValueArr,m_dArrDisValue,m_dArrIntStart,m_dArrIntEnd,disValue);
		if(iRet)//如果找到匹配区间
		{
			bAllInOne=TRUE;//滑动中找到匹配的区间。
			break;
		}
	}
	if(!bAllInOne)//滑动中没有找到匹配的区间。
	{
		double sum=0;
		for(int i=m_dSmoothValueArr.GetSize()-1;i>=0;i--)
		{
			sum+=m_dSmoothValueArr[i];
		}
		double dAvg=sum/m_dSmoothValueArr.GetSize();
		getDisValueByValue(dAvg,m_dArrDisValue,m_dArrIntStart,m_dArrIntEnd,disValue);
	}
	return disValue;
}
void CWaveIntervalBlock::getDisValueByValue(
											  double dAvg,
											  const CArray<double,double> &dArrDisValue,
											  const CArray<double,double> &dArrIntStart,
											  const CArray<double,double> &dArrIntEnd,
											  double &disValue)
{
	disValue=-1;
	for(int i=0;i<dArrDisValue.GetSize();i++)
	{
		if((dAvg>=dArrIntStart[i])&&(dAvg<dArrIntEnd[i]))
		{
			disValue=dArrDisValue[i];
			break;
		}
	}
}
int CWaveIntervalBlock::getDisValueByValueArr(
											  const CArray<double,double> &tempValueArr,
											  const CArray<double,double> &dArrDisValue,
											  const CArray<double,double> &dArrIntStart,
											  const CArray<double,double> &dArrIntEnd,
											  double &disValue)//序列是否稳定运行在某离散区间。
{
	BOOL ret=FALSE;
	if(tempValueArr.GetSize()==0)
		return ret;
	for(int i=0;i<dArrDisValue.GetSize();i++)
	{
		bool bAllInOne=true;
		for(int j=0;j<tempValueArr.GetSize();j++)
		{
			if(!((tempValueArr[j]>=dArrIntStart[i])&&(tempValueArr[j]<dArrIntEnd[i])))
			{
				bAllInOne=false;
				break;
			}
		}
		if(bAllInOne)
		{
			disValue=dArrDisValue[i];
			ret=TRUE;
			break;
		}
	}
	return ret;
}
BOOL CWaveIntervalBlock::getSmoothData(
									   const CArray<double,double> &dOrigValueArr,
									   const CArray<long,long> &dOrigTimeArr,
									   const CArray<short,short> &dOrigStatusArr,
									   int iSmoothWidth,
									   int iSmoothStep,
									   CArray<double,double> &dSmoothValueArr,
									   CArray<long,long> &dSmoothTimeArr,
									   CArray<short,short> &dSmoothStatusArr
									   )
{
	BOOL ret=TRUE;
	if(iSmoothStep==0)
	{
		return FALSE;
	}
	dSmoothValueArr.RemoveAll();
	dSmoothTimeArr.RemoveAll();
	dSmoothStatusArr.RemoveAll();
	
	long lTimeS,lTimeE;
	bool bLast=false;
	for(int i=0;i<dOrigValueArr.GetSize();i++)
	{
		if(i==0)
		{
			lTimeS=dOrigTimeArr[i];
			lTimeE=lTimeS+iSmoothWidth;
		}
		else
		{
			lTimeS+=iSmoothStep;
			lTimeE+=iSmoothStep;
			if(lTimeE>=dOrigTimeArr[dOrigTimeArr.GetSize()-1])//当结尾大于结尾时间的时候取最后时间。
			{
				lTimeE=dOrigTimeArr[dOrigTimeArr.GetSize()-1];
				bLast=true;
			}
		}
		double dSum=0;
		int iCounter=0;
		int iStatus=0;
		for(int j=0;j<dOrigValueArr.GetSize();j++)
		{
			if(dOrigTimeArr[j]<lTimeS)
				continue;
			if(dOrigTimeArr[j]>lTimeE)
				break;
			dSum+=dOrigValueArr[i];
			iStatus=dOrigStatusArr[i];
			iCounter++;
		}
		dSmoothValueArr.Add(dSum/iCounter);
		dSmoothTimeArr.Add(lTimeE);//每次平滑去取平滑宽度末端值，不一定是真实点的时间。
		dSmoothStatusArr.Add(iStatus);//状态去滑动区间最后一个点的状态。
		if(bLast)
		{
			break;
		}
	}
	if(dSmoothValueArr.GetSize()<3)//滤波后个数太少
	{
		ret=FALSE;
	}
	return ret;
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CWaveIntervalBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("SpanIntPercent")==0)
	{
		if(!strItem1.IsEmpty())  {m_dSpanIntPercent=atof(strItem1);}
	}
	else if(strPropName.Compare("SpanKeepTime")==0)
	{
		if(!strItem1.IsEmpty())  {m_iSpanKeepTime=atoi(strItem1);}
	}
	else if(strPropName.Compare("SmoothWidth")==0)
	{
		if(!strItem1.IsEmpty())  {m_iSmoothWidth=atoi(strItem1);}
	}
	else if(strPropName.Compare("SmoothStep")==0)
	{
		if(!strItem1.IsEmpty())  {m_iSmoothStep=atoi(strItem1);}
	}
	else //开始
	{
		int index=strPropName.ReverseFind('.');
		CString strIndex=strPropName.Right(strPropName.GetLength()-index-1);
		CString strName=strPropName.Left(index);
		int propIndex=atoi(strIndex);
		if(strName.Compare("DisData")==0)
		{
			m_dArrDisValue.SetAtGrow(propIndex,atof(strItem1));
			m_dArrIntStart.SetAtGrow(propIndex,atof(strItem2));
			m_dArrIntEnd.SetAtGrow(propIndex,atof(strItem3));
		}
	}
}
//重载以显示不同的属性对象对话框
void CWaveIntervalBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgWaveInterval dlg;
	dlg.m_dSpanIntPercent=m_dSpanIntPercent;
	dlg.m_iSpanKeepTime=m_iSpanKeepTime;
	dlg.m_iSmoothWidth=m_iSmoothWidth;
	dlg.m_iSmoothStep=m_iSmoothStep;
	dlg.m_dArrDisValue.Copy(m_dArrDisValue);
	dlg.m_dArrIntStart.Copy(m_dArrIntStart);
	dlg.m_dArrIntEnd.Copy(m_dArrIntEnd);
	if(IDOK==dlg.DoModal())
	{
		m_iSmoothWidth=dlg.m_iSmoothWidth;
		m_iSmoothStep=dlg.m_iSmoothStep;
		m_dSpanIntPercent=dlg.m_dSpanIntPercent;
		m_iSpanKeepTime=dlg.m_iSpanKeepTime;
		m_dArrDisValue.Copy(dlg.m_dArrDisValue);
		m_dArrIntStart.Copy(dlg.m_dArrIntStart);
		m_dArrIntEnd.Copy(dlg.m_dArrIntEnd);
	}
}
//重载以将本模块写入到数据库中
void CWaveIntervalBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="SpanIntPercent";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%f",m_dSpanIntPercent); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="SpanKeepTime";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iSpanKeepTime); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="SmoothWidth";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iSmoothWidth); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="SmoothStep";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iSmoothStep); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	

	for(int i=0;i<m_dArrDisValue.GetSize();i++)
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("DisData.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
		strFieldValue.Format("%f",m_dArrDisValue[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%f",m_dArrIntStart[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%f",m_dArrIntEnd[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	}
	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CWaveIntervalBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_dSpanIntPercent=((CWaveIntervalBlock*)pBlock)->m_dSpanIntPercent;
	m_iSpanKeepTime=((CWaveIntervalBlock*)pBlock)->m_iSpanKeepTime;
	m_iSmoothWidth=((CWaveIntervalBlock*)pBlock)->m_iSmoothWidth;
	m_iSmoothStep=((CWaveIntervalBlock*)pBlock)->m_iSmoothStep;
	m_dArrDisValue.Copy(((CWaveIntervalBlock*)pBlock)->m_dArrDisValue);
	m_dArrIntStart.Copy(((CWaveIntervalBlock*)pBlock)->m_dArrIntStart);
	m_dArrIntEnd.Copy(((CWaveIntervalBlock*)pBlock)->m_dArrIntEnd);
	
}
//将属性放置传输数据中
void CWaveIntervalBlock::SetBlockPropertyToTransferData(
								CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"SpanIntPercent");
	
	CString str;
	str.Format("%f",m_dSpanIntPercent);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"SpanKeepTime");
	
	str.Format("%d",m_iSpanKeepTime);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"SmoothWidth");
	
	str.Format("%d",m_iSmoothWidth);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"SmoothStep");
	
	str.Format("%d",m_iSmoothStep);
	strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);

	for(int i=0;i<m_dArrDisValue.GetSize();i++)
	{
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		CString str;
		str.Format("DisData.%d",i);
		strcpy(pData->m_cPropName,str);
		
		str.Format("%f",m_dArrDisValue[i]);
		strcpy(pData->m_cValueItem1,str);
		str.Format("%f",m_dArrIntStart[i]);
		strcpy(pData->m_cValueItem2,str);
		str.Format("%f",m_dArrIntEnd[i]);
		strcpy(pData->m_cValueItem3,str);
		dataArr.Add(pData);
	}
}
