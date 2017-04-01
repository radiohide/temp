// PropertyDlgStatAvg.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgStatAvg.h"
#include "DlgVarDataWithReq.h"
#include "AfxDB.h"
#include "DlgProgress.h"
#include "CSpreadSheet.h"
#include "DlgStatAdv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStatAvg dialog


CPropertyDlgStatAvg::CPropertyDlgStatAvg(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgStatAvg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgStatAvg)
	m_iConOn = FALSE;
	m_iCon1Fun = -1;
	m_iCon2Fun = -1;
	m_iConReqType = -1;
	m_strConDesp = _T("");
	m_strConUnit = _T("");
	m_strConVar = _T("");
	m_dCon1Value = 0.0;
	m_dCon2Value = 0.0;
	m_iConSnapShotT = 0;
	//}}AFX_DATA_INIT
}


void CPropertyDlgStatAvg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgStatAvg)
	DDX_Control(pDX, IDC_CHECK_CON, m_BtnCheckCon);
	DDX_Control(pDX, IDC_COMBO_CON_FUN2, m_ComBoConFun2);
	DDX_Control(pDX, IDC_COMBO_CON_REQ_TYPE, m_ComBoConReqType);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_COMBO_DBSEL, m_comboBoxDBSel);
	DDX_Check(pDX, IDC_CHECK_CON, m_iConOn);
	DDX_CBIndex(pDX, IDC_COMBO_CON_FUN1, m_iCon1Fun);
	DDX_CBIndex(pDX, IDC_COMBO_CON_FUN2, m_iCon2Fun);
	DDX_CBIndex(pDX, IDC_COMBO_CON_REQ_TYPE, m_iConReqType);
	DDX_Text(pDX, IDC_EDIT_CON_DESP, m_strConDesp);
	DDX_Text(pDX, IDC_EDIT_CON_UNIT, m_strConUnit);
	DDX_Text(pDX, IDC_EDIT_CON_VARNAME, m_strConVar);
	DDX_Text(pDX, IDC_EDIT_CON_VALUE1, m_dCon1Value);
	DDX_Text(pDX, IDC_EDIT_CON_VALUE2, m_dCon2Value);
	DDX_Text(pDX, IDC_EDIT_SNAP_CON_T, m_iConSnapShotT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgStatAvg, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgStatAvg)
	ON_CBN_SELCHANGE(IDC_COMBO_DBSEL, OnSelchangeComboDbsel)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, OnButtonOutput)
	ON_BN_CLICKED(IDC_BUTTON_INPUT, OnButtonInput)
	ON_BN_CLICKED(IDC_BUTTON_ADV, OnButtonAdv)
	ON_CBN_SELCHANGE(IDC_COMBO_CON_REQ_TYPE, OnSelchangeComboConReqType)
	ON_CBN_SELCHANGE(IDC_COMBO_CON_FUN2, OnSelchangeComboConFun2)
	ON_BN_CLICKED(IDC_CHECK_CON, OnCheckCon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStatAvg message handlers

BOOL CPropertyDlgStatAvg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
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

	initListBoxHead();
	initListBoxBody();
	initConVarExpr();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgStatAvg::initConVarExpr()
{
	if(this->m_iConOn==0)
	{
		GetDlgItem(IDC_EDIT_CON_VARNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CON_UNIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CON_DESP)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CON_REQ_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SNAP_CON_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CON_FUN1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CON_VALUE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CON_FUN2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CON_VALUE2)->EnableWindow(FALSE);
	}
	else
	{
		if(this->m_iConReqType==1)
			GetDlgItem(IDC_EDIT_SNAP_CON_T)->EnableWindow(FALSE);
		if(this->m_iCon2Fun==0)
			GetDlgItem(IDC_EDIT_CON_VALUE2)->EnableWindow(FALSE);
	}
}
void CPropertyDlgStatAvg::SetSourceRDBData(CArray<int,int> &m_ArrSourceRDBTagCon,
										   CStringArray &m_ArrSourceRDBName,
										   CStringArray &m_ArrSourceRDBType
										   )
{
	this->m_ArrSourceRDBTagCon.Copy(m_ArrSourceRDBTagCon);
	this->m_ArrSourceRDBName.Copy(m_ArrSourceRDBName);
	this->m_ArrSourceRDBType.Copy(m_ArrSourceRDBType);
}
void CPropertyDlgStatAvg::initListBoxHead()
{
	UpdateData();
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[7]={"序号","点名称","输出名称","单位","描述","查询方式","快照周期(s)"};
	//列表视的表头
	m_List.InsertColumn (0, Field[0], LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, Field[1], LVCFMT_LEFT, 120);
	m_List.InsertColumn (2, Field[2], LVCFMT_LEFT, 120);
	m_List.InsertColumn (3, Field[3], LVCFMT_LEFT, 40);
	m_List.InsertColumn (4, Field[4], LVCFMT_LEFT, 80);
	m_List.InsertColumn (5, Field[5], LVCFMT_LEFT, 60);
	m_List.InsertColumn (6, Field[6], LVCFMT_LEFT, 70);
}
void CPropertyDlgStatAvg::initListBoxBody()
{
	int nItem=0;
	CString str;
	for(int i=0;i<this->tagNameArr.GetSize();i++)
	{
		str.Format("%d",nItem+1);
		m_List.InsertItem(nItem,str);
		m_List.SetItemText(nItem,1,tagNameArr[i]);
		m_List.SetItemText(nItem,2,strOutPutNameArr[i]);
		m_List.SetItemText(nItem,3,this->unitArr[i]);
		m_List.SetItemText(nItem,4,this->dispArr[i]);
		if(this->reqTypeArr[i]==0)//快照
		{
			str.Format("snapshot");
			m_List.SetItemText(nItem,5,str);
			str.Format("%d",this->snapshotTArr[i]);
			m_List.SetItemText(nItem,6,str);
		}
		else//原始
		{
			str.Format("raw");
			m_List.SetItemText(nItem,5,str);
		}	
		nItem++;
	}
}

