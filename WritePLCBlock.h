// WritePLCBlock.h: interface for the CWritePLCBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITEPLCBLOCK_H__AEF44157_7F4F_44A4_8C06_8F1473DB9842__INCLUDED_)
#define AFX_WRITEPLCBLOCK_H__AEF44157_7F4F_44A4_8C06_8F1473DB9842__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include "PropertyDlgWritePLC.h"
#include "ModbusHead.h"
class CWritePLCBlock : public CCalcBlock  
{
public:
	CWritePLCBlock();
	virtual ~CWritePLCBlock();
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

#endif // !defined(AFX_WRITEPLCBLOCK_H__AEF44157_7F4F_44A4_8C06_8F1473DB9842__INCLUDED_)
