// CalcBlock.cpp: implementation of the CCalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "CalcBlock.h"
#include "CalcModel.h"

#include "FlowchartEditor/DiagramEditor/Tokenizer.h"
#include "FlowchartEditor/FlowchartEditor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CMap<CString,LPCTSTR,CCalcPort*,CCalcPort*&> g_WorkSpace;
extern CSocketServ* g_ServSocketInit(const char* IP,int port);
extern void g_ServSocketExit(CSocketServ*& pSockServ);

CCalcBlock::CCalcBlock()
{
	SetBlockType(_T("basic"));
	m_pModel = NULL;
	m_iOrderID = -1;
	m_bCompared = FALSE;
	m_bInputIntegrity = FALSE;
	pSockServ=NULL;
	m_iLogOnOrOff=0;
	m_iTraceOn=1;//׷�ٿ���1:����0:��
	m_iDebugOn=1;//���Կ���1:����0:��

	m_pModel=NULL;
	m_pModule=NULL;

	m_bShowDesp=TRUE;//�Ƿ���ʾDESP
	m_bShowInPortText=TRUE; //�Ƿ���ʾ����˿ڵ���������
	m_bShowOutPortText=TRUE; //�Ƿ���ʾ����˿ڵ���������
	m_pEditorObj=NULL;//
}

