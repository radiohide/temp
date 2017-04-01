// ViewModuleFrm.cpp : implementation file
//

#include "stdafx.h"
#include "ePerfManage.h"
#include "ViewModuleFrm.h"

#include "FlowchartEditor/FlowchartEntityBox.h"
#include "ePerfManageDoc.h"
#include "DlgCalcOrder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewModuleFrm

IMPLEMENT_DYNCREATE(CViewModuleFrm, CViewUserFrm)

CViewModuleFrm::CViewModuleFrm()
	: CViewUserFrm(CViewModuleFrm::IDD)
{
	//{{AFX_DATA_INIT(CViewModuleFrm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strModuleName = _T("");
	m_screenResolutionX = 0;
}

CViewModuleFrm::~CViewModuleFrm()
{
}

void CViewModuleFrm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewModuleFrm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewModuleFrm, CFormView)
	//{{AFX_MSG_MAP(CViewModuleFrm)
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_SAVE_MODULE, OnFileSaveModule)
	//ON_COMMAND(ID_CFG_ORDERSHOW, OnCfgOrdershow)
	ON_COMMAND(ID_CFG_SET_BLOCK_CALID, OnCfgSetBlockCalID)
	//ON_UPDATE_COMMAND_UI(ID_CFG_ORDERSHOW, OnUpdateCfgOrdershow)
	ON_UPDATE_COMMAND_UI(ID_MAG_PORTVALUESHOW, OnUpdateMagPortvalueshow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewModuleFrm diagnostics

#ifdef _DEBUG
void CViewModuleFrm::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewModuleFrm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

void CViewModuleFrm::RefreshWindow()
{
	if( m_editor.m_hWnd )
	{
		m_editor.RedrawWindow();
	}
}
CFlowchartEntityContainer* CViewModuleFrm::GetData()
{
	    return &m_objs;
}
void CViewModuleFrm::SetUnselAllObj()
{
	m_editor.UnselectAll();
}
void CViewModuleFrm::SetSelBlockInVeiwByName(CString strBlockName)
{
	for(int i=0;i<m_objs.GetSize();i++)
	{
		CFlowchartEntity *pObj=(CFlowchartEntity *)m_objs.GetAt(i);
		if(pObj->GetBlockName()==strBlockName)
		{
			pObj->Select( TRUE );
			break;
		}
	}
	
}
/////////////////////////////////////////////////////////////////////////////
// CViewModuleFrm message handlers

void CViewModuleFrm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	if( !m_editor.m_hWnd )
	{
		// Creating the editor window
		CRect rect;
		GetClientRect( rect );
		GetData()->SetClipboardHandler( &theApp.m_clip );
		m_editor.Create( WS_CHILD | WS_VISIBLE, rect, this, GetData() );

		// We get the screen resolution, which we will use 
		// for scaling to printer. See also OnDraw.
		CClientDC dc( this );
		m_screenResolutionX = dc.GetDeviceCaps( LOGPIXELSX );
		int horzSize = round( ( double ) dc.GetDeviceCaps( HORZSIZE ) / ( double ) m_screenResolutionX );
		int vertSize = round( ( double ) dc.GetDeviceCaps( VERTSIZE ) / ( double ) m_screenResolutionX );

		m_editor.SetVirtualSize( CSize( 15 * m_screenResolutionX, 10 * m_screenResolutionX ) );

		//读取到对象中
		BOOL bRet = ReadModuleEntity(theApp.m_pConLocalModel); 
		
		CEPerfManageDoc* pDoc = (CEPerfManageDoc*)GetDocument();
		m_objs.SetShowValueMode(pDoc->m_BShowPortValue);
		m_editor.SetShowValueMode(pDoc->m_BShowPortValue);	
		m_editor.RedrawWindow();

		m_editor.SetModified( FALSE );

	}
	else
		m_editor.Clear();	
}

void CViewModuleFrm::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(rect);
	if( m_editor.m_hWnd )
		//m_editor.MoveWindow(0,0,cx,cy);	
		m_editor.MoveWindow(rect);
}

