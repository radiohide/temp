# Microsoft Developer Studio Project File - Name="ePerfManage" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ePerfManage - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ePerfManage.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ePerfManage.mak" CFG="ePerfManage - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ePerfManage - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ePerfManage - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ePerfManage - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bin"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /ZI /Od /I "gridCtrl_src" /I "ModeRunControl" /I "XListCtrl_src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /incremental:yes /debug /machine:I386
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "ePerfManage - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "gridCtrl_src" /I "ModeRunControl" /I "XListCtrl_src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ePerfManage - Win32 Release"
# Name "ePerfManage - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\AdvComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\AdvComboBox.h
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\AdvComboEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\AdvComboEdit.h
# End Source File
# Begin Source File

SOURCE=.\AvgSeriesBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\BackCalc.cpp
# End Source File
# Begin Source File

SOURCE=.\BlockGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BlockGridExt.cpp
# End Source File
# Begin Source File

SOURCE=.\BlockListData.cpp
# End Source File
# Begin Source File

SOURCE=.\BlockPropData.cpp
# End Source File
# Begin Source File

SOURCE=.\BlockPropertyTransferData.cpp
# End Source File
# Begin Source File

SOURCE=.\BTree.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcBlockControlFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcBlockPropertyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcModel.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcModule.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcPort.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcUniValue.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassViewBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassViewManageBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassViewModelBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ClockBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\CoalBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\CoalDataBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\CoalKBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\ConDataTag.cpp
# End Source File
# Begin Source File

SOURCE=.\ConDataTag.h
# End Source File
# Begin Source File

SOURCE=.\ConForRuleRecom.cpp
# End Source File
# Begin Source File

SOURCE=.\ConForRuleRecom.h
# End Source File
# Begin Source File

SOURCE=.\ConstBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpreadSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\CycleBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\DBLastPointBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\DBOperator.cpp
# End Source File
# Begin Source File

SOURCE=.\DBPointAdvBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\DBPointBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\DBPointYBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\DiagramClipboardHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\DiagramEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\DiagramEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\DiagramEntityContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\DiagramLine.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\DiagramMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\DiagramPropertyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DiscreteTableBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\DiscreteTableBlock.h
# End Source File
# Begin Source File

SOURCE=.\DivideBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\DivQRBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAddModule.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBlockName.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCalcOrder.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCfgFindBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCfgModuleOrder.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgCfgSETime.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgCfgSETime.h
# End Source File
# Begin Source File

SOURCE=.\DlgCoalFlowTagData.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDelModel.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDepModel.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDiscreteValue.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDiscreteValue.h
# End Source File
# Begin Source File

SOURCE=.\DlgDisData.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFBVar.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFormulaData.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFormulaData.h
# End Source File
# Begin Source File

SOURCE=.\DlgHistTimeSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInputOutPut.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIPPortData.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLocalRunSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLostPowerVar.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgMCtrlCycle.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgMCtrlCycle.h
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgMCtrlDay.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgMCtrlDay.h
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgMCtrlDuty.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgMCtrlDuty.h
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgMCtrlMonth.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgMCtrlMonth.h
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgMCtrlType.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgMCtrlType.h
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgMCtrlWeek.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgMCtrlWeek.h
# End Source File
# Begin Source File

SOURCE=.\DlgModelSynNote.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgModeRunCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgModeRunCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DlgModuleOrder.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMRename.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNewModel.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNotion.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOneSourceRDB.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOpenModel.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPortData.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPortData.h
# End Source File
# Begin Source File

SOURCE=.\DlgPortWithTable.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProBatInput.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProBatOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPropRuleRun.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRuleFBList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRuleOptFun.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgSelMonth.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgSelMonth.h
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgSelWeek.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgSelWeek.h
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgSetTimeAndSETime.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgSetTimeAndSETime.h
# End Source File
# Begin Source File

SOURCE=.\DlgSourceRDB.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStatAdv.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysVar.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTabFindBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgTaskSUpTime.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\DlgTaskSUpTime.h
# End Source File
# Begin Source File

SOURCE=.\DlgVarDataBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVarDataWithReq.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVarSEValue.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVarWithPort.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVarWithTable.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWindPower.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\DropListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\DropListBox.h
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\DropScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\DropScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\DropWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\DropWnd.h
# End Source File
# Begin Source File

SOURCE=.\ElementInOut.cpp
# End Source File
# Begin Source File

