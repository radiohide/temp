// PoteDll.h: interface for the CPoteDll class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POTEDLL_H__403C11AB_6F81_4924_A94C_6B2CB141F70F__INCLUDED_)
#define AFX_POTEDLL_H__403C11AB_6F81_4924_A94C_6B2CB141F70F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPoteDll  
{
public:
	CPoteDll(CString);
	~CPoteDll(void);
	bool IsDllValidate(LPCTSTR dllPath);
	void LoadDll(LPCTSTR dllPath);
	void UnloadDll();
	RTFUNC GetRtFunc(CString FuncName);

public:
	CString DllName;
	HMODULE hDllModule;
	CStringArray lstPoteFuncName;
	RTFUNC* lstPoteFuncAddr;

};

#endif // !defined(AFX_POTEDLL_H__403C11AB_6F81_4924_A94C_6B2CB141F70F__INCLUDED_)
