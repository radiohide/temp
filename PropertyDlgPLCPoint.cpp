// PropertyDlgPLCPoint.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "PropertyDlgPLCPoint.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgPLCPoint dialog


CPropertyDlgPLCPoint::CPropertyDlgPLCPoint(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyDlgPLCPoint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDlgPLCPoint)
	m_iPort = 502;
	m_iFunNo = 0;
	m_iAdd = 0;
	m_strIP = _T("192.168.1.1");
	
	 m_strCurrentPointName="";
	 m_strCurentType="BOOL";
	//}}AFX_DATA_INIT
}


void CPropertyDlgPLCPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDlgPLCPoint)
	DDX_Control(pDX, IDC_COMBO2, m_ComPointName);
	DDX_Control(pDX, IDC_COMBO1, m_ComType);
	DDX_Text(pDX, IDC_EDIT2, m_iPort);
	DDX_Text(pDX, IDC_EDIT4, m_iFunNo);
	DDX_Text(pDX, IDC_EDIT9, m_iAdd);
	DDX_Text(pDX, IDC_EDIT5, m_strIP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyDlgPLCPoint, CDialog)
	//{{AFX_MSG_MAP(CPropertyDlgPLCPoint)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	ON_CBN_EDITCHANGE(IDC_COMBO2, OnEditchangeCombo2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDlgPLCPoint message handlers

BOOL CPropertyDlgPLCPoint::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitMap();

   	int typeindex=m_ComType.FindString(0,m_strCurentType);
	m_ComType.SetCurSel(typeindex);

	m_ComPointName.ResetContent();
	CString Text;        
	m_ComType.GetWindowText(Text);
	CString strPointName,strType;
	POSITION ps = m_typeMap.GetStartPosition();
	
	while( ps )		
	{   		
		m_typeMap.GetNextAssoc(ps, strPointName, strType);
		if(Text==strType)
			m_ComPointName.AddString(strPointName);
	}

	m_ComPointName.SetWindowText(m_strCurrentPointName);
	
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyDlgPLCPoint::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	//记录痕迹
	m_ComType.GetWindowText(m_strCurentType);
	m_ComPointName.GetWindowText(m_strCurrentPointName);

	CString strForsearch;
    if(!m_typeMap.Lookup(m_strCurrentPointName,strForsearch))
	{
		AfxMessageBox("输入点名不存在!");
		return;
	}
	if(m_iFunNo==0)
	{
		AfxMessageBox("功能码不正确!");
		return;
	}
	
	CDialog::OnOK();
}

void CPropertyDlgPLCPoint::InitMap()
{
   	_RecordsetPtr m_pRec;
	
	try   
	{   
		HRESULT hr;
        hr = m_pRec.CreateInstance(__uuidof(Recordset)); 
		CString strSQL;
		strSQL.Format("select * from regset");
        m_pRec->Open((char *)_bstr_t(strSQL),_variant_t((IDispatch*)theApp.m_pConRemoteModel,true),
			adOpenStatic,adLockOptimistic,adCmdText);			
	}   
	catch(_com_error  *e)   
	{   
		AfxMessageBox(e->ErrorMessage());   
	}  
	CString X_TagID,X_TagName,X_TagDesp,X_ModbusAddress,X_FunCode,X_DataType;
	while(!m_pRec->adoEOF)
	{		
		X_TagID =(char *)_bstr_t(m_pRec->GetCollect("TagID"));
		X_TagName =(char *)_bstr_t(m_pRec->GetCollect("TagName"));
		X_TagDesp =(char *)_bstr_t(m_pRec->GetCollect("TagDesp"));
		X_ModbusAddress =(char *)_bstr_t(m_pRec->GetCollect("ModbusAddress"));
		X_FunCode =(char *)_bstr_t(m_pRec->GetCollect("FunCode"));
		X_DataType =(char *)_bstr_t(m_pRec->GetCollect("DataType"));

        m_idMap.SetAt(X_TagName,atoi(X_TagID));
		m_desMap.SetAt(X_TagName,X_TagDesp);
		m_addressMap.SetAt(X_TagName,atoi(X_ModbusAddress));
		m_funcodeMap.SetAt(X_TagName,X_FunCode);
		m_typeMap.SetAt(X_TagName,X_DataType);

		m_pRec->MoveNext();
	}
}

