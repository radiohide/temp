// StableJudgeBlock.cpp: implementation of the CStableJudgeBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StableJudgeBlock.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStableJudgeBlock::CStableJudgeBlock()
{
	m_iCalcNoStable=0;
	m_dDown2 = 90.0;
	m_dDown1 = 95.0;
	m_dFazhi1 = 5.0;
	m_dFazhi2 = 2.0;
	m_iRadio = 0;
	//SYSTME=====设置类型======
	//设置类型
	SetBlockType(_T( "StableJudge" ) );
	ConstructBlockInputOutput();
	//SYSTME=====初始化本模块的属性===
}

CStableJudgeBlock::~CStableJudgeBlock()
{

}
//构造初始的输入输出个数
void CStableJudgeBlock::ConstructBlockInputOutput()
{	
	AddInputPort(_T("In"), VALUE_RTVALUE_SERIES, _T("in1"), _T("in1"));	
	AddOutputPort("Out",VALUE_DOUBLE, _T("out"), _T("out"));
}
//根据类型新建对象的函数
//同时在CCalcBlockControlFactory中，要加入相应的代码
CCalcBlock* CStableJudgeBlock::CreateFromString( const CString& strType )
{
	CStableJudgeBlock* obj = new CStableJudgeBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CStableJudgeBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}