void CPropertyDlgStatAvg::OnSelchangeComboDbsel() 
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

void CPropertyDlgStatAvg::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	CDlgVarDataWithReq dlg;
	if(IDOK==dlg.DoModal())
	{
		tagNameArr.Add(dlg.m_strVarName);
		unitArr.Add(dlg.m_strUnit);
		dispArr.Add(dlg.m_strDesp);
		reqTypeArr.Add(dlg.m_iSnapType);
		snapshotTArr.Add(dlg.m_iSnapshotT);
		strOutPutNameArr.Add(dlg.m_strOutName);
		initListBoxHead();
		initListBoxBody();
	}	
}

void CPropertyDlgStatAvg::OnButtonEdit() 
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
	CDlgVarDataWithReq dlg;
	dlg.m_strVarName=tagNameArr[nSelectedItem];
	dlg.m_strUnit=unitArr[nSelectedItem];
	dlg.m_strDesp=dispArr[nSelectedItem];
	dlg.m_iSnapType=reqTypeArr[nSelectedItem];
	dlg.m_strOutName=strOutPutNameArr[nSelectedItem];
	if(reqTypeArr[nSelectedItem]==0)
	{
		dlg.m_iSnapshotT=snapshotTArr[nSelectedItem];
	}
	if(IDOK==dlg.DoModal())
	{
		tagNameArr[nSelectedItem]=dlg.m_strVarName;
		unitArr[nSelectedItem]=dlg.m_strUnit;
		dispArr[nSelectedItem]=dlg.m_strDesp;
		reqTypeArr[nSelectedItem]=dlg.m_iSnapType;
		snapshotTArr[nSelectedItem]=dlg.m_iSnapshotT;
		strOutPutNameArr[nSelectedItem]=dlg.m_strOutName;
		initListBoxHead();
		initListBoxBody();
	}
}

void CPropertyDlgStatAvg::OnButtonDel() 
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
		tagNameArr.RemoveAt(indexSelArr[i]);
		unitArr.RemoveAt(indexSelArr[i]);
		dispArr.RemoveAt(indexSelArr[i]);
		reqTypeArr.RemoveAt(indexSelArr[i]);
		snapshotTArr.RemoveAt(indexSelArr[i]);
		strOutPutNameArr.RemoveAt(indexSelArr[i]);
	}
	initListBoxHead();
	initListBoxBody();
}

void CPropertyDlgStatAvg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonEdit();
	*pResult = 0;
}