SOURCE=.\ePerfManage.cpp
# End Source File
# Begin Source File

SOURCE=.\ePerfManage.rc
# End Source File
# Begin Source File

SOURCE=.\ePerfManageDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ePerfManageView.cpp
# End Source File
# Begin Source File

SOURCE=.\Expression.cpp
# End Source File
# Begin Source File

SOURCE=.\FileOperate.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\FlowchartClipboardHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\FlowchartControlFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\FlowchartEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\FlowchartEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\FlowchartEntityContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\FormulaBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\FrmViewDesignData.cpp
# End Source File
# Begin Source File

SOURCE=.\FrmViewSysTag.cpp
# End Source File
# Begin Source File

SOURCE=.\FVBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\FViewBlockLink.cpp
# End Source File
# Begin Source File

SOURCE=.\FVModuleView.cpp
# End Source File
# Begin Source File

SOURCE=.\GainBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\gridCtrl_src\GridCell.cpp
# End Source File
# Begin Source File

SOURCE=.\gridCtrl_src\GridCellBase.cpp
# End Source File
# Begin Source File

SOURCE=.\gridCtrl_src\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\gridCtrl_src\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\GroupFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\H7CalcBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\H8CalcBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\HistSeriesBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\HistSeriesBlock.h
# End Source File
# Begin Source File

SOURCE=.\HistSeriesBuffBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\HistSeriesBuffBlock.h
# End Source File
# Begin Source File

SOURCE=.\IfBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\gridCtrl_src\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\IntegrateBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkFromBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkFromProBat.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkFromProBatBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkFromProSerBatBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkFromStatBatBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkFromStatSerBatBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkToBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkToProBatBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkToProSerBatBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkToStatBatBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkToStatSerBat.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkToStatSerBatBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\Log.cpp
# End Source File
# Begin Source File

SOURCE=.\LogicalBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MathFunBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\MCtrlCycle.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\MCtrlCycle.h
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\MCtrlDay.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\MCtrlDay.h
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\MCtrlDuty.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\MCtrlDuty.h
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\MCtrlMonth.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\MCtrlMonth.h
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\MCtrlOffsetSE.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\MCtrlOffsetSE.h
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\MCtrlType.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\MCtrlType.h
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\MCtrlWeek.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\MCtrlWeek.h
# End Source File
# Begin Source File

SOURCE=.\ModbusHead.h
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\ModelCalcControl.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRunControl\ModelCalcControl.h
# End Source File
# Begin Source File

SOURCE=.\ModelData.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelRunView.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelView.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleData.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleLibTree.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleManageTree.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleStopBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleTree.cpp
# End Source File
# Begin Source File

SOURCE=.\MultBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\MyList.cpp
# End Source File
# Begin Source File

SOURCE=.\NumConverToStrBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\OptimalRuleBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputBar.cpp
# End Source File
# Begin Source File

SOURCE=.\PExtStreamBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\PInStreamBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\PLCPointBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\PLCPointBlock.h
# End Source File
# Begin Source File

SOURCE=.\PLossRateBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\PortTransferData.cpp
# End Source File
# Begin Source File

SOURCE=.\PoteDll.cpp
# End Source File
# Begin Source File

SOURCE=.\PreviewPane.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessToStatis.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgAdd.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgAvgSeries.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgBackCalc.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgClock.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgCoal.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgCoal.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgCoalData.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgCoalK.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgConst.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgDBLastPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgDBPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgDBPointAdv.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgDiscreteTable.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgDiscreteTable.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgFormula.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgFormula.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgFormulaPort.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgFormulaPort.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgGain.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgHistSeries.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgHistSeries.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgHistSeriesBuff.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgHistSeriesBuff.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgIF.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgIF.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgIntegrate.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgIter.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgIter.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgIterFormulaList.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgIterFormulaList.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgLimitLostPower.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgLimitLostPowerParaExt.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgLinkFrom.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgLinkFrom.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgLinkFromProBat.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgLinkTo.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgLinkTo.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgLinkToProBat.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgMathFun.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgMult.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgNumToStr.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgOptimalRule.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgPExtStm.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgPExtStm.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgPLCPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgPLCPoint.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgProcToStatis.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgPulse.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRandom.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRDBField.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRDBHistSeriesBat.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRDBHistTagBat.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRDBStatSeriesBat.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRDBStatTagBat.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRTDBHistSeriesBat.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRTDBHistSeriesBatWP.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRTPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRTPointAdv.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRTPointBat.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgSawtooth.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgSineWave.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgSleep.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStableJudge.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStableJudge.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStatAccum.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStatAdd.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStatAvg.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStateLostPower.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStateLostPowerParaExt.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStatEnum.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStatProAvg.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStatProEnum.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStatProEnumParaExt.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStatToProc.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStop.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStop.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStrMapToNum.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStrMerge.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStrMerge.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStrToNumData.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgSumSeries.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgTabCoalKTag.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgTabCoalOut.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgTabOptRuleFun.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgTabRuleVar.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgTime.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgTimeOffset.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgWASP.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgWASP.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgWaveInterval.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgWRDBStat.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgWritePLC.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgWritePLC.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgWriteRDB.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgWriteRDB.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgWriteRTDB.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyStandardWindPowerPerf.cpp
# End Source File
# Begin Source File

