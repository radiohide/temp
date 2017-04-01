// MCtrlOffsetSE.h: interface for the MCtrlOffsetSE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTRLOFFSETSE_H__3B2C8597_E736_45DE_B1F1_CD5A4A8C1FC9__INCLUDED_)
#define AFX_MCTRLOFFSETSE_H__3B2C8597_E736_45DE_B1F1_CD5A4A8C1FC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MCtrlOffsetSE  
{
	//周日~周六为一个整周
public:
	MCtrlOffsetSE();
	virtual ~MCtrlOffsetSE();
	CString GetSTimeStrDesp();
	CString GetETimeStrDesp();
	void GetSETime(CTime calDataTime,CTime &sTime,CTime &eTime);
public:
	MCtrlOffsetSE& operator =( const MCtrlOffsetSE& RightValue);
	void ResetCtrlData();
	int m_iSOffset_Type;
	int m_iSOffset_Value1;
	int m_iSOffset_Value2;
	int m_iSOffset_Value3;
	int m_iSOffset_Value4;
	int m_iSOffset_Value5;
	CString m_strS_Time;

	int m_iEOffset_Type;
	int m_iEOffset_Value1;
	int m_iEOffset_Value2;
	int m_iEOffset_Value3;
	int m_iEOffset_Value4;
	int m_iEOffset_Value5;
	CString m_strE_Time;
};

#endif // !defined(AFX_MCTRLOFFSETSE_H__3B2C8597_E736_45DE_B1F1_CD5A4A8C1FC9__INCLUDED_)
