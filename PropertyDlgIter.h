#if !defined(AFX_PROPERTYDLGITER_H__861C9A4C_C99F_4783_88AF_D67DF1E541BE__INCLUDED_)
#define AFX_PROPERTYDLGITER_H__861C9A4C_C99F_4783_88AF_D67DF1E541BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDlgIter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgIter dialog
#include "PropertyDlgFormulaPort.h"
#include "DlgFormulaData.h"
#include "PropertyDlgIterFormulaList.h"


class CPropertyDlgIter : public CDialog
{
// Construction
public:
	CPropertyDlgIter(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDlgIter)
	enum { IDD = IDD_DIALOG_ITER };
	CTabCtrl	m_TabList2;
	CTabCtrl	m_TabList;
	CTreeCtrl	m_ctlTree;
	CString	m_StrSelFormula;
	CString	m_StrCelVarName;
	//}}AFX_DATA
	CImageList m_imageList;//����ͼ���б�ؼ�
	CArray<CPropertyDlgFormulaPort*,CPropertyDlgFormulaPort*&> DlgTabArr;
	CArray<CPropertyDlgIterFormulaList*,CPropertyDlgIterFormulaList*&> DlgTab2Arr;

	CStringArray* m_pCurrVarArr;//����ǰ����
	CStringArray* m_pCurrFormulaArr;
	CArray<int,int>* m_pCurrVarTypeArr;//0,��ֵ��1,����
	CArray<int,int>* m_pCurrCanBeEditArr;
	CArray<int,int>* m_pCurrCanBeShowArr;
	CPropertyDlgIterFormulaList* m_pCurrSelFormulaDlg;
	int m_currSelFormulaIndex;//��ǰѡ��ʽ��������
	
public:
	int tagConFOrB;//�����ڵ���ǰ���㻹�ǵ�������㣬0�ڵ���ǰ���㣻1�ڵ��������

	CStringArray inPutArr;
	CArray<int,int> inPutTypeArr;//0,��ֵ��1,����
	CArray<int,int> inPutCanBeEdit;//0���ɱ��༭��1�ɱ��༭��

	CStringArray outPutArr;
	CArray<int,int> outPutTypeArr;//0,��ֵ��1,����
	CArray<int,int> outPutCanBeEdit;//0���ɱ��༭��1�ɱ��༭��

	CStringArray IterFVarArr;//����ǰ����
	CStringArray IterFFormulaArr;
	CArray<int,int> IterFVarTypeArr;//0,��ֵ��1,����
	CArray<int,int> IterFCanBeEdit;//0���ɱ��༭��1�ɱ��༭��
	CArray<int,int> IterFCanBeShow;//0������ʾ��1����ʾ��

	CStringArray conVarArr;//��������
	CStringArray conFormulaArr;
	CArray<int,int> conVarTypeArr;//0,��ֵ��1,����
	CArray<int,int> conVarCanBeEdit;//0���ɱ��༭��1�ɱ��༭��
	CArray<int,int> conVarCanBeShow;//0������ʾ��1����ʾ��

	CStringArray IterVarArr;//��������
	CStringArray IterFormulaArr;
	CArray<int,int> IterVarTypeArr;//0,��ֵ��1,����
	CArray<int,int> IterCanBeEdit;//0���ɱ��༭��1�ɱ��༭��
	CArray<int,int> IterCanBeShow;//0������ʾ��1����ʾ��

	CStringArray IterBVarArr;//���������
	CStringArray IterBFormulaArr;
	CArray<int,int> IterBVarTypeArr;//0,��ֵ��1,����
	CArray<int,int> IterBCanBeEdit;//0���ɱ��༭��1�ɱ��༭��
	CArray<int,int> IterBCanBeShow;//0������ʾ��1����ʾ��

private:
	void initTreeCtrl();
	void initTabCtrl();//��ʼ��Tab�ؼ���ʱ�򣬴����˿�����Ҫ������
	void initTabCtrl2();//��ʼ��Tab�ؼ���ʱ�򣬴�����ʽ����Ҫ������
	
public:
	void SetPropData(
		const CStringArray &inPutArr,
		const CArray<int,int> &inPutTypeArr,
		const CStringArray &outPutArr,
		const CArray<int,int> &outPutTypeArr,
		const CStringArray &IterFVarArr,
		const CArray<int,int> &IterFVarTypeArr,
		const CStringArray &IterFFormulaArr,
		const CStringArray &conVarArr,
		const CArray<int,int> &conVarTypeArr,
		const CStringArray &conFormulaArr,
		const CStringArray &IterVarArr,
		const CArray<int,int> &IterVarTypeArr,
		const CStringArray &IterFormulaArr,
		const CStringArray &IterBVarArr,
		const CArray<int,int> &IterBVarTypeArr,
		const CStringArray &IterBFormulaArr
		);
	void SetCanBeEditCanBeShow(
		const CArray<int,int> &inPutCanBeEdit,
		const CArray<int,int> &outPutCanBeEdit,
		const CArray<int,int> &IterFCanBeEdit,
		const CArray<int,int> &IterFCanBeShow,
		const CArray<int,int> &conVarCanBeEdit,
		const CArray<int,int> &conVarCanBeShow,
		const CArray<int,int> &IterCanBeEdit,
		const CArray<int,int> &IterCanBeShow,
		const CArray<int,int> &IterBCanBeEdit,
		const CArray<int,int> &IterBCanBeShow);
	void SetTagConFOrB(int tagConFOrB);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDlgIter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDlgIter)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnClickFormulaList(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBtnAddPort();
	afx_msg void OnBtnEditPort();
	afx_msg void OnBtnDelPort();
	afx_msg void OnBtnUpPort();
	afx_msg void OnBtnDownPort();
	afx_msg void OnBtnAddFormula();
	afx_msg void OnBtnEditFormula();
	afx_msg void OnBtnDelFormula();
	afx_msg void OnBtnUpFormula();
	afx_msg void OnBtnDownFormula();
	afx_msg void OnSelchangeTablist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTablist2(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDLGITER_H__861C9A4C_C99F_4783_88AF_D67DF1E541BE__INCLUDED_)
