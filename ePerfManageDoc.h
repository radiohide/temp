// ePerfManageDoc.h : interface of the CEPerfManageDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPERFMANAGEDOC_H__10846131_4E2A_4EBF_832F_735852306ECB__INCLUDED_)
#define AFX_EPERFMANAGEDOC_H__10846131_4E2A_4EBF_832F_735852306ECB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcModel.h"
#include "ModelData.h"
#include "ModelCalcControl.h"

#include "ThreadRunPara.h"
#include "DlgCfgFindBlock.h"
#include "SocketManage.h"

class CEPerfManageView;
class CEPerfManageDoc : public CDocument
{
protected: // create from serialization only
	CEPerfManageDoc();
	DECLARE_DYNCREATE(CEPerfManageDoc)
/////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Attributes
public:
	HANDLE    m_hLocalThread;//本地循环计算的句柄。add at 2011-12-9
	ThreadRunPara *   m_pLocalThreadPara; //线程参数指针add at 2011-12-9
	BOOL m_bInLocalRun; //add at 2011-12-9
	BOOL m_BShowPortValue;

	int m_iRemoteModeState;//远程模型的状态

	bool m_bTagGettingWS;////true:正在获取；false:取消获取
	bool m_bTagGettingLog;//true:正在获取；false:取消获取
    CCalcModel  *m_pModel;
	ModelCalcControl *m_pMCalControl;
	void UpdateCalcIndex(_ConnectionPtr &pCon);//更新数据库的模块的计算次序值。
	BOOL DelModuleInfoFromDB(_ConnectionPtr &pCon,CString strModelName,CString strModuleName);
	BOOL AddModelInfoToDB(_ConnectionPtr &pCon,CString strModelName,CString strModelDesp);
	BOOL AddModuleInfoToDB(_ConnectionPtr &pCon,CString strModelName,CString strModuleName,CString strModuleDesp);
	static BOOL CreateModuleListTable(_ConnectionPtr &pCon,CString strModelName);
	static BOOL CreateBlockListTable(_ConnectionPtr &pCon,CString strModelName,CString strModuleName) ;
	static BOOL CreateBlockPropTable(_ConnectionPtr &pCon,CString strModelName,CString strModuleName);

	int GetNodeState(CSocketManage *&pSocket,CString sNodeName);//得到远程模型运行状态。
private:
	CDlgCfgFindBlock *pDlgFindBlock;//计算块及属性查找替换块
private:
	int getModuleCount(_ConnectionPtr &pCon,CString strModelName);
	void stopRemoteMode();
	void runRemoteMode();
public:
	void MagEndGetLog();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEPerfManageDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL AddLibBlock(CString strBlockType);
	CEPerfManageView* GetMainView();

	virtual ~CEPerfManageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEPerfManageDoc)
	afx_msg void OnFileNewmodel();
	afx_msg void OnUpdateFileNewmodel(CCmdUI* pCmdUI);
	afx_msg void OnCfgAddModule();
	afx_msg void OnUpdateCfgAddModule(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCfgAddBlock(CCmdUI* pCmdUI);
	afx_msg void OnModuleAddblock();
	afx_msg void OnUpdateModuleAddblock(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewClosepage(CCmdUI* pCmdUI);
	afx_msg void OnFileOpenModel();
	afx_msg void OnFileSavemodel();
	afx_msg void OnRunCompile();
	afx_msg void OnUpdateRunCompile(CCmdUI* pCmdUI);
	afx_msg void OnRunDebug();
	afx_msg void OnCfgModuleorder();
	afx_msg void OnMagRun();
	afx_msg void OnUpdateMagRun(CCmdUI* pCmdUI);
	afx_msg void OnMagStop();
	afx_msg void OnUpdateMagStop(CCmdUI* pCmdUI);
	afx_msg void OnMagPause();
	afx_msg void OnUpdateMagPause(CCmdUI* pCmdUI);
	afx_msg void OnMagResume();
	afx_msg void OnUpdateMagResume(CCmdUI* pCmdUI);
	afx_msg void OnFileUploadmodel();
	afx_msg void OnUpdateFileUploadmodel(CCmdUI* pCmdUI);
	afx_msg void OnMagRefreshws();
	afx_msg void OnUpdateMagRefreshws(CCmdUI* pCmdUI);
	afx_msg void OnCfgAddBlock();
	afx_msg void OnCfgModeRunControl();
	afx_msg void OnUpdateCfgModeRunControl(CCmdUI* pCmdUI);
	afx_msg void OnDepModel();
	afx_msg void OnMagStartGetLog();
	afx_msg void OnUpdateMagStartGetLog(CCmdUI* pCmdUI);
	afx_msg void OnMagEndGetLog();
	afx_msg void OnUpdateMagEndGetLog(CCmdUI* pCmdUI);
	afx_msg void OnMagRefreshWSStart();
	afx_msg void OnUpdateMagRefreshWSStart(CCmdUI* pCmdUI);
	afx_msg void OnMagRefreshWSEnd();
	afx_msg void OnUpdateMagRefreshWSEnd(CCmdUI* pCmdUI);
	afx_msg void OnMagModelSyn();
	afx_msg void OnUpdateMagModelSyn(CCmdUI* pCmdUI);
	afx_msg void OnToolSourceRdb();
	afx_msg void OnUpdateFileSavemodel(CCmdUI* pCmdUI);
	afx_msg void OnFileDelModel();
	afx_msg void OnCfgDelModule();
	afx_msg void OnUpdateCfgDelModule(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolSourceRdb(CCmdUI* pCmdUI);
	afx_msg void OnRunLocalStart();
	afx_msg void OnUpdateRunLocalStart(CCmdUI* pCmdUI);
	afx_msg void OnRunLocalStop();
	afx_msg void OnUpdateRunLocalStop(CCmdUI* pCmdUI);
	afx_msg void OnCfgFindBlock();
	afx_msg void OnUpdateCfgFindBlock(CCmdUI* pCmdUI);
	afx_msg void OnMagPortValueShow();
	afx_msg void OnFileOpenAllModule();
	afx_msg void OnUpdateFileOpenAllModule(CCmdUI* pCmdUI);
	afx_msg void OnFileCloseAllModule();
	afx_msg void OnUpdateFileCloseAllModule(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EPERFMANAGEDOC_H__10846131_4E2A_4EBF_832F_735852306ECB__INCLUDED_)