CCalcBlock::~CCalcBlock()
{
    ClearAllPorts();
}
void CCalcBlock::SetFlowchartEditorPObj(CFlowchartEditor *pEditorObj)
{
	this->m_pEditorObj=pEditorObj;
}
CFlowchartEditor *CCalcBlock::GetFlowchartEditorPObj()
{
	return m_pEditorObj;
}
void CCalcBlock::Clear()
{
	//����˿������copyʱ�ٽ������кͶ���һ���Ķ˿�
	ClearAllPorts();

}
void CCalcBlock::SetBlockPropertyToTransferData(
				CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)//�����Է��ô���������
{
	
}
//��շ��͵�����Arr
void CCalcBlock::ClearPropertyTransferData(
							   CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
	CBlockPropertyTransferData *pData;
	for(int i=0;i<dataArr.GetSize();i++)
	{
		pData=dataArr[i];
		if(pData!=NULL)
		{
			delete pData;
			pData=NULL;
		}
	}
	dataArr.RemoveAll();
}
void CCalcBlock::SetTransferDataToBlockProperty(
				const CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)//���������ݷ��õ�ģ����
{
	CBlockPropertyTransferData *pData;
	for(int i=0;i<dataArr.GetSize();i++)
	{
		pData=dataArr[i];
		CString strPropName=pData->m_cPropName;
		CString strItem1=pData->m_cValueItem1;
		CString strItem2=pData->m_cValueItem2;
		CString strItem3=pData->m_cValueItem3;
		CString strItem4=pData->m_cValueItem4;
		CString strItem5=pData->m_cValueItem5;
		SetPropValue(strPropName,strItem1,strItem2,strItem3,strItem4,strItem5);
	}
}
BOOL CCalcBlock::sendOnePackage(char *pChar,int iLength,CSocketServ* pSocket)
{
	int nErrorLen = -1;
	do
	{
		nErrorLen = pSocket->Send(pChar,iLength);
		if(nErrorLen <= 0)
		{
			if(10035 != GetLastError())//10035���������Ѿ����ˣ��ӳٷ�������
			{
				return FALSE;
			}
			Sleep(10);
		}
	}
	while(nErrorLen <= 0);
	return TRUE;
}
BOOL CCalcBlock::sendCharArr(CArray<char,char> &bufArr,CSocketServ* pSocket)
{
	BOOL ret=TRUE;
	int iNum=bufArr.GetSize()/1024;//��Ҫ���������Ĵ���
	int iR=bufArr.GetSize()%1024;

	if(iNum>0)//������
	{
		char *bufEntireSend=new char[1024];
		for(int i=0;i<iNum;i++)//ѭ��������
		{
			memset(bufEntireSend,0,1024);
			for(int j=i*1024;j<(i+1)*1024;j++)
			{
				bufEntireSend[j-i*1024]=bufArr[j];
			}
			ret=sendOnePackage(bufEntireSend,1024,pSocket);
			if(FALSE==ret)//����ʧ������ѭ������
			{
				break;
			}
		}
		if(bufEntireSend!=NULL)
		{
			delete []bufEntireSend;
			bufEntireSend=NULL;
		}
		if(FALSE==ret)//��һ�η���ʧ�����˳�����
		{
			return ret;
		}
	}
	if(iR>0)//�����µİ�������
	{
		char *bufLaseSend=new char[iR];
		memset(bufLaseSend,0,iR);
		for(int i=iNum*1024;i<bufArr.GetSize();i++)
		{
			bufLaseSend[i-iNum*1024]=bufArr[i];
		}
		ret=sendOnePackage(bufLaseSend,iR,pSocket);
		if(bufLaseSend)
		{
			delete []bufLaseSend;
			bufLaseSend=NULL;
		}
	}
	return ret;
}
void CCalcBlock::putPCharToCharArr(char *pChar,int count,CArray<char,char> &bufArr)
{
	for(int i=0;i<count;i++)
	{
		bufArr.Add(*(pChar+i));
	}
}
void CCalcBlock::putPropertyDataArrToCharArr(
				 const CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr,
				 CArray<char,char> &bufArr)
{
	//����ͷ
	char bufHead[6];memset(bufHead,0,6);
	bufHead[0]='*';
	*(int*)(bufHead+1) = dataArr.GetSize();//1~4  Ԫ�صĳ���
	bufHead[5]='C';//Э���
	putPCharToCharArr(bufHead,6,bufArr);
	
	for(int i=0;i<dataArr.GetSize();i++)
	{
		CBlockPropertyTransferData *pData=dataArr[i];
		if(pData==NULL)
			continue;
		int size=4+sizeof(*pData);
		char *bufData=new char[size];
		
		*(int*)(bufData) = sizeof(*pData);//���ó���
		memcpy(bufData+4,pData,sizeof(*pData));//��������
		putPCharToCharArr(bufData,size,bufArr);
		if(bufData!=NULL)
		{
			delete bufData;
			bufData=NULL;
		}
	}
	char bufTail='$';//����β��
	bufArr.Add(bufTail);
}
void CCalcBlock::SendBlockPropertyTransferData(
				const CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)//��������
{
	if(!pSockServ)
		g_ServSocketExit(pSockServ);
	pSockServ = g_ServSocketInit(m_pModel->m_strModelIP,atoi(m_pModel->m_strPort));
	if(!pSockServ)
	{
		AfxMessageBox("���Ӹü���ģ��ʧ�ܣ�����ģ��δ������������ع���!");
		return;
	}
	CArray<char,char> bufArr;
	putPropertyDataArrToCharArr(dataArr,bufArr);
	sendCharArr(bufArr,pSockServ);
	Sleep(2000);
	g_ServSocketExit(pSockServ);
/*
	char lbuf[1024];	
	memset(lbuf,0,1024);
	
	lbuf[0] = '*';	//��ȫ�������
	lbuf[5] = 'C';	//Э���
	
	//���濪ʼ����dataArr�е�Ԫ��
	int flag = 6;
	*(int*)(lbuf+1) = dataArr.GetSize();
	for(int i=0;i<dataArr.GetSize();i++)
	{
		CBlockPropertyTransferData *pData=dataArr[i];
		*(int*)(lbuf+flag) = sizeof(*pData);
		flag += 4;
		
		memcpy(lbuf+flag,pData,sizeof(*pData));
		flag += sizeof(*pData);
		
		if(flag+sizeof(CBlockPropertyTransferData)+4 > 1024)
		{
			int nErrorLen = -1;
			int nErrorNo = 0;
			do
			{
				nErrorLen = pSockServ->Send(lbuf,flag);
				if(nErrorLen <= 0)
				{
					nErrorNo = GetLastError();
					if(10035 != nErrorNo)//10035���������Ѿ����ˣ��ӳٷ�������
					{
						return;
					}
					Sleep(10);
				}
			}
			while(nErrorLen <= 0);
			flag = 0;
		}
	}
	if(flag != 0)
	{
		int nErrorLen = -1;
		int nErrorNo = 0;
		do
		{
			nErrorLen = pSockServ->Send(lbuf,flag);
			if(nErrorLen <= 0)
			{
				nErrorNo = GetLastError();
				if(10035 != nErrorNo)//10035���������Ѿ����ˣ��ӳٷ�������
				{
					return;
				}
				Sleep(10);
			}
		}
		while(nErrorLen <= 0);	
		flag = 0;
	}
	int ret = pSockServ->Receive(lbuf,1024);
	if(ret > 0)
	{
		if(lbuf[0] != '*')
		{
			return;
		}
		if(lbuf[5] != 'C')
		{
			return;
		}
	}
	
	g_ServSocketExit(pSockServ);
	*/
	
}
void  CCalcBlock::ChangeInPortName(CString strOldPortName,CString strNewPortName)//ֻ�޸�����˿�����
{
	int inNum = GetInputNum();
	for(int k=0;k<inNum;k++)
	{
		CCalcPort *pPortObj = GetInputPortObj(k);
		if(pPortObj->GetPortID()==strOldPortName)
		{
			pPortObj->SetPortID(strNewPortName);
			break;
		}
	}
}
void  CCalcBlock::ChangeOutPortName(CString strOldPortName,CString strNewPortName)//�޸�����˿����ƣ����޸���֮���������ӽ�
{
	if(m_pEditorObj==NULL)
		return;
	int outNum = GetOutputNum();
	for(int k=0;k<outNum;k++)
	{
		CCalcPort *pPortObj = GetOutputPortObj(k);
		if(pPortObj->GetPortID()==strOldPortName)
		{
			pPortObj->SetPortID(strNewPortName);
			break;
		}
	}
	CFlowchartEditor *pEditor=m_pEditorObj;
	int max = pEditor->GetObjectCount();
	for(int i=0;i<max;i++)
	{
		CFlowchartEntity* obj =  (CFlowchartEntity*) (pEditor->GetObject( i ));
		if(obj->m_pBlockInner!=NULL)
		{
			CString strThisBlockName=GetBlockName();
			if(strThisBlockName!=obj->m_pBlockInner->GetBlockName())//���������
			{
				int InNum = obj->GetInputNum();
				for(int k=0;k<InNum;k++)
				{
					//���Ƿ�������
					BOOL bLink = obj->GetBlockPortLinked(k);
					if(bLink)
					{
						//�����ĸ��顢�ڼ������������BlockName,portName,
						CString strFromBlockName = obj->GetPortLinkBlockName(k);
						CString strFromBlockPortName = obj->GetPortLinkBlockPortName(k);
						if((strFromBlockName==strThisBlockName)&&(strFromBlockPortName==strOldPortName))
						{
							obj->SetBlockPortLink(k,strThisBlockName,strNewPortName); //����
						}
					}
				}
			}
		}
	}
}		
void  CCalcBlock::Copy( CCalcBlock* pBlock)
{
	this->m_pModel=pBlock->m_pModel;
	this->m_pModule=pBlock->m_pModule;
	this->m_pEditorObj=pBlock->m_pEditorObj;
	Clear();
	///////////////////////////////////////////////////////
	 SetBlockName(pBlock->GetBlockName());
	 SetBlockDesp(pBlock->GetBlockDesp());
	 SetBlockType(pBlock->GetBlockType());
	 ////////////////////////////////////////////////////
	 int inNum =pBlock->GetInputNum();
	 for(int k=0;k<inNum;k++)
	 {
		 CCalcPort* pPort = pBlock->GetInputPortObj(k);
		AddInputPort(pPort->GetPortID(),pPort->GetPortValueType(),pPort->GetPortDesp(),pPort->GetPortUnit());
		//
	 }
	 int OutNum = pBlock->GetOutputNum();
	  for( k=0;k<OutNum;k++)
	 {
		 CCalcPort* pPort = pBlock->GetOutputPortObj(k);
		AddOutputPort(pPort->GetPortID(),pPort->GetPortValueType(),pPort->GetPortDesp(),pPort->GetPortUnit());
	 }
	 CopyPropertyData(pBlock);
}
void CCalcBlock::CopyPropertyData(CCalcBlock* pBlock)//����ͼ��ʱ����Ҫ�������ԡ�
{
	
}
//��̬���������ɶ�����
 CCalcBlock* CCalcBlock::CreateFromString( const CString& strType )
 {
	CCalcBlock* obj = new CCalcBlock;
	if(strType !=obj->GetBlockType())
	{
			delete obj;
			obj = NULL;
	}
	return obj;
 }
 //�����˿�
 void CCalcBlock::CreatePortsFromString(int InNum,int OutNum,CString InNames,CString strInValueTypes,CString OutNames,CString strOutValueTypes, CString InputLinkStrs)
{
	ClearAllPorts();
	//�������
	CTokenizer tok( InNames ); 
	CTokenizer tokLink( InputLinkStrs ); 
	CTokenizer tokType( strInValueTypes ); 

	int size = tok.GetSize(); //����ĸ���
	int sizeLink = tokLink.GetSize();//���������ַ����ĸ���
	int sizeType = tokType.GetSize();
	//Ҫ��û������ʱ��Ҳ��Ҫ��Ӧ��
	if(InNum== size && size == sizeLink && size==sizeType)
	{
		   for(int count=0;count<size;count++)
			{
				CString strPortName;
				CString strLinkStr;
				CString strPortValueType;
				CString strFromBlock=_T("");
				CString strFromBlockPort=_T("");
				
				tok.GetAt( count, strPortName );
				tokLink.GetAt( count, strLinkStr );
				tokType.GetAt(count,strPortValueType);

                 int type = atoi(strPortValueType);
				 if(!(type>=0 && type<6)  ) type =VALUE_DOUBLE;
				CCalcPort* pPort = AddInputPort(strPortName,type,strPortName);

				CTokenizer blockportTok( strLinkStr ,_T(".")); 
				if(blockportTok.GetSize()==2)
				{
					blockportTok.GetAt(0,strFromBlock);
					blockportTok.GetAt(1,strFromBlockPort);
					strFromBlock.TrimLeft();strFromBlock.TrimRight();
                    strFromBlockPort.TrimLeft();strFromBlockPort.TrimRight();
					if(strFromBlock.IsEmpty() == FALSE && strFromBlockPort.IsEmpty()==FALSE )
					{
						pPort->SetLinkFormBlockName(strFromBlock);
					    pPort->SetLinkFromBlockPortName(strFromBlockPort);
						pPort->SetPortLinked(TRUE);
					}
					else
					{
						pPort->SetLinkFormBlockName(_T(""));
					    pPort->SetLinkFromBlockPortName(_T(""));
						pPort->SetPortLinked(FALSE);

					}
				}
				else
				{
						pPort->SetLinkFormBlockName(_T(""));
					    pPort->SetLinkFromBlockPortName(_T(""));
						pPort->SetPortLinked(FALSE);
			
				}
				
			}
	}
	//��������///////////////////////////////////////////////
	tok.Init(OutNames);
	tokType.Init(strOutValueTypes ); 
	size = tok.GetSize();
	sizeType = tokType.GetSize();
	if(size == OutNum && size == sizeType)
	{
			for(int count=0;count<size;count++)
			{
				CString strPortName;
				CString strPortValueType;

				tok.GetAt( count, strPortName );
				tokType.GetAt(count,strPortValueType);

                 int type = atoi(strPortValueType);
				 if(!(type>=0 && type<6)  ) type =VALUE_DOUBLE;

				CCalcPort* pPort = AddOutputPort(strPortName,type,strPortName);
			}
	}
}
//initial the block for calc,it must be called before 
void CCalcBlock::InitCalc()
{
	//������˿ڶ����ӳ�䵽ȫ����WS
	MapOutputToGlobalWS();
}
//Ĭ�ϸü����ļ���
void CCalcBlock::Calc()
{
}
//�������ԵĶ����ַ���
CString CCalcBlock::GetBlockStrPro()
{
	return "";
}
void CCalcBlock::SetBlockStrPro(CString str)//���ü������ַ�������
{

}
//��������ͼ��
void CCalcBlock::DrawShape(CDC* dc, CRect rect,int CutSize)
{
	CRect rc(rect);
	rc.top = rect.top + 2*CutSize-1;
	rc.bottom = rect.bottom - 2*CutSize+1;
	dc->Rectangle(rc);
}
void CCalcBlock::SetInputIntegrity(BOOL  var)
{
		m_bInputIntegrity = var;
}
BOOL  CCalcBlock::GetInputIntegrity()
{
	return m_bInputIntegrity;
}
//��ģ�����������
BOOL CCalcBlock::CheckBlockCalcIntegrity()
{
	BOOL bRet = TRUE;
	POSITION pos = NULL;
	pos = m_listIn.GetHeadPosition();
	CString strLog;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while(pos)
    {
		CCalcPort *pPortObj = (CCalcPort *)m_listIn.GetNext(pos);

		if(pPortObj->GetLinkFormBlockName().IsEmpty()  || pPortObj->GetLinkFromBlockPortName().IsEmpty())
		{
			bRet = FALSE;
			//��¼����,��¼��ʲô�ط���һ���ļ���
			break;
		}
    }
	return bRet;
}
void CCalcBlock::SetOrderID(int id)
{
	m_iOrderID = id;
}
int CCalcBlock::GetOrderID()
{
	return m_iOrderID;
}
//��ԭ�еĻ��������һ������˿�
void CCalcBlock::AddInputNode(CCalcPort* pPort)
{
	pPort->SetBlockPtr(this);
	m_listIn.AddTail(pPort);
}

