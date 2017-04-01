// StatAddBlock.h: interface for the CStatAddBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATADDBLOCK_H__3313FEB5_B563_4211_A675_144BE2C8F762__INCLUDED_)
#define AFX_STATADDBLOCK_H__3313FEB5_B563_4211_A675_144BE2C8F762__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CStatAddBlock : public CCalcBlock  
{
public:
	CStatAddBlock();
	virtual ~CStatAddBlock();

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
	CString m_strInputEdit;

private:
	void buildInputPort(CStringArray &IDArrLeftOut);

};

#endif // !defined(AFX_STATADDBLOCK_H__3313FEB5_B563_4211_A675_144BE2C8F762__INCLUDED_)
