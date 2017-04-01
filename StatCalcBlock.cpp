// StatCalcBlock.cpp: implementation of the CStatCalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "StatCalcBlock.h"
#include "PropertyDlgStat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatCalcBlock::CStatCalcBlock()
{
	m_strBlockDesp="统计块。";
	m_strTitle="统计块属性页";
	m_iSETimeType=0;
	m_iVarType=0;//1:accum;2enum;3aver;4impt

}

CStatCalcBlock::~CStatCalcBlock()
{

}

//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CStatCalcBlock::ConstructBlockInputOutput ()
{

}
void CStatCalcBlock::Calc()//进行计算的函数,需要重载
{
}
BOOL CStatCalcBlock::IsTableExist(_ConnectionPtr &pConnection, CString strTableName)
{
	CString strSQL;
	_RecordsetPtr        pRecord;
	pRecord.CreateInstance(__uuidof(Recordset));
	pRecord->CursorLocation = adUseClient;
	
	strSQL.Format("select * from %s",strTableName);
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
//初始化计算的函数做特殊的初始化
void CStatCalcBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}

//用于根据参数项和值进行属性的设置，当读取值要用
void CStatCalcBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("DBNameSETimeTypeVarType")==0)
	{
		if(!strItem1.IsEmpty())  {m_strDBName=strItem1;}
		if(!strItem2.IsEmpty())  {m_iSETimeType=atoi(strItem2);}
		if(!strItem3.IsEmpty())  {m_iVarType=atoi(strItem3);}
	}
	else
	{
		if(!strItem1.IsEmpty())  
			m_strVarNameArr.Add(strItem1);
		else
			return;
		if(!strItem2.IsEmpty())  
			m_strVarDespArr.Add(strItem2);
		else
			m_strVarDespArr.Add("");
		if(!strItem3.IsEmpty())  
			m_strVarUnitArr.Add(strItem3);
		else
			m_strVarUnitArr.Add("");
		if(!strItem4.IsEmpty())  
			m_strUnitSetArr.Add(strItem4);
		else
			m_strUnitSetArr.Add("");
	}
}
//重载以显示不同的属性对象对话框
void CStatCalcBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBefore=this->GetInputNum();//新增
	CStringArray  IDArrLeftOut;//计算块原有的输入端口描述
	CArray<int,int> TypeArrLeftOut;//
    IDArrLeftOut.RemoveAll();
	TypeArrLeftOut.RemoveAll();

    for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		IDArrLeftOut.Add(strPortID);
		int iType=this->GetInputPortObj(q)->GetPortValueType();
		TypeArrLeftOut.Add(iType);
	}
    
	CPropertyDlgStat dlg;
	dlg.m_strTitle=m_strTitle;
	dlg.m_strBlockDesp=m_strBlockDesp;
	dlg.m_strDBName=m_strDBName;
	dlg.SetSourceRDBData(this->m_pModel->m_ArrSourceRDBTagCon,
		this->m_pModel->m_ArrSourceRDBName,
		this->m_pModel->m_ArrSourceRDBType);
	dlg.m_iSETimeType=m_iSETimeType;
	dlg.m_pModel=m_pModel;
	dlg.m_iVarType=m_iVarType;
	
	dlg.m_strVarNameArr.Copy(m_strVarNameArr);
	dlg.m_strVarDespArr.Copy(m_strVarDespArr);
	dlg.m_strVarUnitArr.Copy(m_strVarUnitArr);
	dlg.m_strUnitSetArr.Copy(m_strUnitSetArr);
	if(dlg.DoModal()==IDOK)
	{
		m_strDBName=dlg.m_strDBName;
		m_iSETimeType=dlg.m_iSETimeType;
		
		m_strVarNameArr.Copy(dlg.m_strVarNameArr);
		m_strVarDespArr.Copy(dlg.m_strVarDespArr);
		m_strVarUnitArr.Copy(dlg.m_strVarUnitArr);
		m_strUnitSetArr.Copy(dlg.m_strUnitSetArr);
		//ConstructBlockInputOutput();
		CStringArray  IDArrLeftIn;//计算块内部因属性改变，而使外部要更新的输入端口描述
		CArray<int,int> TypeArrLeftIn;//
	    IDArrLeftIn.RemoveAll();
		TypeArrLeftIn.RemoveAll();
		

	if((m_iSETimeType==0)||(m_iSETimeType==2))//系统时间段,数据时间段
	{
	}
	else if(m_iSETimeType==1)//外部输入时间段
	{	
		IDArrLeftIn.Add("STime");
		TypeArrLeftIn.Add(VALUE_DOUBLE);
		IDArrLeftIn.Add("ETime");
		TypeArrLeftIn.Add(VALUE_DOUBLE);
	}
	for(int i=0;i<this->m_strVarNameArr.GetSize();i++)
	{
		IDArrLeftIn.Add(m_strVarNameArr[i]);
		TypeArrLeftIn.Add(VALUE_RTVALUE_SERIES);
	}


	//1、对输入端口的修改	
		for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)	//删除原有而不用的Port,倒着删
		{
			int  runs=0;//遍历次数
			for(int k=0; k<IDArrLeftIn.GetSize(); k++)
			{
				if((IDArrLeftIn[k]!=IDArrLeftOut[j])	
					||(TypeArrLeftOut[j]!=TypeArrLeftIn[k]))
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
				if((IDArrLeftIn[m]!=IDArrLeftOut[n])
					||(TypeArrLeftOut[n]!=TypeArrLeftIn[m]))
				{runs++;}		
			}
			if(runs==IDArrLeftOut.GetSize())
			{
				CString str=IDArrLeftIn[m];
				InSertInputPortBefore(m,str, TypeArrLeftIn[m], _T(str));
			}
		}
		parent->RedrawWindow();
	}
}
//重载以将本模块写入到数据库中
void CStatCalcBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
	strFieldValue.Format("DBNameSETimeTypeVarType");
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名
	strFieldValue.Format("%s",m_strDBName); //属性名
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_iSETimeType); //属性名
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
	strFieldValue.Format("%d",m_iVarType); //属性名
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
	
	for(int i=0;i<m_strVarNameArr.GetSize();i++)//表达式
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //放Name
		strFieldValue.Format("Var.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //属性名

		strFieldValue.Format("%s",m_strVarNameArr[i]); //属性名
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",m_strVarDespArr[i]); //属性名
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",m_strVarUnitArr[i]); //属性名
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //属性名	
		strFieldValue.Format("%s",m_strUnitSetArr[i]); //属性名
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //属性名	
	}
	pRecord->Update();
}

