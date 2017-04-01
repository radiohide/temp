// BlockGridExt.h: interface for the CBlockGridExt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKGRIDEXT_H__0E80E098_8D95_4A26_84D0_E483E516E410__INCLUDED_)
#define AFX_BLOCKGRIDEXT_H__0E80E098_8D95_4A26_84D0_E483E516E410__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GRIDCTRL_SRC\GridCtrl.h"
//本控件显示选中计算块、输入块、输出到的块，分三列显示，输入列、块本身、输出列
//这是一个显示区域，调用而显示信息；单击而获知位置所在得知选择哪个
class CBlockGridExt : public CGridCtrl  
{
public:
	CBlockGridExt();
	virtual ~CBlockGridExt();

};

#endif // !defined(AFX_BLOCKGRIDEXT_H__0E80E098_8D95_4A26_84D0_E483E516E410__INCLUDED_)
