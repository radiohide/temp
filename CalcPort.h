// CalcPort.h: interface for the CCalcPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCPORT_H__E4751111_35D5_4D0D_AB33_E2E79F4D370F__INCLUDED_)
#define AFX_CALCPORT_H__E4751111_35D5_4D0D_AB33_E2E79F4D370F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//����Ķ˿ڶ���
#include "UniValue.h"
class CCalcBlock;
class CCalcPort:public CObject
{
public:
	CCalcPort();
	CCalcPort(BOOL bOut,CString strPortID); //��һ�����캯��
	virtual ~CCalcPort();
public:
	//�������������
	void SetLinkFormBlockName(CString strBlockName);
	CString GetLinkFormBlockName();
	void SetLinkFromBlockPortName(CString strPortName);
	CString GetLinkFromBlockPortName();
	void SetPortLinked(BOOL bLink);
	BOOL GetPortLinked();  
   
	//���ö˿�����
	void SetPortID(CString strName); //���øö˿����ڲ��ı�������
	void SetPortDesp(CString str);   //���øö˿ڵ�����
	void SetPortUnit(CString str);   //���ö˿ڵĵ�λ
    void SetPortLinkID(CString str);  //���������Ͷ˿������ĸ������������˿����������
	void SetBlockPtr(CCalcBlock *ptr);
	CString GetPortID(){return m_portID;} 
	CString GetPortDesp(){return m_sDsp;} 
	CString GetPortUnit(){return m_sUnit;} 
	CString GetPortLinkID(){return m_sLinkID;} 
	CCalcBlock *GetBlockPtr(){return m_ptrBlock;}
	

	void SetPortAsInOrOut(BOOL bInOut);//�������������˿�
	void SetPortIsOut2WS(BOOL bOut2WS);//���ö˿������Ƿ������ȫ��WS��
    void SetPortIsValueForce(BOOL bForce);//�����Ƿ�Ϊǿ�����״̬
	void SetPortValueType(int type);  //���ö˿ڵ���������������
	int   GetPortValueType() ;
	
	//�˿ڵ���������Ĵ���
	void PutValueToGlobalWS();//�Ѷ˿ڵ�ֵ�ŵ����Ӧ��ȫ��WS�У����˿�Ϊ���ʱʹ��
	void MapPortIntoGlobalWS(); //�Ѷ˿�����ӳ�䵽ȫ��WS�У����˿�Ϊ���ʱʹ�á�
	//double GetPortValue(); 
	//void      SetPortValue(double value);

	CUniValue& GetPortUniValue(); 
	void      SetPortUniValue(CUniValue value);
public:
	char m_portID[128];	//�˿ںţ�����
	char m_sLinkID[128]; //��Դ�������ȥ����
	char m_sDsp[128];	//����
	char m_sUnit[128];  //��λ
	CCalcBlock *m_ptrBlock;//�ö˿������ļ����

	BOOL   m_bOut2WS;    //�Ƿ�ŵ�WS��
	BOOL   m_bIsOutPort;// �Ƿ������
	BOOL   m_bForceValue;//

	double m_Value_Force; //ǿ�Ƶ����
	int    m_CheckState;   //У��״̬���������ڲ���У�� BAD��0��GOOD=1;
	int    m_LimitState;  //ֵ���Ƶ�״̬��0��GOOD,1:�����ޣ�����1��������

	int    m_ValueType;

	double m_DoubleValue;     //�˿���ֵ
	long   m_lTime;     //�˿���ֵ��Ӧʱ��

	CUniValue  m_UniValue;
protected:
	 CString m_strLinkPortName;
	CString m_strLinkBlockName;
	BOOL    m_bLinked;


};

#endif // !defined(AFX_CALCPORT_H__E4751111_35D5_4D0D_AB33_E2E79F4D370F__INCLUDED_)
typedef CTypedPtrList<CObList,CCalcPort*> CPortList;
typedef CTypedPtrArray<CObArray,CCalcPort*> CPortArray;