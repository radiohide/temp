// StrMaptoNumBlock.h: interface for the CStrMaptoNumBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRMAPTONUMBLOCK_H__8BF7998B_8E89_423A_BD72_4986EAB2FF68__INCLUDED_)
#define AFX_STRMAPTONUMBLOCK_H__8BF7998B_8E89_423A_BD72_4986EAB2FF68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStrMaptoNumBlock : public CCalcBlock  
{
public:
	CStrMaptoNumBlock();
	virtual ~CStrMaptoNumBlock();
public:
	//��ӱ�Ҫ���麯����
//-----------------------------------------------------------------------------------------------------------
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
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
public:
	CString	m_strTableName;
	CString m_strDBDesp;
private:
	CStringArray m_strDataArr;
	CArray<double,double> m_dDataArr;
private:
	void getTableData(_ConnectionPtr &pConRDB);
};

#endif // !defined(AFX_STRMAPTONUMBLOCK_H__8BF7998B_8E89_423A_BD72_4986EAB2FF68__INCLUDED_)