void CCalcBlock::AddOutputNode(CCalcPort* pPort)
{
	pPort->SetBlockPtr(this);
	m_listOut.AddTail(pPort);
}
//////////////////////////////////////////////////////////
void CCalcBlock::ClearOutPortLink(CCalcPort* pParPort,CWnd* parent)
{
	if(parent==NULL)
		return;
	CFlowchartEditor *pEditor=(CFlowchartEditor *)parent;
	int max = pEditor->GetObjectCount();
	for(int i=0;i<max;i++)
	{
		CFlowchartEntity* obj =  (CFlowchartEntity*) (pEditor->GetObject( i ));
		if(obj->m_pBlockInner!=NULL)
		{
			CString strThisBlockName=GetBlockName();
			if(strThisBlockName==obj->m_pBlockInner->GetBlockName())
				continue;
			CCalcPort* pPort=NULL;
			for(int j=0;j<obj->m_pBlockInner->GetInputNum();j++)
			{
				pPort=obj->m_pBlockInner->GetInputPortObj(j);

				CString str1=pParPort->GetLinkFormBlockName();
				CString str2=pPort->GetLinkFormBlockName();
				CString str3=pParPort->GetLinkFromBlockPortName();
				CString str4=pPort->GetLinkFromBlockPortName();

				if((strThisBlockName==pPort->GetLinkFormBlockName())&&
					(pParPort->GetPortID()==pPort->GetLinkFromBlockPortName()))
				{
					pPort->SetPortLinked(FALSE);
					pPort->SetLinkFormBlockName("");
					pPort->SetLinkFromBlockPortName("");
				}
			}
		}
	}
}
void CCalcBlock::ClearInputPortByIndex(int Index)
{
	POSITION pos=m_listIn.FindIndex(Index);
	if(pos)
	{
		CCalcPort* pPort=(CCalcPort* )m_listIn.GetAt(pos);
		if(pPort!=NULL)
		{
			delete pPort;
			pPort=NULL;
		}
	    m_listIn.RemoveAt(pos);
	}
}