void CPropertyDlgPLCPoint::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here

	CString Text;        
	m_ComType.GetWindowText(Text);

	m_ComPointName.ResetContent();
	m_ComPointName.SetWindowText("");
	m_iFunNo = 0;
	m_iAdd = 0;


	CString strPointName,strType;
	POSITION ps = m_typeMap.GetStartPosition();
	
	while( ps )		
	{   		
		m_typeMap.GetNextAssoc(ps, strPointName, strType);
		if(Text==strType)
			m_ComPointName.AddString(strPointName);
	}
	
	UpdateData(false);
}

void CPropertyDlgPLCPoint::OnSelchangeCombo2() 
{
	// TODO: Add your control notification handler code here
	int Nameindex=m_ComPointName.GetCurSel();
	CString Text; 
	m_ComPointName.GetLBText(Nameindex,Text);
    //设置地址
	int iAddress;
    m_addressMap.Lookup(Text,iAddress);
	m_iAdd=iAddress;
    //设置功能码序列
	CString strFunCodeSer;
    m_funcodeMap.Lookup(Text,strFunCodeSer);
	DealFunCode(strFunCodeSer);
	//
	CString typetemp;
	m_ComType.GetWindowText(typetemp);
	int retCode=0;
	FindRightFunCode(typetemp,retCode);
	m_iFunNo=retCode;
    if(m_iFunNo==0)
	{
		AfxMessageBox("没有找到合适的功能码!");
	}
	UpdateData(false);
}


void CPropertyDlgPLCPoint::OnEditchangeCombo2() 
{
	// TODO: Add your control notification handler code here
	CString typetemp,nametemp;
	m_ComType.GetWindowText(typetemp);
	m_ComPointName.GetWindowText(nametemp);

	CString strForsearch;
    if(!m_typeMap.Lookup(nametemp,typetemp))
	{
		m_iAdd=0;
		m_iFunNo=0;
	}
	else
	{
		//设置地址
		int iAddress;
		m_addressMap.Lookup(nametemp,iAddress);
		m_iAdd=iAddress;
		//设置功能码
		
		CString typetemp;
		m_ComType.GetWindowText(typetemp);
		int retCode=0;
		FindRightFunCode(typetemp,retCode);
		m_iFunNo=retCode;
		if(m_iFunNo==0)
		{
			AfxMessageBox("没有找到合适的功能码!");
		}
	}
	UpdateData(false);
}

void CPropertyDlgPLCPoint::DealFunCode(CString strFunCodeSer)
{
	v_funcodeSer.clear();
	std::vector<int> indexes;//包含了所用对应逗号的索引
	for(int i =0; i<strFunCodeSer.GetLength(); i++)
	{
		char ch =strFunCodeSer.GetAt(i);
		if(44==ch)
		{
           indexes.push_back(i);
		}
	}
	if(indexes.size()==0)
	{
		v_funcodeSer.push_back(atoi(strFunCodeSer));
	}
	if(indexes.size()==1)
	{
		v_funcodeSer.push_back(atoi(strFunCodeSer.Left(indexes[0])));
		v_funcodeSer.push_back(atoi(strFunCodeSer.Right(strFunCodeSer.GetLength()-indexes[0]-1)));
	}
	if(indexes.size()>1)
	{
		for(int j=0; j<=indexes.size();j++)
		{
			if(j==0)
			{
				v_funcodeSer.push_back(atoi(strFunCodeSer.Left(indexes[0])));
			}
			if((j<indexes.size())&&(j>0))
			{
				v_funcodeSer.push_back(atoi(strFunCodeSer.Mid(indexes[j-1]+1,indexes[j]-indexes[j-1]-1)));					
			}
			if(j==indexes.size())
			{
				v_funcodeSer.push_back(atoi(strFunCodeSer.Right(strFunCodeSer.GetLength()-indexes[j-1]-1)));					
			}
		}
	}  
}

BOOL CPropertyDlgPLCPoint::FindRightFunCode(CString dataType,int &FunCode)
{
	BOOL bret=FALSE;
	int  iret=0;
	if(dataType=="BOOL")
	{
		for(int i=0; i<v_funcodeSer.size(); i++)
		{
			if(v_funcodeSer[i]==2)
			{
				bret=TRUE;
				FunCode=2;
				break;
			}
			if(v_funcodeSer[i]==1)
			{
				bret=TRUE;
				FunCode=1;
				break;
			}
		}
	}
	else if((dataType=="DINT") || (dataType=="REAL"))//整数和实数的读功能码都为3
	{
		for(int i=0; i<v_funcodeSer.size(); i++)
		{
			if(v_funcodeSer[i]==3)
			{
				bret=TRUE;
				FunCode=3;
				break;
			}
			if(v_funcodeSer[i]==4)
			{
				bret=TRUE;
				FunCode=4;
				break;
			}
		}
	}
	return bret;
}
