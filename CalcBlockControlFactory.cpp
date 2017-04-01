// CalcBlockControlFactory.cpp: implementation of the CCalcBlockControlFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "CalcBlockControlFactory.h"
#include "ePerfManageDoc.h"

#include "CalcBlock.h"


#include "H7CalcBlock.h"
#include "H8CalcBlock.h"
#include "wMnStmCalcBlock.h"
//算术计算类头文件
#include "AddBlock.h"
#include "MultBlock.h"
#include "DivideBlock.h"
#include "DivQRBlock.h"
#include "MathFunBlock.h"
#include "GainBlock.h"
#include "ConstBlock.h"
#include "SawtoothBlock.h"
#include "ClockBlock.h"
#include "RTPointAdvBlock.h"
#include "RTPointBlock.h"
#include "RTPointBatchBlock.h"


#include "WriteRDBBlock.h"
#include "WriteRTDBBlock.h"
#include "DBPointAdvBlock.h"
#include "DBPointBlock.h"
#include "DBLastPointBlock.h"

#include "RelationalBlock.h"
#include "LogicalBlock.h"
#include "CycleBlock.h"
#include "FormulaBlock.h"
#include "IfBlock.h"
#include "LinkFromBlock.h"
#include "LinkToBlock.h"
#include "StopBlock.h"
#include "ModuleStopBlock.h"
#include "CoalBlock.h"
#include "PLossRateBlock.h"
#include "PExtStreamBlock.h"
#include "PInStreamBlock.h"
#include "SelectorBlock.h"

#include "WASPInPHBlock.h"
#include "WASPInPSBlock.h"
#include "WASPInPTBlock.h"
#include "WASPInPTLBlock.h"
#include "WASPInPVBlock.h"
#include "WASPInPXBlock.h"
#include "WASPInTHBlock.h"
#include "WASPInTSBlock.h"
#include "WASPInTVBlock.h"
#include "WASPInTXBlock.h"
#include "wMnStmCalcBlock.h"

#include "CoalKBlock.h"
#include "CoalDataBlock.h"

#include "SineWaveBlock.h"
#include "RandomBlock.h"
#include "PulseBlock.h"

#include "HistSeriesBlock.h"
#include "HistSeriesBuffBlock.h"

//模式分析块
#include "OptimalRuleBlock.h"
#include "DiscreteTableBlock.h"
#include "StrMergeBlock.h"
#include "StableJudgeBlock.h"
#include "WaveIntervalBlock.h"
//---------------------------------------
#include "RuleRunBlock.h"

//plc
#include "PLCPointBlock.h"
#include "WritePLCBlock.h"
//统计计算
#include "StatAvgBlock.h"
#include "StatEnumBlock.h"
#include "StatAccumBlock.h"

#include "TimeBlock.h"
#include "StrMaptoNumBlock.h"
#include "NumConverToStrBlock.h"
#include "ProcessToStatis.h"
#include "StatisToProcessBlock.h"
#include "IntegrateBlock.h"
#include "AvgSeriesBlock.h"
#include "TimeOffsetBlock.h"

#include "WriteRDBStatBlock.h"
#include "SumSeriesBlock.h"
#include "WriteRDBProBat.h"
#include "RDBHistSeriesBat.h"
#include "StatProAvgBlock.h"
#include "WriteRDBStatBat.h"
#include "RDBStatSeriesBat.h"
#include "StatStatAvgBlock.h"
#include "StatStatSumBlock.h"
#include "StatProEnumBlock.h"
#include "StatStateLostPowerBlock.h"
#include "StatLimitLostPowerBlock.h"
#include "StatAddBlock.h"
#include "RTDBHistSeriesBat.h"
#include "StatWindPowerPerf.h"
#include "RDBHistTagBat.h"
#include "RDBStatTagBat.h"
#include "SleepBlock.h"

#include "BackCalc.h"
#include "LinkToProBatBlock.h"
#include "LinkFromProBat.h"
#include "LinkToProSerBatBlock.h"
#include "LinkFromProSerBatBlock.h"
#include "LinkToStatBatBlock.h"
#include "LinkFromStatBatBlock.h"
#include "LinkToStatSerBatBlock.h"
#include "LinkFromStatSerBatBlock.h"
#include "StatStandardWindPowerPerf.h"
#include "StatUtilizationRate.h"
#include "DBPointYBlock.h"

#include "StatProEnumParaExtBlock.h"
#include "StatWindPowerPerfParaExt.h"
#include "StatStateLostPowerParaExtBlock.h"
#include "StatLimitLostPowerParaExtBlock.h"
#include "RTDBHistSeriesBatWP.h"
#include "StatUtilizationRate15.h"
#include "StatLimitLostPowerParaExtBlock15.h"