SOURCE=.\propertyUtilizationRate.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyWindPowerPerf.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyWindPowerPerfParaExt.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyWriteRDBProBat.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyWriteRDBStatBat.cpp
# End Source File
# Begin Source File

SOURCE=.\PulseBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\RandomBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\RDBHistSeriesBat.cpp
# End Source File
# Begin Source File

SOURCE=.\RDBHistTagBat.cpp
# End Source File
# Begin Source File

SOURCE=.\RDBStatSeriesBat.cpp
# End Source File
# Begin Source File

SOURCE=.\RDBStatTagBat.cpp
# End Source File
# Begin Source File

SOURCE=.\RelationalBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\RTDBHistSeriesBat.cpp
# End Source File
# Begin Source File

SOURCE=.\RTDBHistSeriesBatWP.cpp
# End Source File
# Begin Source File

SOURCE=.\RTPointAdvBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\RTPointBatchBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\RTPointBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\RuleData.cpp
# End Source File
# Begin Source File

SOURCE=.\RuleFB.cpp
# End Source File
# Begin Source File

SOURCE=.\RuleFB.h
# End Source File
# Begin Source File

SOURCE=.\RuleRecomFunTag.cpp
# End Source File
# Begin Source File

SOURCE=.\RuleRecomFunTag.h
# End Source File
# Begin Source File

SOURCE=.\RuleRecommendList.cpp
# End Source File
# Begin Source File

SOURCE=.\RuleRecommendList.h
# End Source File
# Begin Source File

SOURCE=.\RuleRecommendListArr.cpp
# End Source File
# Begin Source File

SOURCE=.\RuleRecommendListArr.h
# End Source File
# Begin Source File

SOURCE=.\RuleRecomResult.cpp
# End Source File
# Begin Source File

SOURCE=.\RuleRecomResult.h
# End Source File
# Begin Source File

SOURCE=.\RuleREPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\RuleREPoint.h
# End Source File
# Begin Source File

SOURCE=.\RuleRTargetValue.cpp
# End Source File
# Begin Source File

SOURCE=.\RuleRTargetValue.h
# End Source File
# Begin Source File

SOURCE=.\RuleRunBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\SawtoothBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\Schemadef.h
# End Source File
# Begin Source File

SOURCE=.\SelectorBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\SineWaveBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\SleepBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\SortCStringArray.h
# End Source File
# Begin Source File

SOURCE=.\StableJudgeBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StableJudgeBlock.h
# End Source File
# Begin Source File

SOURCE=.\StatAccumBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StatAddBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StatAvgBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StatEnumBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisToProcessBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StatLimitLostPowerBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StatLimitLostPowerParaExtBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StatProAvgBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StatProEnumBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StatProEnumParaExtBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StatStandardWindPowerPerf.cpp
# End Source File
# Begin Source File

SOURCE=.\StatStatAvgBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StatStateLostPowerBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StatStateLostPowerParaExtBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StatStatSumBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StatUtilizationRate.cpp
# End Source File
# Begin Source File

SOURCE=.\StatWindPowerPerf.cpp
# End Source File
# Begin Source File

SOURCE=.\StatWindPowerPerfParaExt.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\StopBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StrMaptoNumBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StrMergeBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\StrMergeBlock.h
# End Source File
# Begin Source File

SOURCE=.\SubGroupInfTag.cpp
# End Source File
# Begin Source File

SOURCE=.\SubGroupInfTag.h
# End Source File
# Begin Source File

SOURCE=.\SumSeriesBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\SysVar.cpp
# End Source File
# Begin Source File

