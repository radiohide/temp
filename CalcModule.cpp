// CalcModule.cpp: implementation of the CCalcModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "CalcModule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "CalcModel.h"
#include "CalcBlock.h"

#include "CalcBlockControlFactory.h"

//extern CCalcModel    *g_pCalModel;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcModule::CCalcModule()
{
	m_iModuleWarningCount=0;
	m_iModuleErrorCount=0;

	m_pStrParentModelName=NULL;
	m_iCalcOrderID = -1;//˵��û�а���
	m_bCompared =  FALSE;
	m_iStopTag=0;//��ʼ����������//for stop
	m_iLogOnOrOff=0;
	m_iTraceOn=1;//׷�ٿ���1:����0:��
	m_iDebugOn=1;//���Կ���1:����0:��
}

CCalcModule::~CCalcModule()
{
	ClearAllBlocks();
}
void CCalcModule::ClearAllBlocks()
{
	while (!m_BlockList.IsEmpty ())
	{
		delete m_BlockList.RemoveHead ();
	}
}
int CCalcModule::GetBlockCount()
{
	return m_BlockList.GetCount();
}
//Ѱ��һ�����õ�����
CString CCalcModule::FindCanUseBlockName(CString strBlockType)
{
	if(strBlockType.IsEmpty()) return _T("");
	CString strFactoryID=_T("");
	//����û���������ͣ��м�һ��û����Ϊ1
	int index =-1;
	int size = s_BlockTypeArray.GetSize();
	for(int k=0;k<size;k++)
	{
		if(s_BlockTypeArray.GetAt(k)==strBlockType)
		{
			index = k;
			break;
		}
	}
	if(index ==-1) //�µ�
	{
		int  curID=1;
		CString strID="";
		strFactoryID.Format("%s%s",strBlockType,_T("1"));
		//////
		CCalcBlock* pBlock=FindBlockByName(strFactoryID);
		while(pBlock!=NULL)
		{
			curID++;
			strFactoryID.Format("%s%d",strBlockType,curID);
			pBlock=FindBlockByName(strFactoryID);
		}
		s_BlockTypeArray.Add(strBlockType);
		strID.Format("%d",curID);
		s_BlockIDArray.Add(strID);
	}
	else //����
	{
		int  curID = atoi(s_BlockIDArray.GetAt(index))+1;
		CString strType = s_BlockTypeArray.GetAt(index);
		strFactoryID.Format("%s%d",strType,curID);
		
		//////
		CCalcBlock* pBlock=FindBlockByName(strFactoryID);
		while(pBlock!=NULL)
		{
			curID++;
			strFactoryID.Format("%s%d",strType,curID);
			pBlock=FindBlockByName(strFactoryID);
		}
		CString strID;
		strID.Format("%d",curID);
		s_BlockIDArray.SetAt(index,strID);
	}
	return strFactoryID;
}
void CCalcModule::AddBlock(CCalcBlock* pBlock)
{
	m_BlockList.AddTail(pBlock);
}
void CCalcModule::InitforCalc(CCalcModel* pModel)
{
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos)
	{
		CCalcBlock* pBlock =  (CCalcBlock*)m_BlockList.GetNext(pos);
		pBlock->SetModelPtr(pModel);
		pBlock->SetModulePtr(this);//for stop
		pBlock->InitCalc();
	}
}
//���ü���˳��
void CCalcModule::SetCalcOrderID(int orderID)
{
	m_iCalcOrderID = orderID;
}
int CCalcModule::GetCalcOrderID()
{
	return m_iCalcOrderID;
}
//ÿ��ģ�����������������
void CCalcModule::CheckBlockCalcIntegrity(CStringArray& LogStrings)
{
	CString str;
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos)
	{
		CCalcBlock* pBlock=  (CCalcBlock*)m_BlockList.GetNext(pos);
		BOOL bOK = pBlock->CheckBlockCalcIntegrity();
		if(!bOK)
		{
			str.Format("ģ��:%s,�����:%s,������:%s",GetModuleName(),pBlock->GetBlockName(),pBlock->GetBlockDesp());
			LogStrings.Add(str);
		}
	}
}
//���������˳��
void CCalcModule::AdjustBlockCalcOrder()
{
//1. ������ģ�����ʱ������ı�־ΪFALSE
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos)
	{
		CCalcBlock* pBlock=  (CCalcBlock*)m_BlockList.GetNext(pos);
		pBlock->m_bCompared = FALSE;
	}
   //2. ÿ������Сֵ
	int k=0;
	int max = m_BlockList.GetCount();
	POSITION posFind;
	POSITION pos1;
	int t=0;
	int min;
	while(k<max)
	{
		t=0;
		posFind=NULL;
		pos = m_BlockList.GetHeadPosition();
		pos1 = pos;
		while(pos)
		{
			CCalcBlock* pBlock=  (CCalcBlock*)m_BlockList.GetNext(pos);
			if(pBlock->m_bCompared ==FALSE)
			{
				if(t==0) //��һ��
				{
					t=1;
					min = pBlock->GetOrderID();
					posFind = pos1;
				}
				else //
				{
					if(pBlock->GetOrderID()<min)
					{
						posFind = pos1;
						min = pBlock->GetOrderID();
					}

				}
			}
			pos1 =pos;
		}
		//�ҵ���С��,Ȼ��������ж����ƶ������е�β��
		if(posFind!=NULL)
		{
			CCalcBlock* pBlock = (CCalcBlock*)m_BlockList.GetAt(posFind);
			pBlock->m_bCompared = TRUE;
			m_BlockList.RemoveAt(posFind);
			m_BlockList.AddTail(pBlock);
		}
		k++;
	}
}
int CCalcModule::GetCounterByBType(CString strBType)//�õ�ĳ���ͼ����ĸ���
{
	int iCounter=0;
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos)
	{
		CCalcBlock* pBlock =  (CCalcBlock*)m_BlockList.GetNext(pos);
		if(pBlock->GetBlockType()==strBType)
			iCounter++;
	}
	return iCounter;
}
void CCalcModule::Calc()
{
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos)
	{
		CCalcBlock* pBlock =  (CCalcBlock*)m_BlockList.GetNext(pos);
		pBlock->Calc();
		if(m_iStopTag)
		{
			this->ResetStopCalc();//ģ�鸴λ
			break;
		}
	}
}
void CCalcModule::StopCalc()//for stop
{
	m_iStopTag=1;//ֹͣ����
}
void CCalcModule::ResetStopCalc()//ֹͣ��־��λ//for stop
{
	m_iStopTag=0;//��λ
}
//�����ݿ��ж�ȡģ�������
BOOL CCalcModule::ReadModule(_ConnectionPtr &pCon)
{
	BOOL bRet = TRUE;
	//���ȶ�ȡ������б�ͬʱ�õ�����������
	////////////////////////////////////////////////////
    CString strModuleBlockListTable;//U1Perf_TagPoint_BlockList
	CString strModuleBlockPropTable;//���Եı�
    strModuleBlockListTable.Format("%s_%s_BlockList",*m_pStrParentModelName,GetModuleName());
	strModuleBlockPropTable.Format("%s_%s_BlockProp",*m_pStrParentModelName,GetModuleName());
	////////////////////////////////////////////////////
	//��ӿ飬Ȼ���ٶ�ȡ�������ֵ
	////////////////////////////////////////////////////////////////////
  	_RecordsetPtr  pRecord;
	_variant_t     Tempvalue ;
    VARIANT_BOOL bBOF ;
    pRecord.CreateInstance(__uuidof(Recordset)) ;
    pRecord->CursorLocation = adUseClient;

    CString strSQL;
    strSQL.Format("select * from %s",strModuleBlockListTable);
    try
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		//pRecord->Close() ;
		return FALSE;
	}	
	bBOF = pRecord->GetBOF();
	if(!bBOF)  pRecord->MoveFirst();
	//���б���������֡�����/����
	while(!pRecord->adoEOF )
	{
		CString strName; //��
		CString strDesp;//������
		CString strType;//�����ͣ����������ĸ��������
		long OrderID;

		int InNum,OutNum;
		CString InNames; CString strInValueTypes;
		CString OutNames;CString strOutValueTypes;
		CString InputLinkStrs;

		BOOL bThisOk = TRUE;
		try
		{
			Tempvalue = pRecord->GetCollect("name");
			strName = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("desp");
			strDesp = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("type");
			strType = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("orderid");
			OrderID = long(Tempvalue);

			Tempvalue = pRecord->GetCollect("InNum");
			InNum = short(Tempvalue);
			Tempvalue = pRecord->GetCollect("OutNum");
			OutNum = short(Tempvalue);

			Tempvalue = pRecord->GetCollect("InNames");
			InNames = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("OutNames");
			OutNames = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("InputLinkStrs");
			InputLinkStrs = (LPCSTR)_bstr_t(Tempvalue);

			//��ȡ�˿ڵ�����
			Tempvalue = pRecord->GetCollect("InValueTypes");
			strInValueTypes = (LPCSTR)_bstr_t(Tempvalue);

			
			//��ȡ�˿ڵ�����
			Tempvalue = pRecord->GetCollect("OutValueTypes");
			strOutValueTypes = (LPCSTR)_bstr_t(Tempvalue);

			
		}
		catch(_com_error e)
		{
			//bThisOk = FALSE;
			//bRet = FALSE;
			//break;
		}
		if(bThisOk)
		{
		    //���ݴ˿�����֡����ͣ�Ȼ��ȥ��������ȡ����
			//�������ֶ������;���������ϵ������������Ϊ����ֵ
			CCalcBlock* pnewBlock = CreateBlock(strType,strName,strDesp);
			if(pnewBlock!=NULL)
			{
				pnewBlock->CreatePortsFromString(InNum,OutNum,InNames,strInValueTypes,OutNames,strOutValueTypes,InputLinkStrs);//������������˿�
				pnewBlock->SetOrderID(OrderID);
				pnewBlock->ReadBlock(pCon,strModuleBlockPropTable);
				AddBlock(pnewBlock);
			}
		}
		pRecord->MoveNext();
	}
	pRecord->Close();

	////////////////////////////////////////////////////////////////////
	return bRet;
}
 //���ݴ˿�����֡����ͣ�Ȼ��ȥ��������ȡ����
