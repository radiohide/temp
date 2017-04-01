// ClassViewManageBar.h: interface for the CClassViewManageBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLASSVIEWMANAGEBAR_H__F5EDF72B_B0DF_426D_9C85_10ECF7F2F717__INCLUDED_)
#define AFX_CLASSVIEWMANAGEBAR_H__F5EDF72B_B0DF_426D_9C85_10ECF7F2F717__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//����������Ҫ�Ǹ��𣺲��ά����ú�����ݵ�ά����
//��ģ���л�����еĲ����Ϣ����һ��ͳһ����ʽ����ά������������޸ģ��޸ĺ��Ӱ�죻
//�������޸�
#include "ModuleManageTree.h"
class CClassViewManageBar : public CBCGPDockingControlBar  
{
public:
	void RefreshTreeContent();
	CClassViewManageBar();
	virtual ~CClassViewManageBar();
protected:
	CModuleManageTree	    m_wndManageTree;
	CImageList		m_Images;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClassViewBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CClassViewBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();

	afx_msg void  OnModuleProp();

	afx_msg void OnDblclkModelTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CLASSVIEWMANAGEBAR_H__F5EDF72B_B0DF_426D_9C85_10ECF7F2F717__INCLUDED_)
