// PropertyDlgRTDBHistSeriesBatWP.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgRTDBHistSeriesBatWP.h"
#include "DlgVarWithPort.h"
#include "AfxDB.h"
#include "DlgProgress.h"
#include "CSpreadSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropertyDlgRTDBHistSeriesBatWP dialog


PropertyDlgRTDBHistSeriesBatWP::PropertyDlgRTDBHistSeriesBatWP(CWnd* pParent /*=NULL*/)
	: CDialog(PropertyDlgRTDBHistSeriesBatWP::IDD, pParent)
{
	//{{AFX_DATA_INIT(PropertyDlgRTDBHistSeriesBatWP)
		// NOTE: the ClassWizard will add member initialization here
	iSnapshotSpan = 0;
	//}}AFX_DATA_INIT
}


void PropertyDlgRTDBHistSeriesBatWP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropertyDlgRTDBHistSeriesBatWP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT_SPAN, iSnapshotSpan);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropertyDlgRTDBHistSeriesBatWP, CDialog)
	//{{AFX_MSG_MAP(PropertyDlgRTDBHistSeriesBatWP)
		// NOTE: the ClassWizard will add message map macros here
	ON_BN_CLICKED(IDC_BUTTON_INPUT, OnButtonInput)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, OnButtonOutput)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropertyDlgRTDBHistSeriesBatWP message handlers

BOOL PropertyDlgRTDBHistSeriesBatWP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(iRawOrSnapshot==0)
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);
		((CWnd*)GetDlgItem(IDC_EDIT_SPAN))->EnableWindow(FALSE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1);
	}
	initListBoxHead();
	initListBoxBody();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void PropertyDlgRTDBHistSeriesBatWP::initListBoxHead()
{
	UpdateData();
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[5]={"序号","变量名称","端口名称","单位","描述"};
	//列表视的表头
	m_List.InsertColumn (0, Field[0], LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, Field[1], LVCFMT_LEFT, 120);
	m_List.InsertColumn (2, Field[2], LVCFMT_LEFT, 120);
	m_List.InsertColumn (3, Field[3], LVCFMT_LEFT, 40);
	m_List.InsertColumn (4, Field[4], LVCFMT_LEFT, 80);
}
void PropertyDlgRTDBHistSeriesBatWP::initListBoxBody()
{
	int nItem=0;
	CString str;
	for(int i=0;i<this->strVarArr.GetSize();i++)
	{
		str.Format("%d",nItem+1);
		m_List.InsertItem(nItem,str);
		m_List.SetItemText(nItem,1,strVarArr[i]);
		m_List.SetItemText(nItem,2,strPortArr[i]);
		m_List.SetItemText(nItem,3,this->strUnitArr[i]);
		m_List.SetItemText(nItem,4,this->strTagDespArr[i]);	
		nItem++;
	}
}
void PropertyDlgRTDBHistSeriesBatWP::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	CDlgVarWithPort dlg;
	if(IDOK==dlg.DoModal())
	{
		strVarArr.Add(dlg.m_strVarName);
		strPortArr.Add(dlg.m_strPortName);
		strTagDespArr.Add(dlg.m_strDesp);
		strUnitArr.Add(dlg.m_strUnit);
		initListBoxHead();
		initListBoxBody();
	}
}

void PropertyDlgRTDBHistSeriesBatWP::OnButtonEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL)
		nSelectedItem = m_List.GetNextSelectedItem(pos);
	if(nSelectedItem<0)
	{ 
		 AfxMessageBox("请选择变量。");
		 return;
	}
	CDlgVarWithPort dlg;
	dlg.m_strVarName=strVarArr[nSelectedItem];
	dlg.m_strPortName=strPortArr[nSelectedItem];
	dlg.m_strUnit=strUnitArr[nSelectedItem];
	dlg.m_strDesp=strTagDespArr[nSelectedItem];
	if(IDOK==dlg.DoModal())
	{
		strVarArr[nSelectedItem]=dlg.m_strVarName;
		strPortArr[nSelectedItem]=dlg.m_strPortName;
		strUnitArr[nSelectedItem]=dlg.m_strUnit;
		strTagDespArr[nSelectedItem]=dlg.m_strDesp;
		initListBoxHead();
		initListBoxBody();
	}
}

void PropertyDlgRTDBHistSeriesBatWP::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	CArray<int,int> indexSelArr;
	while(pos != NULL)
	{
		int iSel= m_List.GetNextSelectedItem(pos);
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
		strVarArr.RemoveAt(indexSelArr[i]);
		strPortArr.RemoveAt(indexSelArr[i]);
		strUnitArr.RemoveAt(indexSelArr[i]);
		strTagDespArr.RemoveAt(indexSelArr[i]);
	}
	initListBoxHead();
	initListBoxBody();
}

void PropertyDlgRTDBHistSeriesBatWP::OnButtonInput() 
{
	// TODO: Add your control notification handler code here
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
			if((iColCount>3)&&(iRowCount>1))
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

				strVarArr.RemoveAll();//
				strPortArr.RemoveAll();//
				strUnitArr.RemoveAll();//单位
				strTagDespArr.RemoveAll();//描述

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
					strVarArr.Add(strValueArr[1]);
					strPortArr.Add(strValueArr[2]);
					strUnitArr.Add(strValueArr[3]);
					strTagDespArr.Add(strValueArr[4]);
					
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
				initListBoxHead();
				initListBoxBody();
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
		initListBoxHead();
		initListBoxBody();
	}
}

void PropertyDlgRTDBHistSeriesBatWP::OnButtonOutput() 
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
			sSql.Format("CREATE TABLE %s (ID NUMBER,varName TEXT,portName TEXT,unit TEXT,desp TEXT)",
					strTable);
				database.ExecuteSQL(sSql);
			//导入数据
			for(int i=0;i<strVarArr.GetSize();i++)
			{
				if(ProgDlg.m_bCancel) 
				{
					ProgDlg.PostMessage(WM_CLOSE, 0, 0);
					break;
				}
				sSql.Format("INSERT INTO %s (ID,varName,portName,unit,desp) VALUES (%d,'%s','%s','%s','%s')",
					strTable,
					i+1,
					strVarArr[i],
					strPortArr[i],
					strUnitArr[i],
					strTagDespArr[i]
					);
				database.ExecuteSQL(sSql);//
				
				float persent =(float)((float)i/(float)strVarArr.GetSize())*100;
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


void PropertyDlgRTDBHistSeriesBatWP::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())
	{
		iRawOrSnapshot=0;
	}
	else
	{
		iRawOrSnapshot=1;
	}
	CDialog::OnOK();
}

void PropertyDlgRTDBHistSeriesBatWP::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())
	{
		((CWnd*)GetDlgItem(IDC_EDIT_SPAN))->EnableWindow(FALSE);
	}
	else
	{
		((CWnd*)GetDlgItem(IDC_EDIT_SPAN))->EnableWindow(TRUE);
	}
}

void PropertyDlgRTDBHistSeriesBatWP::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())
	{
		((CWnd*)GetDlgItem(IDC_EDIT_SPAN))->EnableWindow(FALSE);
	}
	else
	{
		((CWnd*)GetDlgItem(IDC_EDIT_SPAN))->EnableWindow(TRUE);
	}
}

void PropertyDlgRTDBHistSeriesBatWP::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonEdit();
	*pResult = 0;
}
