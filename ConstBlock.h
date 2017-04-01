// ConstBlock.h: interface for the CConstBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONSTBLOCK_H__65740352_F1E9_44E4_AA84_E550FAD546B9__INCLUDED_)
#define AFX_CONSTBLOCK_H__65740352_F1E9_44E4_AA84_E550FAD546B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CConstBlock : public CCalcBlock 
{
public:
	CConstBlock();
	virtual ~CConstBlock();
	//��ӱ�Ҫ���麯����
	virtual void DrawShape(CDC* dc, CRect rect,int CutSize); //��������״������Add at 2011-10-30
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
    double m_dConstValue;//ʢ���ϴεĹ̶�ֵ
};

#endif // !defined(AFX_CONSTBLOCK_H__65740352_F1E9_44E4_AA84_E550FAD546B9__INCLUDED_)
