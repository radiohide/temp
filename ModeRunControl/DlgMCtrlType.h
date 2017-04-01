// DlgMCtrlType.h: interface for the CDlgMCtrlType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGMCTRLTYPE_H__FF6888AE_66CE_4D4D_93D5_0DED1C4F2FC1__INCLUDED_)
#define AFX_DLGMCTRLTYPE_H__FF6888AE_66CE_4D4D_93D5_0DED1C4F2FC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MCtrlType.h"

class CDlgMCtrlType : public CDialog  
{
public:
	CDlgMCtrlType(UINT nIDTemplate,CWnd* pParent = NULL);
	virtual ~CDlgMCtrlType();
	void SetMCtrlData(MCtrlType *pMCtrlType);
	virtual void ShowCtrlData();
	virtual void SetCtrlData();

	void ResetCtrlData();
	void SaveCtrlDataToDB();
	void DelCtrlDataIoDB();
public:
	MCtrlType *m_pMCtrlType;
	CString m_strTypeName;

};

#endif // !defined(AFX_DLGMCTRLTYPE_H__FF6888AE_66CE_4D4D_93D5_0DED1C4F2FC1__INCLUDED_)
