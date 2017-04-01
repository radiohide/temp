// PropertyDlgStatProAvg.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgStatProAvg.h"
#include "DlgInputOutPut.h"
#include "AfxDB.h"
#include "DlgProgress.h"
#include "CSpreadSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStatProAvg dialog


CPropertyDlgStatProAvg::CPropertyDlgStatProAvg(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgStatProAvg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgStatProAvg)
	m_iProOrStatOrSum=0;//0:��������	
	m_strBlockDesp = _T("");
	//}}AFX_DATA_INIT
}


void CPropertyDlgStatProAvg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgStatProAvg)
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_STATIC_DESP, m_strBlockDesp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgStatProAvg, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgStatProAvg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_INPUT, OnButtonInput)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, OnButtonOutput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgStatProAvg message handlers

BOOL CPropertyDlgStatProAvg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if(m_iProOrStatOrSum==0)
	{
		m_strBlockDesp="ͳ�Ƽ����ࡣ������Ĺ������ݽ��о�ֵ���㲢��ͳ���������͸�ʽ�����";
		((CWnd*)this)->SetWindowText("ͳ�ƹ������ݾ�ֵ��");
	}
	else if(m_iProOrStatOrSum==1)
	{
		m_strBlockDesp="ͳ�Ƽ����ࡣ�������ͳ�����ݽ��о�ֵ���㲢��ͳ���������͸�ʽ�����";
		((CWnd*)this)->SetWindowText("ͳ��ͳ�����ݾ�ֵ��");
	}
	else
	{
		m_strBlockDesp="ͳ�Ƽ����ࡣ�������ͳ�����ݽ�����ͼ��㲢��ͳ���������͸�ʽ�����";
		((CWnd*)this)->SetWindowText("ͳ��ͳ��������Ϳ�");
	}
	UpdateData(FALSE);
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
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgStatProAvg::initListBoxHead()
{
	UpdateData();
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//��ȡ�б��ؼ�����չ���
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//�����б��ؼ�������չ���
	
	CString Field[5]={"���","�������","�������","��λ","����"};
	//�б��ӵı�ͷ
	m_List.InsertColumn (0, Field[0], LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, Field[1], LVCFMT_LEFT, 120);
	m_List.InsertColumn (2, Field[2], LVCFMT_LEFT, 120);
	m_List.InsertColumn (3, Field[3], LVCFMT_LEFT, 40);
	m_List.InsertColumn (4, Field[4], LVCFMT_LEFT, 80);
}
void CPropertyDlgStatProAvg::initListBoxBody()
{
	int nItem=0;
	CString str;
	for(int i=0;i<this->strInputArr.GetSize();i++)
	{
		str.Format("%d",nItem+1);
		m_List.InsertItem(nItem,str);
		m_List.SetItemText(nItem,1,strInputArr[i]);
		m_List.SetItemText(nItem,2,strOutputArr[i]);
		m_List.SetItemText(nItem,3,this->strUnitArr[i]);
		m_List.SetItemText(nItem,4,this->strTagDespArr[i]);	
		nItem++;
	}
}

void CPropertyDlgStatProAvg::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	CDlgInputOutPut dlg;
	if(IDOK==dlg.DoModal())
	{
		strInputArr.Add(dlg.m_strInput);
		strOutputArr.Add(dlg.m_strOutput);
		strUnitArr.Add(dlg.m_strUnit);
		strTagDespArr.Add(dlg.m_strDesp);
		initListBoxHead();
		initListBoxBody();
	}
}

void CPropertyDlgStatProAvg::OnButtonEdit() 
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
	CDlgInputOutPut dlg;
	dlg.m_strInput=strInputArr[nSelectedItem];
	dlg.m_strOutput=strOutputArr[nSelectedItem];
	dlg.m_strUnit=strUnitArr[nSelectedItem];
	dlg.m_strDesp=strTagDespArr[nSelectedItem];
	if(IDOK==dlg.DoModal())
	{
		strInputArr[nSelectedItem]=dlg.m_strInput;
		strOutputArr[nSelectedItem]=dlg.m_strOutput;
		strUnitArr[nSelectedItem]=dlg.m_strUnit;
		strTagDespArr[nSelectedItem]=dlg.m_strDesp;
		initListBoxHead();
		initListBoxBody();
	}
}

void CPropertyDlgStatProAvg::OnButtonDel() 
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
		strOutputArr.RemoveAt(indexSelArr[i]);
		strUnitArr.RemoveAt(indexSelArr[i]);
		strTagDespArr.RemoveAt(indexSelArr[i]);
	}
	initListBoxHead();
	initListBoxBody();
}

void CPropertyDlgStatProAvg::OnButtonInput() 
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

				strInputArr.RemoveAll();//
				strOutputArr.RemoveAll();//
				strUnitArr.RemoveAll();//��λ
				strTagDespArr.RemoveAll();//����

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
					strOutputArr.Add(strValueArr[2]);
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

void CPropertyDlgStatProAvg::OnButtonOutput() 
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
		// �������ݿ� (��Excel�����ļ�)
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
			sSql.Format("CREATE TABLE %s (ID NUMBER,varName TEXT,portName TEXT,unit TEXT,desp TEXT)",
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
				sSql.Format("INSERT INTO %s (ID,varName,portName,unit,desp) VALUES (%d,'%s','%s','%s','%s')",
					strTable,
					i+1,
					strInputArr[i],
					strOutputArr[i],
					strUnitArr[i],
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

void CPropertyDlgStatProAvg::OnOK() 
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