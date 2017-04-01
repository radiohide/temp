// ViewWSpaceFrm.cpp : implementation file
//

#include "stdafx.h"
#include "eperfmanage.h"
#include "ViewWSpaceFrm.h"
#include "CalcPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMap<CString,LPCTSTR,CCalcPort*,CCalcPort*&> g_WorkSpace;
/////////////////////////////////////////////////////////////////////////////
// CViewWSpaceFrm

IMPLEMENT_DYNCREATE(CViewWSpaceFrm, CViewUserFrm)

CViewWSpaceFrm::CViewWSpaceFrm()
	: CViewUserFrm(CViewWSpaceFrm::IDD),m_rowHeight(25)
{
	//{{AFX_DATA_INIT(CViewWSpaceFrm)
	m_ArrBlockRTType.Add("RTPointAdv");//ʵʱ�������
	m_ArrBlockRTType.Add("RTPoint");
	m_ArrBlockRTType.Add("WriteRTDB");

	m_ArrBlockRType.Add("DBPointAdv");//��ϵ�������
	m_ArrBlockRType.Add("DBPoint");
	m_ArrBlockRType.Add("WriteRDB");
	
	//}}AFX_DATA_INIT
}

CViewWSpaceFrm::~CViewWSpaceFrm()
{
}

void CViewWSpaceFrm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewWSpaceFrm)
	DDX_GridControl(pDX, IDC_GRIDCTRL, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewWSpaceFrm, CFormView)
	//{{AFX_MSG_MAP(CViewWSpaceFrm)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewWSpaceFrm diagnostics

#ifdef _DEBUG
void CViewWSpaceFrm::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewWSpaceFrm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG
void CViewWSpaceFrm::RefreshWindow()
{
	if( m_Grid.m_hWnd )
	{
		FillGCtrl();
		//m_Grid.RedrawWindow();
		//m_Grid.Invalidate();
	}
	//this->Invalidate();
}
/////////////////////////////////////////////////////////////////////////////
// CViewWSpaceFrm message handlers
void CViewWSpaceFrm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	Color = RGB(255,255,255);
	BKColor= RGB(255,0,0);
	FKColor= RGB(0,0,255);

	// TODO: Add extra initialization here
	//���ÿؼ�������ListCtrl�ı���
	m_Grid.SetListMode(TRUE);
	//���ÿؼ���ɫ������GetDefaultCell�����������ֱ��ʾ�Ƿ��ǹ̶��л����ǹ̶��С�������趨��ȱʡ����ɫΪ��ɫ�� 
	m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(Color);

	//���ÿؼ��Ĺ̶���Ϊһ�У�һ���Ǳ����
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);
	//���ÿؼ��Ĺ̶��л��߹̶��в��ܱ�ѡ��
	m_Grid.SetFixedColumnSelection(TRUE);
	m_Grid.SetFixedRowSelection(TRUE);
	//���ÿؼ��Ƿ����������л�����
	m_Grid.EnableColumnHide(FALSE);
	m_Grid.EnableRowHide(FALSE);
	//���ÿؼ��Ƿ�����༭
	m_Grid.SetEditable(FALSE);
	//���ÿؼ��Ƿ�����ѡ��
	m_Grid.EnableSelection(TRUE);
	m_Grid.SetListMode(false);//�����䲻�����б�ʽ��ʾ�������Ե��������Ԫ��
	//���ÿؼ��Ƿ���������ͷ����
	m_Grid.SetHeaderSort(false);
	//���ÿؼ��Ƿ������ѡ���ߵ�ѡ
	m_Grid.SetSingleRowSelection(false);
	m_Grid.SetSingleColSelection(TRUE);
	//���ÿؼ��ĳ�ʼ����������
	m_Grid.SetRowCount(0);
	m_Grid.SetColumnCount(2);
	//���ÿؼ��Ƿ������Զ��������д�С
	m_Grid.SetRowResize(true);
	m_Grid.SetColumnResize(true);
	//���ÿؼ���Ҫ����ͽ������
	m_Grid.SetTrackFocusCell(FALSE);
	m_Grid.SetFrameFocusCell(FALSE);

	CRect rectDummy;
	rectDummy.SetRectEmpty ();
	GetClientRect(rectDummy);
	m_Grid.MoveWindow(rectDummy);

	FillGCtrl();
}

