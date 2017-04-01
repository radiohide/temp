// PropertyDlgFormula.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "PropertyDlgFormula.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgFormula dialog

CPropertyDlgFormula::CPropertyDlgFormula(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgFormula::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgFormula)
	currentSelFormulaIndex=-1;
	m_StrSelFormula = _T("");
	m_StrCelVarName = _T("");
	//}}AFX_DATA_INIT
}


void CPropertyDlgFormula::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgFormula)
	DDX_Control(pDX, IDC_TREE_PORTPAR, m_ctlTree);
	DDX_Control(pDX, IDC_TABLIST, m_TabList);
	DDX_Control(pDX, IDC_LIST_FORMULA, m_List);
	DDX_Text(pDX, IDC_EDIT_FORMULA, m_StrSelFormula);
	DDX_Text(pDX, IDC_EDIT_VAR, m_StrCelVarName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgFormula, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgFormula)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABLIST, OnSelchangeTablist)
	ON_BN_CLICKED(IDC_BtnAddPort, OnBtnAddPort)
	ON_BN_CLICKED(IDC_BtnEditPort, OnBtnEditPort)
	ON_BN_CLICKED(IDC_BtnDelPort, OnBtnDelPort)
	ON_BN_CLICKED(IDC_BtnAddFormula, OnBtnAddFormula)
	ON_BN_CLICKED(IDC_BtnEditFormula, OnBtnEditFormula)
	ON_BN_CLICKED(IDC_BtnDelFormula, OnBtnDelFormula)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FORMULA, OnClickListFormula)
	ON_BN_CLICKED(IDC_BtnUpFormula, OnBtnUpFormula)
	ON_BN_CLICKED(IDC_BtnDownFormula, OnBtnDownFormula)
	ON_BN_CLICKED(IDC_BtnUpPort, OnBtnUpPort)
	ON_BN_CLICKED(IDC_BtnDownPort, OnBtnDownPort)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FORMULA, OnDblclkListFormula)
	ON_BN_CLICKED(IDC_BUTTON_TESTFORMULA, OnButtonTestformula)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgFormula message handlers
void CPropertyDlgFormula::SetPropData(
		const CStringArray &inPutArr,
		const CArray<int,int> &inPutTypeArr,
		const CStringArray &outPutArr,
		const CArray<int,int> &outPutTypeArr,
		const CStringArray &varArr,
		const CArray<int,int> &varTypeArr,
		const CStringArray &formulaArr)
{
	this->inPutArr.Copy(inPutArr);
	this->inPutTypeArr.Copy(inPutTypeArr);
	this->outPutArr.Copy(outPutArr);
	this->outPutTypeArr.Copy(outPutTypeArr);
	this->varArr.Copy(varArr);
	this->varTypeArr.Copy(varTypeArr);
	this->formulaArr.Copy(formulaArr);

	for(int i=0;i<inPutArr.GetSize();i++)//公式块中，默认各个端口均是可以编辑的
	{
		this->inPortCanBeEdit.Add(1);
	}
	for(i=0;i<outPutArr.GetSize();i++)//公式块中，默认各个端口均是可以编辑的
	{
		this->outPortCanBeEdit.Add(1);
	}
}

