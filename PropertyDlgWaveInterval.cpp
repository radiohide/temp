// PropertyDlgWaveInterval.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgWaveInterval.h"
#include "DlgDisData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgWaveInterval dialog


CPropertyDlgWaveInterval::CPropertyDlgWaveInterval(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgWaveInterval::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgWaveInterval)
	m_dSpanIntPercent = 0.0;
	m_iSpanKeepTime = 0;
	m_iSmoothStep = 0;
	m_iSmoothWidth = 0;
	//}}AFX_DATA_INIT
}


void CPropertyDlgWaveInterval::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgWaveInterval)
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT_SPAN_INT_PERCENT, m_dSpanIntPercent);
	DDX_Text(pDX, IDC_EDIT_SPAN_KEEP_TIME, m_iSpanKeepTime);
	DDX_Text(pDX, IDC_EDIT_S_STEP, m_iSmoothStep);
	DDX_Text(pDX, IDC_EDIT_S_WIDTH, m_iSmoothWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgWaveInterval, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgWaveInterval)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgWaveInterval message handlers

BOOL CPropertyDlgWaveInterval::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitListBoxHead();
	InitListBoxBody();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgWaveInterval::InitListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[6]={"序号","离散值","区间起点","区间终点"};
					//列表视的表头
	m_List.InsertColumn(0,Field[0],LVCFMT_LEFT,40);
	m_List.InsertColumn(1,Field[1],LVCFMT_LEFT,80);	
	m_List.InsertColumn(2,Field[2],LVCFMT_LEFT,80);	
	m_List.InsertColumn(3,Field[3],LVCFMT_LEFT,80);
}
void CPropertyDlgWaveInterval::InitListBoxBody()
{
	for(int i=0;i<m_dArrDisValue.GetSize();i++)
	{
		CString strIndex,str;
		strIndex.Format("%d",i+1);
		m_List.InsertItem(i,strIndex);	
		str.Format("%.3f",m_dArrDisValue[i]);
		this->m_List.SetItemText(i,1,str);
		str.Format("%.3f",m_dArrIntStart[i]);
		this->m_List.SetItemText(i,2,str);
		str.Format("%.3f",m_dArrIntEnd[i]);
		this->m_List.SetItemText(i,3,str);
	}
}

void CPropertyDlgWaveInterval::OnOK()
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_iSmoothWidth<=0)
	{
		AfxMessageBox("滑动宽度应为正整数，请重新输入！");
		return;
	}
	if(m_iSmoothStep<=0)
	{
		AfxMessageBox("滑动步长应为正整数，请重新输入！");
		return;
	}
	if(m_dArrDisValue.GetSize()==0)
	{
		AfxMessageBox("请输入离散区间！");
		return;
	}
	if((m_dSpanIntPercent>100)||(m_dSpanIntPercent<0))
	{
		AfxMessageBox("波动区间范围应为[0,100]，请重新输入！");
		return;
	}
	if(m_iSpanKeepTime<=0)
	{
		AfxMessageBox("波动持续时间应为正整数，请重新输入！");
		return;
	}
	//对离散区间进行排序
	for(int i=0;i<m_dArrIntStart.GetSize();i++)
	{
		for(int j=i+1;j<m_dArrIntStart.GetSize();j++)
		{
			if(m_dArrIntStart[i]>m_dArrIntStart[j])
			{
				double temp=m_dArrDisValue[i];
				m_dArrDisValue[i]=m_dArrDisValue[j];
				m_dArrDisValue[j]=temp;

				temp=m_dArrIntStart[i];
				m_dArrIntStart[i]=m_dArrIntStart[j];
				m_dArrIntStart[j]=temp;

				temp=m_dArrIntEnd[i];
				m_dArrIntEnd[i]=m_dArrIntEnd[j];
				m_dArrIntEnd[j]=temp;
			}
		}
	}
	CDialog::OnOK();
}

void CPropertyDlgWaveInterval::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	CDlgDisData dlg;
	if(IDOK==dlg.DoModal())
	{
		m_dArrDisValue.Add(dlg.m_dDisValue);
		m_dArrIntStart.Add(dlg.m_dSValue);
		m_dArrIntEnd.Add(dlg.m_dEValue);
		InitListBoxHead();
		InitListBoxBody();
	}
}

void CPropertyDlgWaveInterval::OnButtonEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos)
	{
		CDlgDisData dlg;
		int index=m_List.GetNextSelectedItem(pos);
		dlg.m_dDisValue=atof(m_List.GetItemText(index,1));
		dlg.m_dSValue=atof(m_List.GetItemText(index,2));
		dlg.m_dEValue=atof(m_List.GetItemText(index,3));
		if(IDOK==dlg.DoModal())
		{
			CString str;
			str.Format("%f",dlg.m_dDisValue);
			m_List.SetItemText(index,1,str);
			str.Format("%f",dlg.m_dSValue);
			m_List.SetItemText(index,2,str);
			str.Format("%f",dlg.m_dEValue);
			m_List.SetItemText(index,3,str);

			m_dArrDisValue[index]=dlg.m_dDisValue;
			m_dArrIntStart[index]=dlg.m_dSValue;
			m_dArrIntEnd[index]=dlg.m_dEValue;
		}
	}
	else
	{
		AfxMessageBox("请选择要删除的离散区间！");
	}
}

void CPropertyDlgWaveInterval::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	CArray<int,int> delIndexArr;
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(!pos)
	{
		AfxMessageBox("请选择要删除的离散区间！");
	}
	while(pos)
	{
		int index=m_List.GetNextSelectedItem(pos);
		delIndexArr.Add(index);
	}
	for(int i=delIndexArr.GetSize()-1;i>=0;i--)
	{
		m_List.DeleteItem(delIndexArr[i]);
		m_dArrDisValue.RemoveAt(delIndexArr[i]);
		m_dArrIntStart.RemoveAt(delIndexArr[i]);
		m_dArrIntEnd.RemoveAt(delIndexArr[i]);
	}
}

void CPropertyDlgWaveInterval::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonEdit();
	*pResult = 0;
}
