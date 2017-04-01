// SubGroupInfTag.h: interface for the SubGroupInfTag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUBGROUPINFTAG_H__49E6D953_7CF8_4CEF_A1D7_E5AF03BF83EA__INCLUDED_)
#define AFX_SUBGROUPINFTAG_H__49E6D953_7CF8_4CEF_A1D7_E5AF03BF83EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//**************************************
//子组的信息
//**************************************
class SubGroupInfTag  
{
public:
	SubGroupInfTag();
	virtual ~SubGroupInfTag();
	CString ID;
	CString Name;
	CString Type;
	CString Desp;
	CString Mark;
};

#endif // !defined(AFX_SUBGROUPINFTAG_H__49E6D953_7CF8_4CEF_A1D7_E5AF03BF83EA__INCLUDED_)