//计算函数，实现本模块的计算
void CStableJudgeBlock::Calc() 
{
	//获得输入端口的数据
	//1稳态;2升;3降;4BR,0未知
	GetInputValueFromGlobalWS();
	double dOutput1=0;//如果阈值1和2都满足，输出1，否则输出0
	double dOutput2=0;//阈值1的实际垂落百分数
	double dOutput3=0;//阈值2的实际垂落百分数
	
	CCalcPort *pPortObjIn = GetInputPortObj(0);	
	CUniValue  &CUV=pPortObjIn->GetPortUniValue();
	if(CUV.GetDataSize()>=3)//当数据个数大于等于三个的时候开始进入计算否则，输出均为0
	{
		//得到均值
		double sum=0;
		double avg=0;
		CArray<double,double> dOrigValueArr;
		for(int i=0; i<CUV.GetDataSize();i++)
		{
			double value;
			CUV.GetDoubleValAt(i,&value);
			dOrigValueArr.Add(value);
			sum+=value;
		}
		avg=sum/CUV.GetDataSize();
		//检测阈值一
		double dlow1=0;
		double dhigh1=0;
		int icont=0;
		dlow1=avg*(1-m_dFazhi1/100);
		dhigh1=avg*(1+m_dFazhi1/100);
		for(int m=0; m<CUV.GetDataSize(); m++)
		{
			double value;
			CUV.GetDoubleValAt(m,&value);
			if((dlow1<value) && (value<dhigh1))
			{
				icont++;
			}
		}
		dOutput2=((double)icont)/((double)CUV.GetDataSize());
		//检测阈值二
		double dlow2=0;
		double dhigh2=0;
		icont=0;
		dlow2=avg*(1-m_dFazhi2/100);
		dhigh2=avg*(1+m_dFazhi2/100);
		for(int n=0; n<CUV.GetDataSize(); n++)
		{
			double value;
			CUV.GetDoubleValAt(n,&value);
			if((dlow2<value) && (value<dhigh2))
			{
				icont++;
			}
		}
		dOutput3=((double)icont)/((double)CUV.GetDataSize());
		//输出
		if((dOutput2*100>=m_dDown1)&&(dOutput3*100>=m_dDown2))
		{
			dOutput1=1.0;
		}
		else
		{
			if(m_iCalcNoStable)
			{
				//1稳态;2升;3降;4BR,0未知
				CArray<double,double> dSmoothArr;
				int iSmoothMinSize=10;//平滑后最短的数组长度
				int iSmoothStep=10;//平滑中每次求均值的个数
				if(getSmoothArr(dOrigValueArr,iSmoothStep,iSmoothMinSize,dSmoothArr))//得到平滑数据
				{
					double dFBDataPart=0.4;//取平滑数据前后百分之多少来判断在均值上下的比例
					double dFDownPer,dFUpPer;//前段，位于均值上下的百分比
					double dBDownPer,dBUpPer;//后端，位于均值上下的百分比
					getDisValue(dSmoothArr,dFBDataPart,dFDownPer,dFUpPer,dBDownPer,dBUpPer);//判断前后段在均值上下的百分比
					double dFBPartBeVauePer=0.7;
					int iUpOrDown=getUpOrDown(dFBPartBeVauePer,dFDownPer,dFUpPer,dBDownPer,dBUpPer);
					if(0==iUpOrDown)//若为未知情况则判断是BR模式还是其他运行状态。
					{
						double dBRVariance=6;//BR均方差底线,BR负荷中统计PI中负荷点真实数据，均方差>10
						double dRealVariance=getVariance(dOrigValueArr);
						if(dRealVariance>dBRVariance)
						{
							//dOutput1=4;//BR模式
							CArray<double,double> dRealVarArr;
							int iPartNum=3;//分段求方差所求的分段数量
							getVariance(dOrigValueArr,iPartNum,dRealVarArr);//分段求方差，判断各段方差是否均大于BR方差底线。
							if(dRealVarArr.GetSize()!=iPartNum)//划分结果出错，原始点太少
							{
								dOutput1=0;//未知模式
							}
							else
							{
								bool bAllBR=true;
								for(int i=0;i<dRealVarArr.GetSize();i++)
								{
									if(dRealVarArr[i]<dBRVariance)
									{
										bAllBR=false;
										break;
									}
								}
								if(bAllBR)
								{
									dOutput1=4;//BR模式
								}
								else
								{
									dOutput1=0;//未知模式
								}
							}
						}
						else
						{
							dOutput1=0;//未知模式
						}
					}
					else
					{
						dOutput1=iUpOrDown;
					}
				}
				else//数据量太少，属于其他情况
				{
					dOutput1=0.0;
				}
			}
			else
			{
				dOutput1=0.0;
			}
		}
	}
	if(m_iRadio==0)
	{
		CUniValue UniVar1;
		CCalcPort *pPortObjOut1 = GetOutputPortObj(0);
		UniVar1.AddData(dOutput1);
		pPortObjOut1->SetPortUniValue(UniVar1);	
	}
	else if(m_iRadio==1)
	{
		CUniValue UniVar1;
		CCalcPort *pPortObjOut1 = GetOutputPortObj(0);
		UniVar1.AddData(dOutput1);
		pPortObjOut1->SetPortUniValue(UniVar1);
		
		CUniValue UniVar2;
		CCalcPort *pPortObjOut2 = GetOutputPortObj(1);
		UniVar2.AddData(dOutput2);
		pPortObjOut2->SetPortUniValue(UniVar2);
		
		CUniValue UniVar3;
		CCalcPort *pPortObjOut3 = GetOutputPortObj(2);
		UniVar3.AddData(dOutput3);
		pPortObjOut3->SetPortUniValue(UniVar3);
	}
	//输出计算
	OutputResultToGlobalWS();
}
void CStableJudgeBlock::getVariance(const CArray<double,double> &dOrigValueArr,int iPartNum,CArray<double,double> &dRealVarArr)
{
	dRealVarArr.RemoveAll();
	if(dOrigValueArr.GetSize()<iPartNum+1)//原始点太少
	{
		return;
	}
	CArray<double,double> dTempArr;
	int partCount=(int)floor(dOrigValueArr.GetSize()/iPartNum);
	for(int i=0;i<iPartNum;i++)
	{
		dTempArr.RemoveAll();
		for(int j=i*partCount;j<i*(partCount+1);j++)
		{
			dTempArr.Add(dOrigValueArr[j]);
		}
		dRealVarArr.Add(getVariance(dTempArr));
	}
}
double CStableJudgeBlock::getVariance(const CArray<double,double> &dOrigValueArr)
{
	double dSum=0;
	for(int i=0;i<dOrigValueArr.GetSize();i++)
	{
		dSum+=dOrigValueArr[i];
	}
	double dAvg=dSum/dOrigValueArr.GetSize();
	dSum=0;
	for(i=0;i<dOrigValueArr.GetSize();i++)
	{
		dSum+=(dOrigValueArr[i]-dAvg)*(dOrigValueArr[i]-dAvg);
	}
	double dAvg2=dSum/dOrigValueArr.GetSize();
	return sqrt(dAvg2);
}
int CStableJudgeBlock::getUpOrDown(double dFBPartBeVauePer,
									double dFDownPer,
									double dFUpPer,
									double dBDownPer,
									double dBUpPer)
{
	int iState=0;
	if((dFDownPer>dFBPartBeVauePer)&&(dBUpPer>dFBPartBeVauePer))
	{
		iState=2;//升负荷
	}
	else if((dFUpPer>dFBPartBeVauePer)&&(dBDownPer>dFBPartBeVauePer))
	{
		iState=3;//降负荷
	}
	return iState;
}
void CStableJudgeBlock::getDisValue(const CArray<double,double> &dSmoothArr,
									double dFBDataPart,
									double &dFDownPer,
									double &dFUpPer,
									double &dBDownPer,
									double &dBUpPer)
{
	dFDownPer=0;
	dFUpPer=0;
	dBDownPer=0;
	dBUpPer=0;
	if(dFBDataPart>1)
		return;
	double dSum=0;
	for(int i=0;i<dSmoothArr.GetSize();i++)
	{
		dSum+=dSmoothArr[i];
	}
	int iSmoothCount=dSmoothArr.GetSize();
	double dAvg=dSum/iSmoothCount;
	int iPartCount=(int)floor(iSmoothCount*dFBDataPart);
	int iFDownCount=0;
	int iFUpCount=0;
	int iBDownCount=0;
	int iBUpCount=0;
	for(i=0;i<iPartCount;i++)
	{
		if(dSmoothArr[i]>dAvg)
		{
			iFUpCount++;
		}
		else if(dSmoothArr[i]<dAvg)
		{
			iFDownCount++;
		}
		if(dSmoothArr[iSmoothCount-1-i]>dAvg)
		{
			iBUpCount++;
		}
		else if(dSmoothArr[iSmoothCount-1-i]<dAvg)
		{
			iBDownCount++;
		}
	}
	dFDownPer=(double)iFDownCount/(double)iPartCount;
	dFUpPer=(double)iFUpCount/(double)iPartCount;
	dBDownPer=(double)iBDownCount/(double)iPartCount;
	dBUpPer=(double)iBUpCount/(double)iPartCount;
}
BOOL CStableJudgeBlock::getSmoothArr(const CArray<double,double> &dOrigValueArr,
									 int iSmoothStep,
									 int iSmoothMinSize,
									 CArray<double,double> &dSmoothArr)
{
	dSmoothArr.RemoveAll();
	BOOL bRet=TRUE;
	if(dOrigValueArr.GetSize()-iSmoothStep+1<iSmoothMinSize)//能够产生平滑大于要保留的最小个数
	{
		bRet=FALSE;
		return bRet;
	}
	double dSumTemp=0;
	for(int i=0;i<dOrigValueArr.GetSize()-iSmoothStep;i++)
	{
		if(i==0)
		{
			for(int j=0;j<iSmoothStep;j++)
			{
				dSumTemp+=dOrigValueArr[j];
			}
			dSmoothArr.Add(dSumTemp);
		}
		else
		{
			dSumTemp-=dOrigValueArr[i-1];
			dSumTemp+=dOrigValueArr[i+iSmoothStep-1];
			dSmoothArr.Add(dSumTemp);
		}
	}
	return bRet;
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CStableJudgeBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("StableJudge")==0)
	{
		if(!strItem1.IsEmpty())
			m_dFazhi1=atof(strItem1);
		if(!strItem2.IsEmpty())
			m_dDown1=atof(strItem2);
		if(!strItem3.IsEmpty())
			m_dFazhi2=atof(strItem3);
		if(!strItem4.IsEmpty())
			m_dDown2=atof(strItem4);
		if(!strItem5.IsEmpty())
			m_iRadio=atoi(strItem5);
	}
	if(strPropName.Compare("CalcNoStable")==0)
	{
		if(!strItem1.IsEmpty())
			m_iCalcNoStable=atoi(strItem1);
	}
}
//重载以显示不同的属性对象对话框
void CStableJudgeBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//计算块原有的输出端口描述
	IDArrRightOut.RemoveAll();

	CStringArray  IDArrRightIn;//计算块内部的输出端口描述
    IDArrRightIn.RemoveAll();

	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}

	CPropertyDlgStableJudge dlg;
    dlg.m_dFazhi1=m_dFazhi1;
	dlg.m_dFazhi2=m_dFazhi2;
	dlg.m_dDown1=m_dDown1;
	dlg.m_dDown2=m_dDown2;
	dlg.m_iRadio=m_iRadio;
	dlg.m_BCalcNoStable=m_iCalcNoStable;

	if(dlg.DoModal()==IDOK)
	{
		m_dFazhi1=dlg.m_dFazhi1;
		m_dFazhi2=dlg.m_dFazhi2;
		m_dDown1=dlg.m_dDown1;
		m_dDown2=dlg.m_dDown2;
		m_iRadio=dlg.m_iRadio;	
		m_iCalcNoStable=dlg.m_BCalcNoStable;
		
		if(m_iRadio==0)
		{
			IDArrRightIn.Add("Out");
		}
		else if(m_iRadio==1)
		{
			IDArrRightIn.Add("Out");
			IDArrRightIn.Add("T1");
			IDArrRightIn.Add("T2");
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
//重载以将本模块写入到数据库中
void CStableJudgeBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1-5：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("StableJudge"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
	//strFieldValue.Format("%d",m_strInputNum);
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(m_dFazhi1) );
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(m_dDown1) );
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(m_dFazhi2) );
	pRecord->PutCollect(_variant_t("value_item4"),_variant_t(m_dDown2) );
	strFieldValue.Format("%d",m_iRadio);
	pRecord->PutCollect(_variant_t("value_item5"),_variant_t(strFieldValue) );
	
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("CalcNoStable"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iCalcNoStable);
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(strFieldValue) );

	pRecord->Update();
}
//复制图形时，需要拷贝属性。
void CStableJudgeBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_dFazhi1=((CStableJudgeBlock*)pBlock)->m_dFazhi1;
	m_dFazhi2=((CStableJudgeBlock*)pBlock)->m_dFazhi2;
	m_dDown1=((CStableJudgeBlock*)pBlock)->m_dDown1;
	m_dDown2=((CStableJudgeBlock*)pBlock)->m_dDown2;
	m_iRadio=((CStableJudgeBlock*)pBlock)->m_iRadio;
	m_iCalcNoStable=((CStableJudgeBlock*)pBlock)->m_iCalcNoStable;			
}
//将属性放置传输数据中
void CStableJudgeBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

	CString str;
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"StableJudge");

	str.Format("%f",m_dFazhi1);strcpy(pData->m_cValueItem1,str);
	str.Format("%f",m_dFazhi2);strcpy(pData->m_cValueItem2,str);
	str.Format("%f",m_dDown1);strcpy(pData->m_cValueItem3,str);
	str.Format("%f",m_dDown2);strcpy(pData->m_cValueItem4,str);
	str.Format("%d",m_iRadio);strcpy(pData->m_cValueItem5,str);

	dataArr.Add(pData);

	pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"CalcNoStable");
	str.Format("%d",m_iCalcNoStable);strcpy(pData->m_cValueItem1,str);
	dataArr.Add(pData);
}
