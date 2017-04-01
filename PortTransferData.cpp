// PortTransferData.cpp: implementation of the CPortTransferData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "PortTransferData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPortTransferData::CPortTransferData()
{
}

CPortTransferData::~CPortTransferData()
{

}

void CPortTransferData::CopyValueFromPortObj(CCalcPort *pPort,int flag)
{
	strcpy(m_portID,pPort->m_portID);
	m_type = pPort->m_UniValue.GetValueType();

	pPort->m_UniValue.GetTimeValAt(flag,&m_plTimeData);
	pPort->m_UniValue.GetDoubleValAt(flag,&m_pdValueData);
	pPort->m_UniValue.GetStatusValAt(flag,&m_pStatusData);
}

void CPortTransferData::CopyValueToPortObj(CCalcPort *pPort)
{
//	strcpy(pPort->m_cKey,m_cKey);
	if(pPort==NULL)
		return;
	strcpy(pPort->m_portID,m_portID);
	pPort->m_UniValue.SetValueType(m_type);

	pPort->m_UniValue.AddData(m_pdValueData,m_plTimeData,m_pStatusData);
	//pPort->m_UniValue.AddData(m_pdValueData);
}