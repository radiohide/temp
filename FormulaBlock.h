// FormulaBlock.h: interface for the CFormulaBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORMULABLOCK_H__D26CB8DB_A85B_43C7_8B8A_67BBE8EF0EB9__INCLUDED_)
#define AFX_FORMULABLOCK_H__D26CB8DB_A85B_43C7_8B8A_67BBE8EF0EB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CFormulaBlock : public CCalcBlock  
{
public:
	CFormulaBlock();
	virtual ~CFormulaBlock();
public:
	int maxInPut;//����������
	int maxOutPut;//����������
	int maxFormula;//���ʽ����������м��������Ҳ�������

	CStringArray inPutArr;
	CArray<int,int> inPutTypeArr;//0,��ֵ��1,����

	CStringArray outPutArr;
	CArray<int,int> outPutTypeArr;//0,��ֵ��1,����

	CStringArray varArr;
	CStringArray formulaArr;
	CArray<int,int> varTypeArr;//0,��ֵ��1,����
public:
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
	void setDataToElem(CUniValue &uniValue,CElementInOut *pElement,int type);//type:0,��ֵ�㣻1,����ֵ
	void setDataToUniValue(CElementInOut *pElement,CUniValue &uniValue,int type);//type:0,��ֵ�㣻1,����ֵ
	void delInnerWorkSpace();//����ڲ�Map
	void establishInnerWorkSpace();//��ʼ���������ڲ�Map��������ʽ
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
	void resetAllElement();//������Ԫ�ؽ��и�λ���Ա��ڽ����´μ��㡣
};

#endif // !defined(AFX_FORMULABLOCK_H__D26CB8DB_A85B_43C7_8B8A_67BBE8EF0EB9__INCLUDED_)
