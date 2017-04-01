// RTPointBatchBlock1.cpp: implementation of the CRTPointBatchBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RTPointBatchBlock.h"
#include "PropertyDlgRTPointBat.h"

#include<fstream>
using namespace std; 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRTPointBatchBlock::CRTPointBatchBlock()
{
	m_iSysOrInput=0;
	pTagData=NULL;
	for(int i=0;i<1000;i++)
	{
		pTagNames[i]=NULL;
	}
	SetBlockType(_T( "RTPointBatch" ) );
	ConstructBlockInputOutput();
}

CRTPointBatchBlock::~CRTPointBatchBlock()
{
	for(int i=0;i<strRTDBTagArr.GetSize();i++)
	{
		if(NULL!=pTagNames[i])
		{
			delete []pTagNames[i];
			pTagNames[i]=NULL;
		}
	}
	if(pTagData!=NULL)
	{
		delete []pTagData;
		pTagData=NULL;
	}
}
//���������½�����ĺ���
CCalcBlock* CRTPointBatchBlock::CreateFromString( const CString& strType )
{
	CRTPointBatchBlock* obj = new CRTPointBatchBlock;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CRTPointBatchBlock::ConstructBlockInputOutput ()
{
	//AddOutputPort("Out",VALUE_DOUBLE);
}
//��ʼ������ĺ���������ĳ�ʼ��
void CRTPointBatchBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	for(int i=0;i<strRTDBTagArr.GetSize();i++)
	{
		pTagNames[i]=new char[48];
		memset(pTagNames[i],0,48);
		strcpy(pTagNames[i],strRTDBTagArr[i]);
	}
	int iCount=strRTDBTagArr.GetSize();
	if(iCount>0)
	{
		pTagData=new TagData[iCount];
	}
	this->m_pModel->LoadRTDBDllAndCon();
}
//�������ԵĶ����ַ���
CString CRTPointBatchBlock::GetBlockStrPro()//to do
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CRTPointBatchBlock::SetBlockStrPro(CString str)//���ü������ַ�������//to do
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
void CRTPointBatchBlock::Calc()//���м���ĺ���,��Ҫ����
{
	if(this->m_pModel->m_bConRTDB)
	{
		GetInputValueFromGlobalWS();

		double dInValue=0;
		long lInTime = CTime::GetCurrentTime().GetTime();
		if(m_iSysOrInput==1)
		{
			CCalcPort *pInPortObj = GetInputPortObj(0);
			pInPortObj->GetPortUniValue().GetDoubleVal(&dInValue);
			lInTime=(long)dInValue;
		}

		int inNum = GetOutputNum();
		int nRet = -1;
		int iCount=strRTDBTagArr.GetSize();
		if(iCount==0)
			return;
		memset(pTagData,0,iCount*sizeof(TagData));

		//�����ƶ�ȡ
		typedef  int (*pGetRTDataByBatch)(char* tagNames[],TagData * pTagData,long nCount);
		pGetRTDataByBatch m_GetRTDataByBatch =(pGetRTDataByBatch)GetProcAddress(this->m_pModel->hDLL,"GetRTDataByBatch");


		//���Դ��룺��ʱ����
		long t1,t2;
		if (this->m_pModel->m_strModelName.Compare("ZJ_w001w003AccumBuffer")==0)
		{
			t1 = GetTickCount();
		}
		
		nRet=m_GetRTDataByBatch(pTagNames,pTagData,iCount);
		if(nRet!=0)
		{
			::Sleep(5);
			memset(pTagData,0,iCount*sizeof(TagData));
			nRet=m_GetRTDataByBatch(pTagNames,pTagData,iCount);//��ѯʧ���ٲ�ѯһ��
		}

		if (this->m_pModel->m_strModelName.Compare("ZJ_w001w003AccumBuffer")==0)
		{
			int nModuleCount = this->m_pModel->GetBlockCount();
			CString sBlockDesp = this->GetBlockName();
			
			t2 = GetTickCount();
			long t3 = t2 - t1;
			
			ofstream outfile("E:\\readrtdb.txt");
			CString sc ;
			sc.Format("block name is %s:Time is %d ms",sBlockDesp,t3);
			
			//if (t3 > 5000)
			{
				outfile<<(LPCTSTR)(_T(sc));
			}
			
			outfile.close();
		}

		//��Ϊ��ʱ���ȡnew++
		/*typedef  int (*pGetHistoryDataByTime)(const char * tagName,long time, TagData *pTagData);
		pGetHistoryDataByTime m_GetHistoryDataByTime =(pGetHistoryDataByTime)GetProcAddress(this->m_pModel->hDLL,"GetHistoryDataByTime");

		for (int i=0;i<inNum;i++)
		{
			nRet = m_GetHistoryDataByTime(pTagNames[i],lInTime, &(pTagData[i]));
			if(nRet!=0)
			{
				for(int j=0;j<50;j++)//����ȡʧ�ܣ�����50�ζ�ȡ��
				{
					memset(&(pTagData[i]),0,sizeof(TagData));
					::Sleep(5);//��ʱ��ȷʵ���ȡʧ�ܣ��ڵڶ��ζ�ȡ��ʱ���ɹ�
					nRet = m_GetHistoryDataByTime(pTagNames[i],lInTime, &(pTagData[i]));
					if(nRet==0)
						break;
				}
				if(nRet!=0)
				{
					pTagData[i].status=0;
					pTagData[i].time=0;
					pTagData[i].value=0;
				}
			}
		}*/

		
		if(nRet==0)//��ѯ�ɹ�
		{
			/*
			long lNow=CTime::GetCurrentTime().GetTime();
			if(m_iSysOrInput==1)//��������ⲿʱ��
			{
				lNow=lInTime;
			}*/
			for(int i=0;i<inNum;i++)
			{
				CCalcPort *pPortObj = GetOutputPortObj(i);
				CUniValue UniVar;
				//UniVar.AddData(pTagData[i].value,lInTime,pTagData[i].status);//��ֵΪ��ǰʱ��
				UniVar.AddData(pTagData[i].value,lInTime,pTagData[i].status,pTagData[i].time);
				pPortObj->SetPortUniValue(UniVar);
			}
		}
		else//��ѯʧ��
		{
			for(int i=0;i<inNum;i++)
			{
				CCalcPort *pPortObj = GetOutputPortObj(i);
				CUniValue UniVar;
				UniVar.AddData(0,0,-1); //ԭ
				//UniVar.AddData(0,0,0);
				pPortObj->SetPortUniValue(UniVar);
			}
		}
	}
	OutputResultToGlobalWS();
}