void CViewWSpaceFrm::FillGCtrl()
{
	//m_Grid.SetActiveWindow();
	//m_Grid.SetFocus();
	m_Grid.DeleteAllItems();
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);
	m_Grid.SetColumnCount(7);
	m_Grid.SetRowHeight(0,m_rowHeight);

	m_Grid.SetItemText(0,0,"ID");m_Grid.SetColumnWidth(0,40);
	m_Grid.SetItemText(0,1,"ģ����");m_Grid.SetColumnWidth(1,80);
	m_Grid.SetItemText(0,2,"�������");m_Grid.SetColumnWidth(2,80);
	m_Grid.SetItemText(0,3,"�˿�");m_Grid.SetColumnWidth(3,80);
	m_Grid.SetItemText(0,4,"ֵ");m_Grid.SetColumnWidth(4,90);
	m_Grid.SetItemText(0,5,"ʱ��");m_Grid.SetColumnWidth(5,90);
	m_Grid.SetItemText(0,6,"״̬");m_Grid.SetColumnWidth(6,40);


	POSITION pos = g_WorkSpace.GetStartPosition();
    while(pos)
    {
		CString strKey;
		CCalcPort* pPort;
        g_WorkSpace.GetNextAssoc(pos, strKey, pPort);
		if(pPort==NULL)
			continue;
		if(TagType!=3)
		{
			if(TagType!=getBlockTypeByPort(pPort))
				continue;
		}

		CString str;
		int rowCount=m_Grid.GetRowCount();
		m_Grid.SetRowCount(rowCount+1);
		str.Format("%d",rowCount);
		m_Grid.SetItemText(rowCount,0,str);
		m_Grid.SetItemText(rowCount,1,((pPort->GetBlockPtr())->GetModulePtr())->GetModuleName());
		m_Grid.SetItemText(rowCount,2,(pPort->GetBlockPtr())->GetBlockName());
		m_Grid.SetItemText(rowCount,3,pPort->GetPortID());
		double value;
		long lTime;
		short sStatus;
		if(pPort->GetPortValueType()==VALUE_DOUBLE)
		{
			pPort->GetPortUniValue().GetDoubleVal(&value);
			str.Format("%f",value);
			m_Grid.SetItemText(rowCount,4,str);

			pPort->GetPortUniValue().GetTimeVal(&lTime);
			if(lTime==0)
				str.Format("��");
			else
			{
				CTime time(lTime);
				str=time.Format("%Y-%m-%d %H:%M:%S");
			}
			m_Grid.SetItemText(rowCount,5,str);

			pPort->GetPortUniValue().GetStatusVal(&sStatus);
			str.Format("%d",sStatus);
			m_Grid.SetItemText(rowCount,6,str);
		}
		else
		{
			long dateSize=pPort->GetPortUniValue().GetDataSize();
			if(dateSize>0)//����ֵ�ռ�
			{
				for(long i=0;i<dateSize;i++)
				{
					rowCount=m_Grid.GetRowCount();
					m_Grid.SetRowCount(rowCount+1);
					str.Format("%d",rowCount);
					m_Grid.SetItemText(rowCount,0,str);
					m_Grid.SetItemText(rowCount,1,((pPort->GetBlockPtr())->GetModulePtr())->GetModuleName());
					m_Grid.SetItemText(rowCount,2,(pPort->GetBlockPtr())->GetBlockName());
					m_Grid.SetItemText(rowCount,3,pPort->GetPortID());
					pPort->GetPortUniValue().GetDoubleValAt(i,&value);
					str.Format("%f",value);
					m_Grid.SetItemText(rowCount,4,str);
					
					pPort->GetPortUniValue().GetTimeValAt(i,&lTime);
					if(lTime==0)
						str.Format("��");
					else
					{
						CTime time(lTime);
						str=time.Format("%Y-%m-%d %H:%M:%S");
					}
					m_Grid.SetItemText(rowCount,5,str);
					
					pPort->GetPortUniValue().GetStatusValAt(i,&sStatus);
					str.Format("%d",sStatus);
					m_Grid.SetItemText(rowCount,6,str);
				}
			}
		}
    }
	m_Grid.Invalidate();
}

void CViewWSpaceFrm::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if( m_Grid.m_hWnd )
		m_Grid.MoveWindow(0,0,cx,cy);		
}
int CViewWSpaceFrm::getBlockTypeByPort(CCalcPort* pPort)
{
	CString blockType=(pPort->GetBlockPtr())->GetBlockType();
	for(int i=0;i<this->m_ArrBlockRTType.GetSize();i++)
	{
		if(m_ArrBlockRTType[i]==blockType)
		{
			return 1;//ʵ���
		}
	}
	for(i=0;i<this->m_ArrBlockRType.GetSize();i++)
	{
		if(m_ArrBlockRType[i]==blockType)
		{
			return 2;//��ϵ���
		}
	}
	return 3;//�����ռ�
}
