// WorkSpaceElement.h: interface for the CWorkSpaceElement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACEELEMENT_H__AD4D646B_FAA6_4FCF_A1FF_5CE61A66AC40__INCLUDED_)
#define AFX_WORKSPACEELEMENT_H__AD4D646B_FAA6_4FCF_A1FF_5CE61A66AC40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCalcBlock;
class CWorkSpaceElement  :public CObject
{
public:
	char LongID[128];	//点号＝名称
	char sDsp[128];	    //描述
	char sUnit[128];    //单位

	void SetParentBlock(CCalcBlock* pBlock);
	CCalcBlock *m_pParentBlock;
	//int  m_PortType;    //单值、（时间、数值）序列、数值序列、时间区间序列、数值区间序列
//////////////////////
	int	PType;			//点类型，0为采集点，1为计算点，2为系数点
	int pTypeGet;		//采集类型,0表示从关系库取;1的表示从实时库取
	double PValue;		//点的结果（包括采集数据、和根据公式计算数据）
	int PState;			//标识是否已经得到正确点值
	time_t PTime;		//标识是否已经得到正确点值
	
	
	CWorkSpaceElement();
	virtual ~CWorkSpaceElement();
};

#endif // !defined(AFX_WORKSPACEELEMENT_H__AD4D646B_FAA6_4FCF_A1FF_5CE61A66AC40__INCLUDED_)
typedef CTypedPtrList<CObList,CWorkSpaceElement*> CElementList;