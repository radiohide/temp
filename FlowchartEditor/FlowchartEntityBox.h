#if !defined(AFX_FLOWCHARTENTITYBOX_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
#define AFX_FLOWCHARTENTITYBOX_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_

#include "FlowchartEntity.h"

class CFlowchartEntityBox : public CFlowchartEntity
{

public:

// Construction/initialization/destruction
	CFlowchartEntityBox();
	virtual ~CFlowchartEntityBox();
	virtual CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual void Draw( CDC* dc, CRect rect );

	// Interaction
	virtual int		GetHitCode( const CPoint& point, const CRect& rect ) const;
	virtual HCURSOR GetCursor( int hit ) const;

protected:
// Overrides
	virtual void	DrawSelectionMarkers( CDC* dc, CRect rect ) const;
};

#endif // !defined(AFX_FLOWCHARTENTITYBOX_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