void CCalcBlock::ClearOutputPortByIndex(int Index,CWnd* parent)
{
	POSITION pos=m_listOut.FindIndex(Index);
	if(pos)
	{
		CCalcPort* pPort=(CCalcPort*)m_listOut.GetAt(pos);
		if(pPort!=NULL)
		{
			ClearOutPortLink(pPort,parent);//�����ö˿�������Ķ˿�����
			try
			{
				delete pPort;
				pPort=NULL;
			}
			catch(...)
			{
				
			}
		}
	    m_listOut.RemoveAt(pos);
	}
}
void CCalcBlock::ClearInputPorts()
{
	POSITION pos = NULL;
	pos = m_listIn.GetHeadPosition();
	CString strType;
	strType.Format("%s",m_sType);
	if(strType!="LinkTo")
	{
		while(pos) {delete  m_listIn.GetNext(pos);}
	}
	m_listIn.RemoveAll();
}
void CCalcBlock::ClearOutputPorts(CWnd* parent)
{
	POSITION pos = NULL;
	pos = m_listOut.GetHeadPosition();
	CString strType;
	strType.Format("%s",m_sType);
	
	while(pos) 
	{
		CCalcPort* pPort=(CCalcPort*)m_listOut.GetNext(pos);
		if(pPort!=NULL)
		{
			ClearOutPortLink(pPort,parent);//�����ö˿�������Ķ˿�����
			if(strType!="LinkFrom")//"LinkFrom"���͵ļ���飬����Ҫִ��delete �ķ�ʽɾ���˿�
			{
				try
				{
					delete  pPort;
					pPort=NULL;
				}
				catch(...)
				{
					
				}
			}
		}
	}
	m_listOut.RemoveAll();
}

