// BTree.h: interface for the CBTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BTREE_H__FBDCC013_AC73_4B2B_A18F_229312831FFB__INCLUDED_)
#define AFX_BTREE_H__FBDCC013_AC73_4B2B_A18F_229312831FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ElementInOut.h"
class CElementInOut;
class RuleREPoint;
class CBTree  
{
public:
	CBTree(void);
	~CBTree(void);

	int type;//0��ֵ��;1ͳ�Ƶ�;
	CPtrArray Leaf;
	unsigned short oprt;//�����
	CString oprtName;	//���ں�������
	double valueD;		//��ǰ������(���ʽ)��ֵ
	CElementInOut* RtPt;	//���ڲ�����

	RuleREPoint* pRuleREPoint;//�����Ƽ������У�����Ŀ��ֵ
};

#endif // !defined(AFX_BTREE_H__FBDCC013_AC73_4B2B_A18F_229312831FFB__INCLUDED_)