BOOL CPropertyDlgFormula::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_imageList.Create(16 , 16, ILC_COLOR, 2, 2);						//创建图像列表
	CBitmap bm;
	bm.LoadBitmap(IDB_INOUT);									//载入Bitmap资源
	m_imageList.Add(&bm, RGB(0, 0, 0));							//添加到图像列表
	bm.DeleteObject();
	bm.LoadBitmap(IDB_INPUT);
	m_imageList.Add(&bm, RGB(0, 0, 0));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_OUTPUT);
	m_imageList.Add(&bm, RGB(0, 0, 0));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_PORT);
	m_imageList.Add(&bm, RGB(0, 0, 0));

	InitFormulaListBoxHead();
	InitFormulaListBoxBody();
	initTabCtrl();
	initTreeCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgFormula::InitFormulaListBoxHead()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));

	DWORD liststyle=m_List.GetExtendedStyle();//获取列表控件的扩展风格
	m_List.SetExtendedStyle(liststyle|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置列表控件的新扩展风格
	
	CString Field[6]={"序号","变量","类型","公式"};
					//列表视的表头
	m_List.InsertColumn(0,Field[0],LVCFMT_LEFT,40);	
	m_List.InsertColumn(1,Field[1],LVCFMT_LEFT,80);	
	m_List.InsertColumn(2,Field[2],LVCFMT_LEFT,80);	
	m_List.InsertColumn(3,Field[3],LVCFMT_LEFT,140);
}
void CPropertyDlgFormula::InitFormulaListBoxBody()
{
	int nItem=0;
	for(int i=0;i<varArr.GetSize();i++)
	{
		if(varArr[i]=="")
			break;
		CString strIndex,str;
		strIndex.Format("%d",nItem+1);
		m_List.InsertItem(nItem,strIndex);
		
		this->m_List.SetItemText(nItem,1,varArr[i]);
		if(varTypeArr[i]==0)
			str="数值";
		else
			str="序列";
		this->m_List.SetItemText(nItem,2,str);

		this->m_List.SetItemText(nItem,3,formulaArr[i]);
		nItem++;
	}
	for(i=0;i<this->m_List.GetItemCount();i++)
	{
		m_List.SetFocus(); 
		m_List.SetItemState(i, 0, LVIS_FOCUSED|LVIS_SELECTED);
	}
	if(currentSelFormulaIndex!=-1)
	{
		m_List.SetFocus(); 
		m_List.SetItemState(currentSelFormulaIndex, LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED); 
	}
}
void CPropertyDlgFormula::initTabCtrl()//初始化Tab控件的时候，创建每页所需要的数据
{
	CPropertyDlgFormulaPort *pDlgPort;
	m_TabList.DeleteAllItems();
	for(int i=0;i<DlgTabArr.GetSize();i++)//清空Tab
	{
		pDlgPort=DlgTabArr[i];
		if(pDlgPort!=NULL)
		{
			delete pDlgPort;
			pDlgPort=NULL;
		}
	}
	DlgTabArr.RemoveAll();
	
	for(i=0;i<2;i++)
	{
		CString str;
		if(i==0)
		{
			str.Format("输入");
		}
		else if(i==1)
			str.Format("输出");
		this->m_TabList.InsertItem(i,_T(str));
		pDlgPort=new CPropertyDlgFormulaPort;
		if(i==0)
		{
			pDlgPort->SetDlgIsInOrOut(1);
			pDlgPort->SetPortPropData(&inPutArr,&inPutTypeArr);
			pDlgPort->SetCanBeEdit(&inPortCanBeEdit);
		}
		else if(i==1)
		{
			pDlgPort->SetDlgIsInOrOut(2);
			pDlgPort->SetPortPropData(&outPutArr,&outPutTypeArr);
			pDlgPort->SetCanBeEdit(&outPortCanBeEdit);
		}
		pDlgPort->Create(IDD_DIALOG_FORMULA_PORT,&m_TabList);//必须执行create
		
		CRect rc;
		m_TabList.GetClientRect(rc);
		rc.top += 21;
		rc.bottom -= 1;
		rc.left += 1;
		rc.right -= 1;
		pDlgPort->MoveWindow(rc);
		pDlgPort->m_List.MoveWindow(rc.left,rc.top-15,rc.Width(),rc.Height()-10);
		if(i==0)
			pDlgPort->ShowWindow(SW_SHOW);
		else
			pDlgPort->ShowWindow(SW_HIDE);
		this->DlgTabArr.Add(pDlgPort);
	}
}