//读取模块的视图对象
BOOL CViewModuleFrm::ReadModuleEntity(_ConnectionPtr &pCon)
{
	CEPerfManageDoc* pDoc = (CEPerfManageDoc*)GetDocument();
	if(pDoc==NULL) return FALSE;
	CString strModelName = pDoc->m_pModel->m_strModelName;
	if(strModelName.IsEmpty()) return FALSE;
	//首先读取计算块列表，同时得到计算块的属性
	////////////////////////////////////////////////////
    CString strModuleBlockListTable;//
	CString strModuleBlockPropTable;//
    strModuleBlockListTable.Format("%s_%s_BlockList",strModelName,m_strModuleName);
	strModuleBlockPropTable.Format("%s_%s_BlockProp",strModelName,m_strModuleName);
	////////////////////////////////////////////////////
	//添加块，然后再读取块的属性值
	////////////////////////////////////////////////////////////////////
  	_RecordsetPtr  pRecord;
	_variant_t     Tempvalue ;
    VARIANT_BOOL bBOF ;
    pRecord.CreateInstance(__uuidof(Recordset)) ;
    pRecord->CursorLocation = adUseClient;

    CString strSQL;
    strSQL.Format("select * from %s ",strModuleBlockListTable);
    try
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}	
	bBOF = pRecord->GetBOF();
	if(!bBOF)  pRecord->MoveFirst();
	//块列表包括：名字、描述/类型
	while(!pRecord->adoEOF )
	{
		CString strName; //块
		CString strDesp;//块描述
		CString strType;//块类型，决定着用哪个构造对象
		long OrderID;
		double left,top,right,bottom;
		int InNum,OutNum;
		CString InNames;CString strInValueTypes;
		CString OutNames;CString strOutValueTypes;
		CString InputLinkStrs;

		BOOL bThisOk = TRUE;
		try
		{
			Tempvalue = pRecord->GetCollect("name");
			strName = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("desp");
			strDesp = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("type");
			strType = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("orderid");
			OrderID = long(Tempvalue);

			Tempvalue = pRecord->GetCollect("left");
			left = double(Tempvalue);
			Tempvalue = pRecord->GetCollect("top");
			top = double(Tempvalue);
			Tempvalue = pRecord->GetCollect("right");
			right = double(Tempvalue);
			Tempvalue = pRecord->GetCollect("bottom");
			bottom = double(Tempvalue);

			Tempvalue = pRecord->GetCollect("InNum");
			InNum = short(Tempvalue);
			Tempvalue = pRecord->GetCollect("OutNum");
			OutNum = short(Tempvalue);

			Tempvalue = pRecord->GetCollect("InNames");
			InNames = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("InValueTypes");
			strInValueTypes = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("OutNames");
			OutNames = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("OutValueTypes");
			strOutValueTypes = (LPCSTR)_bstr_t(Tempvalue);

			Tempvalue = pRecord->GetCollect("InputLinkStrs");
			InputLinkStrs = (LPCSTR)_bstr_t(Tempvalue);
		}
		catch(_com_error e)
		{
			AfxMessageBox("读取模块的计算块有异常发生！");
			//bThisOk = FALSE;
		}
		if(bThisOk) //保证没有读取的错误，开始新建一个
		{
			//CDiagramEntity* obj = CFlowchartControlFactory::CreateFromString( str ); 原来的一行字符串就可定义的
			CFlowchartEntity* obj = new CFlowchartEntity(strType);
			if(obj)
			{
				obj->SetBlockDesp(strDesp); //标题
				obj->SetBlockName(strName);//名称
				obj->SetBlockType(strType);//类型
				obj->SetCalcOrderID(OrderID); //计算号
				
				obj->m_pBlockInner->SetModelPtr(pDoc->m_pModel);//设置该计算块内部模型的指针值
				//设置该计算块内部模块的指针值
				obj->m_pBlockInner->SetModulePtr(pDoc->m_pModel->FindModuleByName(m_strModuleName));

				obj->SetRect(left,top,right,bottom);
				obj->CreatePortsFromString(InNum,OutNum,InNames,strInValueTypes,OutNames,strOutValueTypes,InputLinkStrs);//设置输入输出
				obj->ReadBlockProperties(theApp.m_pConLocalModel,strModuleBlockPropTable);//读取块的属性
				obj->m_pBlockInner->SetFlowchartEditorPObj(&m_editor);//加入CFlowchartEditor对象到计算块
				m_objs.Add(obj);
			}
		}
		pRecord->MoveNext();
	}
	return TRUE;
}

//保存该视图的所有模块和连接关系
//将表清空后再写入，写入模块列表，和模块的属性信息表
//所以需要操作两个表，而且属性需要在图形的对象中也必须体现出来，此时还不生成计算对象
//在编译时，检查错误；在连接时，要形成运行计算的对象。连接后可以进行计算（多种计算）；

