#if !defined(AFX_SOCKETMANAGE_H__47766048_9128_4D96_87F4_05DC5248ABE5__INCLUDED_)
#define AFX_SOCKETMANAGE_H__47766048_9128_4D96_87F4_05DC5248ABE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketManage.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CSocketManage command target

class CSocketManage : public CSocket
{
// Attributes
public:

// Operations
public:
	CSocketManage();
	virtual ~CSocketManage();

// Overrides
public:
	BOOL IsConnect();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketManage)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSocketManage)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETMANAGE_H__47766048_9128_4D96_87F4_05DC5248ABE5__INCLUDED_)
