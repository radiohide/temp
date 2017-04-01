#ifndef _FLOWCHARTENTITYCONTAINER_H_
#define _FLOWCHARTENTITYCONTAINER_H_

#include "DiagramEditor/DiagramEntityContainer.h"
#include "DiagramEditor/DiagramEntity.h"
#include "DiagramEditor/Tokenizer.h"
#include "FlowchartEntity.h"


class CFlowchartEntityContainer : public CDiagramEntityContainer {

public:
	void ClearAllLinkWithObject(int index);
// Construction/initialization/destruction
	CFlowchartEntityContainer();
	virtual ~CFlowchartEntityContainer();


	int					GetSelectCount();
	//CFlowchartEntity*	GetPrimarySelected();
	//CFlowchartEntity*	GetSecondarySelected();

// Overrides
	virtual void	Add( CDiagramEntity* obj );
	virtual void	RemoveAt( int index );

	virtual void	Undo();
	virtual void	Snapshot();
	virtual void	ClearUndo();

	virtual void	Duplicate( CDiagramEntity* obj );



private:
// Private data
	
	double	Dist( CPoint point1, CPoint point2 );

private:
	BOOL m_bInShowOrderMode;
	BOOL m_bInShowValueMode;
public:
	CString DeleteNameStringNum(CString strName,int* Number);
	void SetShowOrderMode(BOOL var);
	BOOL GetShowOrderMode();
	void SetShowValueMode(BOOL var);
	BOOL GetShowValueMode();

	CString MakeSuitableName(CString strOldBlockName);
};

#endif // _FLOWCHARTENTITYCONTAINER_H_
