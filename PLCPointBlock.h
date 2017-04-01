// PLCPointBlock.h: interface for the CPLCPointBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLCPOINTBLOCK_H__844D16EC_CD81_45CB_9B37_4A7659DF5C5E__INCLUDED_)
#define AFX_PLCPOINTBLOCK_H__844D16EC_CD81_45CB_9B37_4A7659DF5C5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include "PropertyDlgPLCPoint.h"
#include "ModbusHead.h"
class CPLCPointBlock : public CCalcBlock  
{
public:
	CPLCPointBlock();
	virtual ~CPLCPointBlock();
	int		m_iPort;
	int		m_iFunNo;
	int		m_iAdd;
	CString	m_strIP;

	CString m_strCurrentPointName;
	CString m_strCurentType;

    HINSTANCE hInst;
public:
	//�������صĺ���
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput(); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
	//��ʼ������ĺ���������ĳ�ʼ��
	virtual void InitCalc();
	//���㺯����ʵ�ֱ�ģ��ļ���
	virtual void Calc(); //���м���ĺ���,��Ҫ����
    //���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
	virtual void SetPropValue(CString strPropName,CString strItem1,CString strItem2="",CString strItem3="",CString strItem4="",CString strItem5="");
	//��������ʾ��ͬ�����Զ���Ի���
    virtual void ShowPropertyDialog( CWnd* parent, BOOL show );
	//�����Խ���ģ��д�뵽���ݿ���
	virtual void PutPropertyToRecord(_RecordsetPtr& pRecord);
	//����ͼ��ʱ����Ҫ�������ԡ�
	virtual void CopyPropertyData(CCalcBlock* pBlock);
	//�����Է��ô���������
	virtual void SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr);

};

#endif // !defined(AFX_PLCPOINTBLOCK_H__844D16EC_CD81_45CB_9B37_4A7659DF5C5E__INCLUDED_)
