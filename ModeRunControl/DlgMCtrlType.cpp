// DlgMCtrlType.cpp: implementation of the CDlgMCtrlType class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\ePerfManage.h"
#include "DlgMCtrlType.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDlgMCtrlType::CDlgMCtrlType(UINT nIDTemplate,CWnd* pParent): 
	CDialog(nIDTemplate, pParent)
{

}

CDlgMCtrlType::~CDlgMCtrlType()
{

}
void CDlgMCtrlType::SetMCtrlData(MCtrlType *pMCtrlType)
{
	m_pMCtrlType=pMCtrlType;
}
void CDlgMCtrlType::ShowCtrlData()
{

}
void CDlgMCtrlType::ResetCtrlData()
{
	m_pMCtrlType->ResetCtrlData();
}
void CDlgMCtrlType::SetCtrlData()
{
}
void CDlgMCtrlType::SaveCtrlDataToDB()
{
	SetCtrlData();
	m_pMCtrlType->SaveCtrlDataToDB();
}
void CDlgMCtrlType::DelCtrlDataIoDB()
{
	m_pMCtrlType->DelCtrlDataIoDB();
}