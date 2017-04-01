// LinkToBlock.h: interface for the CLinkToBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINKTOBLOCK_H__97420110_BF66_4366_B016_C163FBBDF6BF__INCLUDED_)
#define AFX_LINKTOBLOCK_H__97420110_BF66_4366_B016_C163FBBDF6BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include <vector>
class CLinkToBlock : public CCalcBlock  
{
public:
	CLinkToBlock();
	virtual ~CLinkToBlock();
public:
	CString m_sOutPut1,m_sOutPut2,m_sOutPut3,m_sOutPut4,m_sOutPut5;
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
	virtual void CopyPropertyData(CCalcBlock* pBlock);
	//�����Է��ô���������
	virtual void SetBlockPropertyToTransferData(
		CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr);
	//�������ԵĶ����ַ���
	virtual CString GetBlockStrPro();
	virtual void SetBlockStrPro(CString str);//���ü������ַ�������
private:
	void reviewPort(CWnd* parent,CStringArray &IDArrLeftOut,std::vector<int> &ArrLeftDatytypeOut);
	void removeElemFromWSByKey(CString strKey);
	CString getRefToWSTagName(int index);

};

#endif // !defined(AFX_LINKTOBLOCK_H__97420110_BF66_4366_B016_C163FBBDF6BF__INCLUDED_)
