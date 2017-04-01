// ElementInOut.h: interface for the CElementInOut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENTINOUT_H__E53ADFBA_6928_4B05_B88F_157D739312D7__INCLUDED_)
#define AFX_ELEMENTINOUT_H__E53ADFBA_6928_4B05_B88F_157D739312D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WorkSpaceElement.h"
#include "BTree.h"
#include "PoteDll.h"
#include "CalcBlock.h"
class CBTree;
//�ö�����
class CElementInOut :public CWorkSpaceElement
{
public:
	void SetBeCalc();
	void ReSet();//��λb_Calced
	CElementInOut(CString tLongID,int dType,CCalcBlock* pBlock);//��ʼ����Դ��
	CElementInOut(CString tLongID,int dType,CString Formula,CCalcBlock* pBlock);//��ʼ�������
	~CElementInOut();

	int ParseFormula();//������ʽ
	int CalcFormula();//�����ֵ(�����)
	double ReportValue (CBTree* pnod);

	void SetParentBlock(CCalcBlock* pBlock);
	void SetPtrMap(CMap<CString,LPCTSTR,CElementInOut*,CElementInOut*&> *m_pWorkSpaceList);
	CMap<CString,LPCTSTR,CElementInOut*,CElementInOut*&> *m_pWorkSpaceList;
	CCalcBlock *m_pParentBlock;
	CString LongID;		//���
	TAGINFO *pDataAll;//�洢ԭʼ����
	int NumData;//��¼����ԭʼ���ݵĸ���

	RTFUNC rtFunc;//�ⲿ����ָ��
	TAGINFO* PSourceValue;//���ͳ�Ƶ����Դ������
	TAGINFO* pG;

	CBTree* m_BTree;	//��ʽ���ʽ�Ķ�����
	int	PType;			//�����ͣ�0Ϊ�ɼ��㣬1Ϊ����㣬2Ϊϵ����
	int DType;			//�����ͣ�0Ϊ��ֵ�㣬1Ϊ���е�
	CString PFormula;	//��ļ��㹫ʽ
	double PValue;		//��Ľ���������ɼ����ݡ��͸��ݹ�ʽ�������ݣ�
	int PState;			//��ʶ�Ƿ��Ѿ��õ���ȷ��ֵ
	time_t PTime;		//��ʶ�Ƿ��Ѿ��õ���ȷ��ֵ


	double GetValue();
	
	BOOL b_Calced;	//
	
private:
	int	m_Position;		//���ʽ�ַ�����ָ��
	bool IsFormula;		//��ʶ��ʽ�Ƿ���ȷ
	//	RTFUNC rtFunc;
	//	TAGINFO* pG;
	bool IsDigit(char byte);
	CString Substring(CString str,int pos1,int count);
	bool IsLetter(char byte);
	
	void SkipSpaces();
	CBTree* pmOP();
	CBTree* mdOP();
	CBTree* involOP();
	CBTree* logicOP();
	CBTree* logsecOP();
	CBTree* factorOP();
	CBTree* identiOP();
};

#endif // !defined(AFX_ELEMENTINOUT_H__E53ADFBA_6928_4B05_B88F_157D739312D7__INCLUDED_)
//typedef CTypedPtrList<CObList,CElementInOut*> CElementObjList;