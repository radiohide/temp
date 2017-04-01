// CalcPort.h: interface for the CCalcPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCPORT_H__E4751111_35D5_4D0D_AB33_E2E79F4D370F__INCLUDED_)
#define AFX_CALCPORT_H__E4751111_35D5_4D0D_AB33_E2E79F4D370F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//计算的端口对象
#include "UniValue.h"
class CCalcBlock;
class CCalcPort:public CObject
{
public:
	CCalcPort();
	CCalcPort(BOOL bOut,CString strPortID); //另一个构造函数
	virtual ~CCalcPort();
public:
	//输入的连接属性
	void SetLinkFormBlockName(CString strBlockName);
	CString GetLinkFormBlockName();
	void SetLinkFromBlockPortName(CString strPortName);
	CString GetLinkFromBlockPortName();
	void SetPortLinked(BOOL bLink);
	BOOL GetPortLinked();  
   
	//设置端口属性
	void SetPortID(CString strName); //设置该端口在内部的变量名称
	void SetPortDesp(CString str);   //设置该端口的描述
	void SetPortUnit(CString str);   //设置端口的单位
    void SetPortLinkID(CString str);  //设置输入型端口连接哪个输出，或输出端口输出的名称
	void SetBlockPtr(CCalcBlock *ptr);
	CString GetPortID(){return m_portID;} 
	CString GetPortDesp(){return m_sDsp;} 
	CString GetPortUnit(){return m_sUnit;} 
	CString GetPortLinkID(){return m_sLinkID;} 
	CCalcBlock *GetBlockPtr(){return m_ptrBlock;}
	

	void SetPortAsInOrOut(BOOL bInOut);//是输出还是输入端口
	void SetPortIsOut2WS(BOOL bOut2WS);//设置端口数据是否输出到全局WS中
    void SetPortIsValueForce(BOOL bForce);//设置是否为强制输出状态
	void SetPortValueType(int type);  //设置端口的数据是那种类型
	int   GetPortValueType() ;
	
	//端口的输入输出的处理
	void PutValueToGlobalWS();//把端口的值放到其对应的全局WS中，当端口为输出时使用
	void MapPortIntoGlobalWS(); //把端口名称映射到全局WS中，当端口为输出时使用。
	//double GetPortValue(); 
	//void      SetPortValue(double value);

	CUniValue& GetPortUniValue(); 
	void      SetPortUniValue(CUniValue value);
public:
	char m_portID[128];	//端口号＝名称
	char m_sLinkID[128]; //来源于哪里、或去哪里
	char m_sDsp[128];	//描述
	char m_sUnit[128];  //单位
	CCalcBlock *m_ptrBlock;//该端口所属的计算块

	BOOL   m_bOut2WS;    //是否放到WS中
	BOOL   m_bIsOutPort;// 是否是输出
	BOOL   m_bForceValue;//

	double m_Value_Force; //强制的输出
	int    m_CheckState;   //校验状态，仅仅用于测点的校验 BAD＝0；GOOD=1;
	int    m_LimitState;  //值限制的状态＝0：GOOD,1:超高限；＝－1；超低限

	int    m_ValueType;

	double m_DoubleValue;     //端口数值
	long   m_lTime;     //端口数值对应时间

	CUniValue  m_UniValue;
protected:
	 CString m_strLinkPortName;
	CString m_strLinkBlockName;
	BOOL    m_bLinked;


};

#endif // !defined(AFX_CALCPORT_H__E4751111_35D5_4D0D_AB33_E2E79F4D370F__INCLUDED_)
typedef CTypedPtrList<CObList,CCalcPort*> CPortList;
typedef CTypedPtrArray<CObArray,CCalcPort*> CPortArray;