SOURCE=.\SysVarAll.cpp
# End Source File
# Begin Source File

SOURCE=.\SysVarCG.cpp
# End Source File
# Begin Source File

SOURCE=.\SysVarG.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadRunPara.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeOffsetBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\gridCtrl_src\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\Tmschema.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\Tokenizer.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeItemObj.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\UndoItem.cpp
# End Source File
# Begin Source File

SOURCE=.\UniValue.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\Uxtheme.h
# End Source File
# Begin Source File

SOURCE=.\ViewDebugFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewModuleFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewUserFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewWSpaceFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\VisualStylesXP.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\VisualStylesXP.h
# End Source File
# Begin Source File

SOURCE=.\WASPCalcBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\WASPInPHBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\WASPInPSBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\WaspinPTBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\WASPInPTLBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\WASPInPVBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\WASPInPXBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\WASPInTHBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\WASPInTSBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\WASPInTVBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\WASPInTXBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveIntervalBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\WindPowerTag.cpp
# End Source File
# Begin Source File

SOURCE=.\wMnStmCalcBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkSpaceElement.cpp
# End Source File
# Begin Source File

SOURCE=.\WritePLCBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\WritePLCBlock.h
# End Source File
# Begin Source File

SOURCE=.\WriteRDBBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\WriteRDBProBat.cpp
# End Source File
# Begin Source File

SOURCE=.\WriteRDBStatBat.cpp
# End Source File
# Begin Source File

SOURCE=.\WriteRDBStatBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\WriteRTDBBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\XCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\XCombo.h
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\XComboList.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\XComboList.h
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\XEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\XEdit.h
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\XHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\XHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\XListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl_src\XListCtrl.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddBlock.h
# End Source File
# Begin Source File

SOURCE=.\AvgSeriesBlock.h
# End Source File
# Begin Source File

SOURCE=.\BackCalc.h
# End Source File
# Begin Source File

SOURCE=.\BHist_Add.h
# End Source File
# Begin Source File

SOURCE=.\BlockGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BlockGridExt.h
# End Source File
# Begin Source File

SOURCE=.\BlockListData.h
# End Source File
# Begin Source File

SOURCE=.\BlockPropData.h
# End Source File
# Begin Source File

SOURCE=.\BlockPropertyTransferData.h
# End Source File
# Begin Source File

SOURCE=.\BTree.h
# End Source File
# Begin Source File

SOURCE=.\CalcBlock.h
# End Source File
# Begin Source File

SOURCE=.\CalcBlockControlFactory.h
# End Source File
# Begin Source File

SOURCE=.\CalcBlockPropertyDlg.h
# End Source File
# Begin Source File

SOURCE=.\CalcModel.h
# End Source File
# Begin Source File

SOURCE=.\CalcModule.h
# End Source File
# Begin Source File

SOURCE=.\CalcPort.h
# End Source File
# Begin Source File

SOURCE=.\CalcUniValue.h
# End Source File
# Begin Source File

SOURCE=.\gridCtrl_src\CellRange.h
# End Source File
# Begin Source File

SOURCE=.\ClassViewBar.h
# End Source File
# Begin Source File

SOURCE=.\ClassViewManageBar.h
# End Source File
# Begin Source File

SOURCE=.\ClassViewModelBar.h
# End Source File
# Begin Source File

SOURCE=.\ClockBlock.h
# End Source File
# Begin Source File

SOURCE=.\CoalBlock.h
# End Source File
# Begin Source File

SOURCE=.\CoalDataBlock.h
# End Source File
# Begin Source File

SOURCE=.\CoalKBlock.h
# End Source File
# Begin Source File

SOURCE=.\ConstBlock.h
# End Source File
# Begin Source File

SOURCE=.\CSpreadSheet.h
# End Source File
# Begin Source File

SOURCE=.\CycleBlock.h
# End Source File
# Begin Source File

SOURCE=.\DataBuffer.h
# End Source File
# Begin Source File

SOURCE=.\DBLastPointBlock.h
# End Source File
# Begin Source File

SOURCE=.\DBOperator.h
# End Source File
# Begin Source File

SOURCE=.\DBPointAdvBlock.h
# End Source File
# Begin Source File

SOURCE=.\DBPointBlock.h
# End Source File
# Begin Source File

