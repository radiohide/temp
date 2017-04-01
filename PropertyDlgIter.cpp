// PropertyDlgIter.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "PropertyDlgIter.h"
#include "PropertyDlgFormulaPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgIter dialog


CPropertyDlgIter::CPropertyDlgIter(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgIter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgIter)
	m_StrSelFormula = _T("");
	m_StrCelVarName = _T("");

	m_pCurrVarArr=NULL;//迭代前变量
	m_pCurrFormulaArr=NULL;
	m_pCurrVarTypeArr=NULL;//0,数值；1,序列
	m_pCurrCanBeEditArr=NULL;
	m_pCurrCanBeShowArr=NULL;
	m_currSelFormulaIndex=-1;
	//}}AFX_DATA_INIT
}


void CPropertyDlgIter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgIter)
	DDX_Control(pDX, IDC_TABLIST, m_TabList);
	DDX_Control(pDX, IDC_TABLIST2, m_TabList2);
	DDX_Control(pDX, IDC_TREE_PORTPAR, m_ctlTree);
	DDX_Text(pDX, IDC_EDIT_FORMULA, m_StrSelFormula);
	DDX_Text(pDX, IDC_EDIT_VAR, m_StrCelVarName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgIter, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgIter)
	ON_BN_CLICKED(IDC_BtnAddPort, OnBtnAddPort)
	ON_BN_CLICKED(IDC_BtnEditPort, OnBtnEditPort)
	ON_BN_CLICKED(IDC_BtnDelPort, OnBtnDelPort)
	ON_BN_CLICKED(IDC_BtnUpPort, OnBtnUpPort)
	ON_BN_CLICKED(IDC_BtnDownPort, OnBtnDownPort)
	ON_BN_CLICKED(IDC_BtnAddFormula, OnBtnAddFormula)
	ON_BN_CLICKED(IDC_BtnEditFormula, OnBtnEditFormula)
	ON_BN_CLICKED(IDC_BtnDelFormula, OnBtnDelFormula)
	ON_BN_CLICKED(IDC_BtnUpFormula, OnBtnUpFormula)
	ON_BN_CLICKED(IDC_BtnDownFormula, OnBtnDownFormula)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABLIST, OnSelchangeTablist)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABLIST2, OnSelchangeTablist2)
	ON_MESSAGE(WM_CLICK_ITER_FORMULALIST,OnClickFormulaList)
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgIter message handlers
void CPropertyDlgIter::SetPropData(
		const CStringArray &inPutArr,
		const CArray<int,int> &inPutTypeArr,
		const CStringArray &outPutArr,
		const CArray<int,int> &outPutTypeArr,
		const CStringArray &IterFVarArr,
		const CArray<int,int> &IterFVarTypeArr,
		const CStringArray &IterFFormulaArr,
		const CStringArray &conVarArr,
		const CArray<int,int> &conVarTypeArr,
		const CStringArray &conFormulaArr,
		const CStringArray &IterVarArr,
		const CArray<int,int> &IterVarTypeArr,
		const CStringArray &IterFormulaArr,
		const CStringArray &IterBVarArr,
		const CArray<int,int> &IterBVarTypeArr,
		const CStringArray &IterBFormulaArr
		)
{
	this->inPutArr.Copy(inPutArr);
	this->inPutTypeArr.Copy(inPutTypeArr);
	this->outPutArr.Copy(outPutArr);
	this->outPutTypeArr.Copy(outPutTypeArr);

	this->IterFVarArr.Copy(IterFVarArr);
	this->IterFVarTypeArr.Copy(IterFVarTypeArr);
	this->IterFFormulaArr.Copy(IterFFormulaArr);

	this->conVarArr.Copy(conVarArr);
	this->conVarTypeArr.Copy(conVarTypeArr);
	this->conFormulaArr.Copy(conFormulaArr);

	this->IterVarArr.Copy(IterVarArr);
	this->IterVarTypeArr.Copy(IterVarTypeArr);
	this->IterFormulaArr.Copy(IterFormulaArr);

	this->IterBVarArr.Copy(IterBVarArr);
	this->IterBVarTypeArr.Copy(IterBVarTypeArr);
	this->IterBFormulaArr.Copy(IterBFormulaArr);
}
void CPropertyDlgIter::SetCanBeEditCanBeShow(
		const CArray<int,int> &inPutCanBeEdit,
		const CArray<int,int> &outPutCanBeEdit,
		const CArray<int,int> &IterFCanBeEdit,
		const CArray<int,int> &IterFCanBeShow,
		const CArray<int,int> &conVarCanBeEdit,
		const CArray<int,int> &conVarCanBeShow,
		const CArray<int,int> &IterCanBeEdit,
		const CArray<int,int> &IterCanBeShow,
		const CArray<int,int> &IterBCanBeEdit,
		const CArray<int,int> &IterBCanBeShow)
{
	this->inPutCanBeEdit.Copy(inPutCanBeEdit);
	this->outPutCanBeEdit.Copy(outPutCanBeEdit);
	this->IterFCanBeEdit.Copy(IterFCanBeEdit);
	this->IterFCanBeShow.Copy(IterFCanBeShow);
	this->conVarCanBeEdit.Copy(conVarCanBeEdit);
	this->conVarCanBeShow.Copy(conVarCanBeShow);
	this->IterCanBeEdit.Copy(IterCanBeEdit);
	this->IterCanBeShow.Copy(IterCanBeShow);
	this->IterBCanBeEdit.Copy(IterBCanBeEdit);
	this->IterBCanBeShow.Copy(IterBCanBeShow);
}
void CPropertyDlgIter::SetTagConFOrB(int tagConFOrB)
{
	this->tagConFOrB=tagConFOrB;
}
BOOL CPropertyDlgIter::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
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

	initTabCtrl();
	initTabCtrl2();
	initTreeCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPropertyDlgIter::initTabCtrl()//初始化Tab控件的时候，创建每页所需要的数据
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
			pDlgPort->SetCanBeEdit(&inPutCanBeEdit);
		}
		else if(i==1)
		{
			pDlgPort->SetDlgIsInOrOut(2);
			pDlgPort->SetPortPropData(&outPutArr,&outPutTypeArr);
			pDlgPort->SetCanBeEdit(&outPutCanBeEdit);
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
void CPropertyDlgIter::initTabCtrl2()//初始化Tab控件的时候，创建每页所需要的数据
{
	CPropertyDlgIterFormulaList *pDlgFoumula;
	m_TabList2.DeleteAllItems();
	for(int i=0;i<DlgTab2Arr.GetSize();i++)//清空Tab
	{
		pDlgFoumula=DlgTab2Arr[i];
		if(pDlgFoumula!=NULL)
		{
			delete pDlgFoumula;
			pDlgFoumula=NULL;
		}
	}
	DlgTab2Arr.RemoveAll();
	
	for(i=0;i<4;i++)
	{
		pDlgFoumula=new CPropertyDlgIterFormulaList;
		CString str;
		if(i==0)
		{
			str.Format("循环前");
			pDlgFoumula->SetFormulaData(&IterFVarArr,&IterFVarTypeArr,&IterFFormulaArr);
			pDlgFoumula->SetCurrentSelIndex(&m_currSelFormulaIndex);
			pDlgFoumula->SetCanBeEditCanBeShow(&IterFCanBeEdit,&IterFCanBeShow);
		}
		else if(i==1)
		{
			str.Format("循环条件");
			pDlgFoumula->SetFormulaData(&conVarArr,&conVarTypeArr,&conFormulaArr);
			pDlgFoumula->SetCurrentSelIndex(&m_currSelFormulaIndex);
			pDlgFoumula->SetPtrTagConFOrB(&tagConFOrB);
			pDlgFoumula->SetCanBeEditCanBeShow(&conVarCanBeEdit,&conVarCanBeShow);
		}
		else if(i==2)
		{
			str.Format("循环内容");
			pDlgFoumula->SetFormulaData(&IterVarArr,&IterVarTypeArr,&IterFormulaArr);
			pDlgFoumula->SetCurrentSelIndex(&m_currSelFormulaIndex);
			pDlgFoumula->SetCanBeEditCanBeShow(&IterCanBeEdit,&IterCanBeShow);
		}
		else if(i==3)
		{
			str.Format("循环后");
			pDlgFoumula->SetFormulaData(&IterBVarArr,&IterBVarTypeArr,&IterBFormulaArr);
			pDlgFoumula->SetCurrentSelIndex(&m_currSelFormulaIndex);
			pDlgFoumula->SetCanBeEditCanBeShow(&IterBCanBeEdit,&IterBCanBeShow);
		}
		this->m_TabList2.InsertItem(i,_T(str));
		pDlgFoumula->Create(IDD_DIALOG_ITER_FORMULALIST,&m_TabList2);//必须执行create
		
		CRect rc;
		m_TabList2.GetClientRect(rc);
		rc.top += 21;
		rc.bottom -= 1;
		rc.left += 1;
		rc.right -= 1;
		pDlgFoumula->MoveWindow(rc);
		pDlgFoumula->m_List.MoveWindow(rc.left,rc.top-15,rc.Width(),rc.Height()-10);
		if(i==1)
			pDlgFoumula->ShowConFOrBCtrl(rc);
		else
			pDlgFoumula->HideConFOrBCtrl();
		if(i==0)
		{
			pDlgFoumula->ShowWindow(SW_SHOW);
			m_pCurrVarArr=pDlgFoumula->pVNameArr;//迭代前变量
			m_pCurrFormulaArr=pDlgFoumula->pFormulaArr;
			m_pCurrVarTypeArr=pDlgFoumula->pTypeArr;//0,数值；1,序列
			m_pCurrCanBeEditArr=pDlgFoumula->pCanBeEditArr;
			m_pCurrCanBeShowArr=pDlgFoumula->pCanBeShowArr;
			
			m_pCurrSelFormulaDlg=pDlgFoumula;
		}
		else
			pDlgFoumula->ShowWindow(SW_HIDE);
		this->DlgTab2Arr.Add(pDlgFoumula);
	}
}
void CPropertyDlgIter::initTreeCtrl()
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
	m_ctlTree.SetBkColor(RGB(255,255,255));//设置树形控件的背景色
	m_ctlTree.SetTextColor(RGB(0,0,0));//设置文本颜色
	m_ctlTree.Select(hroot,TVGN_CARET);
}
LRESULT CPropertyDlgIter::OnClickFormulaList(WPARAM wParam,LPARAM lParam)
{
	this->m_StrCelVarName=(*m_pCurrVarArr)[m_currSelFormulaIndex];
	if((*m_pCurrCanBeShowArr)[m_currSelFormulaIndex]==1)
	{
		this->m_StrSelFormula=(*m_pCurrFormulaArr)[m_currSelFormulaIndex];
	}
	else
		this->m_StrSelFormula="HIDE";
	this->UpdateData(false);
	return 1;
}
void CPropertyDlgIter::OnBtnAddPort() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgFormulaPort *pDlgPort=DlgTabArr[m_TabList.GetCurSel()];
	if(pDlgPort->AddPort())//加入端口成功
		initTreeCtrl();
}

