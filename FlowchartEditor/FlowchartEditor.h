#if !defined(AFX_FLOWCHARTEDITOR_H__609E7C31_5443_4059_8ED9_DF9A743CFD2F__INCLUDED_)
#define AFX_FLOWCHARTEDITOR_H__609E7C31_5443_4059_8ED9_DF9A743CFD2F__INCLUDED_

#include "DiagramEditor\DiagramEditor.h"
#include "FlowchartEntity.h"

class CFlowchartEditor : public CDiagramEditor
{
public:
// Construction/initialization/destruction
	CFlowchartEditor();
	virtual ~CFlowchartEditor();
protected:
	virtual afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void DrawObjects( CDC* dc, double zoom ) const;

	CPoint GetPointFromByLinkString(CString strBlock,CString strPortName) const;
   
private:
	BOOL m_bInShowOrderMode;
	BOOL m_bInShowValueMode;
public:
	void SetShowOrderMode(BOOL var);
	BOOL GetShowOrderMode();
	void SetShowValueMode(BOOL var);
	BOOL GetShowValueMode();
	
	CFlowchartEntity* m_pObjLinkFrom;  //保存上一次

	CFlowchartEntity*  FindByName(CString strBlockName) const;
	int GetSameNameBlockCount(CString strBlockName);
	void ChangeBlockName(CFlowchartEntity* obj,CString strNameNew );
// Command handlers
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_FLOWCHARTEDITOR_H__609E7C31_5443_4059_8ED9_DF9A743CFD2F__INCLUDED_)