SOURCE=.\DBPointYBlock.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\DiagramClipboardHandler.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\DiagramEditor.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\DiagramEntity.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\DiagramEntityContainer.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\DiagramLine.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\DiagramMenu.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\DiagramPropertyDlg.h
# End Source File
# Begin Source File

SOURCE=.\DivideBlock.h
# End Source File
# Begin Source File

SOURCE=.\DivQRBlock.h
# End Source File
# Begin Source File

SOURCE=.\DlgAddModule.h
# End Source File
# Begin Source File

SOURCE=.\DlgBlockName.h
# End Source File
# Begin Source File

SOURCE=.\DlgCalcOrder.h
# End Source File
# Begin Source File

SOURCE=.\DlgCfgFindBlock.h
# End Source File
# Begin Source File

SOURCE=.\DlgCfgModuleOrder.h
# End Source File
# Begin Source File

SOURCE=.\DlgCoalFlowTagData.h
# End Source File
# Begin Source File

SOURCE=.\DlgDelModel.h
# End Source File
# Begin Source File

SOURCE=.\DlgDepModel.h
# End Source File
# Begin Source File

SOURCE=.\DlgDisData.h
# End Source File
# Begin Source File

SOURCE=.\DlgFBVar.h
# End Source File
# Begin Source File

SOURCE=.\DlgHistTimeSet.h
# End Source File
# Begin Source File

SOURCE=.\DlgInputOutPut.h
# End Source File
# Begin Source File

SOURCE=.\DlgIPPortData.h
# End Source File
# Begin Source File

SOURCE=.\DlgLocalRunSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgLostPowerVar.h
# End Source File
# Begin Source File

SOURCE=.\DlgModelSynNote.h
# End Source File
# Begin Source File

SOURCE=.\DlgModuleOrder.h
# End Source File
# Begin Source File

SOURCE=.\DlgMRename.h
# End Source File
# Begin Source File

SOURCE=.\DlgNewModel.h
# End Source File
# Begin Source File

SOURCE=.\DlgNotion.h
# End Source File
# Begin Source File

SOURCE=.\DlgOneSourceRDB.h
# End Source File
# Begin Source File

SOURCE=.\DlgOpenModel.h
# End Source File
# Begin Source File

SOURCE=.\DlgPortWithTable.h
# End Source File
# Begin Source File

SOURCE=.\DlgProBatInput.h
# End Source File
# Begin Source File

SOURCE=.\DlgProBatOutput.h
# End Source File
# Begin Source File

SOURCE=.\DlgProgress.h
# End Source File
# Begin Source File

SOURCE=.\DlgPropRuleRun.h
# End Source File
# Begin Source File

SOURCE=.\DlgRuleFBList.h
# End Source File
# Begin Source File

SOURCE=.\DlgRuleOptFun.h
# End Source File
# Begin Source File

SOURCE=.\DlgSourceRDB.h
# End Source File
# Begin Source File

SOURCE=.\DlgStatAdv.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysVar.h
# End Source File
# Begin Source File

SOURCE=.\DlgTabFindBlock.h
# End Source File
# Begin Source File

SOURCE=.\DlgVarDataBase.h
# End Source File
# Begin Source File

SOURCE=.\DlgVarDataWithReq.h
# End Source File
# Begin Source File

SOURCE=.\DlgVarSEValue.h
# End Source File
# Begin Source File

SOURCE=.\DlgVarWithPort.h
# End Source File
# Begin Source File

SOURCE=.\DlgVarWithTable.h
# End Source File
# Begin Source File

SOURCE=.\DlgWindPower.h
# End Source File
# Begin Source File

SOURCE=.\ElementInOut.h
# End Source File
# Begin Source File

SOURCE=.\ePerfManage.h
# End Source File
# Begin Source File

SOURCE=.\ePerfManageDoc.h
# End Source File
# Begin Source File

SOURCE=.\ePerfManageView.h
# End Source File
# Begin Source File

SOURCE=.\Expression.h
# End Source File
# Begin Source File

SOURCE=.\FileOperate.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\FlowchartClipboardHandler.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\FlowchartControlFactory.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\FlowchartEditor.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\FlowchartEntity.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\FlowchartEntityContainer.h
# End Source File
# Begin Source File

SOURCE=.\FormulaBlock.h
# End Source File
# Begin Source File

SOURCE=.\FrmViewDesignData.h
# End Source File
# Begin Source File

SOURCE=.\FrmViewSysTag.h
# End Source File
# Begin Source File

