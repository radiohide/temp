// RTPointBatchBlock1.cpp: implementation of the CRTPointBatchBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RTPointBatchBlock.h"
#include "PropertyDlgRTPointBat.h"

#include<fstream>
using namespace std; 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRTPointBatchBlock::CRTPointBatchBlock()
{
	m_iSysOrInput=0;
	pTagData=NULL;
	for(int i=0;i<1000;i++)
	{
		pTagNames[i]=NULL;
	}
	SetBlockType(_T( "RTPointBatch" ) );
	ConstructBlockInputOutput();
}

CRTPointBatchBlock::~CRTPointBatchBlock()
{
	for(int i=0;i<strRTDBTagArr.GetSize();i++)
	{
		if(NULL!=pTagNames[i])
		{
			delete []pTagNames[i];
			pTagNames[i]=NULL;
		}
	}
	if(pTagData!=NULL)
	{
		delete []pTagData;
		pTagData=NULL;
	}
}
//根据类型新建对象的函数
CCalcBlock* CRTPointBatchBlock::CreateFromString( const CString& strType )
{
	CRTPointBatchBlock* obj = new CRTPointBatchBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CRTPointBatchBlock::ConstructBlockInputOutput ()
{
	//AddOutputPort("Out",VALUE_DOUBLE);
}
//初始化计算的函数做特殊的初始化
void CRTPointBatchBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	for(int i=0;i<strRTDBTagArr.GetSize();i++)
	{
		pTagNames[i]=new char[48];
		memset(pTagNames[i],0,48);
		strcpy(pTagNames[i],strRTDBTagArr[i]);
	}
	int iCount=strRTDBTagArr.GetSize();
	if(iCount>0)
	{
		pTagData=new TagData[iCount];
	}
	this->m_pModel->LoadRTDBDllAndCon();
}
//返回属性的逗点字符串
CString CRTPointBatchBlock::GetBlockStrPro()//to do
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CRTPointBatchBlock::SetBlockStrPro(CString str)//设置计算块的字符串属性//to do
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
void CRTPointBatchBlock::Calc()//进行计算的函数,需要重载
{
	if(this->m_pModel->m_bConRTDB)
	{
		GetInputValueFromGlobalWS();

		double dInValue=0;
		long lInTime = CTime::GetCurrentTime().GetTime();
		if(m_iSysOrInput==1)
		{
			CCalcPort *pInPortObj = GetInputPortObj(0);
			pInPortObj->GetPortUniValue().GetDoubleVal(&dInValue);
			lInTime=(long)dInValue;
		}

		int inNum = GetOutputNum();
		int nRet = -1;
		int iCount=strRTDBTagArr.GetSize();
		if(iCount==0)
			return;
		memset(pTagData,0,iCount*sizeof(TagData));

		//按名称读取
		typedef  int (*pGetRTDataByBatch)(char* tagNames[],TagData * pTagData,long nCount);
		pGetRTDataByBatch m_GetRTDataByBatch =(pGetRTDataByBatch)GetProcAddress(this->m_pModel->hDLL,"GetRTDataByBatch");


		//调试代码：耗时测试
		long t1,t2;
		if (this->m_pModel->m_strModelName.Compare("ZJ_w001w003AccumBuffer")==0)
		{
			t1 = GetTickCount();
		}
		
		nRet=m_GetRTDataByBatch(pTagNames,pTagData,iCount);
		if(nRet!=0)
		{
			::Sleep(5);
			memset(pTagData,0,iCount*sizeof(TagData));
			nRet=m_GetRTDataByBatch(pTagNames,pTagData,iCount);//查询失败再查询一次
		}

		if (this->m_pModel->m_strModelName.Compare("ZJ_w001w003AccumBuffer")==0)
		{
			int nModuleCount = this->m_pModel->GetBlockCount();
			CString sBlockDesp = this->GetBlockName();
			
			t2 = GetTickCount();
			long t3 = t2 - t1;
			
			ofstream outfile("E:\\readrtdb.txt");
			CString sc ;
			sc.Format("block name is %s:Time is %d ms",sBlockDesp,t3);
			
			//if (t3 > 5000)
			{
				outfile<<(LPCTSTR)(_T(sc));
			}
			
			outfile.close();
		}

		//改为按时间读取new++
		/*typedef  int (*pGetHistoryDataByTime)(const char * tagName,long time, TagData *pTagData);
		pGetHistoryDataByTime m_GetHistoryDataByTime =(pGetHistoryDataByTime)GetProcAddress(this->m_pModel->hDLL,"GetHistoryDataByTime");

		for (int i=0;i<inNum;i++)
		{
			nRet = m_GetHistoryDataByTime(pTagNames[i],lInTime, &(pTagData[i]));
			if(nRet!=0)
			{
				for(int j=0;j<50;j++)//若读取失败，则尝试50次读取。
				{
					memset(&(pTagData[i]),0,sizeof(TagData));
					::Sleep(5);//有时候确实会读取失败，在第二次读取的时候会成功
					nRet = m_GetHistoryDataByTime(pTagNames[i],lInTime, &(pTagData[i]));
					if(nRet==0)
						break;
				}
				if(nRet!=0)
				{
					pTagData[i].status=0;
					pTagData[i].time=0;
					pTagData[i].value=0;
				}
			}
		}*/

		
		if(nRet==0)//查询成功
		{
			/*
			long lNow=CTime::GetCurrentTime().GetTime();
			if(m_iSysOrInput==1)//如果来自外部时间
			{
				lNow=lInTime;
			}*/
			for(int i=0;i<inNum;i++)
			{
				CCalcPort *pPortObj = GetOutputPortObj(i);
				CUniValue UniVar;
				//UniVar.AddData(pTagData[i].value,lInTime,pTagData[i].status);//赋值为当前时间
				UniVar.AddData(pTagData[i].value,lInTime,pTagData[i].status,pTagData[i].time);
				pPortObj->SetPortUniValue(UniVar);
			}
		}
		else//查询失败
		{
			for(int i=0;i<inNum;i++)
			{
				CCalcPort *pPortObj = GetOutputPortObj(i);
				CUniValue UniVar;
				UniVar.AddData(0,0,-1); //原
				//UniVar.AddData(0,0,0);
				pPortObj->SetPortUniValue(UniVar);
			}
		}
	}
	OutputResultToGlobalWS();
}

