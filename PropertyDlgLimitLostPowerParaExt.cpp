// PropertyDlgLimitLostPowerParaExt.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgLimitLostPowerParaExt.h"
#include "AfxDB.h"
#include "DlgProgress.h"
#include "CSpreadSheet.h"
#include "DlgWindPower.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgLimitLostPowerParaExt dialog


CPropertyDlgLimitLostPowerParaExt::CPropertyDlgLimitLostPowerParaExt(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgLimitLostPowerParaExt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgLimitLostPowerParaExt)
		// NOTE: the ClassWizard will add member initialization here
	dMaxPower = 0.0;
	//}}AFX_DATA_INIT
}


void CPropertyDlgLimitLostPowerParaExt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgLimitLostPowerParaExt)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LIST4, m_List2);
	DDX_Text(pDX, IDC_EDIT_VALID, dMaxPower);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgLimitLostPowerParaExt, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgLimitLostPowerParaExt)
	// NOTE: the ClassWizard will add message map macros here
	ON_BN_CLICKED(IDC_BUTTON_INPUT2, OnButtonInput2)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT2, OnButtonOutput2)
	ON_BN_CLICKED(IDC_BUTTON_ADD2, OnButtonAdd2)
	ON_BN_CLICKED(IDC_BUTTON_EDIT2, OnButtonEdit2)
	ON_BN_CLICKED(IDC_BUTTON_DEL2, OnButtonDel2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST4, OnDblclkList4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgLimitLostPowerParaExt message handlers


BOOL CPropertyDlgLimitLostPowerParaExt::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(iTimeSelfOrInput==0)
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1);
	}
	initListBoxHead2();
	initListBoxBody2();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgLimitLostPowerParaExt::initListBoxHead2()
{
	UpdateData();
	m_List2.DeleteAllItems();
	while(m_List2.DeleteColumn(0));

	DWORD liststyle=m_List2.GetExtendedStyle();//获取列表控件的扩展风格
	m_List2.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[3]={"序号","风速(m/s)","功率(kw)"};
	//列表视的表头
	m_List2.InsertColumn (0, Field[0], LVCFMT_LEFT, 40);
	m_List2.InsertColumn (1, Field[1], LVCFMT_LEFT, 120);
	m_List2.InsertColumn (2, Field[2], LVCFMT_LEFT, 120);
}
void CPropertyDlgLimitLostPowerParaExt::initListBoxBody2()
{
	int nItem=0;
	CString str;
	for(int i=0;i<this->dWindSpeedArr.GetSize();i++)
	{
		str.Format("%d",nItem+1);
		m_List2.InsertItem(nItem,str);
		str.Format("%f",dWindSpeedArr[i]);
		m_List2.SetItemText(nItem,1,str);
		str.Format("%f",dPowerArr[i]);
		m_List2.SetItemText(nItem,2,str);
		nItem++;
	}
}
void CPropertyDlgLimitLostPowerParaExt::OnButtonInput2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_List2.DeleteAllItems();
	while(m_List2.DeleteColumn(0));

	DWORD liststyle=m_List2.GetExtendedStyle();//获取列表控件的扩展风格
	m_List2.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[3]={"序号","风速(m/s)","功率(kw)"};
	//列表视的表头
	m_List2.InsertColumn (0, Field[0], LVCFMT_LEFT, 40);
	m_List2.InsertColumn (1, Field[1], LVCFMT_LEFT, 120);
	m_List2.InsertColumn (2, Field[2], LVCFMT_LEFT, 120);

	CFileDialog       dlg( TRUE,".xls", NULL, OFN_NOCHANGEDIR, "Worksheet Files (*.xls)|*.xls||",   NULL   );   
	DWORD   MAXFILE   = 4000 ; 
	dlg.m_ofn.nMaxFile   = MAXFILE;   
	char*  pc =new  char[MAXFILE];   
	dlg.m_ofn.lpstrFile   = pc;   
	dlg.m_ofn.lpstrFile[0]   = NULL;
	bool bInput=false;
    
	int   iReturn  =dlg.DoModal();   
	if(iReturn == IDOK)   
	{   
		int   nCount   =   0;   
		POSITION   pos   =   dlg.GetStartPosition();
		CString FileName;
		CDlgProgress ProgDlg;
		if   (pos   !=   NULL)   
		{
			FileName = dlg.GetNextPathName(pos);	
			int iItem=0;
			CString strSheetName="VarData";
			CSpreadSheet SS(FileName,strSheetName,false);
			int iRowCount=SS.GetTotalRows();
			int iColCount=SS.GetTotalColumns();
			if((iColCount>2)&&(iRowCount>1))
			{
				MSG msg;
				ProgDlg.Create(IDD_DLGPROGRESS, NULL);
				ProgDlg.ShowWindow(SW_SHOW);
				ProgDlg.CenterWindow();
				ProgDlg.UpdateData(FALSE);
				ProgDlg.m_bCancel = FALSE;
				
				CStringArray headArr;
				SS.ReadRow(headArr,1);
				CStringArray strValueArr;

				dWindSpeedArr.RemoveAll();//
				dPowerArr.RemoveAll();//

				bInput=true;
				for(int j=2;j<=iRowCount;j++)
				{
					if(ProgDlg.m_bCancel) 
					{
						ProgDlg.PostMessage(WM_CLOSE, 0, 0);
						break;
					}
					SS.ReadRow(strValueArr,j);
					if(strValueArr.GetSize()<3)//数据不全
					{
						continue;
					}
					if(strValueArr[1]=="")//点名为空
					{
						continue;
					}
					dWindSpeedArr.Add(atof(strValueArr[1]));
					dPowerArr.Add(atof(strValueArr[2]));
					
					float persent =(float)((float)iItem/(float)(iRowCount-1))*100;
					ProgDlg.SendMessage(WM_MSG_PROGESS,(unsigned int)persent,0);
					while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
					{
						if (!ProgDlg.IsDialogMessage(&msg))
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}
					}
					iItem++;
				}
				ProgDlg.ShowWindow(SW_HIDE);
				ProgDlg.SendMessage(WM_CLOSE, 0, 0);
				initListBoxHead2();
				initListBoxBody2();
			}
		}
		if(!ProgDlg.m_bCancel)
		{
			AfxMessageBox("变量导入成功！");
		}
	}
	delete   []pc;
	pc=NULL;
	if(bInput)
	{
		initListBoxHead2();
		initListBoxBody2();
	}
}