SOURCE=.\FVBlock.h
# End Source File
# Begin Source File

SOURCE=.\FViewBlockLink.h
# End Source File
# Begin Source File

SOURCE=.\FVModuleView.h
# End Source File
# Begin Source File

SOURCE=.\g_define.h
# End Source File
# Begin Source File

SOURCE=.\GainBlock.h
# End Source File
# Begin Source File

SOURCE=.\gridCtrl_src\GridCell.h
# End Source File
# Begin Source File

SOURCE=.\gridCtrl_src\GridCellBase.h
# End Source File
# Begin Source File

SOURCE=.\gridCtrl_src\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\gridCtrl_src\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\GroupFactory.h
# End Source File
# Begin Source File

SOURCE=.\H7CalcBlock.h
# End Source File
# Begin Source File

SOURCE=.\H8CalcBlock.h
# End Source File
# Begin Source File

SOURCE=.\hasp_api.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\HitParams.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\HitParamsRect.h
# End Source File
# Begin Source File

SOURCE=.\IfBlock.h
# End Source File
# Begin Source File

SOURCE=.\gridCtrl_src\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\IntegrateBlock.h
# End Source File
# Begin Source File

SOURCE=.\LinkFromBlock.h
# End Source File
# Begin Source File

SOURCE=.\LinkFromProBat.h
# End Source File
# Begin Source File

SOURCE=.\LinkFromProBatBlock.h
# End Source File
# Begin Source File

SOURCE=.\LinkFromProSerBatBlock.h
# End Source File
# Begin Source File

SOURCE=.\LinkFromStatBatBlock.h
# End Source File
# Begin Source File

SOURCE=.\LinkFromStatSerBatBlock.h
# End Source File
# Begin Source File

SOURCE=.\LinkToBlock.h
# End Source File
# Begin Source File

SOURCE=.\LinkToProBatBlock.h
# End Source File
# Begin Source File

SOURCE=.\LinkToProSerBatBlock.h
# End Source File
# Begin Source File

SOURCE=.\LinkToStatBatBlock.h
# End Source File
# Begin Source File

SOURCE=.\LinkToStatSerBat.h
# End Source File
# Begin Source File

SOURCE=.\LinkToStatSerBatBlock.h
# End Source File
# Begin Source File

SOURCE=.\Log.h
# End Source File
# Begin Source File

SOURCE=.\LogicalBlock.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MathFunBlock.h
# End Source File
# Begin Source File

SOURCE=.\gridCtrl_src\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\ModelData.h
# End Source File
# Begin Source File

SOURCE=.\ModelFrame.h
# End Source File
# Begin Source File

SOURCE=.\ModelRunView.h
# End Source File
# Begin Source File

SOURCE=.\ModelView.h
# End Source File
# Begin Source File

SOURCE=.\ModuleData.h
# End Source File
# Begin Source File

SOURCE=.\ModuleLibTree.h
# End Source File
# Begin Source File

SOURCE=.\ModuleManageTree.h
# End Source File
# Begin Source File

SOURCE=.\ModuleStopBlock.h
# End Source File
# Begin Source File

SOURCE=.\ModuleTree.h
# End Source File
# Begin Source File

SOURCE=.\MultBlock.h
# End Source File
# Begin Source File

SOURCE=.\MyList.h
# End Source File
# Begin Source File

SOURCE=.\NumConverToStrBlock.h
# End Source File
# Begin Source File

SOURCE=.\OptimalRuleBlock.h
# End Source File
# Begin Source File

SOURCE=.\OutputBar.h
# End Source File
# Begin Source File

SOURCE=.\PExtStreamBlock.h
# End Source File
# Begin Source File

SOURCE=.\PInStreamBlock.h
# End Source File
# Begin Source File

SOURCE=.\PLossRateBlock.h
# End Source File
# Begin Source File

SOURCE=.\PortTransferData.h
# End Source File
# Begin Source File

SOURCE=.\PoteDll.h
# End Source File
# Begin Source File

SOURCE=.\PreviewPane.h
# End Source File
# Begin Source File

