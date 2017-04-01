// CalcPort.cpp: implementation of the CCalcPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "CalcPort.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


extern CMap<CString,LPCTSTR,CCalcPort*,CCalcPort*&> g_WorkSpace;
CCalcPort::CCalcPort()
{
	strcpy(m_portID,"N/A");	//点号＝名称
	strcpy(m_sLinkID,"");	
	strcpy(m_sDsp,"");
	strcpy(m_sUnit,"");
	m_bIsOutPort = 0;
	m_bOut2WS=0; 
	m_CheckState =0; //
	m_LimitState =0;
	m_ValueType = VALUE_DOUBLE;
	
	m_bForceValue = FALSE;
    m_strLinkPortName.Empty();
	m_strLinkBlockName.Empty();
	m_bLinked = FALSE;

	m_DoubleValue =0;

	m_UniValue.SetValueType(m_ValueType);
	m_UniValue.AddData(0.0);
}

CCalcPort::CCalcPort(BOOL bOut,CString strPortID)
{
	m_bIsOutPort = bOut;
	m_bOut2WS =    bOut;
	strcpy(m_portID,strPortID);	//点号＝名称

	strcpy(m_sLinkID,"");
	strcpy(m_sDsp,strPortID);
	strcpy(m_sUnit,"");
	m_CheckState =0; //
	m_LimitState =0;
	m_ValueType = VALUE_DOUBLE;
	
	m_bForceValue = FALSE;

	 m_strLinkPortName.Empty();
	m_strLinkBlockName.Empty();
	m_bLinked = FALSE;

	m_DoubleValue =0;

	m_UniValue.SetValueType(m_ValueType);
	m_UniValue.AddData(0.0);
}

CCalcPort::~CCalcPort()
{
  
}
void CCalcPort::SetPortID(CString strName)
{
	strcpy(m_portID,strName);
}
void CCalcPort::SetPortDesp(CString str)
{
	strcpy(m_sDsp,str);
}
void CCalcPort::SetPortUnit(CString str)
{
	strcpy(m_sUnit,str);
}
void  CCalcPort::SetPortAsInOrOut(BOOL bInOut)//是输出还是输入端口
{
	 m_bIsOutPort= bInOut;
}
void  CCalcPort::SetPortIsOut2WS(BOOL bOut2WS)//设置端口数据是否输出到全局WS中
{
	m_bOut2WS=bOut2WS;
}
void  CCalcPort::SetPortIsValueForce(BOOL bForce)//设置是否为强制输出状态
{
	m_bForceValue = bForce;
}
void  CCalcPort::SetPortValueType(int type)  //设置端口的数据是那种类型
{
	m_ValueType = type;
	m_UniValue.SetValueType(type);
}
int  CCalcPort::GetPortValueType()  
{
	return m_ValueType;
	//return m_UniValue.GetValueType();
}
void  CCalcPort::SetPortLinkID(CString strID)
{
	strcpy(m_sLinkID,strID);
}

//只针对
void CCalcPort::MapPortIntoGlobalWS()
{
	CCalcPort* pPort = this;
	CString strVarName = GetPortLinkID();
	g_WorkSpace.SetAt(strVarName,pPort);
}
void CCalcPort::PutValueToGlobalWS()
{
	CString strVarName = GetPortLinkID();
	CCalcPort *pCalcPort=g_WorkSpace[strVarName];
	pCalcPort->m_UniValue = m_UniValue;
}

//////////////////////////////////////////////////
CUniValue& CCalcPort::GetPortUniValue()
{
	return m_UniValue;
}
void  CCalcPort::SetPortUniValue(CUniValue value)
{
	m_UniValue = value;
}
//设置该端口连接的块名称
void CCalcPort::SetLinkFormBlockName(CString strBlockName)
{
	m_strLinkBlockName= strBlockName;

}
CString CCalcPort::GetLinkFormBlockName()
{
	return m_strLinkBlockName;
}
void CCalcPort::SetLinkFromBlockPortName(CString strPortName)
{
	 m_strLinkPortName= strPortName;
	
}
CString CCalcPort::GetLinkFromBlockPortName()
{
	return m_strLinkPortName;
}
void CCalcPort::SetPortLinked(BOOL bLink)
{
	m_bLinked = bLink;
}
BOOL CCalcPort::GetPortLinked()
{
	return m_bLinked;
}
void CCalcPort::SetBlockPtr(CCalcBlock *ptr)
{
	this->m_ptrBlock=ptr;
}