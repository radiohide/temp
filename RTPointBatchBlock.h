// RTPointBatchBlock.h: interface for the CRTPointBatchBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTPOINTBATCHBLOCK_H__668C9146_D129_4C1E_B0BA_1BA57DF39E6C__INCLUDED_)
#define AFX_RTPOINTBATCHBLOCK_H__668C9146_D129_4C1E_B0BA_1BA57DF39E6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CRTPointBatchBlock : public CCalcBlock  
{
public:
	CRTPointBatchBlock();
	virtual ~CRTPointBatchBlock();
public:
	//virtual void DrawShape(CDC* dc, CRect rect,int CutSize); //��������״������Add at 2011-10-30
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
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
private:
	char *pTagNames[1000];
	TagData *pTagData;
private:
	CStringArray strRTDBTagArr;//ʵʱ�����
	CStringArray strPortTagArr;//�˿ڵ���
	CStringArray strTagDespArr;//��������
	CStringArray strUnitArr;//��λ

	int m_iSysOrInput;

	
};

#endif // !defined(AFX_RTPOINTBATCHBLOCK_H__668C9146_D129_4C1E_B0BA_1BA57DF39E6C__INCLUDED_)