void CPropertyDlgLimitLostPowerParaExt::OnButtonOutput2() 
{
	// TODO: Add your control notification handler code here
	CString filename;
    CFileDialog FileDlg (FALSE, _T("xls"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
						 _T("Excel工作薄 (*.xls)|*.xls"),
						 this);
	
	if (FileDlg.DoModal() != IDOK ) return;
	else
	{
		filename = FileDlg.GetPathName();
	}
	CDatabase database;
	CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel安装驱动
	CString sSql;
	CDlgProgress ProgDlg;
	try
	{
		// 创建进行存取的字符串
		sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, filename, filename);
		// 创建数据库 (既Excel表格文件)
		if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )
		{
			MSG msg;
			ProgDlg.Create(IDD_DLGPROGRESS, NULL);
			ProgDlg.ShowWindow(SW_SHOW);
			ProgDlg.CenterWindow();
			ProgDlg.UpdateData(FALSE);
			ProgDlg.m_bCancel = FALSE;
			//建表
			CString strTable;
			strTable.Format("VarData");
			sSql.Format("CREATE TABLE %s (ID NUMBER,WindSpeed TEXT,Power TEXT)",
					strTable);
				database.ExecuteSQL(sSql);
			//导入数据
			for(int i=0;i<dWindSpeedArr.GetSize();i++)
			{
				if(ProgDlg.m_bCancel) 
				{
					ProgDlg.PostMessage(WM_CLOSE, 0, 0);
					break;
				}
				sSql.Format("INSERT INTO %s (ID,WindSpeed,Power) VALUES (%d,'%f','%f')",
					strTable,
					i+1,
					dWindSpeedArr[i],
					dPowerArr[i]
					);
				database.ExecuteSQL(sSql);//
				
				float persent =(float)((float)i/(float)dWindSpeedArr.GetSize())*100;
				ProgDlg.SendMessage(WM_MSG_PROGESS,(unsigned int)persent,0);
				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					if (!ProgDlg.IsDialogMessage(&msg))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
			}
			ProgDlg.ShowWindow(SW_HIDE);
			ProgDlg.SendMessage(WM_CLOSE, 0, 0);
		}
		// 关闭数据库
		database.Close();
		if(!ProgDlg.m_bCancel)
		{
			AfxMessageBox("Excel文件写入成功！");
		}
	}
	catch(CException ex)
	{
		ex.ReportError();
	}
}

