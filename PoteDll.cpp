// PoteDll.cpp: implementation of the CPoteDll class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PoteDll.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPoteDll::CPoteDll(CString strName)
{
	DllName = strName;
//	lstPoteFuncName = new CString[FUNC_MAX];
	lstPoteFuncName.SetSize(FUNC_MAX);
	lstPoteFuncAddr = new RTFUNC[FUNC_MAX];
}

CPoteDll::~CPoteDll(void)
{
	lstPoteFuncName.RemoveAll();
//	delete[] lstPoteFuncName;
	delete[] lstPoteFuncAddr;
}

//判断dll是否可用
bool CPoteDll::IsDllValidate(LPCTSTR dllPath)
{
	HINSTANCE hDll = ::LoadLibrary(dllPath);

	if( hDll )
	{
		void (*query)(int index, char* cbuf, int clen);
		query = (void (*)(int index, char* cbuf, int clen))::GetProcAddress(hDll, "QueryFC_1234567");
		if(query==NULL)
		{
			::FreeLibrary(hDll);
			hDll = NULL;
		}
		else
		{
			::FreeLibrary(hDll);
		}
	}
	return ( hDll!=NULL );
}

//加载dll
void CPoteDll::LoadDll(LPCTSTR dllPath)
{
	UnloadDll();
	// 建立外部动态连接库算法函数指针map
	hDllModule = ::LoadLibrary(dllPath);
	int i=0;
	if( hDllModule )
	{
		void (*query)(int index, char* cbuf, int clen);
		query = (void (*)(int index, char* cbuf, int clen))::GetProcAddress(hDllModule, "QueryFC_1234567");
		if(query!=NULL)
		{
			char funcc[128];
			memset(funcc, 0, 128);
			int index = 10001;
			do{
				(*query)(index, funcc, 127);
				if( lstrlen(funcc)>0 )
				{
					RTFUNC rtFunc = (RTFUNC)::GetProcAddress(hDllModule, funcc);
					CString sfuncc(funcc);	sfuncc.MakeUpper();
					lstPoteFuncName[i]=sfuncc;
					lstPoteFuncAddr[i]=rtFunc;
					i++;
				}
				index++;
			}
			while( lstrlen(funcc)>0 );
		}
	}
}

void CPoteDll::UnloadDll()
{
	if(hDllModule) 
		::FreeLibrary(hDllModule);
}

RTFUNC CPoteDll::GetRtFunc(CString FuncName)
{
	for (int i=0; i<lstPoteFuncName.GetSize();i++)
		if (lstPoteFuncName[i]==FuncName)
			return (RTFUNC)lstPoteFuncAddr[i];

	return NULL;
}
