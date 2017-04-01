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
	char LongID[128];	//��ţ�����
	char sDsp[128];	    //����
	char sUnit[128];    //��λ

	void SetParentBlock(CCalcBlock* pBlock);
	CCalcBlock *m_pParentBlock;
	//int  m_PortType;    //��ֵ����ʱ�䡢��ֵ�����С���ֵ���С�ʱ���������С���ֵ��������
//////////////////////
	int	PType;			//�����ͣ�0Ϊ�ɼ��㣬1Ϊ����㣬2Ϊϵ����
	int pTypeGet;		//�ɼ�����,0��ʾ�ӹ�ϵ��ȡ;1�ı�ʾ��ʵʱ��ȡ
	double PValue;		//��Ľ���������ɼ����ݡ��͸��ݹ�ʽ�������ݣ�
	int PState;			//��ʶ�Ƿ��Ѿ��õ���ȷ��ֵ
	time_t PTime;		//��ʶ�Ƿ��Ѿ��õ���ȷ��ֵ
	
	
	CWorkSpaceElement();
	virtual ~CWorkSpaceElement();
};

#endif // !defined(AFX_WORKSPACEELEMENT_H__AD4D646B_FAA6_4FCF_A1FF_5CE61A66AC40__INCLUDED_)
typedef CTypedPtrList<CObList,CWorkSpaceElement*> CElementList;