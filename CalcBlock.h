// CalcBlock.h: interface for the CCalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCBLOCK_H__795D3B40_CD3E_4F2F_892F_975B64387D07__INCLUDED_)
#define AFX_CALCBLOCK_H__795D3B40_CD3E_4F2F_892F_975B64387D07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WorkSpaceElement.h"
#include "ElementInOut.h"
#include "CalcPort.h"
#include "SocketServ.h"
#include "BlockPropertyTransferData.h"


class CElementInOut;
class CCalcModel;
class CCalcModule;
class CFlowchartEditor;//�ü�����ͼ�ν������,calcserv����Ҫ
class CCalcBlock  :public CObject
{
public:

	CCalcBlock();
	virtual ~CCalcBlock();
	///////////////////////////////////////////////////////////////////////
    //�漰��ͼ�εĺ���
	//��������״������Add at 2011-10-30
	virtual void DrawShape(CDC* dc, CRect rect,int CutSize); //
    BOOL m_bShowDesp;//�Ƿ���ʾDESP
	BOOL m_bShowInPortText; //�Ƿ���ʾ����˿ڵ���������
	BOOL m_bShowOutPortText; //�Ƿ���ʾ����˿ڵ���������
	////////////////////////////////////////////////////////////////////////
protected:
	CFlowchartEditor *m_pEditorObj;//�ü�����ͼ�ν������
public:
	void  ChangeInPortName(CString strOldPortName,CString strNewPortName);//ֻ�޸�����˿�����
	void  ChangeOutPortName(CString strOldPortName,CString strNewPortName);//�޸�����˿����ƣ����޸���֮���������ӽ�
	void SetFlowchartEditorPObj(CFlowchartEditor *pEditorObj);
	CFlowchartEditor *GetFlowchartEditorPObj();
	virtual void Copy( CCalcBlock* pBlock);
	
	static  CCalcBlock* CreateFromString( const CString& strType );
	 void CreatePortsFromString(int InNum,int OutNum,CString InNames,CString strInValueTypes,CString OutNames,CString strOutValueTypes, CString InputLinkStrs);

	virtual void CopyPropertyData(CCalcBlock* pBlock);//����ͼ��ʱ����Ҫ�������ԡ�
	virtual void RunCompile();//��������
	virtual void Calc();      //���м���ĺ���
	virtual BOOL ReadBlock(_ConnectionPtr &pCon,CString strPropTable);
	virtual void InitCalc();   //��ʼ����������
	virtual void ConstructBlockInputOutput(){}; //���ݲ�ͬ�Ķ�����й���̶����������
	//�����ԴӶ�ȡ�������������н������Ե�����
	virtual	void SetPropValue(CString strPropName,CString strItem1,CString strItem2="",CString strItem3="",CString strItem4="",CString strItem5="") {};
    virtual void ShowPropertyDialog( CWnd* parent, BOOL show ){};
	virtual void PutPropertyToRecord(_RecordsetPtr& pRecord){};
	//////////////////////////////////////////////////////////
	virtual void  ShowBlockInfoInListCtrl(CListCtrl* pListCtrl);
	virtual CString GetBlockStrPro();
	virtual void SetBlockStrPro(CString str);//���ü������ַ�������
protected:
	virtual void Clear(); //�����ص�����ֵ��ΪCOPY��׼��

	//void CalcExpressionList(); //����
	CUniValue  GetInputValueFromGlobalWS_direct(CString strName);
	
	void OutputResultToGlobalWS();
	
	void ClearOutPortLink(CCalcPort* pPort,CWnd* parent=NULL);
	void ClearInputPorts();
	void ClearOutputPorts(CWnd* parent=NULL);
	void ClearOutputPortByIndex(int Index,CWnd* parent=NULL);
	void ClearInputPortByIndex(int Index);
	//��շ��͵�����Arr
	void ClearPropertyTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr);
public:
	//�����ڲ����м���Ĺ����������������ɹ�ʽҪ���м��㣬�ɶ����ڲ���ʽ����
	CMap<CString,LPCTSTR,CElementInOut*,CElementInOut*&> block_WorkSpaceList;				//���Ԫ�أ������ռ䣬Ԫ������������
    CMap<CString,LPCTSTR,CElementInOut*,CElementInOut*&> blockIter_WorkSpaceList;	//������ʹ�ã������ڵ���ǰ���㣬���������еĵ���Ԫ�ظ�ֵ
	char m_sName[128]; //������
	char m_sType[50];  //������=normal,coalprocess
	char m_sDesp[128]; //������

	BOOL m_bCompared;
    