BOOL CViewModuleFrm::SaveModuleEntity(_ConnectionPtr &pCon)
{
    BOOL bRet=TRUE;
	//////////////////////////////////////////////////////////////////////////////////
	//获得
	CEPerfManageDoc* pDoc = (CEPerfManageDoc*)GetDocument();
	if(pDoc==NULL) return FALSE;

	CString strModelName = pDoc->m_pModel->m_strModelName;
	//首先读取计算块列表，同时得到计算块的属性
    CString strModuleBlockListTable;//
    strModuleBlockListTable.Format("%s_%s_BlockList",strModelName,m_strModuleName);
	//////////////////////////////////////////////////////////////////////////////////
	//清空计算块列表
	CString strSQL;
	strSQL.Format("delete * from %s",strModuleBlockListTable);
	try
	{
	   pCon->Execute((const char*)strSQL,0,adCmdText); 
	}
   	catch(_com_error e)
	{
		bRet=FALSE;
		return bRet;
	}
	//////////////////////////////////////////////////////////////////////////////////
	////打开表记录，然后
	_RecordsetPtr  pRecord;
	_variant_t     Tempvalue ;
    //VARIANT_BOOL bBOF ;
    pRecord.CreateInstance(__uuidof(Recordset)) ;
    pRecord->CursorLocation = adUseClient;
    strSQL.Format("select * from %s",strModuleBlockListTable); //写信息到模块列表数据表
    try
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		//pRecord->Close() ;
		return FALSE;
	}	
    //不断写入
	if(!pRecord->adoEOF) pRecord->MoveLast();
	int max = m_objs.GetSize();
	CFlowchartEntity* obj;
	for( int t = 0 ; t < max ; t++ )
	{
		obj =  (CFlowchartEntity*)( m_objs.GetAt( t ) );
		pRecord->AddNew();
		CString strName=obj->GetName();
		pRecord->PutCollect(_T("name"),_variant_t(obj->GetName()) ); //放Name
		pRecord->PutCollect(_T("desp"),_variant_t(obj->GetBlockDesp()) ); //放DESP
		pRecord->PutCollect(_T("type"),_variant_t(obj->GetBlockType()) ); //放
		pRecord->PutCollect(_T("orderid"),short(obj->GetCalcOrderID()) ); //放

		pRecord->PutCollect(_T("left"),_variant_t(double(obj->GetLeft())) ); //放
		pRecord->PutCollect(_T("top"),_variant_t(double(obj->GetTop())) ); //放
		pRecord->PutCollect(_T("right"),_variant_t(double(obj->GetRight())) ); //放
		pRecord->PutCollect(_T("bottom"),_variant_t(double(obj->GetBottom())) ); //放

		pRecord->PutCollect(_T("InNum"),_variant_t(short(obj->GetInputNum())) ); //放
		pRecord->PutCollect(_T("OutNum"),_variant_t(short(obj->GetOutputNum())) ); //放

		CString InNames(_T(""));
		CString OutNames(_T(""));
		CString InputLinkStrs(_T(""));
		CString strInValueTypes(_T(""));
		CString strOutValueTypes(_T(""));
		
		CString strItem;
		int InNum = obj->GetInputNum();
		int OutNum = obj->GetOutputNum();
		for(int k=0;k<InNum;k++)
		{
			
			if(k==InNum-1)
			{
				InNames += obj->GetInPortID(k) ;
				InputLinkStrs += obj->GetPortLinkBlockName(k)+_T(".")+obj->GetPortLinkBlockPortName(k);
				
                strItem.Format("%d",obj->GetInputPortValueType(k));
				strInValueTypes+=strItem;
			}
			else
			{
				InNames += obj->GetInPortID(k) + _T(",");
				InputLinkStrs += obj->GetPortLinkBlockName(k)+_T(".")+obj->GetPortLinkBlockPortName(k)+ _T(",");
				
				strItem.Format("%d",obj->GetInputPortValueType(k));
				strInValueTypes+=strItem+ _T(",");
			}
			
		}
		for(k=0;k<OutNum;k++)
		{
			if(k==OutNum-1)
			{
				OutNames += obj->GetOutPortID(k) ;
				strItem.Format("%d",obj->GetOutputPortValueType(k));
				strOutValueTypes+=strItem;
			}
			else
			{
				OutNames += obj->GetOutPortID(k) + _T(",");
				strItem.Format("%d",obj->GetOutputPortValueType(k));
				strOutValueTypes+=strItem+ _T(",");
			}
			
		}
		
		pRecord->PutCollect(_T("InNames"),_variant_t(InNames) ); //放
		pRecord->PutCollect(_T("InValueTypes"),_variant_t(strInValueTypes) ); //放
		
		pRecord->PutCollect(_T("OutNames"),_variant_t(OutNames) ); //放
		pRecord->PutCollect(_T("OutValueTypes"),_variant_t(strOutValueTypes) ); //放
		
		pRecord->PutCollect(_T("InputLinkStrs"),_variant_t(InputLinkStrs) ); //放
		pRecord->Update();
	}
	pRecord->Close();
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	//写入计算块的信息，首先清除掉，然后顺序写入各个块
	//首先读取计算块列表，同时得到计算块的属性
    CString strModuleBlockPropTable;//
    strModuleBlockPropTable.Format("%s_%s_BlockProp",strModelName,m_strModuleName);
	//////////////////////////////////////////////////////////////////////////////////
	//清空计算块列表
	strSQL.Format("delete * from %s",strModuleBlockPropTable);
	try
	{
	   pCon->Execute((const char*)strSQL,0,adCmdText); 
	}
   	catch(_com_error e)
	{
		bRet=FALSE;
		return bRet;
	}
	//////////////////////////////////////////////////////////////////////////////////
	////打开表记录，然后
    strSQL.Format("select * from %s",strModuleBlockPropTable); //写信息到模块列表数据表
    try
	{
		pRecord->Open((const char*)strSQL,pCon.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
    }
   	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		bRet=FALSE;
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	for(  t = 0 ; t < max ; t++ )
	{
		obj =  (CFlowchartEntity*)( m_objs.GetAt( t ) );
		obj->PutPropertyToRecord(pRecord);
		
	}
	////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////
	m_objs.SetModified(FALSE);
	return bRet;
}

