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
class CFlowchartEditor;//该计算块的图形界面对象,calcserv不需要
class CCalcBlock  :public CObject
{
public:

	CCalcBlock();
	virtual ~CCalcBlock();
	///////////////////////////////////////////////////////////////////////
    //涉及到图形的函数
	//画特征形状以区别Add at 2011-10-30
	virtual void DrawShape(CDC* dc, CRect rect,int CutSize); //
    BOOL m_bShowDesp;//是否显示DESP
	BOOL m_bShowInPortText; //是否显示输入端口的名称文字
	BOOL m_bShowOutPortText; //是否显示输入端口的名称文字
	////////////////////////////////////////////////////////////////////////
protected:
	CFlowchartEditor *m_pEditorObj;//该计算块的图形界面对象
public:
	void  ChangeInPortName(CString strOldPortName,CString strNewPortName);//只修改输入端口名称
	void  ChangeOutPortName(CString strOldPortName,CString strNewPortName);//修改输出端口名称，并修改于之相连的连接接
	void SetFlowchartEditorPObj(CFlowchartEditor *pEditorObj);
	CFlowchartEditor *GetFlowchartEditorPObj();
	virtual void Copy( CCalcBlock* pBlock);
	
	static  CCalcBlock* CreateFromString( const CString& strType );
	 void CreatePortsFromString(int InNum,int OutNum,CString InNames,CString strInValueTypes,CString OutNames,CString strOutValueTypes, CString InputLinkStrs);

	virtual void CopyPropertyData(CCalcBlock* pBlock);//复制图形时，需要拷贝属性。
	virtual void RunCompile();//编译自身
	virtual void Calc();      //进行计算的函数
	virtual BOOL ReadBlock(_ConnectionPtr &pCon,CString strPropTable);
	virtual void InitCalc();   //初始化函数，在
	virtual void ConstructBlockInputOutput(){}; //根据不同的对象进行构造固定的输入输出
	//重载以从读取到的属性数据中进行属性的设置
	virtual	void SetPropValue(CString strPropName,CString strItem1,CString strItem2="",CString strItem3="",CString strItem4="",CString strItem5="") {};
    virtual void ShowPropertyDialog( CWnd* parent, BOOL show ){};
	virtual void PutPropertyToRecord(_RecordsetPtr& pRecord){};
	//////////////////////////////////////////////////////////
	virtual void  ShowBlockInfoInListCtrl(CListCtrl* pListCtrl);
	virtual CString GetBlockStrPro();
	virtual void SetBlockStrPro(CString str);//设置计算块的字符串属性
protected:
	virtual void Clear(); //清除相关的属性值，为COPY做准备

	//void CalcExpressionList(); //计算
	CUniValue  GetInputValueFromGlobalWS_direct(CString strName);
	
	void OutputResultToGlobalWS();
	
	void ClearOutPortLink(CCalcPort* pPort,CWnd* parent=NULL);
	void ClearInputPorts();
	void ClearOutputPorts(CWnd* parent=NULL);
	void ClearOutputPortByIndex(int Index,CWnd* parent=NULL);
	void ClearInputPortByIndex(int Index);
	//清空发送的数据Arr
	void ClearPropertyTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr);
public:
	//用于内部进行计算的共享，假设内容有若干公式要进行计算，可定义内部公式对象
	CMap<CString,LPCTSTR,CElementInOut*,CElementInOut*&> block_WorkSpaceList;				//输出元素，工作空间，元素在数据总线
    CMap<CString,LPCTSTR,CElementInOut*,CElementInOut*&> blockIter_WorkSpaceList;	//迭代中使用，用于在迭代前计算，并给迭代中的迭代元素赋值
	char m_sName[128]; //块名称
	char m_sType[50];  //块类型=normal,coalprocess
	char m_sDesp[128]; //块描述

	BOOL m_bCompared;
    
public:
	CCalcPort * InSertInputPortBefore(int index,CString strPortID,int ValueType,CString strDesp="in",CString strUnit="");
	CCalcPort * InSertInputPortBehind(int index,CString strPortID,int ValueType,CString strDesp="in",CString strUnit="");
	CCalcPort * InSertOutputPortBefore(int index,CString strPortID,int ValueType,CString strDesp="in",CString strUnit="");
	CCalcPort * InSertOutputPortBehind(int index,CString strPortID,int ValueType,CString strDesp="in",CString strUnit="");

	void GetInputValueFromGlobalWS();//
	void InitCompile(int *pModelWarningCount,int *pModelErrorCount,CWnd *pWnd,int iMsg,CLog *pLog,
					int *pModuleWarningCount,int *pModuleErrorCount);
	void SetWriteLogOnOrOff(int iOnOrOff);//设置日志开关
	void SetTraceOnOrOff(int iOnOrOff);//设置追踪开关
	void SetDebugOnOrOff(int iOnOrOff);//设置调试开关
	BOOL IsLinkVarExistInGlobalWS(CString strVarName);//判断块的端口是否有全局变量对应
	void SetOrderID(int id);
	int GetOrderID();
	//获得
	void SetModelPtr(CCalcModel* p) {m_pModel=p;}
	CCalcModel* GetModelPtr() {return m_pModel;}

	//设置ModulePtr
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
	int  GetOutputNum(); //获得输出的个数
	virtual int  GetInputNum();  //获得输入的个数
	void ClearAllPorts(CWnd* parent=NULL);

    virtual BOOL CheckBlockCalcIntegrity();
    void SetInputIntegrity(BOOL  var);
	BOOL  GetInputIntegrity();

//Socket操作属性值
	virtual void SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr);//将属性放置传输数据中
	void SetTransferDataToBlockProperty(
			const CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr);//将传输数据放置到模块中
	void SendBlockPropertyTransferData(
		const CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr);//发送数据
//Socket操作属性值
	
protected:
	void AddOutputNode(CCalcPort* pPort);//添加一个输出节点
	void AddInputNode(CCalcPort* pPort); //添加一个输入节点

    CPortList m_listIn;	// 输入列表
	CPortList m_listOut;	// 输出列表
	CCalcModel* m_pModel;
	CCalcModule* m_pModule;

	int  m_iOrderID;
	BOOL m_bInputIntegrity;

	CSocketServ* pSockServ;//连接计算站

	CWnd *m_pWnd;
	int m_iMsg;
	CLog *m_pLog;
	int m_iLogOnOrOff;//日志开或关1:开，0：关
	int m_iTraceOn;//追踪开关1:开，0:关
	int m_iDebugOn;//调试开关1:开，0:关
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