//用于根据参数项和值进行属性的设置，当读取值要用
void CRTPointBatchBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("SysOrInput")==0)
	{
		if(!strItem1.IsEmpty())  {m_iSysOrInput=atoi(strItem1);}
	}
	else
	{
		int propType,propIndex;
		getPropTypeByName(strPropName,propType,propIndex);
		if(propType==2)
		{
			if(!strItem1.IsEmpty())  {strRTDBTagArr.SetAtGrow(propIndex,strItem1);}
			if(!strItem2.IsEmpty())  {strPortTagArr.SetAtGrow(propIndex,strItem2);}
			if(!strItem3.IsEmpty())  {strUnitArr.SetAtGrow(propIndex,strItem3);}
			if(!strItem4.IsEmpty())  {strTagDespArr.SetAtGrow(propIndex,strItem4);}
		}
	}
}

void CRTPointBatchBlock::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
{
	int dIndex=strPropName.ReverseFind('.');
	CString strIndex=strPropName.Right(strPropName.GetLength()-dIndex-1);
	CString strName=strPropName.Left(dIndex);
	propIndex=atoi(strIndex);

	if(strName=="input")//输入为1
	{
		propType=1;
	}
	else if(strName=="output")//输出为2
	{
		propType=2;
	}
	else if(strName=="formula")//公式为3
	{
		propType=3;
	}
}
//重载以显示不同的属性对象对话框
void CRTPointBatchBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgRTPointBat dlg;
	
	int portnumLinkBefore=this->GetInputNum();//新增
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//计算块原有的输出端口描述
	CStringArray  IDArrRightIn;//计算块内部的输出端口描述
	
	CStringArray  IDArrLeftOut;//计算块原有的输入端口描述
	CStringArray  IDArrLeftIn;//计算块内部的输入端口描述

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

	dlg.strPortTagArr.Copy(strPortTagArr);
	dlg.strRTDBTagArr.Copy(strRTDBTagArr);
	dlg.strTagDespArr.Copy(strTagDespArr);
	dlg.strUnitArr.Copy(strUnitArr);
	dlg.m_iSysOrInput=m_iSysOrInput;
	if(dlg.DoModal()==IDOK)
	{
		strPortTagArr.Copy(dlg.strPortTagArr);
		strRTDBTagArr.Copy(dlg.strRTDBTagArr);
		strTagDespArr.Copy(dlg.strTagDespArr);
		strUnitArr.Copy(dlg.strUnitArr);
		
		m_iSysOrInput=dlg.m_iSysOrInput;

		//1、对输入端口的修改
		if(m_iSysOrInput==1)
		{
			IDArrLeftIn.Add("time");
		}
		
		//time动态变化代码
		for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)//删除原有而不用的Port,倒着删
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
		

		//对输出端口的修改
		for(int i=0;i<strPortTagArr.GetSize();i++)
		{
			IDArrRightIn.Add(strPortTagArr[i]);
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
				InSertOutputPortBefore(mm,str, VALUE_DOUBLE, _T(str));
			}
		}
		parent->RedrawWindow();
	}
}
//重载以将本模块写入到数据库中
void CRTPointBatchBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	for(int i=0;i<strRTDBTagArr.GetSize();i++)//输出
	{
		CString strOutPut=strRTDBTagArr[i];
		if(strOutPut=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("output.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%s",strRTDBTagArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",strPortTagArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",strUnitArr[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",strTagDespArr[i]); //属性名
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名
	}
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue="SysOrInput";//属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iSysOrInput); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	
	pRecord->Update();
}	
//复制图形时，需要拷贝属性。
void CRTPointBatchBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->strRTDBTagArr.Copy(((CRTPointBatchBlock*)pBlock)->strRTDBTagArr);
	this->strPortTagArr.Copy(((CRTPointBatchBlock*)pBlock)->strPortTagArr);
	this->strTagDespArr.Copy(((CRTPointBatchBlock*)pBlock)->strTagDespArr);
	this->strUnitArr.Copy(((CRTPointBatchBlock*)pBlock)->strUnitArr);
	this->m_iSysOrInput=((CRTPointBatchBlock*)pBlock)->m_iSysOrInput;
	
}
//将属性放置传输数据中
void CRTPointBatchBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}
