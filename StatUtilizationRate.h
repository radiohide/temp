// StatUtilizationRate.h: interface for the CStatUtilizationRate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUTILIZATIONRATE_H__9444B7FA_FEBA_4C50_8066_E2357E1A95B4__INCLUDED_)
#define AFX_STATUTILIZATIONRATE_H__9444B7FA_FEBA_4C50_8066_E2357E1A95B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStatUtilizationRate : public CCalcBlock  
{
public:
	CStatUtilizationRate();
	virtual ~CStatUtilizationRate();

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

#endif // !defined(AFX_STATUTILIZATIONRATE_H__9444B7FA_FEBA_4C50_8066_E2357E1A95B4__INCLUDED_)
