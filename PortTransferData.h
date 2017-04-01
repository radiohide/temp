// PortTransferData.h: interface for the CPortTransferData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PORTTRANSFERDATA_H__4614A8B0_269C_401E_8AD5_6767AC21AD87__INCLUDED_)
#define AFX_PORTTRANSFERDATA_H__4614A8B0_269C_401E_8AD5_6767AC21AD87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPortTransferData  
{
public:
	CPortTransferData();
	virtual ~CPortTransferData();
public:
	void CPortTransferData::CopyValueFromPortObj(CCalcPort *pPort,int flag);
	void CopyValueToPortObj(CCalcPort *pPort);


public:
	char m_cKey[128];//g_WorkSpace中的Key值
	char m_portID[128];	//端口号＝名称

	//UniValue相关数据
	int m_type;
	long   m_lSize; //   当前的实际数据大小
	long   m_lBufSize; //已经分配的空间
	long   m_lBufIncSize; //每次缓存增长的长度值

	long m_plTimeData;
	double m_pdValueData;
	short m_pStatusData;

};

#endif // !defined(AFX_PORTTRANSFERDATA_H__4614A8B0_269C_401E_8AD5_6767AC21AD87__INCLUDED_)
