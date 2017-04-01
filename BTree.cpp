// BTree.cpp: implementation of the CBTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "PrHSCalc.h"
#include "BTree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBTree::CBTree(void)
{
//	Leaf=NULL;
	oprt = 0;
	valueD = 0;
	RtPt=NULL;
	type=0;//默认为单值点
}

CBTree::~CBTree(void)
{
	Leaf.RemoveAll();
	RtPt=NULL;
}