void CPropertyDlgIter::OnBtnEditPort() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgFormulaPort *pDlgPort=DlgTabArr[m_TabList.GetCurSel()];
	if(pDlgPort->EditPort())//编辑端口成功
		initTreeCtrl();
}

void CPropertyDlgIter::OnBtnDelPort() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgFormulaPort *pDlgPort=DlgTabArr[m_TabList.GetCurSel()];
	if(pDlgPort->DelPort())//删除端口成功
		initTreeCtrl();
}
void CPropertyDlgIter::OnBtnUpPort() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgFormulaPort *pDlgPort=DlgTabArr[m_TabList.GetCurSel()];
	if(pDlgPort->MoveUpPort())//移动端口成功
		initTreeCtrl();
}

void CPropertyDlgIter::OnBtnDownPort() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgFormulaPort *pDlgPort=DlgTabArr[m_TabList.GetCurSel()];
	if(pDlgPort->MoveDownPort())//移动端口成功
		initTreeCtrl();
}
void CPropertyDlgIter::OnBtnAddFormula() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgIterFormulaList *pDlgFormula=DlgTab2Arr[m_TabList2.GetCurSel()];
	if(pDlgFormula->AddVar())//加入变量成功
	{

	}
}
void CPropertyDlgIter::OnBtnEditFormula() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgIterFormulaList *pDlgFormula=DlgTab2Arr[m_TabList2.GetCurSel()];
	if(pDlgFormula->EditVar())//编辑变量成功
	{
		
	}
}

