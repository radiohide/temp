// BlockPropertyTransferData.cpp: implementation of the CBlockPropertyTransferData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "BlockPropertyTransferData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlockPropertyTransferData::CBlockPropertyTransferData()
{
	memset(m_cModelName,0,64);
	memset(m_cModuleName,0,64);
	memset(m_cBlockName,0,64);

	memset(m_cPropName,0,64);
	memset(m_cValueItem1,0,128);
	memset(m_cValueItem2,0,128);
	memset(m_cValueItem3,0,128);
	memset(m_cValueItem4,0,128);
	memset(m_cValueItem5,0,128);
}

CBlockPropertyTransferData::~CBlockPropertyTransferData()
{

}
