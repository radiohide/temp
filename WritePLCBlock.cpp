// WritePLCBlock.cpp: implementation of the CWritePLCBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WritePLCBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWritePLCBlock::CWritePLCBlock()
{
	m_iPort=502;
	m_iFunNo=0;
	m_iAdd=0;
	m_strIP="192.168.1.1";
	
	 m_strCurrentPointName="";
	 m_strCurentType="BOOL";
		//SYSTME=====设置类型======
	//设置类型
	SetBlockType(_T( "WritePLC" ) );
	ConstructBlockInputOutput();
	//SYSTME=====初始化本模块的属性===

}

CWritePLCBlock::~CWritePLCBlock()
{

}
//构造初始的输入输出个数
void CWritePLCBlock::ConstructBlockInputOutput()
{	
	AddInputPort(_T("In"), VALUE_DOUBLE, _T("in1"), _T("in1"));
}
//根据类型新建对象的函数
//同时在CCalcBlockControlFactory中，要加入相应的代码
CCalcBlock* CWritePLCBlock::CreateFromString( const CString& strType )
{
	CWritePLCBlock* obj = new CWritePLCBlock;
	if(strType !=obj->GetBlockType())
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//初始化计算的函数做特殊的初始化
void CWritePLCBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	 hInst=LoadLibrary("ModbusInterface.dll");
}
//计算函数，实现本模块的计算
void CWritePLCBlock::Calc() 
{
	typedef int (*pInitConnect)(CString strIP,int iPort);
	pInitConnect InitConnect=(pInitConnect)::GetProcAddress(hInst,"InitConnect");	//测点快照数据查询
	
	typedef int (*pAppendTagDataByTagName_bool)(const PLCInsertData_bool  &InsertElement);
	pAppendTagDataByTagName_bool AppendTagDataByTagName_bool=(pAppendTagDataByTagName_bool)::GetProcAddress(hInst,"AppendTagDataByTagName_bool");
    typedef int (*pAppendTagDataByBatch_long)(PLCInsertData_long *pInsertElementBatch,int nCount);
	pAppendTagDataByBatch_long AppendTagDataByBatch_long=(pAppendTagDataByBatch_long)::GetProcAddress(hInst,"AppendTagDataByBatch_long");	
 	typedef int (*pAppendTagDataByBatch_float)(PLCInsertData_float *pInsertElementBatch,int nCount);
	pAppendTagDataByBatch_float AppendTagDataByBatch_float=(pAppendTagDataByBatch_float)::GetProcAddress(hInst,"AppendTagDataByBatch_float");	
 

	typedef int (*pReleaseConnect)();
	pReleaseConnect ReleaseConnect=(pReleaseConnect)::GetProcAddress(hInst,"ReleaseConnect");

	//获得输入端口的数据
	GetInputValueFromGlobalWS();
	//进行计算
	
	CCalcPort *pInPutPort = GetInputPortObj(0);		
	double dInput;
	pInPutPort->GetPortUniValue().GetDoubleVal(&dInput);
	

	int nret;
	nret=InitConnect(m_strIP,m_iPort);
	if(m_strCurentType=="BOOL")
	{	
		//写线圈，写BOOL
		PLCInsertData_bool  databW;
		memset(&databW,0,sizeof(databW));
		strcpy(databW.type,(LPSTR)(LPCTSTR)"BOOL");
		databW.add=m_iAdd;
		if(dInput!=0)
			databW.value=TRUE;
		else
			databW.value=FALSE;
		databW.funcode=5;
		nret=AppendTagDataByTagName_bool(databW);
		if(nret!=0)
		{
			CString str;
			str.Format("PLC写入BOOL失败!");
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
		else
		{
			CString str;
			str.Format("PLC写入BOOL成功!");
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
		ReleaseConnect();
	}
	else if(m_strCurentType=="REAL")
	{
		//写实数		
		PLCInsertData_float *pInsertElementBatch=new  PLCInsertData_float[1];
		for(int i=0; i<1; i++)
		{
			pInsertElementBatch[i].add=m_iAdd;
			pInsertElementBatch[i].funcode=16;
			strcpy(	pInsertElementBatch[i].type,(LPSTR)(LPCTSTR)"REAL");
			pInsertElementBatch[i].value=(float)dInput;
		}
		nret=AppendTagDataByBatch_float(pInsertElementBatch,1);
		if(nret!=0)
		{
			CString str;
			str.Format("PLC写入REAL失败!");
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
		else
		{
			CString str;
			str.Format("PLC写入REAL成功!");
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
		
		ReleaseConnect();
	}
	else if(m_strCurentType=="DINT")
	{
		//写整数		
		PLCInsertData_long *pInsertElementBatch=new  PLCInsertData_long[1];
		for(int i=0; i<1; i++)
		{
			pInsertElementBatch[i].add=m_iAdd;
			pInsertElementBatch[i].funcode=16;
			strcpy(	pInsertElementBatch[i].type,(LPSTR)(LPCTSTR)"DINT");
			pInsertElementBatch[i].value=(long)dInput;
		}
		nret=AppendTagDataByBatch_long(pInsertElementBatch,1);
		if(nret!=0)
		{
			CString str;
			str.Format("PLC写入DINT失败!");
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
		else
		{
			CString str;
			str.Format("PLC写入DINT成功!");
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
		ReleaseConnect();
	}	
	//输出计算
	OutputResultToGlobalWS();
}
//用于根据参数项和值进行属性的设置，当读取值要用
void CWritePLCBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("WritePLC1")==0)
	{
		if(!strItem1.IsEmpty())
			m_strIP=strItem1;
		if(!strItem2.IsEmpty())
			m_iPort=atoi(strItem2);
		if(!strItem3.IsEmpty())
			m_strCurentType=strItem3;
		if(!strItem4.IsEmpty())
			m_strCurrentPointName=strItem4;
	}
	if(strPropName.Compare("WritePLC2")==0)
	{
		if(!strItem1.IsEmpty())
			m_iAdd=atoi(strItem1);
		if(!strItem2.IsEmpty())
			m_iFunNo=atoi(strItem2);
	}
}
//重载以显示不同的属性对象对话框
void CWritePLCBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{

	CPropertyDlgWritePLC  dlg;

	dlg.m_strIP=m_strIP;
	dlg.m_iPort=m_iPort;
	dlg.m_strCurentType=m_strCurentType;
	dlg.m_strCurrentPointName=m_strCurrentPointName;
	dlg.m_iAdd=m_iAdd;
	dlg.m_iFunNo=m_iFunNo;
	
	if(dlg.DoModal()==IDOK)
	{
		m_strIP=dlg.m_strIP;
		m_iPort=dlg.m_iPort;
		m_strCurentType=dlg.m_strCurentType;
		m_strCurrentPointName=dlg.m_strCurrentPointName;
		m_iAdd=dlg.m_iAdd;
		m_iFunNo=dlg.m_iFunNo;

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
void CWritePLCBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	//每个属性为一个记录
	//属性1-5：数据测点标签名////////////////////////////////////////////////////////////
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("WritePLC1"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	//对测点属性进行写入
	//strFieldValue.Format("%d",m_strInputNum);
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(m_strIP) );
	strFieldValue.Format("%d",m_iPort);
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(strFieldValue) );
	pRecord->PutCollect(_variant_t("value_item3"),_variant_t(m_strCurentType) );
	pRecord->PutCollect(_variant_t("value_item4"),_variant_t(m_strCurrentPointName) );

	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue=_T("WritePLC2"); //属性名
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%d",m_iAdd);
	pRecord->PutCollect(_variant_t("value_item1"),_variant_t(strFieldValue) );
	strFieldValue.Format("%d",m_iFunNo);
	pRecord->PutCollect(_variant_t("value_item2"),_variant_t(strFieldValue) );

	pRecord->Update();

}
//复制图形时，需要拷贝属性。
void CWritePLCBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	m_strIP=((CWritePLCBlock*)pBlock)->m_strIP;
	m_iPort=((CWritePLCBlock*)pBlock)->m_iPort;
	m_strCurentType=((CWritePLCBlock*)pBlock)->m_strCurentType;
	m_strCurrentPointName=((CWritePLCBlock*)pBlock)->m_strCurrentPointName;
	m_iAdd=((CWritePLCBlock*)pBlock)->m_iAdd;
	m_iFunNo=((CWritePLCBlock*)pBlock)->m_iFunNo;
				
}
//将属性放置传输数据中
void CWritePLCBlock::SetBlockPropertyToTransferData(
												  CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CString str;
	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"WritePLC1");

	str.Format("%s",m_strIP);strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iPort);strcpy(pData->m_cValueItem2,str);
	str.Format("%s",m_strCurentType);strcpy(pData->m_cValueItem3,str);
	str.Format("%s",m_strCurrentPointName);strcpy(pData->m_cValueItem4,str);
	dataArr.Add(pData);

    pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"WritePLC2");
	str.Format("%d",m_iAdd);strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iFunNo);strcpy(pData->m_cValueItem2,str);
	dataArr.Add(pData);
}