//������е���������˿�
void CCalcBlock::ClearAllPorts(CWnd* parent)
{
	ClearInputPorts();
	ClearOutputPorts(parent);
}
int CCalcBlock::GetInputNum()
{
	return m_listIn.GetCount();
}

int CCalcBlock::GetOutputNum()
{
	return m_listOut.GetCount();
}
CString CCalcBlock::GetOutputName(int index)
{
	CString str=_T("");
	CCalcPort* pPort = GetOutputPortObj(index);
	if(pPort!=NULL)
	{
		str = pPort->m_portID; 
	}
	return str;
}

CString CCalcBlock::GetOutputDesp(int index)
{
	CString str=_T("");
	CCalcPort* pPort = GetOutputPortObj(index);
	if(pPort!=NULL)
	{
		str = pPort->m_sDsp; 
	}
	return str;
}
void CCalcBlock::SetOutputName(int index,CString str)
{
	CCalcPort* pPort = GetOutputPortObj(index);
	if(pPort!=NULL)
	{
		pPort->SetPortID(str);
	}
}
void CCalcBlock::SetOutputDesp(int index,CString str)
{
	CCalcPort* pPort = GetOutputPortObj(index);
	if(pPort!=NULL)
	{
		pPort->SetPortDesp(str);
	}
}

BOOL CCalcBlock::GetOutputValue(int index,CUniValue* UniValue )
{
	CCalcPort* pPort = GetOutputPortObj(index);
	
	if(pPort!=NULL)
	{
		*UniValue = pPort->GetPortUniValue();
		return 	TRUE;

	}
	else
		return FALSE;
}
BOOL CCalcBlock::GetOutputValue(CString strName,CUniValue* UniValue )
{
	CCalcPort* pPort = GetOutputPortObj(strName);
	if(pPort!=NULL)
	{
		*UniValue = pPort->GetPortUniValue();
		return 	TRUE;

	}
	else
		return FALSE;
}

BOOL CCalcBlock::GetInputValue(CString strName,CUniValue* UniValue )
{
	CCalcPort* pPort = GetInputPortObj(strName);
	if(pPort!=NULL)
	{
		*UniValue = pPort->GetPortUniValue();
		return 	TRUE;

	}
	else
		return FALSE;
}
BOOL CCalcBlock::GetInputValue(int index,CUniValue* UniValue)
{
	CCalcPort* pPort = GetInputPortObj(index);
	if(pPort!=NULL)
	{
		*UniValue = pPort->GetPortUniValue();
		return 	TRUE;

	}
	else
		return FALSE;
}