//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CRTPointBatchBlock::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("SysOrInput")==0)
	{
		if(!strItem1.IsEmpty())  {m_iSysOrInput=atoi(strItem1);}
	}
	else
	{
		int propType,propIndex;
		getPropTypeByName(strPropName,propType,propIndex);
		if(propType==2)
		{
			if(!strItem1.IsEmpty())  {strRTDBTagArr.SetAtGrow(propIndex,strItem1);}
			if(!strItem2.IsEmpty())  {strPortTagArr.SetAtGrow(propIndex,strItem2);}
			if(!strItem3.IsEmpty())  {strUnitArr.SetAtGrow(propIndex,strItem3);}
			if(!strItem4.IsEmpty())  {strTagDespArr.SetAtGrow(propIndex,strItem4);}
		}
	}
}

void CRTPointBatchBlock::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
{
	int dIndex=strPropName.ReverseFind('.');
	CString strIndex=strPropName.Right(strPropName.GetLength()-dIndex-1);
	CString strName=strPropName.Left(dIndex);
	propIndex=atoi(strIndex);

	if(strName=="input")//����Ϊ1
	{
		propType=1;
	}
	else if(strName=="output")//���Ϊ2
	{
		propType=2;
	}
	else if(strName=="formula")//��ʽΪ3
	{
		propType=3;
	}
}
//��������ʾ��ͬ�����Զ���Ի���
void CRTPointBatchBlock::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	CPropertyDlgRTPointBat dlg;
	
	int portnumLinkBefore=this->GetInputNum();//����
	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//�����ԭ�е�����˿�����
	CStringArray  IDArrRightIn;//������ڲ�������˿�����
	
	CStringArray  IDArrLeftOut;//�����ԭ�е�����˿�����
	CStringArray  IDArrLeftIn;//������ڲ�������˿�����

	for(int q=0; q<portnumLinkBefore;q++)
	{
		CString strPortID;
		strPortID=this->GetInputPortObj(q)->GetPortID();
		IDArrLeftOut.Add(strPortID);
	}

	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}

	dlg.strPortTagArr.Copy(strPortTagArr);
	dlg.strRTDBTagArr.Copy(strRTDBTagArr);
	dlg.strTagDespArr.Copy(strTagDespArr);
	dlg.strUnitArr.Copy(strUnitArr);
	dlg.m_iSysOrInput=m_iSysOrInput;
	if(dlg.DoModal()==IDOK)
	{
		strPortTagArr.Copy(dlg.strPortTagArr);
		strRTDBTagArr.Copy(dlg.strRTDBTagArr);
		strTagDespArr.Copy(dlg.strTagDespArr);
		strUnitArr.Copy(dlg.strUnitArr);
		
		m_iSysOrInput=dlg.m_iSysOrInput;

		//1��������˿ڵ��޸�
		if(m_iSysOrInput==1)
		{
			IDArrLeftIn.Add("time");
		}
		
		//time��̬�仯����
		for(int j=IDArrLeftOut.GetSize()-1; j>-1; j--)//ɾ��ԭ�ж����õ�Port,����ɾ
		{
			int  runs=0;//��������
			for(int k=0; k<IDArrLeftIn.GetSize(); k++)
			{
				if(IDArrLeftIn[k]!=IDArrLeftOut[j])	
				{runs++;}
			}
			if(runs==IDArrLeftIn.GetSize())
			{
				this->ClearInputPortByIndex(j);
			}
		}	
		for(int m=0; m<IDArrLeftIn.GetSize(); m++)	//����µ�Port
		{
			int  runs=0;//��������
			for(int n=0; n<IDArrLeftOut.GetSize(); n++)
			{
				if(IDArrLeftIn[m]!=IDArrLeftOut[n])
				{runs++;}		
			}
			if(runs==IDArrLeftOut.GetSize())
			{
				CString str=IDArrLeftIn[m];
				InSertInputPortBefore(m,str, VALUE_DOUBLE, _T(str));
			}
		}
		

		//������˿ڵ��޸�
		for(int i=0;i<strPortTagArr.GetSize();i++)
		{
			IDArrRightIn.Add(strPortTagArr[i]);
		}		
		for(int jj=IDArrRightOut.GetSize()-1; jj>-1; jj--)//ɾ��ԭ�ж����õ�Port,����ɾ
		{
			int  runs=0;//��������
			for(int kk=0; kk<IDArrRightIn.GetSize(); kk++)
			{
				if((IDArrRightIn[kk]!=IDArrRightOut[jj]))	
				{runs++;}
			}
			if(runs==IDArrRightIn.GetSize())
			{
				this->ClearOutputPortByIndex(jj,parent);
			}
		}		
		for(int mm=0; mm<IDArrRightIn.GetSize(); mm++)//����µ�Port
		{
			int  runs=0;//��������
			for(int nn=0; nn<IDArrRightOut.GetSize(); nn++)
			{
				if((IDArrRightIn[mm]!=IDArrRightOut[nn]))
				{runs++;}		
			}
			if(runs==IDArrRightOut.GetSize())
			{
				CString str=IDArrRightIn[mm];
				InSertOutputPortBefore(mm,str, VALUE_DOUBLE, _T(str));
			}
		}
		parent->RedrawWindow();
	}
}
//�����Խ���ģ��д�뵽���ݿ���
void CRTPointBatchBlock::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	for(int i=0;i<strRTDBTagArr.GetSize();i++)//���
	{
		CString strOutPut=strRTDBTagArr[i];
		if(strOutPut=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("output.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������

		strFieldValue.Format("%s",strRTDBTagArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",strPortTagArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",strUnitArr[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",strTagDespArr[i]); //������
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������
	}
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue="SysOrInput";//������
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	strFieldValue.Format("%d",m_iSysOrInput); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	
	pRecord->Update();
}	
//����ͼ��ʱ����Ҫ�������ԡ�
void CRTPointBatchBlock::CopyPropertyData(CCalcBlock* pBlock)
{
	this->strRTDBTagArr.Copy(((CRTPointBatchBlock*)pBlock)->strRTDBTagArr);
	this->strPortTagArr.Copy(((CRTPointBatchBlock*)pBlock)->strPortTagArr);
	this->strTagDespArr.Copy(((CRTPointBatchBlock*)pBlock)->strTagDespArr);
	this->strUnitArr.Copy(((CRTPointBatchBlock*)pBlock)->strUnitArr);
	this->m_iSysOrInput=((CRTPointBatchBlock*)pBlock)->m_iSysOrInput;
	
}
//�����Է��ô���������
void CRTPointBatchBlock::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{

}
