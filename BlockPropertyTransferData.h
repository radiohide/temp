// BlockPropertyTransferData.h: interface for the CBlockPropertyTransferData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKPROPERTYTRANSFERDATA_H__94B88D09_BD16_4ADE_9B40_72FC918E699F__INCLUDED_)
#define AFX_BLOCKPROPERTYTRANSFERDATA_H__94B88D09_BD16_4ADE_9B40_72FC918E699F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBlockPropertyTransferData  
{
public:
	CBlockPropertyTransferData();
	virtual ~CBlockPropertyTransferData();
public:
	char m_cModelName[64];//模型名字
	char m_cModuleName[64];//模块名字
	
	char m_cBlockName[64];//模块名字
	char m_cPropName[64];//属性名字
	char m_cValueItem1[128];//属性值1
	char m_cValueItem2[128];//属性值2
	char m_cValueItem3[128];//属性值3
	char m_cValueItem4[128];//属性值4
	char m_cValueItem5[128];//属性值5
};

#endif // !defined(AFX_BLOCKPROPERTYTRANSFERDATA_H__94B88D09_BD16_4ADE_9B40_72FC918E699F__INCLUDED_)