// read block and made the input,output,and expressions
//
BOOL CCalcBlock::ReadBlock(_ConnectionPtr &pCon,CString strPropTable)
{
	BOOL bRet = TRUE;
	////////////////////////////////////////////////////////////////////
	//���Ȳ�ѯ���Ա������ڸÿ����ֵ������У��Ӷ��õ�����ֵ
	//��ģ�����������е����������û����ʽ���õ�Ҫ��Ĭ��ֵ
	////////////////////////////////////////////////////////////////////
  	_RecordsetPtr  pRecord;
	_variant_t     Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
    pRecord->CursorLocation = adUseClient;
    //��������������
    CString strSQL;
	CString strField = "block_name";
	CString strKey = GetBlockName();
	strSQL.Format("SELECT * FROM %s WHERE %s = '%s'",strPropTable,strField,strKey);
    try
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		pRecord->Close() ;
		return FALSE;
	}	
	if(!pRecord->GetBOF())  pRecord->MoveFirst();
	//�����ȡ�������ԣ�ÿ�����Զ�Ӧ�����б�һ������Ҫ�����ݼ��л�þ����ֵ
	//�����ж��������֣�������������������ֵ��
	while(!pRecord->adoEOF)
	{
		CString strPropName; //��
		CString strItem1;//
		CString strItem2;//
		CString strItem3;//
		CString strItem4;//
		CString strItem5;//

		BOOL bThisOk = TRUE;
		try{Tempvalue = pRecord->GetCollect("prop_name");
			if(Tempvalue.vt != VT_NULL) strPropName = (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e) {AfxMessageBox(e.ErrorMessage());}

		try{Tempvalue = pRecord->GetCollect("value_item1");
		    if(Tempvalue.vt != VT_NULL) strItem1 = (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e) {AfxMessageBox(e.ErrorMessage());}

		try{Tempvalue = pRecord->GetCollect("value_item2");
			if(Tempvalue.vt != VT_NULL) strItem2 = (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e) {AfxMessageBox(e.ErrorMessage());}

		try{Tempvalue = pRecord->GetCollect("value_item3");
			if(Tempvalue.vt != VT_NULL) strItem3= (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e) {AfxMessageBox(e.ErrorMessage());}

		try{Tempvalue = pRecord->GetCollect("value_item4");
			if(Tempvalue.vt != VT_NULL) strItem4 = (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e) {AfxMessageBox(e.ErrorMessage());}

		try{Tempvalue = pRecord->GetCollect("value_item5");
			if(Tempvalue.vt != VT_NULL) strItem5 = (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e) {AfxMessageBox(e.ErrorMessage());}
	
		if(bThisOk)
		{
		    //����ȡû���������ֵ��������
			SetPropValue(strPropName,strItem1,strItem2,strItem3,strItem4,strItem5);
		}
		pRecord->MoveNext();
	}
	pRecord->Close();

	////////////////////////////////////////////////////////////////////
	return bRet;
}

CUniValue  CCalcBlock::GetInputValueFromGlobalWS_direct(CString strName)
{
	    CUniValue var;
       //�������Ķ˿ڶ���
		CCalcPort *pPortObj = GetInputPortObj(strName);
		//�ж϶˿��Ƿ�������
		if(pPortObj->GetPortLinked())
		{
			//�õ����ӵ��ڿռ��еı�������
			CString strVarName;
			strVarName.Format("%s.%s.%s",m_pModule->GetModuleName(),pPortObj->GetLinkFormBlockName(),pPortObj->GetLinkFromBlockPortName());
			//��ÿռ��еı���
			CCalcPort *pGWSObj = (CCalcPort*)g_WorkSpace[strVarName];
			if(pGWSObj !=NULL)
			{
				//�õ��ռ������
				return  pGWSObj->GetPortUniValue();
			}
		}
		return var;
}
BOOL CCalcBlock::IsLinkVarExistInGlobalWS(CString strPortName)
{
	CCalcPort *pPortObj = GetInputPortObj(strPortName);
	if(pPortObj ==NULL) return FALSE;
	//�ж϶˿��Ƿ�������
	if(pPortObj->GetPortLinked()) 
	{
		//�õ����ӵ��ڿռ��еı�������
		CString strVarName1;
		strVarName1.Format("%s.%s.%s",m_pModule->GetModuleName(),pPortObj->GetLinkFormBlockName(),pPortObj->GetLinkFromBlockPortName());
		CCalcPort *pGWSObj = (CCalcPort*)g_WorkSpace[strVarName1];
  	    if(pGWSObj !=NULL) return TRUE;
	    else return FALSE;
	}
	else
		return FALSE;

}
void CCalcBlock::GetInputValueFromGlobalWS()
{
	CString strVarName;
	int inNum = GetInputNum();
	for(int k=0;k<inNum;k++)
	{
		   //�������Ķ˿ڶ���
			CCalcPort *pPortObj = GetInputPortObj(k);
			//�ж϶˿��Ƿ�������
			if(pPortObj->GetPortLinked()) 
			{
				//�õ����ӵ��ڿռ��еı�������
				strVarName.Format("%s.%s.%s",m_pModule->GetModuleName(),pPortObj->GetLinkFormBlockName(),pPortObj->GetLinkFromBlockPortName());
				//��ÿռ��еı���
				CCalcPort *pGWSObj = (CCalcPort*)g_WorkSpace[strVarName];
				if(pGWSObj !=NULL)
				{
					//�õ��ռ������
					CUniValue UniVar = pGWSObj->GetPortUniValue();
					//��������ĸö˿ڵ����ݽ��и���
					pPortObj->SetPortUniValue(UniVar);
				}
			}
	}
}
void CCalcBlock::OutputResultToGlobalWS()
{
	POSITION pos = NULL;
	pos = m_listOut.GetHeadPosition();
    while(pos)
    {
		CCalcPort *pPortObj = (CCalcPort *)m_listOut.GetNext(pos);
		if(pPortObj->m_bOut2WS)  pPortObj->PutValueToGlobalWS();
    }
}

void CCalcBlock::MapOutputToGlobalWS()
{
	POSITION pos = m_listOut.GetHeadPosition();
    while(pos)
    {
		CCalcPort *pPortObj = (CCalcPort *)m_listOut.GetNext(pos);
		CString strLinkID;
		strLinkID.Format("%s.%s.%s",this->m_pModule->GetModuleName(),GetBlockName(),pPortObj->m_portID);//jch do
		pPortObj->SetPortLinkID(strLinkID); 
		pPortObj->MapPortIntoGlobalWS();
    }

}
CCalcPort* CCalcBlock::GetOutputPortObj(int index)
{
	CCalcPort *pPortObj =NULL;
	if((index>=0)&&(index<m_listOut.GetCount()))
	{
		POSITION pos = m_listOut.FindIndex(index);
		if(pos)  pPortObj =  (CCalcPort *)m_listOut.GetAt(pos);
	}
	return pPortObj;
}

CCalcPort* CCalcBlock::GetInputPortObj(int index)
{
	CCalcPort *pPortObj =NULL;
	if((index>=0)&&(index<m_listIn.GetCount()))
	{
		POSITION pos = m_listIn.FindIndex(index);
		if(pos)  pPortObj =  (CCalcPort *)m_listIn.GetAt(pos);
	}
	return pPortObj;
}
//�������ֻ�ö˿�
CCalcPort* CCalcBlock::GetInputPortObj(CString strName)
{
    CCalcPort *pPortObj =NULL;
	POSITION pos = m_listIn.GetHeadPosition();
    while(pos)
    {
		pPortObj = (CCalcPort *)m_listIn.GetNext(pos);
		if(pPortObj->GetPortID().Compare(strName)==0)
		{
			break;
		}
		else
			pPortObj=NULL;
	}
	return pPortObj;
}
CCalcPort* CCalcBlock::GetOutputPortObj(CString strName)
{
	CCalcPort *pPortObj =NULL;
	POSITION pos = m_listOut.GetHeadPosition();
    while(pos)
    {
		pPortObj = (CCalcPort *)m_listOut.GetNext(pos);
		if(pPortObj->GetPortID().Compare(strName)==0)
		{
			break;
		}
		else
			pPortObj=NULL;
	}
	return pPortObj;
}

void CCalcBlock::SetInputSourceID(int index, CString strID)
{
	CCalcPort* pPort = GetInputPortObj(index);
	if(pPort!=NULL) strcpy(pPort->m_sLinkID,strID);
}

void CCalcBlock::ForceOutputPortValue(int index,double value)
{
	CCalcPort* pPort = GetOutputPortObj(index);
	if(pPort!=NULL)
	{
		pPort->m_bForceValue = TRUE;
		pPort->m_Value_Force = value;
	}
}
void CCalcBlock::SetForceValueState(int index,BOOL bEnable)
{
	CCalcPort* pPort = GetOutputPortObj(index);
	if(pPort!=NULL)
	{
		pPort->m_bForceValue = bEnable;
	}
}


//���һ������˿ڣ�ָ�����ͺ��ڲ��ţ����ڽ��й���
CCalcPort* CCalcBlock::AddInputPort(CString strPortID, int ValueType, CString strDesp, CString strUnit)
{
	CCalcPort* pPort = new CCalcPort();
	pPort->SetPortAsInOrOut(0);//���
	pPort->SetPortID(strPortID);//�ڲ��ı������� 
	pPort->SetPortDesp(strDesp) ;
	pPort->SetPortUnit(strUnit) ;
	pPort->SetPortIsOut2WS(0); //����Ƿ������ȫ�ֹ����ռ��� 
	pPort->SetPortValueType(ValueType);
	pPort->SetBlockPtr(this);
	m_listIn.AddTail(pPort);
	return pPort;
}

CCalcPort* CCalcBlock::AddOutputPort(CString strPortID, int ValueType, CString strDesp, CString strUnit)
{
	CCalcPort* pPort = new CCalcPort();
	pPort->SetPortAsInOrOut(1);//���
	pPort->SetPortID(strPortID);//�ڲ��ı������� 
	pPort->SetPortDesp(strDesp) ;
	pPort->SetPortUnit(strUnit) ;
	pPort->SetPortIsOut2WS(1); //����Ƿ������ȫ�ֹ����ռ��� 
	pPort->SetPortValueType(ValueType);
	pPort->SetBlockPtr(this);
	CString strLinkID;
	strLinkID.Format("%s.%s.%s",this->m_pModule->GetModuleName(),GetBlockName(),strPortID);
	pPort->SetPortLinkID(strLinkID); 
	m_listOut.AddTail(pPort);
	return pPort;
}

CString CCalcBlock::GetOutLinkID(int index)
{
	//����ĸ�ʽΪ��Module.BlockName.OutportID
	CString str=_T("");
	CCalcPort* pPort = GetOutputPortObj(index);
	if(pPort!=NULL)
	{
		CString strLinkID;
		strLinkID.Format("%s.%s.%s",m_pModule->GetModuleName(),GetBlockName(),pPort->m_portID);
		str = strLinkID;
	}
	return str;
}

void CCalcBlock::ShowBlockInfoInListCtrl(CListCtrl *pListCtrl)
{
	pListCtrl->DeleteAllItems();
	//	//////////////////////////////////////////
	CString strItem;
	int nItem=0;
	POSITION pos = m_listIn.GetHeadPosition();
    while(pos)
    {
		CCalcPort *pPortObj = (CCalcPort *)m_listIn.GetNext(pos);
		strItem.Format("Input%d:%s",nItem+1,pPortObj->GetPortID());
		pListCtrl->InsertItem(nItem,strItem);
		double value = pPortObj->GetPortUniValue().GetValueDouble();
		strItem.Format("%f",value);
		pListCtrl->SetItemText(nItem,1,strItem);
		pListCtrl->SetItemText(nItem,2,GetBlockName());
		nItem++;
	}

	pos = m_listOut.GetHeadPosition();
	int nId=0;
    while(pos)
    {
		CCalcPort *pPortObj = (CCalcPort *)m_listOut.GetNext(pos);
		strItem.Format("Output%d:%s",++nId,pPortObj->GetPortID());
		pListCtrl->InsertItem(nItem,strItem);
	   double value = pPortObj->GetPortUniValue().GetValueDouble();
		strItem.Format("%f",value);

		pListCtrl->SetItemText(nItem,1,strItem);
		pListCtrl->SetItemText(nItem,2,GetBlockName());
		nItem++;
	}
}
void CCalcBlock::InitCompile(int *pModelWarningCount,int *pModelErrorCount,CWnd *pWnd,int iMsg,CLog *pLog,
							 int *pModuleWarningCount,int *pModuleErrorCount)
{
	m_pModelWarningCount=pModelWarningCount;
	m_pModelErrorCoutn=pModelErrorCount;
	this->m_pWnd=pWnd;
	this->m_iMsg=iMsg;
	this->m_pLog=pLog;

	m_pModuleWarningCount=pModuleWarningCount;
	m_pModuleErrorCount=pModuleErrorCount;
}

void CCalcBlock::RunCompile()
{
	BOOL bOK = CheckBlockCalcIntegrity();
	if(!bOK)
	{
		CString str;
		str.Format("���棺����δ����'�����:%s��������:%s'",GetBlockName(),GetBlockDesp());
		m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
		(*m_pModuleWarningCount)++;
		(*m_pModelWarningCount)++;
	}

}
void CCalcBlock::SetDebugOnOrOff(int iOnOrOff)
{
	m_iDebugOn=iOnOrOff;//���õ��Կ���
}
void CCalcBlock::SetTraceOnOrOff(int iOnOrOff)
{
	m_iTraceOn=iOnOrOff;//������־����
}
void CCalcBlock::SetWriteLogOnOrOff(int iOnOrOff)
{
	this->m_iLogOnOrOff=iOnOrOff;
}

CCalcPort * CCalcBlock::InSertInputPortBehind(int index, CString strPortID, int ValueType, CString strDesp, CString strUnit)
{
	CCalcPort* pPort = new CCalcPort();
	pPort->SetPortAsInOrOut(0);//���
	pPort->SetPortID(strPortID);//�ڲ��ı������� 
	pPort->SetPortDesp(strDesp) ;
	pPort->SetPortUnit(strUnit) ;
	pPort->SetPortIsOut2WS(0); //����Ƿ������ȫ�ֹ����ռ��� 
	pPort->SetPortValueType(ValueType);
	pPort->SetBlockPtr(this);

	if(index<0)
		return NULL;
	if(index<m_listIn.GetCount())
	{
		POSITION pos = m_listIn.FindIndex(index);
		m_listIn.InsertAfter(pos,pPort);
	}
	else
	{
		m_listIn.AddTail(pPort);
	}
	return pPort;
}

CCalcPort * CCalcBlock::InSertInputPortBefore(int index, CString strPortID, int ValueType, CString strDesp, CString strUnit)
{
	CCalcPort* pPort = new CCalcPort();
	pPort->SetPortAsInOrOut(0);//���
	pPort->SetPortID(strPortID);//�ڲ��ı������� 
	pPort->SetPortDesp(strDesp) ;
	pPort->SetPortUnit(strUnit) ;
	pPort->SetPortIsOut2WS(0); //����Ƿ������ȫ�ֹ����ռ��� 
	pPort->SetPortValueType(ValueType);
	pPort->SetBlockPtr(this);

	if(index<0)
		return NULL;
	if(index<m_listIn.GetCount())
	{
		POSITION pos = m_listIn.FindIndex(index);
		m_listIn.InsertBefore(pos,pPort);
	}
    else
	{
		m_listIn.AddTail(pPort);
	}
	return pPort;
}
CCalcPort * CCalcBlock::InSertOutputPortBehind(int index, CString strPortID, int ValueType, CString strDesp, CString strUnit)
{
	CCalcPort* pPort = new CCalcPort();
	pPort->SetPortAsInOrOut(0);//���
	pPort->SetPortID(strPortID);//�ڲ��ı������� 
	pPort->SetPortDesp(strDesp) ;
	pPort->SetPortUnit(strUnit) ;
	pPort->SetPortIsOut2WS(0); //����Ƿ������ȫ�ֹ����ռ��� 
	pPort->SetPortValueType(ValueType);
	pPort->SetBlockPtr(this);

	if(index<0)
		return NULL;
	if(index<m_listOut.GetCount())
	{
		POSITION pos = m_listOut.FindIndex(index);
		m_listOut.InsertAfter(pos,pPort);
	}
	else
	{
		m_listOut.AddTail(pPort);
	}
	return pPort;
}

CCalcPort * CCalcBlock::InSertOutputPortBefore(int index, CString strPortID, int ValueType, CString strDesp, CString strUnit)
{
	CCalcPort* pPort = new CCalcPort();
	pPort->SetPortAsInOrOut(0);//���
	pPort->SetPortID(strPortID);//�ڲ��ı������� 
	pPort->SetPortDesp(strDesp) ;
	pPort->SetPortUnit(strUnit) ;
	pPort->SetPortIsOut2WS(0); //����Ƿ������ȫ�ֹ����ռ��� 
	pPort->SetPortValueType(ValueType);
	pPort->SetBlockPtr(this);

	if(index<0)
		return NULL;
	if(index<m_listOut.GetCount())
	{
		POSITION pos = m_listOut.FindIndex(index);
		m_listOut.InsertBefore(pos,pPort);
	}
    else
	{
		m_listOut.AddTail(pPort);
	}
	return pPort;
}
