// ConDataTag.cpp: implementation of the ConDataTag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "ConDataTag.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ConDataTag::ConDataTag()
{
	condition_code="";
	record_numble=0;
	load_desp="";
	coal_desp="";
	temp_desp="";
	RID_early=0;
	RID_late=0;
	BeHaveTag=-1;//历史中是否存在标志
	record_numble_hist=0;
}

ConDataTag::~ConDataTag()
{

}