void CViewModuleFrm::OnButtonBlock(CString strBlockType,CString strBlockName,CString strBlockDesp)
{
	CFlowchartEntity* pBlockEntity = new CFlowchartEntity(strBlockType);
	if(pBlockEntity && pBlockEntity->m_pBlockInner!=NULL)
	{
		CEPerfManageDoc* pDoc = (CEPerfManageDoc*)GetDocument();
		pBlockEntity->SetModelPtr(pDoc->m_pModel);
		pBlockEntity->SetModulePtr(pDoc->m_pModel->FindModuleByName(m_strModuleName));
		pBlockEntity->SetBlockName(strBlockName);
	    pBlockEntity->SetBlockDesp(strBlockDesp); 
		pBlockEntity->m_pBlockInner->SetFlowchartEditorPObj(&m_editor);//加入CFlowchartEditor对象到计算块
	    m_editor.StartDrawingObject(pBlockEntity );	
	}
	else
	{
		AfxMessageBox("不支持该类型的计算块.");
	}
}
void CViewModuleFrm::OnFileSaveModule() 
{
	// TODO: Add your command handler code here
	BOOL bOK = SaveModuleEntity(theApp.m_pConLocalModel);
}

void CViewModuleFrm::OnCfgSetBlockCalID()
{
	int iOldOrder=-1;
	bool bFind=false;
	for(int i=0;i<m_objs.GetSize();i++)
	{
		CFlowchartEntity *pObj=(CFlowchartEntity *)m_objs.GetAt(i);
		if(pObj->IsSelected())
		{
			iOldOrder=pObj->GetCalcOrderID();
			bFind=true;
			break;
		}
	}
	if(!bFind)
	{
		AfxMessageBox("请先选中计算块！");
		return;
	}
	CDlgCalcOrder  dlg;
	dlg.m_iOrderID = iOldOrder;
	if(dlg.DoModal()==IDOK)
	{
		for(int i=0;i<m_objs.GetSize();i++)
		{
			CFlowchartEntity *pObj=(CFlowchartEntity *)m_objs.GetAt(i);
			if(pObj->IsSelected())
			{
				pObj->SetCalcOrderID(dlg.m_iOrderID);
				pObj->Select( TRUE );
			}
		}
		RefreshWindow();
	}
}
/*
void CViewModuleFrm::OnCfgOrdershow() 
{
	// TODO: Add your command handler code here
	BOOL bOrderMode = !m_editor.GetShowOrderMode();
	m_objs.SetShowOrderMode(bOrderMode);
	m_editor.SetShowOrderMode(bOrderMode);
	m_editor.RedrawWindow();
}
void CViewModuleFrm::OnUpdateCfgOrdershow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bOrderMode = m_editor.GetShowOrderMode();
	pCmdUI->SetCheck(bOrderMode);
	
}
*/

void CViewModuleFrm::ShowPortValue()
{
	// TODO: Add your command handler code here
	CEPerfManageDoc* pDoc = (CEPerfManageDoc*)GetDocument();
	BOOL bValueMode = !m_editor.GetShowValueMode();
	pDoc->m_BShowPortValue=bValueMode;
	m_objs.SetShowValueMode(bValueMode);
	m_editor.SetShowValueMode(bValueMode);
	m_editor.RedrawWindow();
}

