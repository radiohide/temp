#if !defined(AFX_FLOWCHARTENTITY_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
#define AFX_FLOWCHARTENTITY_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_

#include "DiagramEditor\DiagramEntity.h"
#include "DiagramEditor/Tokenizer.h"
#include "..\CalcPort.h"
#include "..\CalcBlock.h"

#define DEHT_INPUT                      10
#define DEHT_OUTPUT                  11
#define DEHT_BLOCKNAME          12
#define DEHT_BLOCKDESP            13
#define DEHT_BLOCKORDER        14

class CFlowchartEntity : public CDiagramEntity
{
public:
	CCalcBlock* m_pBlockInner;
public:
	//2011-11�ӣ���ʾͼ�����
	BOOL GetIsShowDesp();//�Ƿ���ʾDESP
	BOOL GetIsShowInPortText();
	BOOL GetIsShowOutPortText();
public:
	//���ڼ�¼�Ƿ�˿�˫������Ϣ
	int m_FocusPortID;
	BOOL m_bFocusPort;
	//��ü����
	//�����������ĸ���
	int GetOutputNum() const;
	int GetInputNum() const;
	CString GetInPortID(int k);
	CString GetOutPortID(int k);
	CString GetInPortValue(int k);
	CString GetOutPortValue(int k);

	int GetInputPortValueType(int k);
	int GetOutputPortValueType(int k);
	void SetInputPortValueType(int k,int type);
	void SetOutputPortValueType(int k ,int type);

	//���ӵ���ϢҪ��¼�������������������ĸ�
    
	void SetBlockPortLink(int portID,CString strBlockName,CString strPortName);
	CString GetPortLinkBlockName(int portID);
	CString GetPortLinkBlockPortName(int portID);
	BOOL GetBlockPortLinked(int portID);
	void  ClearBlockPortLink(int k);

	CPoint   GetPortPoint(UINT marker,CRect rect,int PortNum,int portID) const;
	CPoint   GetPortPointByName(UINT marker,CString strPortName) const;

	int FindInPortIdByName(CString strName) const;
	int FindOutPortIdByName(CString strName) const;

	////////////////////////////////////////////////////////////////
	//��Block����������
	CString GetBlockType();
	void SetBlockType(CString str);
	CString GetBlockName();
	void SetBlockName(CString str);
	CString GetBlockDesp();
	void SetBlockDesp(CString str);
	void SetModelPtr(CCalcModel* p);
	void SetModulePtr(CCalcModule* p) ;
	CString GetBlockStrPro();//�õ��������ַ�������
	void SetBlockStrPro(CString str);//���ü������ַ�������
	void SetCalcOrderID(int id);
	int GetCalcOrderID();
  ////////////////////////////////////////////////////////////////////
	void CreatePortsFromString(int InNum,int OutNum,CString InNames,CString strInValueTypes,CString OutNames,CString strOutValueTypes, CString InputLinkStrs);
	//void CreatePortsFromString(int InNum,int OutNum,CString InNames,CString OutNames,CString InputLinkStrs);
	//void ReadBlockProperties();
	void ReadBlockProperties(_ConnectionPtr &pCon,CString strPropTable);

	void ShowPropDialog(CWnd* parent, BOOL show = TRUE);
	void ShowOrderDialog(CWnd* parent, BOOL show = TRUE);
	CString ShowBlockNameDialog(CWnd* parent, BOOL show = TRUE);
	CString ShowBlockDespDialog(CWnd* parent, BOOL show = TRUE);
	
protected:
	CString m_strBlockType;
	//CPortArray m_InPortArray;	// �����б�
	//CPortArray m_OutPortArray;	// ����б�
	//////////////////////////////////////////////////////////////////////////////
	 CRect   GetPortTextRect(UINT marker,CRect rect,int PortNum,int portID);
public:
// Construction/initialization/destruction
	CFlowchartEntity(CString strType);
	virtual	~CFlowchartEntity();
	virtual CDiagramEntity* Clone();
	virtual void	Copy( CDiagramEntity* obj );
    
	static	CDiagramEntity* CreateFromString( const CString& str );
   
// Overrides
	virtual int		GetHitCode( CPoint point ) const;
	virtual int		GetHitCode( const CPoint& point, const CRect& rect ) const;
	virtual int    GetHitCodeExt( const CPoint& point, const CRect& rect,int* portID ) const;
	virtual HCURSOR GetCursor( int hit ) const;
	
	virtual void Draw( CDC* dc, CRect rect );
	virtual void	ShowProperties( CWnd* parent, BOOL show = TRUE );
    virtual void PutPropertyToRecord(_RecordsetPtr& pRecord); //�������������������¼��ǰλ��
	

	void			SetMoved( BOOL moved );
	BOOL		 GetMoved();

////////////////////////////////////////////////////////////////////////////

protected:
// Overrides
	virtual void	DrawSelectionMarkers( CDC* dc, CRect rect ) const;
	virtual CRect   GetPortMarkerRect(UINT marker,CRect rect,int PortNum,int portID) const;//�õ��˿ڵ��������
	virtual CRect   GetBlockNameRect(CRect rect) const;  //������ֵľ��ξ���
	virtual CRect   GetBlockDespRect(CRect rect) const; //��������ľ�������
	virtual CRect   GetBlockOrderRect(CRect rect) const; //��ü���˳���������


private:
// Private data
	//CPropertyDialog	m_dlg; ���ԶԻ���������Ӧ��ͬ�ļ������CCalcBlock
	//CDlgPropRTPoint  m_dlg;
	BOOL			m_moved;

};

#endif // !defined(AFX_FLOWCHARTENTITY_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