SOURCE=.\ProcessToStatis.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgAdd.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgAvgSeries.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgBackCalc.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgClock.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgCoalData.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgCoalK.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgConst.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgDBLastPoint.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgDBPoint.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgDBPointAdv.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgGain.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgIntegrate.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgLimitLostPower.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgLimitLostPowerParaExt.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgLinkFromProBat.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgLinkToProBat.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgMathFun.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgMult.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgNumToStr.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgOptimalRule.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgProcToStatis.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgPulse.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRandom.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRDBField.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRDBHistSeriesBat.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRDBHistTagBat.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRDBStatSeriesBat.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRDBStatTagBat.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRTDBHistSeriesBat.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRTDBHistSeriesBatWP.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRTPoint.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRTPointAdv.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgRTPointBat.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgSawtooth.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgSineWave.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgSleep.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStatAccum.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStatAdd.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStatAvg.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStateLostPower.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStateLostPowerParaExt.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStatEnum.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStatProAvg.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStatProEnum.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStatProEnumParaExt.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStatToProc.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStrMapToNum.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgStrToNumData.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgSumSeries.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgTabCoalKTag.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgTabCoalOut.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgTabOptRuleFun.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgTabRuleVar.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgTime.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgTimeOffset.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgWaveInterval.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgWRDBStat.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDlgWriteRTDB.h
# End Source File
# Begin Source File

SOURCE=.\PropertyStandardWindPowerPerf.h
# End Source File
# Begin Source File

SOURCE=.\propertyUtilizationRate.h
# End Source File
# Begin Source File

SOURCE=.\PropertyWindPowerPerf.h
# End Source File
# Begin Source File

SOURCE=.\PropertyWindPowerPerfParaExt.h
# End Source File
# Begin Source File

SOURCE=.\PropertyWriteRDBProBat.h
# End Source File
# Begin Source File

SOURCE=.\PropertyWriteRDBStatBat.h
# End Source File
# Begin Source File

SOURCE=.\PulseBlock.h
# End Source File
# Begin Source File

SOURCE=.\RandomBlock.h
# End Source File
# Begin Source File

SOURCE=.\RDBHistSeriesBat.h
# End Source File
# Begin Source File

SOURCE=.\RDBHistTagBat.h
# End Source File
# Begin Source File

SOURCE=.\RDBStatSeriesBat.h
# End Source File
# Begin Source File

SOURCE=.\RDBStatTagBat.h
# End Source File
# Begin Source File

SOURCE=.\RelationalBlock.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RTDBHistSeriesBat.h
# End Source File
# Begin Source File

SOURCE=.\RTDBHistSeriesBatWP.h
# End Source File
# Begin Source File

SOURCE=.\RTPointAdvBlock.h
# End Source File
# Begin Source File

SOURCE=.\RTPointBatchBlock.h
# End Source File
# Begin Source File

SOURCE=.\RTPointBlock.h
# End Source File
# Begin Source File

SOURCE=.\RuleData.h
# End Source File
# Begin Source File

SOURCE=.\RuleRunBlock.h
# End Source File
# Begin Source File

SOURCE=.\SawtoothBlock.h
# End Source File
# Begin Source File

SOURCE=.\SelectorBlock.h
# End Source File
# Begin Source File

SOURCE=.\SineWaveBlock.h
# End Source File
# Begin Source File

SOURCE=.\SleepBlock.h
# End Source File
# Begin Source File

SOURCE=.\SocketManage.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketManage.h
# End Source File
# Begin Source File

SOURCE=.\SocketServ.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketServ.h
# End Source File
# Begin Source File

SOURCE=.\StatAccumBlock.h
# End Source File
# Begin Source File

SOURCE=.\StatAddBlock.h
# End Source File
# Begin Source File

SOURCE=.\StatAvgBlock.h
# End Source File
# Begin Source File

SOURCE=.\StatEnumBlock.h
# End Source File
# Begin Source File

SOURCE=.\StatisToProcessBlock.h
# End Source File
# Begin Source File

SOURCE=.\StatLimitLostPowerBlock.h
# End Source File
# Begin Source File

SOURCE=.\StatLimitLostPowerParaExtBlock.h
# End Source File
# Begin Source File

SOURCE=.\StatProAvgBlock.h
# End Source File
# Begin Source File

SOURCE=.\StatProEnumBlock.h
# End Source File
# Begin Source File

SOURCE=.\StatProEnumParaExtBlock.h
# End Source File
# Begin Source File

SOURCE=.\StatStandardWindPowerPerf.h
# End Source File
# Begin Source File

SOURCE=.\StatStatAvgBlock.h
# End Source File
# Begin Source File

SOURCE=.\StatStateLostPowerBlock.h
# End Source File
# Begin Source File