//�������ֶ������;���������ϵ������������Ϊ����ֵ
CCalcBlock* CCalcModule::CreateBlock(CString strType,CString strName,CString strDesp)
{
	CCalcBlock* pBlock = NULL;
    //strType.MakeLower();
	pBlock = CCalcBlockControlFactory::CreateFromString(strType);
	if(pBlock)
	{
		pBlock->SetBlockName(strName);
		pBlock->SetBlockDesp(strDesp);
	}
	return pBlock;
}

BOOL CCalcModule::CheckTableExist(_ConnectionPtr &pCon)
{
	CString strModuleBlockListTable;//U1Perf_TagPoint_BlockList
	CString strModuleBlockPropTable;//���Եı�
    strModuleBlockListTable.Format("%s_%s_BlockList",*m_pStrParentModelName,GetModuleName());
	strModuleBlockPropTable.Format("%s_%s_BlockProp",*m_pStrParentModelName,GetModuleName());

	if(gIsTableExistInMDB(pCon,strModuleBlockListTable) && gIsTableExistInMDB(pCon,strModuleBlockListTable))
		return TRUE;
	else
		return FALSE;
}
CCalcBlock* CCalcModule::GetBlockByIndex(int k)
{
	CCalcBlock* pBlock=NULL;
	if((k>=0)&&(k<m_BlockList.GetCount()))
	{
		POSITION pos = m_BlockList.FindIndex(k);
		pBlock=(CCalcBlock* )m_BlockList.GetAt(pos);
	}
	return pBlock;
}
CCalcBlock* CCalcModule::FindBlockByName(CString str)
{
	CCalcBlock* pBlock=NULL;
	POSITION pos = m_BlockList.GetHeadPosition();
	int n=m_BlockList.GetCount();
	while(pos)
	{
		pBlock =  (CCalcBlock*)m_BlockList.GetNext(pos);
		CString s=pBlock->GetBlockName();
		if(pBlock->GetBlockName()==str)
		{
			break;
		}
		else
			pBlock=NULL;
	}
	return pBlock;
}
void CCalcModule::InitCompile(int *pModelWarningCount,int *pModelErrorCoutn,CWnd *pWnd,int iMsg,CLog *pLog)
{
	m_pModelWarningCount=pModelWarningCount;
	m_pModelErrorCoutn=pModelErrorCoutn;
	this->m_pWnd=pWnd;
	this->m_iMsg=iMsg;
	this->pLog=pLog;
	m_iModuleWarningCount=0;
	m_iModuleErrorCount=0;

	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos)
	{
		CCalcBlock* pBlock =  (CCalcBlock*)m_BlockList.GetNext(pos);
		pBlock->InitCompile(pModelWarningCount,pModelErrorCoutn,pWnd,iMsg,pLog,&m_iModuleWarningCount,&m_iModuleErrorCount);
	}
}

