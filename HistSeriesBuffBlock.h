// HistSeriesBuffBlock.h: interface for the CHistSeriesBuffBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTSERIESBUFFBLOCK_H__6339C031_D3B7_483C_9B39_B3E813CCB56B__INCLUDED_)
#define AFX_HISTSERIESBUFFBLOCK_H__6339C031_D3B7_483C_9B39_B3E813CCB56B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcBlock.h"
#include "PropertyDlgHistSeriesBuff.h"
#include <vector>
class CHistSeriesBuffBlock : public CCalcBlock  
{
public:
	CHistSeriesBuffBlock();
	virtual ~CHistSeriesBuffBlock();

	CList<TagData*,TagData*>  m_listDataBuffer;
	CString	m_strPointName;
	int		m_iDataSource;
	int		m_iReadModel;
	int		m_ISnapStep;
	int		m_iOutputTime;
	int		m_iBuffTime;

	HINSTANCE  hDLL;

	//1��������ʱ��Ӧ���ڵ������ʱ�䣬���������д��ڵ�����
	//2��������������ˣ��򻺳���β�������ͬʱ����Ҫɾ��ͷ����������
	//3����һ����仺����ʱ��Ӧ�����������һ�����ʱ���ӻ�������ȡ�����ݿ��ܲ�����
	//�˱����������3����Ƶ�
	BOOL  bSureFull;
	//����̬ʱ��ν�С 10����һ��
	std::vector<long> Temp_begintime;
	std::vector<long> Temp_endtime;
public:
	void RemoveElementFromBuffer(long lEndOfInputTime);
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

#endif // !defined(AFX_HISTSERIESBUFFBLOCK_H__6339C031_D3B7_483C_9B39_B3E813CCB56B__INCLUDED_)
