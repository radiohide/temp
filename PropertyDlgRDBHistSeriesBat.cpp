// PropertyDlgRDBHistSeriesBat.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgRDBHistSeriesBat.h"
#include "PropertyDlgRDBField.h"
#include "DlgVarWithTable.h"
#include "AfxDB.h"
#include "DlgProgress.h"
#include "CSpreadSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRDBHistSeriesBat dialog


CPropertyDlgRDBHistSeriesBat::CPropertyDlgRDBHistSeriesBat(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgRDBHistSeriesBat::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgRDBHistSeriesBat)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropertyDlgRDBHistSeriesBat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgRDBHistSeriesBat)
	DDX_Control(pDX, IDC_COMBO_DBSEL, m_comboBoxDBSel);
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgRDBHistSeriesBat, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgRDBHistSeriesBat)
	ON_CBN_SELCHANGE(IDC_COMBO_DBSEL, OnSelchangeComboDbsel)
	ON_BN_CLICKED(IDC_BUTTON_RDBFIELD, OnButtonRdbfield)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_INPUT, OnButtonInput)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, OnButtonOutput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgRDBHistSeriesBat message handlers
void CPropertyDlgRDBHistSeriesBat::SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
		CStringArray &m_ArrSourceRDBName,
		CStringArray &m_ArrSourceRDBType
		)
{
	this->m_ArrSourceRDBTagCon.Copy(m_ArrSourceRDBTagCon);
	this->m_ArrSourceRDBName.Copy(m_ArrSourceRDBName);
	this->m_ArrSourceRDBType.Copy(m_ArrSourceRDBType);
}

BOOL CPropertyDlgRDBHistSeriesBat::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	for(int i=0;i<m_ArrSourceRDBTagCon.GetSize();i++)
	{
		if(m_ArrSourceRDBTagCon[i]==1)
		{
			despArr.Add(m_ArrSourceRDBName[i]);
			typeArr.Add(m_ArrSourceRDBType[i]);
			this->m_comboBoxDBSel.AddString(m_ArrSourceRDBName[i]);
		}
	}
	GetDlgItem(IDC_COMBO_DBSEL)->SetWindowText(m_strDBName);
	int iBeFind=-1;
	for(i=0;i<despArr.GetSize();i++)
	{
		if(despArr[i]==m_strDBName)
		{
			iBeFind=i;
			break;
		}
	}
	if(iBeFind>=0)
	{
		m_comboBoxDBSel.SetCurSel(iBeFind);
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(typeArr[iBeFind]);
	}
	
	// TODO: Add extra initialization here
	initListBoxHead();
	initListBoxBody();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgRDBHistSeriesBat::initListBoxHead()
{
	UpdateData();
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[5]={"序号","表名称","变量名称","单位","描述"};
	//列表视的表头
	m_List.InsertColumn (0, Field[0], LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, Field[1], LVCFMT_LEFT, 120);
	m_List.InsertColumn (2, Field[2], LVCFMT_LEFT, 120);
	m_List.InsertColumn (3, Field[3], LVCFMT_LEFT, 40);
	m_List.InsertColumn (4, Field[4], LVCFMT_LEFT, 80);
}
void CPropertyDlgRDBHistSeriesBat::initListBoxBody()
{
	int nItem=0;
	CString str;
	for(int i=0;i<this->strTableArr.GetSize();i++)
	{
		str.Format("%d",nItem+1);
		m_List.InsertItem(nItem,str);
		m_List.SetItemText(nItem,1,strTableArr[i]);
		m_List.SetItemText(nItem,2,strTagArr[i]);
		m_List.SetItemText(nItem,3,this->strUnitArr[i]);
		m_List.SetItemText(nItem,4,this->strTagDespArr[i]);	
		nItem++;
	}
}

void CPropertyDlgRDBHistSeriesBat::OnSelchangeComboDbsel() 
{
	// TODO: Add your control notification handler code here
	int selIndex=this->m_comboBoxDBSel.GetCurSel();
	if(selIndex>=0)
	{
		CString str=typeArr[selIndex];
		m_strDBName=despArr[selIndex];
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	else
	{
		CString str="";
		m_strDBName="";
		GetDlgItem(IDC_EDIT_DBTYPE)->SetWindowText(str);
	}
	m_comboBoxDBSel.SetCurSel(selIndex);
	UpdateData(false);
}

void CPropertyDlgRDBHistSeriesBat::OnButtonRdbfield() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgRDBField dlg;	
	dlg.m_pStrFieldVarName=&m_strFieldVarName;
	dlg.m_pStrFieldDateTime=&m_strFieldDateTime;
	dlg.m_pStrFieldState=&m_strFieldState;
	dlg.m_pStrFieldValue=&m_strFieldValue;
	dlg.DoModal();
}

void CPropertyDlgRDBHistSeriesBat::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	CDlgVarWithTable dlg;
	if(IDOK==dlg.DoModal())
	{
		strTableArr.Add(dlg.m_strTableName);
		strTagArr.Add(dlg.m_strVarName);
		strTagDespArr.Add(dlg.m_strDesp);
		strUnitArr.Add(dlg.m_strUnit);
		initListBoxHead();
		initListBoxBody();
	}
}

void CPropertyDlgRDBHistSeriesBat::OnButtonEdit() 
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
	CDlgVarWithTable dlg;
	dlg.m_strTableName=strTableArr[nSelectedItem];
	dlg.m_strVarName=strTagArr[nSelectedItem];
	dlg.m_strUnit=strUnitArr[nSelectedItem];
	dlg.m_strDesp=strTagDespArr[nSelectedItem];
	if(IDOK==dlg.DoModal())
	{
		strTableArr[nSelectedItem]=dlg.m_strTableName;
		strTagArr[nSelectedItem]=dlg.m_strVarName;
		strUnitArr[nSelectedItem]=dlg.m_strUnit;
		strTagDespArr[nSelectedItem]=dlg.m_strDesp;
		initListBoxHead();
		initListBoxBody();
	}
}

void CPropertyDlgRDBHistSeriesBat::OnButtonDel() 
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
		strTableArr.RemoveAt(indexSelArr[i]);
		strTagArr.RemoveAt(indexSelArr[i]);
		strUnitArr.RemoveAt(indexSelArr[i]);
		strTagDespArr.RemoveAt(indexSelArr[i]);
	}
	initListBoxHead();
	initListBoxBody();
}

void CPropertyDlgRDBHistSeriesBat::OnButtonInput() 
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

				strTableArr.RemoveAll();//
				strTagArr.RemoveAll();//
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
					strTableArr.Add(strValueArr[1]);
					strTagArr.Add(strValueArr[2]);
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

void CPropertyDlgRDBHistSeriesBat::OnButtonOutput() 
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
			sSql.Format("CREATE TABLE %s (ID NUMBER,tableName TEXT,varName TEXT,unit TEXT,desp TEXT)",
					strTable);
				database.ExecuteSQL(sSql);
			//导入数据
			for(int i=0;i<strTableArr.GetSize();i++)
			{
				if(ProgDlg.m_bCancel) 
				{
					ProgDlg.PostMessage(WM_CLOSE, 0, 0);
					break;
				}
				sSql.Format("INSERT INTO %s (ID,tableName,varName,unit,desp) VALUES (%d,'%s','%s','%s','%s')",
					strTable,
					i+1,
					strTableArr[i],
					strTagArr[i],
					strUnitArr[i],
					strTagDespArr[i]
					);
				database.ExecuteSQL(sSql);//
				
				float persent =(float)((float)i/(float)strTableArr.GetSize())*100;
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
