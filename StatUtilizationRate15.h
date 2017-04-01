// StatUtilizationRate15.h: interface for the CStatUtilizationRate15 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUTILIZATIONRATE15_H__D63A9C8C_E3F7_4DE8_A90F_4CC7B723572F__INCLUDED_)
#define AFX_STATUTILIZATIONRATE15_H__D63A9C8C_E3F7_4DE8_A90F_4CC7B723572F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStatUtilizationRate15 : public CCalcBlock  
{
public:
	CStatUtilizationRate15();
	virtual ~CStatUtilizationRate15();

public:
	virtual void ConstructBlockInputOutput (); 
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
	
private:
	BOOL gIsTableExist(_ConnectionPtr &pConnection, CString strTableName);
	void writeDataToTable(CString strTurbine,CString strTableName,TagData stateTable[],double dUtiliRate);
	void DataRevise(TagData stateTable[],double &dSum);
	void DataReviseR(TagData stateTable[],double dSum);
	
private:
	CString m_strDBName;
	CString m_strTableName;
	CString m_strTurbine;
	
};

#endif // !defined(AFX_STATUTILIZATIONRATE15_H__D63A9C8C_E3F7_4DE8_A90F_4CC7B723572F__INCLUDED_)