void CViewModuleFrm::OnUpdateMagPortvalueshow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//BOOL bValueMode = m_editor.GetShowValueMode();
	//pCmdUI->SetCheck(bValueMode);
}
void CViewModuleFrm::ReplaceBlockName(CString strOldBName,CString strOldReplace,CString strNewReplace)
{
	int max = m_objs.GetSize();
	CFlowchartEntity* obj;
	for( int t = 0 ; t < max ; t++ )
	{
		obj =  (CFlowchartEntity*)( m_objs.GetAt( t ) );
		CString strBName=obj->GetBlockName();
		if(strBName==strOldBName)
		{
			strOldBName.Replace(strOldReplace,strNewReplace);
			if(m_editor.GetSameNameBlockCount(strOldBName)>=1)  
			{
				AfxMessageBox("名字已存在，请选用别的块名字.");
			}
			else
			{
				m_editor.ChangeBlockName(obj,strOldBName);
			}
			break;
		}
	}
}
void CViewModuleFrm::ReplaceBlockDesp(CString strOldBName,CString strOldReplace,CString strNewReplace)
{
	int max = m_objs.GetSize();
	CFlowchartEntity* obj;
	for( int t = 0 ; t < max ; t++ )
	{
		obj =  (CFlowchartEntity*)( m_objs.GetAt( t ) );
		CString strBName=obj->GetBlockName();
		if(strBName==strOldBName)
		{
			CString strDesp=obj->GetBlockDesp();
			strDesp.Replace(strOldReplace,strNewReplace);
			obj->SetBlockDesp(strDesp);
			break;
		}
	}
}
void CViewModuleFrm::ReplaceBlockPro(CString strOldBName,CString strOldReplace,CString strNewReplace)
{
	int max = m_objs.GetSize();
	CFlowchartEntity* obj;
	for( int t = 0 ; t < max ; t++ )
	{
		obj =  (CFlowchartEntity*)( m_objs.GetAt( t ) );
		CString strBName=obj->GetBlockName();
		if(strBName==strOldBName)
		{

			CString strPro=obj->GetBlockStrPro();
			strPro.Replace(strOldReplace,strNewReplace);
			obj->SetBlockStrPro(strPro);
			break;
		}
	}
}
void CViewModuleFrm::FindBlockByStrName(CString strFind,BOOL bAllSame,CArray<CCalcBlock*,CCalcBlock*&> &blockFindArr)
{
	int max = m_objs.GetSize();
	CFlowchartEntity* obj;
	for( int t = 0 ; t < max ; t++ )
	{
		obj =  (CFlowchartEntity*)( m_objs.GetAt( t ) );
		CString strBName=obj->GetBlockName();
		if(bAllSame)//完全匹配
		{
			if(strBName==strFind)
				blockFindArr.Add(obj->m_pBlockInner);
		}
		else//包含字符串
		{
			if(strBName.Find(strFind)>=0)
				blockFindArr.Add(obj->m_pBlockInner);
		}
	}
}
void CViewModuleFrm::FindBlockByStrDesp(CString strFind,BOOL bAllSame,CArray<CCalcBlock*,CCalcBlock*&> &blockFindArr)
{
	int max = m_objs.GetSize();
	CFlowchartEntity* obj;
	for( int t = 0 ; t < max ; t++ )
	{
		obj =  (CFlowchartEntity*)( m_objs.GetAt( t ) );
		CString strBDesp=obj->GetBlockDesp();
		if(bAllSame)//完全匹配
		{
			if(strBDesp==strFind)
				blockFindArr.Add(obj->m_pBlockInner);
		}
		else//包含字符串
		{
			if(strBDesp.Find(strFind)>=0)
				blockFindArr.Add(obj->m_pBlockInner);
		}
	}
}
void CViewModuleFrm::FindBlockByStrPro(CString strFind,BOOL bAllSame,CArray<CCalcBlock*,CCalcBlock*&> &blockFindArr)
{
	int max = m_objs.GetSize();
	CFlowchartEntity* obj;
	for( int t = 0 ; t < max ; t++ )
	{
		obj =  (CFlowchartEntity*)( m_objs.GetAt( t ) );
		CString strBPro=obj->GetBlockStrPro();
		if(bAllSame)//完全匹配
		{
			if(strBPro==strFind)
				blockFindArr.Add(obj->m_pBlockInner);
		}
		else//包含字符串
		{
			if(strBPro.Find(strFind)>=0)
				blockFindArr.Add(obj->m_pBlockInner);
		}
	}
}
