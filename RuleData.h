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

	int  *m_piTagToFOrB;//前向推理和反向推理标记，0:前向(eg:从状态到性能)(默认),1:后向(eg:从状态到操作)
	CStringArray *m_NameFArr;
	CStringArray *m_NameBArr;
	CArray<int,int> *m_piDisValueFArr;//前件的离散值
	CArray<int,int> *m_piDisValueBArr;//后件的离散值
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
	bool beDiffValueByName(CString strName,int iDisValue);//若有差异返回true
	int calcArrDif(CStringArray *nameArr,CArray<int,int> *pTagBeInArr,CArray<int,int> *piDisValueArr);
};

#endif // !defined(AFX_RULEDATA_H__BB4A07AE_DB33_47A7_B06A_04688D01B020__INCLUDED_)