#include "zylTest1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcBlock* CCalcBlockControlFactory::CreateFromString( const CString& strtype )
/* ============================================================
   ============================================================*/
{
	CCalcBlock* obj=NULL;
		obj = CH7CalcBlock::CreateFromString(strtype);
	if(!obj)
		obj = CWaveIntervalBlock::CreateFromString(strtype);
	if(!obj)
		obj = CH8CalcBlock::CreateFromString(strtype);
	if(!obj)
		obj = CwMnStmCalcBlock::CreateFromString(strtype);	
	if(!obj)
		obj = CWASPInPHBlock::CreateFromString(strtype);
	if(!obj)
		obj = CWASPInPSBlock::CreateFromString(strtype);
	if(!obj)
		obj = CWASPInPTBlock::CreateFromString(strtype);
	if(!obj)
		obj = CWASPInPTLBlock::CreateFromString(strtype);
	if(!obj)
		obj = CWASPInPVBlock::CreateFromString(strtype);
	if(!obj)
		obj = CWASPInPXBlock::CreateFromString(strtype);
	if(!obj)
		obj = CWASPInTHBlock::CreateFromString(strtype);
	if(!obj)
		obj = CWASPInTSBlock::CreateFromString(strtype);
	if(!obj)
		obj = CWASPInTVBlock::CreateFromString(strtype);
	if(!obj)
		obj = CWASPInTXBlock::CreateFromString(strtype);
	if(!obj)
		obj = CwMnStmCalcBlock::CreateFromString(strtype);
	if(!obj)
		obj = CAddBlock::CreateFromString(strtype);
	if(!obj)
		obj = CMultBlock::CreateFromString(strtype);
	if(!obj)
		obj = CDivideBlock::CreateFromString(strtype);
	if(!obj)
		obj = CDivQRBlock::CreateFromString(strtype);
	if(!obj)
		obj = CMathFunBlock::CreateFromString(strtype);
	if(!obj)
		obj = CGainBlock::CreateFromString(strtype);
	if(!obj)
		obj = CConstBlock::CreateFromString(strtype);
	if(!obj)
		obj = CClockBlock::CreateFromString(strtype);
	if(!obj)
		obj = CTimeBlock::CreateFromString(strtype);
	if(!obj)
		obj = CRTPointAdvBlock::CreateFromString(strtype);
	if(!obj)
		obj = CRelationalBlock::CreateFromString(strtype);
	if(!obj)
		obj = CLogicalBlock::CreateFromString(strtype);
	if(!obj)
		obj = CCycleBlock::CreateFromString(strtype);
	if(!obj)
		obj = CFormulaBlock::CreateFromString(strtype);
	if(!obj)
		obj = CIfBlock::CreateFromString(strtype);
	if(!obj)
		obj = CLinkFromBlock::CreateFromString(strtype);
	if(!obj)
		obj = CLinkToBlock::CreateFromString(strtype);
	if(!obj)
		obj = CStopBlock::CreateFromString(strtype);
	if(!obj)
		obj = CModuleStopBlock::CreateFromString(strtype);
	if(!obj)
		obj = CCoalBlock::CreateFromString(strtype);
	if(!obj)
		obj = CPLossRateBlock::CreateFromString(strtype);
	if(!obj)
		obj = CPExtStreamBlock::CreateFromString(strtype);
	if(!obj)
		obj = CPInStreamBlock::CreateFromString(strtype);
	if(!obj)
		obj = CSelectorBlock::CreateFromString(strtype);
	if(!obj)
		obj = CStrMaptoNumBlock::CreateFromString(strtype);
	if(!obj)
		obj = CWriteRDBBlock::CreateFromString(strtype);
	if(!obj)
		obj = CWriteRTDBBlock::CreateFromString(strtype);
	if(!obj)
		obj = CDBPointAdvBlock::CreateFromString(strtype);
	if(!obj)
		obj = CDBPointBlock::CreateFromString(strtype);
	if(!obj)
		obj = CRTPointBlock::CreateFromString(strtype);
	if(!obj)//(正弦函数块)
		obj = CSineWaveBlock::CreateFromString(strtype);
	if(!obj)//(随机数)
		obj = CRandomBlock::CreateFromString(strtype);
	if(!obj)//(方波函数快)
		obj = CPulseBlock::CreateFromString(strtype);
	if(!obj)
		obj = COptimalRuleBlock::CreateFromString(strtype);
	if(!obj)//(查表离散化)
		obj = CDiscreteTableBlock::CreateFromString(strtype);
	if(!obj)//(字符合并块)
		obj = CStrMergeBlock::CreateFromString(strtype);
	if(!obj)//(读取历史序列)
		obj = CHistSeriesBlock::CreateFromString(strtype);
	if(!obj)//(带缓存的读取历史序列)
		obj = CHistSeriesBuffBlock::CreateFromString(strtype);
	if(!obj)
		obj = CStableJudgeBlock::CreateFromString(strtype);
	if(!obj)
		obj = CCoalKBlock::CreateFromString(strtype);
	if(!obj)
		obj = CCoalDataBlock::CreateFromString(strtype);
	if(!obj)
		obj = CPLCPointBlock::CreateFromString(strtype);
	if(!obj)
		obj = CWritePLCBlock::CreateFromString(strtype);
	if(!obj)
		obj = CStatAvgBlock::CreateFromString(strtype);
	if(!obj)
		obj = CStatEnumBlock::CreateFromString(strtype);
	if(!obj)
		obj = CStatAccumBlock::CreateFromString(strtype);
	if(!obj)
		obj = CSawtoothBlock::CreateFromString(strtype);	
    //规则推理块
	if(!obj)
		obj = CRuleRunBlock::CreateFromString(strtype);
	if(!obj)
		obj = CNumConverToStrBlock::CreateFromString(strtype);
	if(!obj)
		obj = CProcessToStatisBlock::CreateFromString(strtype);
	if(!obj)
		obj = CStatisToProcessBlock::CreateFromString(strtype);
	if(!obj)
		obj = CIntegrateBlock::CreateFromString(strtype);
	if(!obj)
		obj = CAvgSeriesBlock::CreateFromString(strtype);
	if(!obj)
		obj = CTimeOffsetBlock::CreateFromString(strtype);
	if(!obj)
		obj = CWriteRDBStatBlock::CreateFromString(strtype);
	if(!obj)
		obj = CSumSeriesBlock::CreateFromString(strtype);
	if(!obj)
		obj=CRTPointBatchBlock::CreateFromString(strtype);
	if(!obj)
		obj=CWriteRDBProBat::CreateFromString(strtype);
	if(!obj)
		obj=CRDBHistSeriesBat::CreateFromString(strtype);
	if(!obj)
		obj=CStatProAvgBlock::CreateFromString(strtype);
	if(!obj)
		obj=CWriteRDBStatBat::CreateFromString(strtype);
	if(!obj)
		obj=CRDBStatSeriesBat::CreateFromString(strtype);
	if(!obj)
		obj=CStatStatAvgBlock::CreateFromString(strtype);
	if(!obj)
		obj=CStatStatSumBlock::CreateFromString(strtype);
	if(!obj)
		obj=CStatProEnumBlock::CreateFromString(strtype);
	if(!obj)
		obj=CStatStateLostPowerBlock::CreateFromString(strtype);
	if(!obj)
		obj=CStatLimitLostPowerBlock::CreateFromString(strtype);
	if(!obj)
		obj=CStatAddBlock::CreateFromString(strtype);
	if(!obj)
		obj=CRTDBHistSeriesBat::CreateFromString(strtype);
	if(!obj)
		obj=CStatWindPowerPerf::CreateFromString(strtype);
	if(!obj)
		obj=CRDBHistTagBat::CreateFromString(strtype);
	if(!obj)
		obj=CRDBStatTagBat::CreateFromString(strtype);
	if(!obj)
		obj=CSleepBlock::CreateFromString(strtype);
	if(!obj)
		obj=CBackCalc::CreateFromString(strtype);
	if(!obj)
		obj=CLinkToProBatBlock::CreateFromString(strtype);
	if(!obj)
		obj=CLinkFromProBat::CreateFromString(strtype);
	if(!obj)
		obj=CLinkToProSerBatBlock::CreateFromString(strtype);
	if(!obj)
		obj=CLinkFromProSerBatBlock::CreateFromString(strtype);
	if(!obj)
		obj=CLinkToStatBatBlock::CreateFromString(strtype);
	if(!obj)
		obj=CLinkFromStatBatBlock::CreateFromString(strtype);
	if(!obj)
		obj=CLinkToStatSerBatBlock::CreateFromString(strtype);
	if(!obj)
		obj=CLinkFromStatSerBatBlock::CreateFromString(strtype);
	if(!obj)
		obj=CDBLastPointBlock::CreateFromString(strtype);
	if(!obj)
		obj=CStatStandardWindPowerPerf::CreateFromString(strtype);
	if(!obj)
		obj=CStatUtilizationRate::CreateFromString(strtype);
	if(!obj)
		obj=DBPointYBlock::CreateFromString(strtype);
	if(!obj)
		obj=CStatProEnumParaExtBlock::CreateFromString(strtype);
	if(!obj)
		obj=CStatWindPowerPerfParaExt::CreateFromString(strtype);
	if(!obj)
		obj=CStatStateLostPowerParaExtBlock::CreateFromString(strtype);
	if(!obj)
		obj=CStatLimitLostPowerParaExtBlock::CreateFromString(strtype);
	if(!obj)
		obj=CRTDBHistSeriesBatWP::CreateFromString(strtype);
	if(!obj)
		obj=CStatUtilizationRate15::CreateFromString(strtype);
	if(!obj)
		obj=CStatLimitLostPowerParaExtBlock15::CreateFromString(strtype);


	if(!obj)
		obj=CzylTest1::CreateFromString(strtype);
	return obj;
}
