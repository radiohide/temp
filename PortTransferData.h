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
	char m_cKey[128];//g_WorkSpace�е�Keyֵ
	char m_portID[128];	//�˿ںţ�����

	//UniValue�������
	int m_type;
	long   m_lSize; //   ��ǰ��ʵ�����ݴ�С
	long   m_lBufSize; //�Ѿ�����Ŀռ�
	long   m_lBufIncSize; //ÿ�λ��������ĳ���ֵ

	long m_plTimeData;
	double m_pdValueData;
	short m_pStatusData;

};

#endif // !defined(AFX_PORTTRANSFERDATA_H__4614A8B0_269C_401E_8AD5_6767AC21AD87__INCLUDED_)
