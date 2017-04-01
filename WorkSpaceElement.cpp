// WorkSpaceElement.cpp: implementation of the CWorkSpaceElement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WorkSpaceElement.h"
#include "CalcBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWorkSpaceElement::CWorkSpaceElement()
{
	PState = 1;	//³õÊ¼ÎªºÃ
	m_pParentBlock = NULL;
}
CWorkSpaceElement::~CWorkSpaceElement()
{

}
void CWorkSpaceElement::SetParentBlock(CCalcBlock* pBlock)
{
	m_pParentBlock = pBlock;
}
