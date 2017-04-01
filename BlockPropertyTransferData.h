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
	char m_cModelName[64];//ģ������
	char m_cModuleName[64];//ģ������
	
	char m_cBlockName[64];//ģ������
	char m_cPropName[64];//��������
	char m_cValueItem1[128];//����ֵ1
	char m_cValueItem2[128];//����ֵ2
	char m_cValueItem3[128];//����ֵ3
	char m_cValueItem4[128];//����ֵ4
	char m_cValueItem5[128];//����ֵ5
};

#endif // !defined(AFX_BLOCKPROPERTYTRANSFERDATA_H__94B88D09_BD16_4ADE_9B40_72FC918E699F__INCLUDED_)
