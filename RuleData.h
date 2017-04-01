// RuleData.h: interface for the CRuleData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RULEDATA_H__BB4A07AE_DB33_47A7_B06A_04688D01B020__INCLUDED_)
#define AFX_RULEDATA_H__BB4A07AE_DB33_47A7_B06A_04688D01B020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRuleData  
{
public:
	CStringArray *m_pStrNameArrDB;
	CStringArray *m_ptypeFBArrDB;

	int  *m_piTagToFOrB;//ǰ������ͷ��������ǣ�0:ǰ��(eg:��״̬������)(Ĭ��),1:����(eg:��״̬������)
	CStringArray *m_NameFArr;
	CStringArray *m_NameBArr;
	CArray<int,int> *m_piDisValueFArr;//ǰ������ɢֵ
	CArray<int,int> *m_piDisValueBArr;//�������ɢֵ
	CString *m_pStrInPutConCode;
	CArray<int,int> *m_pTagBeInFArr;
	CArray<int,int> *m_pTagBeInBArr;

	CString m_strConCode;
	int m_iWeight;
	long m_learRID;
	long m_llateRID;
	CArray<int,int> m_iDisValue;
	int m_iDiffValue;
public:
	void CalcDiffValue();
	int GetDisValueByName(CString strVarName);
	CRuleData();
	virtual ~CRuleData();
private:
	bool beDiffValueByName(CString strName,int iDisValue);//���в��췵��true
	int calcArrDif(CStringArray *nameArr,CArray<int,int> *pTagBeInArr,CArray<int,int> *piDisValueArr);
};

#endif // !defined(AFX_RULEDATA_H__BB4A07AE_DB33_47A7_B06A_04688D01B020__INCLUDED_)