void CPropertyDlgFormula::OnSelchangeTablist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgFormulaPort *pDlgPort;
	for(int i=0;i<2;i++)
	{
		pDlgPort=DlgTabArr[i];
		if(i==m_TabList.GetCurSel())
		{
			pDlgPort->ShowWindow(SW_SHOW);
		}
		else
		{
			pDlgPort->ShowWindow(SW_HIDE);
		}
	}
	*pResult = 0;
}
void CPropertyDlgFormula::initTreeCtrl()
{
	m_ctlTree.DeleteAllItems();
	

	m_ctlTree.SetImageList(&m_imageList,TVSIL_NORMAL); //为树形控件添加图像列表

	HTREEITEM hroot=m_ctlTree.InsertItem("所有变量",0,0,TVI_ROOT);//添加一级结点
	HTREEITEM h1=m_ctlTree.InsertItem("输入变量",1,1,hroot);//添加二级结点
	HTREEITEM h2=m_ctlTree.InsertItem("输出变量",2,2,hroot);//添加二级结点

	for(int i=0;i<inPutArr.GetSize();i++)
	{
		if(inPutArr[i]=="")
			break;
		m_ctlTree.InsertItem(inPutArr[i],3,3,h1);//添加三级结点

	}
	for(i=0;i<outPutArr.GetSize();i++)
	{
		if(outPutArr[i]=="")
			break;
		m_ctlTree.InsertItem(outPutArr[i],3,3,h2);//添加三级结点
	}
	m_ctlTree.SetBkColor(RGB(200,200,255));//设置树形控件的背景色
	m_ctlTree.SetTextColor(RGB(127,0,0));//设置文本颜色
	m_ctlTree.Select(hroot,TVGN_CARET);
}

void CPropertyDlgFormula::OnBtnAddPort() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgFormulaPort *pDlgPort=DlgTabArr[m_TabList.GetCurSel()];
	if(pDlgPort->AddPort())//加入端口成功
		initTreeCtrl();
}

void CPropertyDlgFormula::OnBtnEditPort() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgFormulaPort *pDlgPort=DlgTabArr[m_TabList.GetCurSel()];
	if(pDlgPort->EditPort())//编辑端口成功
		initTreeCtrl();
}

void CPropertyDlgFormula::OnBtnDelPort() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgFormulaPort *pDlgPort=DlgTabArr[m_TabList.GetCurSel()];
	if(pDlgPort->DelPort())//删除端口成功
		initTreeCtrl();
}
void CPropertyDlgFormula::OnBtnUpPort() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgFormulaPort *pDlgPort=DlgTabArr[m_TabList.GetCurSel()];
	if(pDlgPort->MoveUpPort())//移动端口成功
		initTreeCtrl();
}

void CPropertyDlgFormula::OnBtnDownPort() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgFormulaPort *pDlgPort=DlgTabArr[m_TabList.GetCurSel()];
	if(pDlgPort->MoveDownPort())//移动端口成功
		initTreeCtrl();
}
void CPropertyDlgFormula::OnBtnAddFormula() 
{
	// TODO: Add your control notification handler code here
	int currentCount=getAvailFormulaCount();
	if(currentCount==varArr.GetSize())
	{
		AfxMessageBox("已经达到最多公式个数!");
		return;
	}
	CDlgFormulaData dlgFormulaData;
	dlgFormulaData.SetType(0);//默认增加数值类型
	if(dlgFormulaData.DoModal()==IDOK)
	{
		varArr[currentCount]=dlgFormulaData.m_EditVarName;
		formulaArr[currentCount]=dlgFormulaData.m_EditFormula;
		varTypeArr[currentCount]=dlgFormulaData.GetType();
		InitFormulaListBoxHead();
		InitFormulaListBoxBody();
	}
}
int CPropertyDlgFormula::getAvailFormulaCount()
{
	int result=varArr.GetSize();
	for(int i=0;i<varArr.GetSize();i++)
	{
		if(varArr[i]=="")
		{
			result=i;
			break;
		}
	}
	return result;
}
void CPropertyDlgFormula::OnBtnEditFormula() 
{
	// TODO: Add your control notification handler code here
	int index;
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{	
		index =this->m_List.GetNextSelectedItem(pos);
		CDlgFormulaData dlgFormulaData;
		dlgFormulaData.m_EditVarName=varArr[index];
		dlgFormulaData.m_EditFormula=formulaArr[index];
		dlgFormulaData.SetType(varTypeArr[index]);

		if(dlgFormulaData.DoModal()==IDOK)
		{
			varArr[index]=dlgFormulaData.m_EditVarName;
			formulaArr[index]=dlgFormulaData.m_EditFormula;
			varTypeArr[index]=dlgFormulaData.GetType();
			InitFormulaListBoxHead();
			InitFormulaListBoxBody();
		}
	}
	else
	{
		AfxMessageBox("请选择要编辑的公式记录!");
	}
}