void CPropertyDlgStatAvg::OnButtonOutput() 
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
			sSql.Format("CREATE TABLE %s (ID NUMBER,varName TEXT,varOut TEXT,unit TEXT,desp TEXT,reqType TEXT,snapeshotPeriod TEXT)",
					strTable);
				database.ExecuteSQL(sSql);
			//导入数据
			for(int i=0;i<tagNameArr.GetSize();i++)
			{
				if(ProgDlg.m_bCancel) 
				{
					ProgDlg.PostMessage(WM_CLOSE, 0, 0);
					break;
				}
				sSql.Format("INSERT INTO %s (ID,varName,varOut,unit,desp,reqType,snapeshotPeriod) VALUES (%d,'%s','%s','%s','%s','%d','%d')",
					strTable,
					i+1,
					tagNameArr[i],
					strOutPutNameArr[i],
					unitArr[i],
					dispArr[i],
					reqTypeArr[i],
					snapshotTArr[i]);
				database.ExecuteSQL(sSql);//
				
				float persent =(float)((float)i/(float)tagNameArr.GetSize())*100;
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

void CPropertyDlgStatAvg::OnButtonInput() 
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
			if((iColCount>5)&&(iRowCount>1))
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

				tagNameArr.RemoveAll();//点名
				unitArr.RemoveAll();//单位
				dispArr.RemoveAll();//描述
				reqTypeArr.RemoveAll();//查询方式
				snapshotTArr.RemoveAll();//快照周期
				strOutPutNameArr.RemoveAll();
				bInput=true;
				for(int j=2;j<=iRowCount;j++)
				{
					if(ProgDlg.m_bCancel) 
					{
						ProgDlg.PostMessage(WM_CLOSE, 0, 0);
						break;
					}
					SS.ReadRow(strValueArr,j);
					if(strValueArr.GetSize()<5)//数据不全
					{
						continue;
					}
					if(strValueArr[1]=="")//点名为空
					{
						continue;
					}
					tagNameArr.Add(strValueArr[1]);
					strOutPutNameArr.Add(strValueArr[2]);
					unitArr.Add(strValueArr[3]);
					dispArr.Add(strValueArr[4]);
					CString strReqType=strValueArr[5];
					CString strT=strValueArr[6];

					if((strReqType=="")||(strReqType=="0"))//默认取快照
					{
						reqTypeArr.Add(0);
						if(strT=="")
						{
							snapshotTArr.Add(300);
						}
						else
						{
							snapshotTArr.Add(atoi(strT));
						}
					}
					else
					{
						reqTypeArr.Add(atoi(strReqType));
						snapshotTArr.Add(atoi(strT));
					}
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

void CPropertyDlgStatAvg::OnButtonAdv() 
{
	// TODO: Add your control notification handler code here
	CDlgStatAdv dlg;
	if(this->m_iTagTimeIntM==1)
	{
		dlg.m_BCheckTimeMInt=TRUE;
	}
	else
	{
		dlg.m_BCheckTimeMInt=FALSE;
	}
	if(IDOK==dlg.DoModal())
	{
		if(dlg.m_BCheckTimeMInt)
		{
			this->m_iTagTimeIntM=1;
		}
		else
		{
			this->m_iTagTimeIntM=0;
		}
	}
}

void CPropertyDlgStatAvg::OnSelchangeComboConReqType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	int selIndex=m_ComBoConReqType.GetCurSel ();
	if(selIndex==0)
	{
		GetDlgItem(IDC_EDIT_SNAP_CON_T)->EnableWindow(true);
	}
	else if(selIndex==1)
	{
		GetDlgItem(IDC_EDIT_SNAP_CON_T)->EnableWindow(false);
	}
}

void CPropertyDlgStatAvg::OnSelchangeComboConFun2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	int selIndex=m_ComBoConFun2.GetCurSel ();
	if(selIndex==0)
	{
		GetDlgItem(IDC_EDIT_CON_VALUE2)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_CON_VALUE2)->EnableWindow(true);
	}
}

void CPropertyDlgStatAvg::OnCheckCon() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	int selState=this->m_BtnCheckCon.GetCheck();
	if(selState==0)
	{
		GetDlgItem(IDC_EDIT_CON_VARNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CON_UNIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CON_DESP)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CON_REQ_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SNAP_CON_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CON_FUN1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CON_VALUE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CON_FUN2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CON_VALUE2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_CON_VARNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CON_UNIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CON_DESP)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_CON_REQ_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SNAP_CON_T)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_CON_FUN1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CON_VALUE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_CON_FUN2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CON_VALUE2)->EnableWindow(TRUE);

		if(this->m_iConReqType==1)
			GetDlgItem(IDC_EDIT_SNAP_CON_T)->EnableWindow(FALSE);
		if(this->m_iCon2Fun==0)
			GetDlgItem(IDC_EDIT_CON_VALUE2)->EnableWindow(FALSE);
	}
}
