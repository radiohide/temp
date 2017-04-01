// CycleBlock.h: interface for the CCycleBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CYCLEBLOCK_H__765FF222_572B_423A_AAD3_335D86532EAA__INCLUDED_)
#define AFX_CYCLEBLOCK_H__765FF222_572B_423A_AAD3_335D86532EAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CCycleBlock : public CCalcBlock  
{
public:
	CCycleBlock();
	virtual ~CCycleBlock();
public:
	int maxInPut;//����������
	int maxOutPut;//����������
	int maxFormula;//���ʽ����������м��������Ҳ�������
	int tagConFOrB;//�����ڵ���ǰ���㻹�ǵ�������㣬0�ڵ���ǰ���㣻1�ڵ��������

	CStringArray inPutArr;
	CArray<int,int> inPutTypeArr;//0,��ֵ��1,����
	CArray<int,int> inPutCanBeEdit;//0���ɱ��༭��1�ɱ��༭��

	CStringArray outPutArr;
	CArray<int,int> outPutTypeArr;//0,��ֵ��1,����
	CArray<int,int> outPutCanBeEdit;//0���ɱ��༭��1�ɱ��༭��

	CStringArray IterFVarArr;//����ǰ����
	CStringArray IterFFormulaArr;
	CArray<int,int> IterFVarTypeArr;//0,��ֵ��1,����
	CArray<int,int> IterFCanBeEdit;//0���ɱ��༭��1�ɱ��༭��
	CArray<int,int> IterFCanBeShow;//0������ʾ��1����ʾ��

	CStringArray conVarArr;//��������
	CStringArray conFormulaArr;
	CArray<int,int> conVarTypeArr;//0,��ֵ��1,����
	CArray<int,int> conVarCanBeEdit;//0���ɱ��༭��1�ɱ��༭��
	CArray<int,int> conVarCanBeShow;//0������ʾ��1����ʾ��

	CStringArray IterVarArr;//��������
	CStringArray IterFormulaArr;
	CArray<int,int> IterVarTypeArr;//0,��ֵ��1,����
	CArray<int,int> IterCanBeEdit;//0���ɱ��༭��1�ɱ��༭��
	CArray<int,int> IterCanBeShow;//0������ʾ��1����ʾ��

	CStringArray IterBVarArr;//���������
	CStringArray IterBFormulaArr;
	CArray<int,int> IterBVarTypeArr;//0,��ֵ��1,����
	CArray<int,int> IterBCanBeEdit;//0���ɱ��༭��1�ɱ��༭��
	CArray<int,int> IterBCanBeShow;//0������ʾ��1����ʾ��

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
	//����ѭ������
	virtual void CalcIter(); 
public:
	void BulidInPutEle();
	void BulidIterFEle();
	void BulidConVarEle();
	void BulidIterEle();
	void BulidIterBEle();

	void ElementParseFormula();
	void DelInnerWorkSpace();//���Map
private:
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
	void setDataToElem(CUniValue &uniValue,CElementInOut *pElement,int type);//type:0,��ֵ�㣻1,����ֵ
	void setDataToUniValue(CElementInOut *pElement,CUniValue &uniValue,int type);//type:0,��ֵ�㣻1,����ֵ
	bool calcCon(); //��������
	void setBlokcElemBeCalc(); //��������Ԫ�ر������
	void setAllElemReSet(); //��������Ԫ��δ�������

};

#endif // !defined(AFX_CYCLEBLOCK_H__765FF222_572B_423A_AAD3_335D86532EAA__INCLUDED_)
