// PropertyDlgLinkFromProBat.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgLinkFromProBat.h"
#include "DlgProBatOutput.h"
#include "DlgProgress.h"
#include "CSpreadSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgLinkFromProBat dialog


CPropertyDlgLinkFromProBat::CPropertyDlgLinkFromProBat(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgLinkFromProBat::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgLinkFromProBat)
	m_showText = _T("");
	//}}AFX_DATA_INIT
}


void CPropertyDlgLinkFromProBat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgLinkFromProBat)
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_STATIC_DESP, m_showText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgLinkFromProBat, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgLinkFromProBat)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_INPUT, OnButtonInput)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, OnButtonOutput)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPropertyDlgLinkFromProBat::OnInitDialog()
{
	CDialog::OnInitDialog();

	initListBoxHead();
	initListBoxBody();

	switch(m_nWindowsType)
	{
	case 1:
		m_showText = "基本功能类。从公共区域获取过程数据。";
		break;
	case 2:
		m_showText = "基本功能类。从公共区域获取过程数据序列。";
		break;
	case 3:
		m_showText = "基本功能类。从公共区域获取统计数据。";
		break;
	case 4:
		m_showText = "基本功能类。从公共区域获取统计数据序列。";
		break;
	default:
		break;
	}
	UpdateData(FALSE);

	return TRUE;
}

void CPropertyDlgLinkFromProBat::initListBoxBody()
{
	int nItem=0;
	CString str;
	for(int i=0;i<this->strOutputArr.GetSize();i++)
	{
		str.Format("%d",nItem+1);
		m_List.InsertItem(nItem,str);
		//m_List.SetItemText(nItem,1,strInputArr[i]);
		m_List.SetItemText(nItem,1,strOutputArr[i]);
		//m_List.SetItemText(nItem,3,this->strUnitArr[i]);
		m_List.SetItemText(nItem,2,this->strTagDespArr[i]);	
		nItem++;
	}
}

void CPropertyDlgLinkFromProBat::initListBoxHead()
{
	UpdateData();
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	//窗口初始化
	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[3]={"序号","输入变量","描述"};
	//列表视的表头
	m_List.InsertColumn (0, Field[0], LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, Field[1], LVCFMT_LEFT, 120);
	//m_List.InsertColumn (2, Field[2], LVCFMT_LEFT, 120);
	//m_List.InsertColumn (3, Field[3], LVCFMT_LEFT, 40);
	m_List.InsertColumn (2, Field[2], LVCFMT_LEFT, 80);
}



/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgProcToStatis message handlers

void CPropertyDlgLinkFromProBat::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	CDlgProBatOutput dlg;
	if(IDOK==dlg.DoModal())
	{
		//strInputArr.Add(dlg.m_strInput);
		strOutputArr.Add(dlg.m_strOutput);
		//strUnitArr.Add(dlg.m_strUnit);
		strTagDespArr.Add(dlg.m_strDesp);
		initListBoxHead();
		initListBoxBody();
	}
}

void CPropertyDlgLinkFromProBat::OnButtonEdit() 
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
	CDlgProBatOutput dlg;
	//dlg.m_strInput=strInputArr[nSelectedItem];
	dlg.m_strOutput=strOutputArr[nSelectedItem];
	//dlg.m_strUnit=strUnitArr[nSelectedItem];
	dlg.m_strDesp=strTagDespArr[nSelectedItem];
	if(IDOK==dlg.DoModal())
	{
		//strInputArr[nSelectedItem]=dlg.m_strInput;
		strOutputArr[nSelectedItem]=dlg.m_strOutput;
		//strUnitArr[nSelectedItem]=dlg.m_strUnit;
		strTagDespArr[nSelectedItem]=dlg.m_strDesp;
		initListBoxHead();
		initListBoxBody();
	}
	
}

void CPropertyDlgLinkFromProBat::OnButtonDel() 
{
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
		//strInputArr.RemoveAt(indexSelArr[i]);
		strOutputArr.RemoveAt(indexSelArr[i]);
		//strUnitArr.RemoveAt(indexSelArr[i]);
		strTagDespArr.RemoveAt(indexSelArr[i]);
	}
	initListBoxHead();
	initListBoxBody();
}

void CPropertyDlgLinkFromProBat::OnButtonInput() 
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
				
				//strInputArr.RemoveAll();//
				strOutputArr.RemoveAll();//
				//strUnitArr.RemoveAll();//单位
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
					//strInputArr.Add(strValueArr[1]);
					strOutputArr.Add(strValueArr[1]);
					//strUnitArr.Add(strValueArr[3]);
					strTagDespArr.Add(strValueArr[2]);
					
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

void CPropertyDlgLinkFromProBat::OnButtonOutput() 
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
			sSql.Format("CREATE TABLE %s (ID NUMBER,varName TEXT,desp TEXT)",
				strTable);
			database.ExecuteSQL(sSql);
			//导入数据
			for(int i=0;i<strOutputArr.GetSize();i++)
			{
				if(ProgDlg.m_bCancel) 
				{
					ProgDlg.PostMessage(WM_CLOSE, 0, 0);
					break;
				}
				sSql.Format("INSERT INTO %s (ID,varName,desp) VALUES (%d,'%s','%s')",
					strTable,
					i+1,
					strOutputArr[i],
					strTagDespArr[i]
					);
				database.ExecuteSQL(sSql);//
				
				float persent =(float)((float)i/(float)strOutputArr.GetSize())*100;
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

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgLinkFromProBat message handlers
