// CalcBlockControlFactory.h: interface for the CCalcBlockControlFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCBLOCKCONTROLFACTORY_H__656E02AD_778B_41AB_8E52_3A31E060324C__INCLUDED_)
#define AFX_CALCBLOCKCONTROLFACTORY_H__656E02AD_778B_41AB_8E52_3A31E060324C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCalcBlock;

class CCalcBlockControlFactory  
{
public:
// Implementation
	static CCalcBlock* CreateFromString( const CString& strType );

};

#endif // !defined(AFX_CALCBLOCKCONTROLFACTORY_H__656E02AD_778B_41AB_8E52_3A31E060324C__INCLUDED_)
