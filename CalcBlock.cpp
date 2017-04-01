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
	m_iTraceOn=1;//追踪开关1:开，0:关
	m_iDebugOn=1;//调试开关1:开，0:关

	m_pModel=NULL;
	m_pModule=NULL;

	m_bShowDesp=TRUE;//是否显示DESP
	m_bShowInPortText=TRUE; //是否显示输入端口的名称文字
	m_bShowOutPortText=TRUE; //是否显示输入端口的名称文字
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
	//清除端口情况，copy时再建立所有和对象一样的端口
	ClearAllPorts();

}
void CCalcBlock::SetBlockPropertyToTransferData(
				CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)//将属性放置传输数据中
{
	
}
//清空发送的数据Arr
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
				const CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)//将传输数据放置到模块中
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
			if(10035 != GetLastError())//10035代表缓冲区已经满了，延迟发送数据
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
	int iNum=bufArr.GetSize()/1024;//需要发送整包的次数
	int iR=bufArr.GetSize()%1024;

	if(iNum>0)//发整包
	{
		char *bufEntireSend=new char[1024];
		for(int i=0;i<iNum;i++)//循环发整包
		{
			memset(bufEntireSend,0,1024);
			for(int j=i*1024;j<(i+1)*1024;j++)
			{
				bufEntireSend[j-i*1024]=bufArr[j];
			}
			ret=sendOnePackage(bufEntireSend,1024,pSocket);
			if(FALSE==ret)//发包失败跳出循环发包
			{
				break;
			}
		}
		if(bufEntireSend!=NULL)
		{
			delete []bufEntireSend;
			bufEntireSend=NULL;
		}
		if(FALSE==ret)//有一次发包失败则退出发送
		{
			return ret;
		}
	}
	if(iR>0)//有余下的包，则发送
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
	//加入头
	char bufHead[6];memset(bufHead,0,6);
	bufHead[0]='*';
	*(int*)(bufHead+1) = dataArr.GetSize();//1~4  元素的长度
	bufHead[5]='C';//协议号
	putPCharToCharArr(bufHead,6,bufArr);
	
	for(int i=0;i<dataArr.GetSize();i++)
	{
		CBlockPropertyTransferData *pData=dataArr[i];
		if(pData==NULL)
			continue;
		int size=4+sizeof(*pData);
		char *bufData=new char[size];
		
		*(int*)(bufData) = sizeof(*pData);//放置长度
		memcpy(bufData+4,pData,sizeof(*pData));//拷贝数据
		putPCharToCharArr(bufData,size,bufArr);
		if(bufData!=NULL)
		{
			delete bufData;
			bufData=NULL;
		}
	}
	char bufTail='$';//加入尾巴
	bufArr.Add(bufTail);
}
void CCalcBlock::SendBlockPropertyTransferData(
				const CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)//发送数据
{
	if(!pSockServ)
		g_ServSocketExit(pSockServ);
	pSockServ = g_ServSocketInit(m_pModel->m_strModelIP,atoi(m_pModel->m_strPort));
	if(!pSockServ)
	{
		AfxMessageBox("连接该计算模型失败，可能模型未启动或网络相关故障!");
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
	
	lbuf[0] = '*';	//安全传输符号
	lbuf[5] = 'C';	//协议号
	
	//下面开始传送dataArr中的元素
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
					if(10035 != nErrorNo)//10035代表缓冲区已经满了，延迟发送数据
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
				if(10035 != nErrorNo)//10035代表缓冲区已经满了，延迟发送数据
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
void  CCalcBlock::ChangeInPortName(CString strOldPortName,CString strNewPortName)//只修改输入端口名称
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
void  CCalcBlock::ChangeOutPortName(CString strOldPortName,CString strNewPortName)//修改输出端口名称，并修改于之相连的连接接
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
			if(strThisBlockName!=obj->m_pBlockInner->GetBlockName())//其他计算块
			{
				int InNum = obj->GetInputNum();
				for(int k=0;k<InNum;k++)
				{
					//看是否有连接
					BOOL bLink = obj->GetBlockPortLinked(k);
					if(bLink)
					{
						//找是哪个块、第几个输出，根据BlockName,portName,
						CString strFromBlockName = obj->GetPortLinkBlockName(k);
						CString strFromBlockPortName = obj->GetPortLinkBlockPortName(k);
						if((strFromBlockName==strThisBlockName)&&(strFromBlockPortName==strOldPortName))
						{
							obj->SetBlockPortLink(k,strThisBlockName,strNewPortName); //设置
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
void CCalcBlock::CopyPropertyData(CCalcBlock* pBlock)//复制图形时，需要拷贝属性。
{
	
}
//静态方法，生成对象本身
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
 //创建端口
 void CCalcBlock::CreatePortsFromString(int InNum,int OutNum,CString InNames,CString strInValueTypes,CString OutNames,CString strOutValueTypes, CString InputLinkStrs)
{
	ClearAllPorts();
	//检查输入
	CTokenizer tok( InNames ); 
	CTokenizer tokLink( InputLinkStrs ); 
	CTokenizer tokType( strInValueTypes ); 

	int size = tok.GetSize(); //输入的个数
	int sizeLink = tokLink.GetSize();//输入连接字符串的个数
	int sizeType = tokType.GetSize();
	//要求当没有连接时，也需要对应；
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
	//输出的情况///////////////////////////////////////////////
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
	//把输出端口对象均映射到全局中WS
	MapOutputToGlobalWS();
}
//默认该计算块的计算
void CCalcBlock::Calc()
{
}
//返回属性的逗点字符串
CString CCalcBlock::GetBlockStrPro()
{
	return "";
}
void CCalcBlock::SetBlockStrPro(CString str)//设置计算块的字符串属性
{

}
//画出特征图形
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
//看模块的输入的情况
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
			//记录下来,记录到什么地方，一个文件里
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
//在原有的基础上添加一个输入端口
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
			ClearOutPortLink(pPort,parent);//清空与该端口相关联的端口连线
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
			ClearOutPortLink(pPort,parent);//清空与该端口相关联的端口连线
			if(strType!="LinkFrom")//"LinkFrom"类型的计算块，不需要执行delete 的方式删除端口
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

//清除所有的输入输出端口
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
	//首先查询属性表中属于该块名字的所有行，从而得到属性值
	//各模块自身检查所有的属性情况，没有显式设置的要给默认值
	////////////////////////////////////////////////////////////////////
  	_RecordsetPtr  pRecord;
	_variant_t     Tempvalue ;
    pRecord.CreateInstance(__uuidof(Recordset));
    pRecord->CursorLocation = adUseClient;
    //检索到所有属性
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
	//逐个读取各个属性，每个属性对应属性列表不一样，需要从数据集中获得具体的值
	//首先判断属性名字，按名字来处理后面的数值域
	while(!pRecord->adoEOF)
	{
		CString strPropName; //块
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
		    //当读取没问题后，则将数值给该属性
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
       //获得输入的端口对象
		CCalcPort *pPortObj = GetInputPortObj(strName);
		//判断端口是否有连接
		if(pPortObj->GetPortLinked())
		{
			//得到连接的在空间中的变量名字
			CString strVarName;
			strVarName.Format("%s.%s.%s",m_pModule->GetModuleName(),pPortObj->GetLinkFormBlockName(),pPortObj->GetLinkFromBlockPortName());
			//获得空间中的变量
			CCalcPort *pGWSObj = (CCalcPort*)g_WorkSpace[strVarName];
			if(pGWSObj !=NULL)
			{
				//得到空间的数据
				return  pGWSObj->GetPortUniValue();
			}
		}
		return var;
}
BOOL CCalcBlock::IsLinkVarExistInGlobalWS(CString strPortName)
{
	CCalcPort *pPortObj = GetInputPortObj(strPortName);
	if(pPortObj ==NULL) return FALSE;
	//判断端口是否有连接
	if(pPortObj->GetPortLinked()) 
	{
		//得到连接的在空间中的变量名字
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
		   //获得输入的端口对象
			CCalcPort *pPortObj = GetInputPortObj(k);
			//判断端口是否有连接
			if(pPortObj->GetPortLinked()) 
			{
				//得到连接的在空间中的变量名字
				strVarName.Format("%s.%s.%s",m_pModule->GetModuleName(),pPortObj->GetLinkFormBlockName(),pPortObj->GetLinkFromBlockPortName());
				//获得空间中的变量
				CCalcPort *pGWSObj = (CCalcPort*)g_WorkSpace[strVarName];
				if(pGWSObj !=NULL)
				{
					//得到空间的数据
					CUniValue UniVar = pGWSObj->GetPortUniValue();
					//将本对象的该端口的数据进行复制
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
//根据名字获得端口
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


//添加一个输入端口，指明类型和内部号，用于进行构造
CCalcPort* CCalcBlock::AddInputPort(CString strPortID, int ValueType, CString strDesp, CString strUnit)
{
	CCalcPort* pPort = new CCalcPort();
	pPort->SetPortAsInOrOut(0);//输出
	pPort->SetPortID(strPortID);//内部的变量名称 
	pPort->SetPortDesp(strDesp) ;
	pPort->SetPortUnit(strUnit) ;
	pPort->SetPortIsOut2WS(0); //输出是否输出到全局工作空间中 
	pPort->SetPortValueType(ValueType);
	pPort->SetBlockPtr(this);
	m_listIn.AddTail(pPort);
	return pPort;
}

CCalcPort* CCalcBlock::AddOutputPort(CString strPortID, int ValueType, CString strDesp, CString strUnit)
{
	CCalcPort* pPort = new CCalcPort();
	pPort->SetPortAsInOrOut(1);//输出
	pPort->SetPortID(strPortID);//内部的变量名称 
	pPort->SetPortDesp(strDesp) ;
	pPort->SetPortUnit(strUnit) ;
	pPort->SetPortIsOut2WS(1); //输出是否输出到全局工作空间中 
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
	//输出的格式为：Module.BlockName.OutportID
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
		str.Format("警告：输入未连接'计算块:%s，块描述:%s'",GetBlockName(),GetBlockDesp());
		m_pWnd->SendMessage(m_iMsg, 0, (LPARAM) AllocBuffer(str));
		(*m_pModuleWarningCount)++;
		(*m_pModelWarningCount)++;
	}

}
void CCalcBlock::SetDebugOnOrOff(int iOnOrOff)
{
	m_iDebugOn=iOnOrOff;//设置调试开关
}
void CCalcBlock::SetTraceOnOrOff(int iOnOrOff)
{
	m_iTraceOn=iOnOrOff;//设置日志开关
}
void CCalcBlock::SetWriteLogOnOrOff(int iOnOrOff)
{
	this->m_iLogOnOrOff=iOnOrOff;
}

CCalcPort * CCalcBlock::InSertInputPortBehind(int index, CString strPortID, int ValueType, CString strDesp, CString strUnit)
{
	CCalcPort* pPort = new CCalcPort();
	pPort->SetPortAsInOrOut(0);//输出
	pPort->SetPortID(strPortID);//内部的变量名称 
	pPort->SetPortDesp(strDesp) ;
	pPort->SetPortUnit(strUnit) ;
	pPort->SetPortIsOut2WS(0); //输出是否输出到全局工作空间中 
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
	pPort->SetPortAsInOrOut(0);//输出
	pPort->SetPortID(strPortID);//内部的变量名称 
	pPort->SetPortDesp(strDesp) ;
	pPort->SetPortUnit(strUnit) ;
	pPort->SetPortIsOut2WS(0); //输出是否输出到全局工作空间中 
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
	pPort->SetPortAsInOrOut(0);//输出
	pPort->SetPortID(strPortID);//内部的变量名称 
	pPort->SetPortDesp(strDesp) ;
	pPort->SetPortUnit(strUnit) ;
	pPort->SetPortIsOut2WS(0); //输出是否输出到全局工作空间中 
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
	pPort->SetPortAsInOrOut(0);//输出
	pPort->SetPortID(strPortID);//内部的变量名称 
	pPort->SetPortDesp(strDesp) ;
	pPort->SetPortUnit(strUnit) ;
	pPort->SetPortIsOut2WS(0); //输出是否输出到全局工作空间中 
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
