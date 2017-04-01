// WriteRDBProBat.h: interface for the CWriteRDBProBat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITERDBPROBAT_H__1CC4979F_6095_4739_BBCC_D944C9ED30B7__INCLUDED_)
#define AFX_WRITERDBPROBAT_H__1CC4979F_6095_4739_BBCC_D944C9ED30B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"

class CWriteRDBProBat : public CCalcBlock  
{
public:
	CWriteRDBProBat();
	virtual ~CWriteRDBProBat();
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
	//�������ԵĶ����ַ���
	virtual CString GetBlockStrPro();
	virtual void SetBlockStrPro(CString str);//���ü������ַ�������
private:
	void getPropTypeByName(CString strPropName,int &propType,int &propIndex);
	BOOL gIsTableExist(_ConnectionPtr &pConnection, CString strTableName);
private:
	CString m_strFieldVarName;
	CString m_strFieldDateTime;
	CString m_strFieldRealDateTime;
	CString m_strFieldState;
	CString m_strFieldValue;
private:
	int m_iOnlySave;//ֻ�����־
	int m_iSaveSeconds;//ֻ�������n���ڵ����ݡ�
	CString m_strDBName;//���ݿ�����

	CStringArray tagNameArr;//����
	CStringArray unitArr;//��λ
	CStringArray dispArr;//����
	CStringArray strOutPutNameArr;//д���ϵ��ı����ƣ����������׺��

};

#endif // !defined(AFX_WRITERDBPROBAT_H__1CC4979F_6095_4739_BBCC_D944C9ED30B7__INCLUDED_)
