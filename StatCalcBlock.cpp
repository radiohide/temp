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
	m_strBlockDesp="ͳ�ƿ顣";
	m_strTitle="ͳ�ƿ�����ҳ";
	m_iSETimeType=0;
	m_iVarType=0;//1:accum;2enum;3aver;4impt

}

CStatCalcBlock::~CStatCalcBlock()
{

}

//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CStatCalcBlock::ConstructBlockInputOutput ()
{

}
void CStatCalcBlock::Calc()//���м���ĺ���,��Ҫ����
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
//��ʼ������ĺ���������ĳ�ʼ��
void CStatCalcBlock::InitCalc()
{
	CCalcBlock::InitCalc();
}

//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
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
//��������ʾ��ͬ�����Զ���Ի���
void CStatCalcBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	int portnumLinkBefore=this->GetInputNum();//����
	CStringArray  IDArrLeftOut;//�����ԭ�е�����˿�����
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
		CStringArray  IDArrLeftIn;//������ڲ������Ըı䣬��ʹ�ⲿҪ���µ�����˿�����
		CArray<int,int> TypeArrLeftIn;//
	    IDArrLeftIn.RemoveAll();
		TypeArrLeftIn.RemoveAll();
		

	if((m_iSETimeType==0)||(m_iSETimeType==2))//ϵͳʱ���,����ʱ���
	{
	}
	else if(m_iSETimeType==1)//�ⲿ����ʱ���
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


	//1��������˿ڵ��޸�	
		for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)	//ɾ��ԭ�ж����õ�Port,����ɾ
		{
			int  runs=0;//��������
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
		for(int m=0; m<IDArrLeftIn.GetSize(); m++)	//����µ�Port
		{
			int  runs=0;//��������
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
//�����Խ���ģ��д�뵽���ݿ���
void CStatCalcBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue.Format("DBNameSETimeTypeVarType");
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%s",m_strDBName); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iSETimeType); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",m_iVarType); //������
	pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
	
	for(int i=0;i<m_strVarNameArr.GetSize();i++)//���ʽ
	{
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("Var.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������

		strFieldValue.Format("%s",m_strVarNameArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",m_strVarDespArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",m_strVarUnitArr[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",m_strUnitSetArr[i]); //������
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������	
	}
	pRecord->Update();
}

//����ͼ��ʱ����Ҫ�������ԡ�
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
//�����Է��ô���������
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
	for(int i=0;i<m_strVarNameArr.GetSize();i++)//���ʽ
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
