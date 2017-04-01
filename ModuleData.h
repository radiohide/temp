// ModuleData.h: interface for the ModuleData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODULEDATA_H__16579FC5_8FFF_4BE4_A3BF_333A2AC63846__INCLUDED_)
#define AFX_MODULEDATA_H__16579FC5_8FFF_4BE4_A3BF_333A2AC63846__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BlockPropData.h"
#include "BlockListData.h"

class ModuleData  
{
public:
	CString module_name;
	CString module_desp;
	int calcindex;
	int GraphCx;
	int GraphCy;
	CArray<BlockListData*,BlockListData*&> BlockListDataArr;
	CArray<BlockPropData*,BlockPropData*&> BlockPropDataArr;
public:
	ModuleData();
	virtual ~ModuleData();
	void ReadDate(_ConnectionPtr &pCon,CString strModelName);
	void WriteDate(_ConnectionPtr &pCon,CString strModelName);
private:
	void readBlockListData(_ConnectionPtr &pCon,CString strModelName);
	void readBlockPropData(_ConnectionPtr &pCon,CString strModelName);
};

#endif // !defined(AFX_MODULEDATA_H__16579FC5_8FFF_4BE4_A3BF_333A2AC63846__INCLUDED_)
