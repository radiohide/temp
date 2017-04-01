// PropertyDlgLinkTo.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "PropertyDlgLinkTo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgLinkTo dialog
extern CMap<CString,LPCTSTR,CCalcPort*,CCalcPort*&> g_WorkSpace;

CPropertyDlgLinkTo::CPropertyDlgLinkTo(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgLinkTo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgLinkTo)
	m_VarTag1 = _T("");
	m_VarTag2 = _T("");
	m_VarTag3 = _T("");
	m_VarTag4 = _T("");
	m_VarTag5 = _T("");

	m_StrType1 = _T("");
	m_StrType2 = _T("");
	m_StrType3 = _T("");
	m_StrType4 = _T("");
	m_StrType5 = _T("");

	//}}AFX_DATA_INIT
}


void CPropertyDlgLinkTo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgLinkTo)
	DDX_Text(pDX, IDC_EDIT1, m_VarTag1);
	DDX_Text(pDX, IDC_EDIT2, m_VarTag2);
	DDX_Text(pDX, IDC_EDIT3, m_VarTag3);
	DDX_Text(pDX, IDC_EDIT4, m_VarTag4);
	DDX_Text(pDX, IDC_EDIT5, m_VarTag5);

	DDX_CBString(pDX, IDC_COMBO_TYPE1, m_StrType1);
	DDX_CBString(pDX, IDC_COMBO_TYPE2, m_StrType2);
	DDX_CBString(pDX, IDC_COMBO_TYPE3, m_StrType3);
	DDX_CBString(pDX, IDC_COMBO_TYPE4, m_StrType4);
	DDX_CBString(pDX, IDC_COMBO_TYPE5, m_StrType5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgLinkTo, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgLinkTo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgLinkTo message handlers

void CPropertyDlgLinkTo::OnOK() 
{
	UpdateData(true);
	// TODO: Add extra validation here
	if((((m_VarTag1!="")&&((m_VarTag1==m_VarTag2)||(m_VarTag1==m_VarTag3)||(m_VarTag1==m_VarTag4)||(m_VarTag1==m_VarTag5)))||
		((m_VarTag2!="")&&((m_VarTag2==m_VarTag3)||(m_VarTag2==m_VarTag4)||(m_VarTag2==m_VarTag5)))||
		((m_VarTag3!="")&&((m_VarTag3==m_VarTag4)||(m_VarTag3==m_VarTag5)))||
		((m_VarTag4!="")&&((m_VarTag4==m_VarTag5)))))
	{
		AfxMessageBox("��ǩ���Ʋ����ظ�,����������!");
		return;
	}
	CCalcPort *pPortObj;
	CString str;
	if(g_WorkSpace.Lookup(m_VarTag1,pPortObj))
	{
		str.Format("��ǩ1: %s �Ѿ�����,����������!",m_VarTag1);
		AfxMessageBox(str);
		return;
	}
	if(g_WorkSpace.Lookup(m_VarTag2,pPortObj))
	{
		str.Format("��ǩ2: %s �Ѿ�����,����������!",m_VarTag2);
		AfxMessageBox(str);
		return;
	}
	if(g_WorkSpace.Lookup(m_VarTag3,pPortObj))
	{
		str.Format("��ǩ3: %s �Ѿ�����,����������!",m_VarTag3);
		AfxMessageBox(str);
		return;
	}
	if(g_WorkSpace.Lookup(m_VarTag4,pPortObj))
	{
		str.Format("��ǩ4: %s �Ѿ�����,����������!",m_VarTag4);
		AfxMessageBox(str);
		return;
	}
	if(g_WorkSpace.Lookup(m_VarTag5,pPortObj))
	{
		str.Format("��ǩ5: %s �Ѿ�����,����������!",m_VarTag5);
		AfxMessageBox(str);
		return;
	}
	CDialog::OnOK();
}

void CPropertyDlgLinkTo::SetType(int num,int type)
{
	switch(num)
	{
	case 1:
		if(type==0)
		{
			this->m_StrType1="��ֵ";
		}
		else
			this->m_StrType1="����";
		break;
	case 2:
		if(type==0)
		{
			this->m_StrType2="��ֵ";
		}
		else
			this->m_StrType2="����";
		break;
	case 3:
		if(type==0)
		{
			this->m_StrType3="��ֵ";
		}
		else
			this->m_StrType3="����";
		break;
	case 4:
		if(type==0)
		{
			this->m_StrType4="��ֵ";
		}
		else
			this->m_StrType4="����";
		break;
	case 5:
		if(type==0)
		{
			this->m_StrType5="��ֵ";
		}
		else
			this->m_StrType5="����";
		break;
	}
}
int CPropertyDlgLinkTo::GetType(int num)
{
	int type=0;
	switch(num)
	{
	case 1:
		if(this->m_StrType1=="��ֵ")
		{
			type=0;
		}
		else
		{
			type=1;
		}
		break;
	case 2:
		if(this->m_StrType2=="��ֵ")
		{
			type=0;
		}
		else
		{
			type=1;
		}
		break;
	case 3:
		if(this->m_StrType3=="��ֵ")
		{
			type=0;
		}
		else
		{
			type=1;
		}
		break;
	case 4:
		if(this->m_StrType4=="��ֵ")
		{
			type=0;
		}
		else
		{
			type=1;
		}
		break;
	case 5:
		if(this->m_StrType5=="��ֵ")
		{
			type=0;
		}
		else
		{
			type=1;
		}
		break;
	}
	return type;
}