void CCalcModule::RunCompile()
{
	//�������������Ϣ
	CString str;
	str.Format("---------- ģ�飺%s ----------",this->m_Name);
	m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
	str.Format("��ʼ����...");
	m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
	//
	bool beHave=false;
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos)
	{
		CCalcBlock* pBlock =  (CCalcBlock*)m_BlockList.GetNext(pos);
		pBlock->RunCompile();
		beHave=true;
	}
	if(!beHave)
	{
		str.Format("���棺��ģ���޼�������.");
		m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
		m_iModuleWarningCount++;
		(*m_pModelWarningCount)++;
	}
	str.Format("ģ�飺'%s'������ɣ�",this->m_Name);
	m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
	str.Format("");
	m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
	str.Format("ģ�飺'%s' -- ���������%d�����������%d",this->m_Name,m_iModuleErrorCount,m_iModuleWarningCount);
	m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
}
void CCalcModule::SetDebugOnOrOff(int iOnOrOff)
{
	m_iLogOnOrOff=iOnOrOff;//������־����
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos)
	{
		CCalcBlock* pBlock =  (CCalcBlock*)m_BlockList.GetNext(pos);
		pBlock->SetDebugOnOrOff(iOnOrOff);
	}
}
void CCalcModule::SetTraceOnOrOff(int iOnOrOff)
{
	m_iTraceOn=iOnOrOff;//������־����
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos)
	{
		CCalcBlock* pBlock =  (CCalcBlock*)m_BlockList.GetNext(pos);
		pBlock->SetTraceOnOrOff(iOnOrOff);
	}
}
void CCalcModule::SetWriteLogOnOrOff(int iOnOrOff)
{
	m_iLogOnOrOff=iOnOrOff;//������־����
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos)
	{
		CCalcBlock* pBlock =  (CCalcBlock*)m_BlockList.GetNext(pos);
		pBlock->SetWriteLogOnOrOff(iOnOrOff);
	}
}

void CCalcModule::GetBlockInputValueFromGlobalWS()
{
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos)
	{
		CCalcBlock* pBlock =  (CCalcBlock*)m_BlockList.GetNext(pos);
		pBlock->GetInputValueFromGlobalWS();
	}
}