void CPropertyDlgFormula::OnBtnDelFormula() 
{
	// TODO: Add your control notification handler code here
	int index;
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{	
		CString strMsg;
		strMsg.Format("确定要删除选中的公式?");
		int int_selOK=AfxMessageBox(strMsg,MB_OKCANCEL);
		if(int_selOK==1)
		{
			index =this->m_List.GetNextSelectedItem(pos);
			varArr.RemoveAt(index);
			formulaArr.RemoveAt(index);
			varTypeArr.RemoveAt(index);
			varArr.Add("");
			formulaArr.Add(0);
			varTypeArr.Add(0);
			InitFormulaListBoxHead();
			InitFormulaListBoxBody();
		}
	}
	else
	{
		AfxMessageBox("请选择要删除的端口!");
	}
}

void CPropertyDlgFormula::OnClickListFormula(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int index;
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		index =this->m_List.GetNextSelectedItem(pos);
		currentSelFormulaIndex=index;
		this->m_StrCelVarName=varArr[index];
		this->m_StrSelFormula=formulaArr[index];
		this->UpdateData(false);
	}
	else
		currentSelFormulaIndex=-1;
	*pResult = 0;
}

void CPropertyDlgFormula::OnBtnUpFormula() 
{
	// TODO: Add your control notification handler code here
	if(currentSelFormulaIndex!=-1)
	{
		if(currentSelFormulaIndex!=0)
		{
			CString strTemp=varArr[currentSelFormulaIndex];
			varArr[currentSelFormulaIndex]=varArr[currentSelFormulaIndex-1];
			varArr[currentSelFormulaIndex-1]=strTemp;
			
			strTemp=formulaArr[currentSelFormulaIndex];
			formulaArr[currentSelFormulaIndex]=formulaArr[currentSelFormulaIndex-1];
			formulaArr[currentSelFormulaIndex-1]=strTemp;

			int iTemp=varTypeArr[currentSelFormulaIndex];
			varTypeArr[currentSelFormulaIndex]=varTypeArr[currentSelFormulaIndex-1];
			varTypeArr[currentSelFormulaIndex-1]=iTemp;
			
			currentSelFormulaIndex--;
			InitFormulaListBoxHead();
			InitFormulaListBoxBody();
		}
		else
			AfxMessageBox("该公式已处于最表顶!");
	}
	else
	{
		AfxMessageBox("请选择要移动的公式!");
	}
}

void CPropertyDlgFormula::OnBtnDownFormula() 
{
	// TODO: Add your control notification handler code here
	if(currentSelFormulaIndex!=-1)
	{
		if(currentSelFormulaIndex!=m_List.GetItemCount()-1)
		{
			CString strTemp=varArr[currentSelFormulaIndex];
			varArr[currentSelFormulaIndex]=varArr[currentSelFormulaIndex+1];
			varArr[currentSelFormulaIndex+1]=strTemp;
			
			strTemp=formulaArr[currentSelFormulaIndex];
			formulaArr[currentSelFormulaIndex]=formulaArr[currentSelFormulaIndex+1];
			formulaArr[currentSelFormulaIndex+1]=strTemp;

			int iTemp=varTypeArr[currentSelFormulaIndex];
			varTypeArr[currentSelFormulaIndex]=varTypeArr[currentSelFormulaIndex+1];
			varTypeArr[currentSelFormulaIndex+1]=iTemp;
			
			currentSelFormulaIndex++;
			InitFormulaListBoxHead();
			InitFormulaListBoxBody();
		}
		else
			AfxMessageBox("该公式已处于最表低!");
	}
	else
	{
		AfxMessageBox("请选择要移动的公式!");
	}
}

void CPropertyDlgFormula::OnDblclkListFormula(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnBtnEditFormula();
	*pResult = 0;
}

void CPropertyDlgFormula::OnButtonTestformula() 
{
	// TODO: Add your control notification handler code here
	
}