void CPropertyDlgIter::OnBtnDelFormula() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgIterFormulaList *pDlgFormula=DlgTab2Arr[m_TabList2.GetCurSel()];
	if(pDlgFormula->DelVar())//删除变量成功
	{
		
	}
}
void CPropertyDlgIter::OnBtnUpFormula() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgIterFormulaList *pDlgFormula=DlgTab2Arr[m_TabList2.GetCurSel()];
	if(pDlgFormula->MoveUpVar())//上移变量成功
	{
		OnClickFormulaList(0,0);
	}
}

void CPropertyDlgIter::OnBtnDownFormula() 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgIterFormulaList *pDlgFormula=DlgTab2Arr[m_TabList2.GetCurSel()];
	if(pDlgFormula->MoveDownVar())//下移变量成功
	{
		OnClickFormulaList(0,0);
	}
}
void CPropertyDlgIter::OnSelchangeTablist(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CPropertyDlgIter::OnSelchangeTablist2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPropertyDlgIterFormulaList *pDlgFormula;
	for(int i=0;i<4;i++)
	{
		pDlgFormula=DlgTab2Arr[i];
		if(i==m_TabList2.GetCurSel())
		{
			m_pCurrVarArr=pDlgFormula->pVNameArr;//迭代前变量
			m_pCurrFormulaArr=pDlgFormula->pFormulaArr;
			m_pCurrVarTypeArr=pDlgFormula->pTypeArr;//0,数值；1,序列
			m_pCurrCanBeEditArr=pDlgFormula->pCanBeEditArr;
			m_pCurrCanBeShowArr=pDlgFormula->pCanBeShowArr;
			m_pCurrSelFormulaDlg=pDlgFormula;
			pDlgFormula->ShowWindow(SW_SHOW);
		}
		else
		{
			pDlgFormula->ShowWindow(SW_HIDE);
		}
	}
	*pResult = 0;
}