void CPropertyDlgLimitLostPowerParaExt::OnButtonAdd2() 
{
	// TODO: Add your control notification handler code here
	CDlgWindPower dlg;
	if(IDOK==dlg.DoModal())
	{
		dWindSpeedArr.Add(dlg.m_dWindSpeed);
		dPowerArr.Add(dlg.m_dPower);
		initListBoxHead2();
		initListBoxBody2();
	}
}

void CPropertyDlgLimitLostPowerParaExt::OnButtonEdit2() 
{
	// TODO: Add your control notification handler code here
		POSITION pos = m_List2.GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL)
		nSelectedItem = m_List2.GetNextSelectedItem(pos);
	if(nSelectedItem<0)
	{ 
		 AfxMessageBox("请选择变量。");
		 return;
	}
	CDlgWindPower dlg;
	dlg.m_dWindSpeed=dWindSpeedArr[nSelectedItem];
	dlg.m_dPower=dPowerArr[nSelectedItem];
	if(IDOK==dlg.DoModal())
	{
		dWindSpeedArr[nSelectedItem]=dlg.m_dWindSpeed;
		dPowerArr[nSelectedItem]=dlg.m_dPower;
		initListBoxHead2();
		initListBoxBody2();
	}
}

void CPropertyDlgLimitLostPowerParaExt::OnButtonDel2() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_List2.GetFirstSelectedItemPosition();
	CArray<int,int> indexSelArr;
	while(pos != NULL)
	{
		int iSel= m_List2.GetNextSelectedItem(pos);
		indexSelArr.Add(iSel);
	}
	if(indexSelArr.GetSize()==0)
	{ 
		 AfxMessageBox("请选择变量。");
		 return;
	}
	for(int i=0;i<indexSelArr.GetSize();i++)//索引从大到小排序
	{
		for(int j=i+1;j<indexSelArr.GetSize();j++)
		{
			if(indexSelArr[i]<indexSelArr[j])
			{
				int iTemp=indexSelArr[i];
				indexSelArr[i]=indexSelArr[j];
				indexSelArr[j]=iTemp;
			}
		}
	}
	for(i=0;i<indexSelArr.GetSize();i++)
	{
		dWindSpeedArr.RemoveAt(indexSelArr[i]);
		dPowerArr.RemoveAt(indexSelArr[i]);
	}
	initListBoxHead2();
	initListBoxBody2();
}

void CPropertyDlgLimitLostPowerParaExt::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())
	{
		iTimeSelfOrInput=0;
	}
	else
	{
		iTimeSelfOrInput=1;
	}
	CDialog::OnOK();
}

void CPropertyDlgLimitLostPowerParaExt::OnDblclkList4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonEdit2();	
	*pResult = 0;
}