SOURCE=.\StatStateLostPowerParaExtBlock.h
# End Source File
# Begin Source File

SOURCE=.\StatStatSumBlock.h
# End Source File
# Begin Source File

SOURCE=.\StatUtilizationRate.h
# End Source File
# Begin Source File

SOURCE=.\StatWindPowerPerf.h
# End Source File
# Begin Source File

SOURCE=.\StatWindPowerPerfParaExt.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StopBlock.h
# End Source File
# Begin Source File

SOURCE=.\StrMaptoNumBlock.h
# End Source File
# Begin Source File

SOURCE=.\SumSeriesBlock.h
# End Source File
# Begin Source File

SOURCE=.\SysVar.h
# End Source File
# Begin Source File

SOURCE=.\SysVarAll.h
# End Source File
# Begin Source File

SOURCE=.\SysVarCG.h
# End Source File
# Begin Source File

SOURCE=.\SysVarG.h
# End Source File
# Begin Source File

SOURCE=.\testlib.h
# End Source File
# Begin Source File

SOURCE=.\ThreadRunPara.h
# End Source File
# Begin Source File

SOURCE=.\TimeBlock.h
# End Source File
# Begin Source File

SOURCE=.\TimeOffsetBlock.h
# End Source File
# Begin Source File

SOURCE=.\gridCtrl_src\TitleTip.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\Tokenizer.h
# End Source File
# Begin Source File

SOURCE=.\TreeItemObj.h
# End Source File
# Begin Source File

SOURCE=.\FlowchartEditor\DiagramEditor\UndoItem.h
# End Source File
# Begin Source File

SOURCE=.\UniValue.h
# End Source File
# Begin Source File

SOURCE=.\ViewDebugFrm.h
# End Source File
# Begin Source File

SOURCE=.\ViewModuleFrm.h
# End Source File
# Begin Source File

SOURCE=.\ViewUserFrm.h
# End Source File
# Begin Source File

SOURCE=.\ViewWSpaceFrm.h
# End Source File
# Begin Source File

SOURCE=.\WASPCalcBlock.h
# End Source File
# Begin Source File

SOURCE=.\WASPInPHBlock.h
# End Source File
# Begin Source File

SOURCE=.\WASPInPSBlock.h
# End Source File
# Begin Source File

SOURCE=.\WaspinPTBlock.h
# End Source File
# Begin Source File

SOURCE=.\WASPInPTLBlock.h
# End Source File
# Begin Source File

SOURCE=.\WASPInPVBlock.h
# End Source File
# Begin Source File

SOURCE=.\WASPInPXBlock.h
# End Source File
# Begin Source File

SOURCE=.\WASPInTHBlock.h
# End Source File
# Begin Source File

SOURCE=.\WASPInTSBlock.h
# End Source File
# Begin Source File

SOURCE=.\WASPInTVBlock.h
# End Source File
# Begin Source File

SOURCE=.\WASPInTXBlock.h
# End Source File
# Begin Source File

SOURCE=.\WaveIntervalBlock.h
# End Source File
# Begin Source File

SOURCE=.\WindPowerTag.h
# End Source File
# Begin Source File

SOURCE=.\wMnStmCalcBlock.h
# End Source File
# Begin Source File

SOURCE=.\WorkSpaceElement.h
# End Source File
# Begin Source File

SOURCE=.\WriteRDBBlock.h
# End Source File
# Begin Source File

SOURCE=.\WriteRDBProBat.h
# End Source File
# Begin Source File

SOURCE=.\WriteRDBStatBat.h
# End Source File
# Begin Source File

SOURCE=.\WriteRDBStatBlock.h
# End Source File
# Begin Source File

SOURCE=.\WriteRTDBBlock.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\classview.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\ePerfManage.ico
# End Source File
# Begin Source File

SOURCE=.\res\ePerfManage.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ePerfManageDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\header.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_mode.ico
# End Source File
# Begin Source File

SOURCE=.\res\list.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\modelfra.bmp
# End Source File
# Begin Source File

SOURCE=.\res\shortcuts.bmp
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tools_page3.bmp
# End Source File
# Begin Source File

SOURCE=".\res\∞Ô÷˙16.ico"
# End Source File
# Begin Source File

SOURCE=".\res\–‘ƒ‹º∆À„16.ico"
# End Source File
# End Group
# Begin Source File

SOURCE=.\msado15.tlh
# End Source File
# Begin Source File

SOURCE=.\msado15.tli
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
