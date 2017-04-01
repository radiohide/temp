#if !defined(AFX_SOCKETSERV_H__063CE15A_116B_4158_BD12_B0AD025D286C__INCLUDED_)
#define AFX_SOCKETSERV_H__063CE15A_116B_4158_BD12_B0AD025D286C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketServ.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CSocketServ command target

class CSocketServ : public CSocket
{
// Attributes
public:

// Operations
public:
	CSocketServ();
	virtual ~CSocketServ();

// Overrides
public:
	BOOL IsConnect();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketServ)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSocketServ)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETSERV_H__063CE15A_116B_4158_BD12_B0AD025D286C__INCLUDED_)
