// WASPInPTLBlock.h: interface for the CWASPInPTLBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WASPINPTLBLOCK_H__648D6BD3_E42C_4F0C_9806_BA4C3B0EE61E__INCLUDED_)
#define AFX_WASPINPTLBLOCK_H__648D6BD3_E42C_4F0C_9806_BA4C3B0EE61E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WASPCalcBlock.h"

class CWASPInPTLBlock : public CWASPCalcBlock  
{
public:
	CWASPInPTLBlock();
	virtual ~CWASPInPTLBlock();
public:
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput (); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
	//���㺯����ʵ�ֱ�ģ��ļ���
	virtual void Calc(); //���м���ĺ���,��Ҫ����
};

#endif // !defined(AFX_WASPINPTLBLOCK_H__648D6BD3_E42C_4F0C_9806_BA4C3B0EE61E__INCLUDED_)