//复制图形时，需要拷贝属性。
void CStatCalcBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->m_strDBName=((CStatCalcBlock*)pBlock)->m_strDBName;
	this->m_iSETimeType=((CStatCalcBlock*)pBlock)->m_iSETimeType;
	this->m_iVarType=((CStatCalcBlock*)pBlock)->m_iVarType;

	this->m_strVarNameArr.Copy(((CStatCalcBlock*)pBlock)->m_strVarNameArr);
	this->m_strVarDespArr.Copy(((CStatCalcBlock*)pBlock)->m_strVarDespArr);
	this->m_strVarUnitArr.Copy(((CStatCalcBlock*)pBlock)->m_strVarUnitArr);
	this->m_strUnitSetArr.Copy(((CStatCalcBlock*)pBlock)->m_strUnitSetArr);
}
//将属性放置传输数据中
void CStatCalcBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

	CBlockPropertyTransferData *pData=new CBlockPropertyTransferData();
	strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
	strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
	strcpy(pData->m_cBlockName,GetBlockName());
	strcpy(pData->m_cPropName,"DBNameSETimeTypeVarType");	
	CString str;
	str.Format("%s",m_strDBName);strcpy(pData->m_cValueItem1,str);
	str.Format("%d",m_iSETimeType);strcpy(pData->m_cValueItem2,str);
	str.Format("%d",m_iVarType);strcpy(pData->m_cValueItem3,str);
	dataArr.Add(pData);

	
	CString strFieldValue;
	for(int i=0;i<m_strVarNameArr.GetSize();i++)//表达式
	{
		pData=new CBlockPropertyTransferData();
		strcpy(pData->m_cModelName,this->m_pModel->m_strModelName);
		strcpy(pData->m_cModuleName,this->m_pModule->m_Name);
		strcpy(pData->m_cBlockName,GetBlockName());
		strFieldValue.Format("Var.%d",i);
		strcpy(pData->m_cPropName,strFieldValue);
		str.Format("%s",m_strVarNameArr[i]);
		strcpy(pData->m_cValueItem1,str);
		str.Format("%s",m_strVarDespArr[i]);
		strcpy(pData->m_cValueItem2,str);
		str.Format("%s",m_strVarUnitArr[i]);
		strcpy(pData->m_cValueItem3,str);
		str.Format("%s",m_strUnitSetArr[i]);
		strcpy(pData->m_cValueItem4,str);
		dataArr.Add(pData);
	}
}
