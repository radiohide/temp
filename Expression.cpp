// Expression.cpp: implementation of the CExpression class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "Expression.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExpression::CExpression()
{
	m_pValue=NULL;
}

CExpression::~CExpression()
{
   if(m_pValue!=NULL) delete m_pValue;
}
void CExpression::Initial()
{
	m_pValue = new double[100];
}
