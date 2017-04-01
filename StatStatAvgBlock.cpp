// StatStatAvgBlock.cpp: implementation of the CStatStatAvgBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatStatAvgBlock.h"
#include "PropertyDlgStatProAvg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatStatAvgBlock::CStatStatAvgBlock(int iTimeSelfOrInput):CStatProAvgBlock(iTimeSelfOrInput)
{
	SetBlockType(_T( "StatStatAvg" ) );
	this->iTimeSelfOrInput=iTimeSelfOrInput;//默认不取外部时间
}

CStatStatAvgBlock::~CStatStatAvgBlock()
{

}
//根据类型新建对象的函数
CCalcBlock* CStatStatAvgBlock::CreateFromString( const CString& strType )
{
	CStatStatAvgBlock* obj = new CStatStatAvgBlock(0);
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CStatStatAvgBlock::Calc()//进行计算的函数,需要重载
{
	GetInputValueFromGlobalWS();
    //计算过程
	int lSTime=0;
	int lETime=0;
	int iInputDataStart=0;
	if(iTimeSelfOrInput==1)
	{
		double dInValue=0;
		CCalcPort *pInPortObj1 = GetInputPortObj(0);
		pInPortObj1->GetPortUniValue().GetDoubleVal(&dInValue);
		lSTime=(long)dInValue;

		CCalcPort *pInPortObj2 = GetInputPortObj(1);
		pInPortObj2->GetPortUniValue().GetDoubleVal(&dInValue);
		lETime=(long)dInValue;
		iInputDataStart=2;
	}
	for(int i=0;i<strInputArr.GetSize();i++)
	{
		CCalcPort *pPortObjIn3 = GetInputPortObj(iInputDataStart+i);	
		CUniValue  &CUV=pPortObjIn3->GetPortUniValue();
		
		double dAvg=0;
		double dMin=0;
		double dMax=0;	
		long lRealSTime=0;
		long lRealETime=0;
		int iCount=0; 
		statData(CUV,dAvg,dMin,dMax,lRealSTime,lRealETime,iCount);
		if(iTimeSelfOrInput==1)//修正为外部时间段。
		{
			lRealSTime=lSTime;
			lRealETime=lETime;
		}
		StatData oneStatData;
		oneStatData.lSTime=lRealSTime;
		oneStatData.lETime=lRealETime;
		oneStatData.lInsertTime=0;
		oneStatData.dMin=dMin;
		oneStatData.dMax=dMax;
		oneStatData.dValue=dAvg;
		oneStatData.iType=0;
		oneStatData.iCount=iCount;

		CUniValue UniVarOut;
		CCalcPort *pPortObjOut = GetOutputPortObj(i);
		UniVarOut.AddStatData(oneStatData);
		pPortObjOut->SetPortUniValue(UniVarOut);
	}
	//输出计算
	OutputResultToGlobalWS();
}
void CStatStatAvgBlock::statData(CUniValue  &CUV,
							 double &dAvg,
							 double &dMin,
							 double &dMax,
							 long &lRealSTime,
							 long &lRealETime,
							 int &iCount)
{
	dAvg=0;
	dMin=0;
	dMax=0;
	lRealSTime=0;
	lRealETime=0;
	iCount=CUV.GetStatDataSize();
	double dSum=0;
	for(int j=0; j<CUV.GetStatDataSize();j++)
	{
		double value=0;
		StatData statData;
		CUV.GetStatDataAt(j,&statData);
		if(j==0)
		{
			dMin=statData.dMin;
			dMax=statData.dMax;
			lRealSTime=statData.lSTime;
			lRealETime=statData.lETime;
		}
		else
		{
			if(statData.dMin<dMin)
			{
				dMin=statData.dMin;
			}
			if(statData.dMax>dMax)
			{
				dMax=statData.dMax;
			}
		}
		dSum+=statData.dValue;
		if(j+1==CUV.GetStatDataSize())
		{
			lRealETime=statData.lETime;
		}
	}
	if(CUV.GetStatDataSize()>0)
	{
		dAvg=dSum/CUV.GetStatDataSize();
	}
}
void CStatStatAvgBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgStatProAvg dlg;
	dlg.m_iProOrStatOrSum=1;

	int portnumLinkBefore=this->GetInputNum();//新增
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//计算块原有的输出端口描述
	CStringArray  IDArrRightIn;//计算块内部的输出端口描述
	
	CStringArray  IDArrLeftOut;//计算块原有的输入端口描述
	CArray<int,int> IDArrLeftOutType;//计算块原有的输入端口类型
	CStringArray  IDArrLeftIn;//计算块内部的输入端口描述
	CArray<int,int>  IDArrLeftInType;//计算块内部的输入端口类型
	
	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		int iPortType=this->GetInputPortObj(q)->GetPortValueType();
		IDArrLeftOut.Add(strPortID);
		IDArrLeftOutType.Add(iPortType);
	}

	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}

	dlg.strInputArr.Copy(strInputArr);//输入变量
	dlg.strOutputArr.Copy(strOutputArr);//输出变量
	dlg.strTagDespArr.Copy(strTagDespArr);//描述点名
	dlg.strUnitArr.Copy(strUnitArr);//单位
	dlg.iTimeSelfOrInput=iTimeSelfOrInput;//标注时间段：0数据本身；1外部输入
	if(dlg.DoModal()==IDOK)
	{
		strInputArr.Copy(dlg.strInputArr);//输入变量
		strOutputArr.Copy(dlg.strOutputArr);//输出变量
		strTagDespArr.Copy(dlg.strTagDespArr);//描述点名
		strUnitArr.Copy(dlg.strUnitArr);//单位
		iTimeSelfOrInput=dlg.iTimeSelfOrInput;//标注时间段：0数据本身；1外部输入


		//1、对输入端口的修改
		if(iTimeSelfOrInput==1)
		{
			IDArrLeftIn.Add("STime");
			IDArrLeftInType.Add(VALUE_DOUBLE);
			IDArrLeftIn.Add("ETime");
			IDArrLeftInType.Add(VALUE_DOUBLE);
		}
		for(int i=0;i<strInputArr.GetSize();i++)
		{
			IDArrLeftIn.Add(strInputArr[i]);
			IDArrLeftInType.Add(VALUE_STAT_SERIES);
		}
		for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)	//删除原有而不用的Port,倒着删
		{
			int  runs=0;//遍历次数
			for(int k=0; k<IDArrLeftIn.GetSize(); k++)
			{
				if((IDArrLeftIn[k]!=IDArrLeftOut[j])||(IDArrLeftInType[k]!=IDArrLeftOutType[j]))	
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
				if((IDArrLeftIn[m]!=IDArrLeftOut[n])||(IDArrLeftInType[m]!=IDArrLeftOutType[n]))
				{runs++;}		
			}
			if(runs==IDArrLeftOut.GetSize())
			{
				CString str=IDArrLeftIn[m];
				InSertInputPortBefore(m,str, IDArrLeftInType[m], _T(str));
			}
		}

		//对输出端口的修改
		for(i=0;i<strOutputArr.GetSize();i++)
		{
			IDArrRightIn.Add(strOutputArr[i]);
		}		
		for(int jj=IDArrRightOut.GetSize()-1; jj>-1; jj--)//删除原有而不用的Port,倒着删
		{
			int  runs=0;//遍历次数
			for(int kk=0; kk<IDArrRightIn.GetSize(); kk++)
			{
				if((IDArrRightIn[kk]!=IDArrRightOut[jj]))	
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
				if((IDArrRightIn[mm]!=IDArrRightOut[nn]))
				{runs++;}		
			}
			if(runs==IDArrRightOut.GetSize())
			{
				CString str=IDArrRightIn[mm];
				InSertOutputPortBefore(mm,str, VALUE_STAT, _T(str));
			}
		}
		parent->RedrawWindow();
	}
}	