public:
	CCalcPort * InSertInputPortBefore(int index,CString strPortID,int ValueType,CString strDesp="in",CString strUnit="");
	CCalcPort * InSertInputPortBehind(int index,CString strPortID,int ValueType,CString strDesp="in",CString strUnit="");
	CCalcPort * InSertOutputPortBefore(int index,CString strPortID,int ValueType,CString strDesp="in",CString strUnit="");
	CCalcPort * InSertOutputPortBehind(int index,CString strPortID,int ValueType,CString strDesp="in",CString strUnit="");

	void GetInputValueFromGlobalWS();//
	void InitCompile(int *pModelWarningCount,int *pModelErrorCount,CWnd *pWnd,int iMsg,CLog *pLog,
					int *pModuleWarningCount,int *pModuleErrorCount);
	void SetWriteLogOnOrOff(int iOnOrOff);//������־����
	void SetTraceOnOrOff(int iOnOrOff);//����׷�ٿ���
	void SetDebugOnOrOff(int iOnOrOff);//���õ��Կ���
	BOOL IsLinkVarExistInGlobalWS(CString strVarName);//�жϿ�Ķ˿��Ƿ���ȫ�ֱ�����Ӧ
	void SetOrderID(int id);
	int GetOrderID();
	//���
	void SetModelPtr(CCalcModel* p) {m_pModel=p;}
	CCalcModel* GetModelPtr() {return m_pModel;}

	//����ModulePtr
	void SetModulePtr(CCalcModule* p) {m_pModule=p;}
	CCalcModule* GetModulePtr() {return m_pModule;}
	//get the value 
	CString GetOutputName(int index);
	CString GetOutputDesp(int index);
	void SetOutputName(int index,CString str);
	void SetOutputDesp(int index,CString str);
    
	BOOL GetOutputValue(int index,CUniValue* UniValue);
	BOOL GetOutputValue(CString strName,CUniValue* UniValue);

	BOOL GetInputValue(CString strName,CUniValue* UniValue);
	BOOL GetInputValue(int index,CUniValue* UniValue);

	//about block property
	void SetBlockName(CString str) {strcpy(m_sName,str);}
	CString GetBlockName() {return m_sName;}
	void SetBlockDesp(CString str) {strcpy(m_sDesp,str);}
	CString GetBlockDesp() {return m_sDesp;}
	void SetBlockType(CString str) {strcpy(m_sType,str);}
	CString GetBlockType() {return m_sType;}
	//about config block
	virtual CCalcPort* AddInputPort(CString strPortID,int ValueType,CString strDesp="in",CString strUnit="");
	//virtual void AddExpression(CString strResultID,CString strExpress,CString strDesp="express");
	CCalcPort* AddOutputPort(CString strPortID,int ValueType,CString strDesp="out",CString strUnit="");
    //about link
	void SetInputSourceID(int index,CString strID);
	CString GetOutLinkID(int index);

	void ForceOutputPortValue(int index,double value); 
    void SetForceValueState(int index,BOOL val);

	CCalcPort* GetInputPortObj(int index);
	CCalcPort* GetOutputPortObj(int index);

	CCalcPort* GetInputPortObj(CString strName);
	CCalcPort* GetOutputPortObj(CString strName);

	void MapOutputToGlobalWS();
	int  GetOutputNum(); //�������ĸ���
	virtual int  GetInputNum();  //�������ĸ���
	void ClearAllPorts(CWnd* parent=NULL);

    virtual BOOL CheckBlockCalcIntegrity();
    void SetInputIntegrity(BOOL  var);
	BOOL  GetInputIntegrity();

//Socket��������ֵ
	virtual void SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr);//�����Է��ô���������
	void SetTransferDataToBlockProperty(
			const CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr);//���������ݷ��õ�ģ����
	void SendBlockPropertyTransferData(
		const CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr);//��������
//Socket��������ֵ
	
protected:
	void AddOutputNode(CCalcPort* pPort);//���һ������ڵ�
	void AddInputNode(CCalcPort* pPort); //���һ������ڵ�

    CPortList m_listIn;	// �����б�
	CPortList m_listOut;	// ����б�
	CCalcModel* m_pModel;
	CCalcModule* m_pModule;

	int  m_iOrderID;
	BOOL m_bInputIntegrity;

	CSocketServ* pSockServ;//���Ӽ���վ

	CWnd *m_pWnd;
	int m_iMsg;
	CLog *m_pLog;
	int m_iLogOnOrOff;//��־�����1:����0����
	int m_iTraceOn;//׷�ٿ���1:����0:��
	int m_iDebugOn;//���Կ���1:����0:��
private:
	void putPropertyDataArrToCharArr(const CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr,CArray<char,char> &bufArr);
	void putPCharToCharArr(char *pChar,int count,CArray<char,char> &bufArr);
	BOOL sendCharArr(CArray<char,char> &bufArr,CSocketServ* pSocket);
	BOOL sendOnePackage(char *pChar,int iLength,CSocketServ* pSocket);

	int *m_pModuleWarningCount;
	int	*m_pModuleErrorCount;
	int *m_pModelWarningCount;
	int *m_pModelErrorCoutn;
};

#endif // !defined(AFX_CALCBLOCK_H__795D3B40_CD3E_4F2F_892F_975B64387D07__INCLUDED_)
typedef CTypedPtrList<CObList,CCalcBlock*> CBlockList;