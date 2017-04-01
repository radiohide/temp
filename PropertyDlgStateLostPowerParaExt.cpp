// PropertyDlgStateLostPowerParaExt.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgStateLostPowerParaExt.h"
#include "AfxDB.h"
#include "DlgProgress.h"
#include "CSpreadSheet.h"
#include "DlgLostPowerVar.h"
#include "DlgWindPower.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStateLostPowerParaExt dialog

CPropertyDlgStateLostPowerParaExt::CPropertyDlgStateLostPowerParaExt(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgStateLostPowerParaExt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgStateLostPowerParaExt)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropertyDlgStateLostPowerParaExt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgStateLostPowerParaExt)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LIST4, m_List2);
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgStateLostPowerParaExt, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgStateLostPowerParaExt)
	// NOTE: the ClassWizard will add message map macros here
	ON_BN_CLICKED(IDC_BUTTON_INPUT, OnButtonInput)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, OnButtonOutput)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON_INPUT2, OnButtonInput2)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT2, OnButtonOutput2)
	ON_BN_CLICKED(IDC_BUTTON_ADD2, OnButtonAdd2)
	ON_BN_CLICKED(IDC_BUTTON_EDIT2, OnButtonEdit2)
	ON_BN_CLICKED(IDC_BUTTON_DEL2, OnButtonDel2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST4, OnDblclkList4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStateLostPowerParaExt message handlers

BOOL CPropertyDlgStateLostPowerParaExt::OnInitDialog() 
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
	// TODO: Add extra initialization here
	initListBoxHead();
	initListBoxBody();
	initListBoxHead2();
	initListBoxBody2();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgStateLostPowerParaExt::initListBoxHead()
{
	UpdateData();
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//��ȡ�б�ؼ�����չ���
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//�����б�ؼ�������չ���
	
	CString Field[5]={"���","�������","����ʱ����С��ֵ","����ʱ�������ֵ","����"};
	//�б��ӵı�ͷ
	m_List.InsertColumn (0, Field[0], LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, Field[1], LVCFMT_LEFT, 120);
	m_List.InsertColumn (2, Field[2], LVCFMT_LEFT, 150);
	m_List.InsertColumn (3, Field[3], LVCFMT_LEFT, 150);
	m_List.InsertColumn (4, Field[4], LVCFMT_LEFT, 120);
}
void CPropertyDlgStateLostPowerParaExt::initListBoxBody()
{
	int nItem=0;
	CString str;
	for(int i=0;i<this->strInputArr.GetSize();i++)
	{
		str.Format("%d",nItem+1);
		m_List.InsertItem(nItem,str);
		m_List.SetItemText(nItem,1,strInputArr[i]);
		str.Format("%d",iValidMin[i]);
		m_List.SetItemText(nItem,2,str);
		str.Format("%d",iValidMax[i]);
		m_List.SetItemText(nItem,3,str);
		m_List.SetItemText(nItem,4,this->strTagDespArr[i]);
		nItem++;
	}
}
void CPropertyDlgStateLostPowerParaExt::initListBoxHead2()
{
	UpdateData();
	m_List2.DeleteAllItems();
	while(m_List2.DeleteColumn(0));

	DWORD liststyle=m_List2.GetExtendedStyle();//��ȡ�б�ؼ�����չ���
	m_List2.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//�����б�ؼ�������չ���
	
	CString Field[3]={"���","����(m/s)","����(kw)"};
	//�б��ӵı�ͷ
	m_List2.InsertColumn (0, Field[0], LVCFMT_LEFT, 40);
	m_List2.InsertColumn (1, Field[1], LVCFMT_LEFT, 120);
	m_List2.InsertColumn (2, Field[2], LVCFMT_LEFT, 120);
}
void CPropertyDlgStateLostPowerParaExt::initListBoxBody2()
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
void CPropertyDlgStateLostPowerParaExt::OnButtonInput() 
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

				strInputArr.RemoveAll();//
				iValidMin.RemoveAll();//
				iValidMax.RemoveAll();//
				strTagDespArr.RemoveAll();//��λ

				bInput=true;
				for(int j=2;j<=iRowCount;j++)
				{
					if(ProgDlg.m_bCancel) 
					{
						ProgDlg.PostMessage(WM_CLOSE, 0, 0);
						break;
					}
					SS.ReadRow(strValueArr,j);
					if(strValueArr.GetSize()<3)//���ݲ�ȫ
					{
						continue;
					}
					if(strValueArr[1]=="")//����Ϊ��
					{
						continue;
					}
					strInputArr.Add(strValueArr[1]);
					iValidMin.Add(atoi(strValueArr[2]));
					iValidMax.Add(atoi(strValueArr[3]));
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
			AfxMessageBox("��������ɹ���");
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

void CPropertyDlgStateLostPowerParaExt::OnButtonOutput() 
{
	// TODO: Add your control notification handler code here
	CString filename;
    CFileDialog FileDlg (FALSE, _T("xls"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
						 _T("Excel������ (*.xls)|*.xls"),
						 this);
	
	if (FileDlg.DoModal() != IDOK ) return;
	else
	{
		filename = FileDlg.GetPathName();
	}
	CDatabase database;
	CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
	CString sSql;
	CDlgProgress ProgDlg;
	try
	{
		// �������д�ȡ���ַ���
		sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, filename, filename);
		// �������ݿ� (��Excel����ļ�)
		if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )
		{
			MSG msg;
			ProgDlg.Create(IDD_DLGPROGRESS, NULL);
			ProgDlg.ShowWindow(SW_SHOW);
			ProgDlg.CenterWindow();
			ProgDlg.UpdateData(FALSE);
			ProgDlg.m_bCancel = FALSE;
			//����
			CString strTable;
			strTable.Format("VarData");
			sSql.Format("CREATE TABLE %s (ID NUMBER,varName TEXT,iValidMin NUMBER,iValidMax NUMBER,desp TEXT)",
					strTable);
				database.ExecuteSQL(sSql);
			//��������
			for(int i=0;i<strInputArr.GetSize();i++)
			{
				if(ProgDlg.m_bCancel) 
				{
					ProgDlg.PostMessage(WM_CLOSE, 0, 0);
					break;
				}
				sSql.Format("INSERT INTO %s (ID,varName,iValidMin,iValidMax,desp) VALUES (%d,'%s',%d,%d,'%s')",
					strTable,
					i+1,
					strInputArr[i],
					iValidMin[i],
					iValidMax[i],
					strTagDespArr[i]
					);
				database.ExecuteSQL(sSql);//
				
				float persent =(float)((float)i/(float)strInputArr.GetSize())*100;
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
		// �ر����ݿ�
		database.Close();
		if(!ProgDlg.m_bCancel)
		{
			AfxMessageBox("Excel�ļ�д��ɹ���");
		}
	}
	catch(CException ex)
	{
		ex.ReportError();
	}
}

void CPropertyDlgStateLostPowerParaExt::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	CDlgLostPowerVar dlg;
	if(IDOK==dlg.DoModal())
	{
		strInputArr.Add(dlg.m_strInput);
		iValidMin.Add(dlg.m_iValidMin);
		iValidMax.Add(dlg.m_iValidMax);
		strTagDespArr.Add(dlg.m_strDesp);
		initListBoxHead();
		initListBoxBody();
	}
}

void CPropertyDlgStateLostPowerParaExt::OnButtonEdit() 
{
	// TODO: Add your control notification handler code here
		POSITION pos = m_List.GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL)
		nSelectedItem = m_List.GetNextSelectedItem(pos);
	if(nSelectedItem<0)
	{ 
		 AfxMessageBox("��ѡ�������");
		 return;
	}
	CDlgLostPowerVar dlg;
	dlg.m_strInput=strInputArr[nSelectedItem];
	dlg.m_iValidMin=iValidMin[nSelectedItem];
	dlg.m_iValidMax=iValidMax[nSelectedItem];
	dlg.m_strDesp=strTagDespArr[nSelectedItem];
	if(IDOK==dlg.DoModal())
	{
		strInputArr[nSelectedItem]=dlg.m_strInput;
		iValidMin[nSelectedItem]=dlg.m_iValidMin;
		iValidMax[nSelectedItem]=dlg.m_iValidMax;
		strTagDespArr[nSelectedItem]=dlg.m_strDesp;
		initListBoxHead();
		initListBoxBody();
	}
}

void CPropertyDlgStateLostPowerParaExt::OnButtonDel() 
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
		 AfxMessageBox("��ѡ�������");
		 return;
	}
	for(int i=0;i<indexSelArr.GetSize();i++)//�����Ӵ�С����
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
		strInputArr.RemoveAt(indexSelArr[i]);
		iValidMin.RemoveAt(indexSelArr[i]);
		iValidMax.RemoveAt(indexSelArr[i]);
		strTagDespArr.RemoveAt(indexSelArr[i]);
	}
	initListBoxHead();
	initListBoxBody();
}

