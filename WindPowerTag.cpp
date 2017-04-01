// WindPowerTag.cpp: implementation of the WindPowerTag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "WindPowerTag.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WindPowerTag::WindPowerTag()
{
	bDel=false;
	dPowerAvg=0;
	dPowerVar=0;
	iBinNum=0;
	dMidSpeed=0;

	dWindSpeed=0;
	dPower=0;
	lTime=0;
	lrealTime=0;
}

WindPowerTag::~WindPowerTag()
{

}
