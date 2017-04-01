// CalcModule.h: interface for the CCalcModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCMODULE_H__270912CE_DCAD_4EDE_9CC7_EFB72C516A64__INCLUDED_)
#define AFX_CALCMODULE_H__270912CE_DCAD_4EDE_9CC7_EFB72C516A64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//����һ������ģ�飬�������ɶ������飬ģ�����˳������
#include "CalcBlock.h"

class CCalcModel;
class CCalcModule:public CObject
{
public:
	CCalcModule();
	virtual ~CCalcModule();

	CCalcBlock* GetBlockByIndex(int k);
	CCalcBlock* FindBlockByName(CString str);
	void AddBlock(CCalcBlock* pBlock);

	void CheckBlockCalcIntegrity(CStringArray& LogStrings);
	void AdjustBlockCalcOrder(); //���ݸ���ĺ��룬���������˳��
	void InitforCalc(CCalcModel* pModel);
	void Calc();

	void ClearAllBlocks(); //������еļ����

	void SetModuleName(CString str){strcpy(m_Name,str);}
	CString GetModuleName() {CString str =m_Name; return str;}
	void SetModuleDesp(CString str){strcpy(m_Desp,str);}
	CString GetModuleDesp() {CString str =m_Desp; return str;}
	void SetParentName(CString *pstr) {m_pStrParentModelName=pstr;}
	CString GetParentName() {return *m_pStrParentModelName;}
	void SetCalcOrderID(int orderID);
	int GetCalcOrderID();
	int GetCounterByBType(CString strBType);//�õ�ĳ���ͼ����ĸ���
public:
	CString FindCanUseBlockName(CString strBlockType=_T(""));
	int GetBlockCount();
	void GetBlockInputValueFromGlobalWS();
	void SetWriteLogOnOrOff(int iOnOrOff);//������־����
	void SetTraceOnOrOff(int iOnOrOff);//����׷�ٿ���
	void SetDebugOnOrOff(int iOnOrOff);//���õ��Կ���
	void RunCompile();
	void InitCompile(int *pModelWarningCount,int *pModelErrorCoutn,CWnd *pWnd,int iMsg,CLog *pLog);
	void ResetStopCalc();//ֹͣ��־��λ//for stop
	void StopCalc();//for stop
	BOOL CheckTableExist(_ConnectionPtr &pCon);
	BOOL ReadModule(_ConnectionPtr &pCon);
	
	CCalcBlock* CreateBlock(CString strType,CString strName,CString strDesp);

	char m_Name[128]; //ģ������,����Ϊģ���б������ΪӢ��
	char m_Desp[128]; //ģ������

    CString* m_pStrParentModelName;

	CBlockList m_BlockList;

	BOOL m_bCompared; //˵�������������ʱ����
protected:
	//�����˳��
	int m_iCalcOrderID;
private:
	 CStringArray s_BlockTypeArray;
	 CStringArray s_BlockIDArray;
	int m_iStopTag;//0:���У�1:ֹͣ//for stop

	int m_iModuleWarningCount;
	int	m_iModuleErrorCount;
	int *m_pModelWarningCount;
	int *m_pModelErrorCoutn;
	CWnd *m_pWnd;
	int m_iMsg;
	CLog *pLog;
	int m_iLogOnOrOff;//��־�����1:����0����
	int m_iTraceOn;//׷�ٿ���1:����0:��
	int m_iDebugOn;//���Կ���1:����0:��
};

#endif // !defined(AFX_CALCMODULE_H__270912CE_DCAD_4EDE_9CC7_EFB72C516A64__INCLUDED_)
typedef CTypedPtrList<CObList,CCalcModule*> CModuleList;