void CPropertyDlgStateLostPowerParaExt::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonEdit();
	*pResult = 0;
}

void CPropertyDlgStateLostPowerParaExt::OnButtonInput2() 
{
	// TODO: Add your control notification handler code here
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
					if(strValueArr.GetSize()<3)//���ݲ�ȫ
					{
						continue;
					}
					if(strValueArr[1]=="")//����Ϊ��
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
			AfxMessageBox("��������ɹ���");
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

void CPropertyDlgStateLostPowerParaExt::OnButtonOutput2() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CString filename;
    CFileDialog FileDlg (FALSE, _T("xls"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
						 _T("Excel������ (*.xls)|*.xls"),
						 this);
	
	if (FileDlg.DoModal() != IDOK ) return;
	else
	{
		filename = FileDlg.GetPathName();
	}
	CDatabase database;
	CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
	CString sSql;
	CDlgProgress ProgDlg;
	try
	{
		// �������д�ȡ���ַ���
		sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, filename, filename);
		// �������ݿ� (��Excel����ļ�)
		if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )
		{
			MSG msg;
			ProgDlg.Create(IDD_DLGPROGRESS, NULL);
			ProgDlg.ShowWindow(SW_SHOW);
			ProgDlg.CenterWindow();
			ProgDlg.UpdateData(FALSE);
			ProgDlg.m_bCancel = FALSE;
			//����
			CString strTable;
			strTable.Format("VarData");
			sSql.Format("CREATE TABLE %s (ID NUMBER,WindSpeed TEXT,Power TEXT)",
					strTable);
				database.ExecuteSQL(sSql);
			//��������
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
		// �ر����ݿ�
		database.Close();
		if(!ProgDlg.m_bCancel)
		{
			AfxMessageBox("Excel�ļ�д��ɹ���");
		}
	}
	catch(CException ex)
	{
		ex.ReportError();
	}
}

void CPropertyDlgStateLostPowerParaExt::OnButtonAdd2() 
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

void CPropertyDlgStateLostPowerParaExt::OnButtonEdit2() 
{
	// TODO: Add your control notification handler code here
		POSITION pos = m_List2.GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL)
		nSelectedItem = m_List2.GetNextSelectedItem(pos);
	if(nSelectedItem<0)
	{ 
		 AfxMessageBox("��ѡ�������");
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

void CPropertyDlgStateLostPowerParaExt::OnButtonDel2() 
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
		 AfxMessageBox("��ѡ�������");
		 return;
	}
	for(int i=0;i<indexSelArr.GetSize();i++)//�����Ӵ�С����
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

void CPropertyDlgStateLostPowerParaExt::OnOK() 
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

void CPropertyDlgStateLostPowerParaExt::OnDblclkList4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonEdit2();
	*pResult = 0;
}