// HistSeriesBlock.h: interface for the CHistSeriesBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTSERIESBLOCK_H__EFF05324_8313_4C4F_8463_2D9C1B332CB0__INCLUDED_)
#define AFX_HISTSERIESBLOCK_H__EFF05324_8313_4C4F_8463_2D9C1B332CB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include "PropertyDlgHistSeries.h"
#include <vector>
class CHistSeriesBlock : public CCalcBlock  
{
public:
	CHistSeriesBlock();
	virtual ~CHistSeriesBlock();

	CString	m_strPointName;
	int		m_iDataSource;
	int		m_iReadModel;
	int		m_ISnapStep;

	HINSTANCE  hDLL;

	//����̬ʱ��ν�С 10����һ��
	std::vector<long> Temp_begintime;
	std::vector<long> Temp_endtime;
public:
	void Separate(long stablebegin, long stableend);
	//�������صĺ���
	//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
	virtual void ConstructBlockInputOutput(); 
	//���������½�����ĺ���
	static CCalcBlock* CreateFromString( const CString& strType );
	//��ʼ������ĺ���������ĳ�ʼ��
	virtual void InitCalc();
	//���㺯����ʵ�ֱ�ģ��ļ���
	virtual void Calc(); //���м���ĺ���,��Ҫ����
    //���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
	virtual void SetPropValue(CString strPropName,CString strItem1,CString strItem2="",CString strItem3="",CString strItem4="",CString strItem5="");
	//��������ʾ��ͬ�����Զ���Ի���
    virtual void ShowPropertyDialog( CWnd* parent, BOOL show );
	//�����Խ���ģ��д�뵽���ݿ���
	virtual void PutPropertyToRecord(_RecordsetPtr& pRecord);
	//����ͼ��ʱ����Ҫ�������ԡ�
	virtual void CopyPropertyData(CCalcBlock* pBlock);
	//�����Է��ô���������
	virtual void SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr);
};

#endif // !defined(AFX_HISTSERIESBLOCK_H__EFF05324_8313_4C4F_8463_2D9C1B332CB0__INCLUDED_)
