// Expression.h: interface for the CExpression class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPRESSION_H__CA989BD1_20CD_4981_9F25_FD2AB57F44EB__INCLUDED_)
#define AFX_EXPRESSION_H__CA989BD1_20CD_4981_9F25_FD2AB57F44EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CExpression : public CObject  
{
public:
	CExpression();
	virtual ~CExpression();
	void Initial();
	void SetFormula(CString str){m_strPFormula=str;}
	CString GetFormula(){return m_strPFormula;}
protected:
	double * m_pValue;
	CString m_strPFormula;
};

#endif // !defined(AFX_EXPRESSION_H__CA989BD1_20CD_4981_9F25_FD2AB57F44EB__INCLUDED_)
typedef CTypedPtrList<CObList,CExpression*> CExpressionList;