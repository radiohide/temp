// LinkFromBlock.h: interface for the CLinkFromBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINKFROMBLOCK_H__9BD72B82_EE32_43C3_827E_F05ACB779B62__INCLUDED_)
#define AFX_LINKFROMBLOCK_H__9BD72B82_EE32_43C3_827E_F05ACB779B62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include <vector>
class CLinkFromBlock : public CCalcBlock  
{
public:
	CLinkFromBlock();
	virtual ~CLinkFromBlock();
public:
	CString m_sInPut1,m_sInPut2,m_sInPut3,m_sInPut4,m_sInPut5;
	int m_iDataType1,m_iDataType2,m_iDataType3,m_iDataType4,m_iDataType5;
public:
	virtual void DrawShape(CDC* dc, CRect rect,int CutSize); //��������״������Add at 2011-10-30
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
	//�������ԵĶ����ַ���
	virtual CString GetBlockStrPro();
	virtual void SetBlockStrPro(CString str);//���ü������ַ�������
private:
	void reviewPort(CWnd* parent,CStringArray &IDArrRightOut,std::vector<int> &ArrRightDatytypeOut);
	CCalcPort* getRefFromWSByTagIndex(int index);

};

#endif // !defined(AFX_LINKFROMBLOCK_H__9BD72B82_EE32_43C3_827E_F05ACB779B62__INCLUDED_)
