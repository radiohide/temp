// RTDBHistSeriesBatWP.cpp: implementation of the CRTDBHistSeriesBatWP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RTDBHistSeriesBatWP.h"
#include "PropertyDlgRTDBHistSeriesBatWP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRTDBHistSeriesBatWP::CRTDBHistSeriesBatWP()
{
	dLimitValue=10000000000000000000;
	iRawOrSnapshot=1;
	iSnapshotSpan=60;
	SetBlockType(_T( "RTDBHistSeriesBatWP" ) );
	ConstructBlockInputOutput();
}

CRTDBHistSeriesBatWP::~CRTDBHistSeriesBatWP()
{

}

//���������½�����ĺ���
CCalcBlock* CRTDBHistSeriesBatWP::CreateFromString( const CString& strType )
{
	CRTDBHistSeriesBatWP* obj = new CRTDBHistSeriesBatWP;
	if(strType !=obj->GetBlockType())//�ô����ж�����������Ƿ���4.1�����趨������һ��
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
//���ݲ�ͬ�Ķ�����й���̶����������,Ĭ�ϵ��������
void CRTDBHistSeriesBatWP::ConstructBlockInputOutput ()
{
	//AddInputPort("Enable",VALUE_DOUBLE);
	AddInputPort("StartTime",VALUE_DOUBLE);
	AddInputPort("EndTime",VALUE_DOUBLE);
}
//��ʼ������ĺ���������ĳ�ʼ��
void CRTDBHistSeriesBatWP::InitCalc()
{
	CCalcBlock::InitCalc();
	this->m_pModel->LoadRTDBDllAndCon();
}
//�������ԵĶ����ַ���
CString CRTDBHistSeriesBatWP::GetBlockStrPro()//to do
{
	CString str;
	//str.Format("%s@%s",m_strTagName,m_strTagDesp);
	return str;
}
void CRTDBHistSeriesBatWP::SetBlockStrPro(CString str)//���ü������ַ�������//to do
{
	//m_strTagName=str.Left(str.Find('@'));
	//m_strTagDesp=str.Right(str.GetLength()-str.ReverseFind('@')-1);
}
void CRTDBHistSeriesBatWP::Calc()//���м���ĺ���,��Ҫ����
{
	if(!this->m_pModel->m_bConRTDB)//���δ�����򲻽��м��㡣
		return;
	GetInputValueFromGlobalWS();
	
	double dInValue=0;
	long lSTime=0;
	long lETime=0;
	CCalcPort *pInPortObj1 = GetInputPortObj(0);
	pInPortObj1->GetPortUniValue().GetDoubleVal(&dInValue);
	lSTime=(long)dInValue;
	CCalcPort *pInPortObj2 = GetInputPortObj(1);
	pInPortObj2->GetPortUniValue().GetDoubleVal(&dInValue);
	lETime=(long)dInValue;
	if(lSTime>lETime)
	{
		long lTemp=lSTime;
		lSTime=lETime;
		lETime=lTemp;
	}
	//CTime st(lSTime);
	//CTime et(lETime);
	//CString strSTime = st.Format("%Y-%m-%d %H:%M:%S");
	//CString strETime = et.Format("%Y-%m-%d %H:%M:%S");

	
	for(int i=0;i<strVarArr.GetSize();i++)
	{
		CString strVarName=strVarArr[i];
		std::vector<TagData> tagDataArr;

		ReadHiDataRequest req;
		CCalcPort *pPortObjOut = GetOutputPortObj(i);
		CUniValue UniVar;//�������
		UniVar.SetValueType(1);//����ΪVALUE_RTVALUE_SERIES����

		if(i == 0 || i == 3)//�˴�����Ƶ�����µ����ݺͱ���Ÿ��µĵ����ݣ�д������ʱ�����д��
		{

			//��ȡʵʱ����ʷֵ
			typedef  int (*pGetRawDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData * & pTagData,long * nCount);
		    pGetRawDataByTagName m_GetRawDataByTagName =(pGetRawDataByTagName)GetProcAddress(this->m_pModel->hDLL,"GetRawDataByTagName");

			typedef int (*pRTDBFreePointer)(void *p);
			pRTDBFreePointer RTDBFreePointer=(pRTDBFreePointer)GetProcAddress(this->m_pModel->hDLL,"RTDBFreePointer");

			TagData *tagDatas =NULL;
			req.stTime =lSTime;
			req.enTime =lETime;
			req.tPeriod =0;
			req.reqType =0;	
		    strcpy(	req.pointName,strVarName);
			long nCount =0;//ÿ�εõ���ĸ���

			int nRet =m_GetRawDataByTagName(&req,tagDatas,&nCount);
			if (nRet!=0)
			{
				::Sleep(5);
				nRet =m_GetRawDataByTagName(&req,tagDatas,&nCount);
			}

			if(nRet!=0)
			{
				if (tagDatas != NULL)
				{
					RTDBFreePointer(tagDatas);
					tagDatas = NULL;
				}
			}

			//5sһ��ȡ�����������飬����realdatetime������ÿ��������
			if(nCount > 0)
			{
				int nTimeInterval = 5;
				long lDataTableLen = (req.enTime - req.stTime)/nTimeInterval;
				long lcurTime = req.stTime;
				int ncurIndex = 0;
				for (int j=0;j<lDataTableLen;j++)
				{
					ncurIndex = findTagIndex(lcurTime,tagDatas,ncurIndex,nCount);
					UniVar.AddData(tagDatas[ncurIndex].value,lcurTime,tagDatas[ncurIndex].status,tagDatas[ncurIndex].time);
					lcurTime = lcurTime + nTimeInterval;
				}
			}
			else
			{
				int nTimeInterval = 5;
				long lDataTableLen = (req.enTime - req.stTime)/nTimeInterval;
				for (int j=0;j<lDataTableLen;j++)
				{
					UniVar.AddData(0,0,-1);//����rt�д���ʽ������������ģ��ʱ����һ��
				}
			}

			if (tagDatas != NULL)
			{
				RTDBFreePointer(tagDatas);
				tagDatas = NULL;
			}
		}
		else
		{
			typedef int (*pGetHistoryDataByTime)(const char * tagName,long time, TagData *pTagData);
            pGetHistoryDataByTime m_GetHistoryDataByTime =(pGetHistoryDataByTime)GetProcAddress(this->m_pModel->hDLL,"GetHistoryDataByTime");


			int nTimeInterval = 5;
			long lDataTableLen = (req.enTime - req.stTime)/nTimeInterval;
			long lcurTime = req.stTime;
			int ncurIndex = 0;
			for (int j=0;j<lDataTableLen;j++)
			{
				TagData tagDatas;
				int nRet =m_GetHistoryDataByTime(strVarName,lcurTime,&tagDatas);
				if (nRet!=0)
				{
					::Sleep(5);
					nRet =m_GetHistoryDataByTime(strVarName,lcurTime,&tagDatas);
				}

				if(nRet == 0)
				{
					UniVar.AddData(tagDatas.value,lcurTime,tagDatas.status,tagDatas.time);
					lcurTime = lcurTime + nTimeInterval;
				}
				else
				{
					UniVar.AddData(0,0,-1);
					lcurTime = lcurTime + nTimeInterval;
				}
			}
		}

		pPortObjOut->SetPortUniValue(UniVar);
	}
	OutputResultToGlobalWS();
}

int CRTDBHistSeriesBatWP::findTagIndex(long lcurTime,TagData *tagDatas,int& ncurIndex,long nCount)
{
	bool bFind = true;
	ncurIndex = ncurIndex + 1;
	while(bFind)
	{
		int ncurIndexPre = ncurIndex -1;
		if (ncurIndexPre < 0)
		{
			ncurIndexPre = 0;
		}

		int ncurIndexNext = ncurIndex + 1;
		if (ncurIndexNext > nCount)
		{
			ncurIndexNext = nCount;
		}

		if (tagDatas[ncurIndex].time == lcurTime)
		{
			break;
		}

		//ʵ��ֵʱ���֮ǰ����
		if (lcurTime < tagDatas[ncurIndexPre].time)
		{
			ncurIndex = ncurIndexPre;
			break;
		}

		//������0��nCount�����β���
		if (lcurTime >=tagDatas[ncurIndexPre].time && lcurTime < tagDatas[ncurIndexNext].time)
		{
			ncurIndex = ncurIndexPre;
			break;
		}

		//δ�ҵ��������������һ��ֵ
		if (ncurIndex >= (nCount-1))
		{
			ncurIndex = nCount-1;
			break;
		}

		ncurIndex++;
	}

	return ncurIndex;
}

void CRTDBHistSeriesBatWP::readDataFromRTDB(
									 CString strName,
									 long lSTime,
									 long lETime,
									 int iReqType,//0:ԭʼ;1:����;
									 int isnapshotT,
									 std::vector<TagData> &tagDataArr)
{
	tagDataArr.clear();
	typedef int (*pGetRawDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData * & pTagData,long * nCount);
	pGetRawDataByTagName m_GetRawDataByTagName =(pGetRawDataByTagName)GetProcAddress(this->m_pModel->hDLL,"GetRawDataByTagName");
	
	typedef  int (*pGetSnapshotDataByTagName)(ReadHiDataRequest * pReadHiDataRequest, TagData *  pTagData);
	pGetSnapshotDataByTagName m_GetSnapshotDataByTagName =(pGetSnapshotDataByTagName)GetProcAddress(this->m_pModel->hDLL,"GetSnapshotDataByTagName");

	typedef int (*pRTDBFreePointer)(void *p);
	pRTDBFreePointer RTDBFreePointer=(pRTDBFreePointer)GetProcAddress(this->m_pModel->hDLL,"RTDBFreePointer");
	
	ReadHiDataRequest req;
	req.stTime=lSTime;
	req.enTime=lETime;
	strcpy(req.pointName,strName);
	TagData tagTemp;
	long nCount = 0;
	if(iReqType==1)//����
	{
		req.tPeriod=isnapshotT;
		req.reqType=4;
		
		if ((req.enTime - req.stTime)%req.tPeriod){	nCount = (req.enTime - req.stTime)/req.tPeriod+2;} 
		else{	nCount = (req.enTime - req.stTime)/req.tPeriod+1;}
		TagData *tagDatas = new TagData[nCount];
		memset(tagDatas,0,sizeof(TagData)*nCount);
		int nRet = m_GetSnapshotDataByTagName(&req, tagDatas);
		if(nRet)
		{
			if(tagDatas!=NULL)
			{
				delete[] tagDatas;
				tagDatas=NULL;
			}
			return;
		}
		for(int i=0;i<nCount;i++)
		{
			if((tagDatas[i].value<dLimitValue)&&(tagDatas[i].value>-1*dLimitValue)&&((tagDatas[i].status==0)||(tagDatas[i].status==2)))//״̬Ϊ��ȷ
			{
				tagTemp.value=tagDatas[i].value;
				tagTemp.time=tagDatas[i].time;
				tagTemp.status=tagDatas[i].status;
				tagDataArr.push_back(tagTemp);
			}
		}
		if(tagDatas!=NULL)
		{
			delete[] tagDatas;
			tagDatas=NULL;
		}
	}
	else if(iReqType==0)//ԭʼ
	{
		req.tPeriod=0;
		req.reqType=0;
		TagData *tagDatas=NULL;
		int nRet=0;
		do
		{			
			nRet = m_GetRawDataByTagName(&req,tagDatas,&nCount);
			if(nRet)
			{
				RTDBFreePointer(tagDatas);
				return;
			}
			else
			{
				for (int j=0; j<nCount; j++)
				{
					if((tagDatas[j].value<dLimitValue)&&(tagDatas[j].value>-1*dLimitValue)&&((tagDatas[j].status==0)||(tagDatas[j].status==2)))//״̬Ϊ��ȷ
					{
						tagTemp.value=tagDatas[j].value;
						tagTemp.time=tagDatas[j].time;
						tagTemp.status=tagDatas[j].status;
						tagDataArr.push_back(tagTemp);
					}
				}
				if(nCount>0)
				{
					req.stTime=tagTemp.time+1;	//�޸Ĳ�ѯ��ʼʱ��,�����һ����Ҫƫ��1�룬��ֹ�ظ���
				}
			}
			RTDBFreePointer(tagDatas);
		}while ((nCount==1024) && (req.stTime !=req.enTime));
	}
}
//���ڸ��ݲ������ֵ�������Ե����ã�����ȡֵҪ��
void CRTDBHistSeriesBatWP::SetPropValue(CString strPropName,CString strItem1,CString strItem2,CString strItem3,CString strItem4,CString strItem5)
{
	if(strPropName.Compare("readfun")==0)
	{
		if(!strItem1.IsEmpty())  {iRawOrSnapshot=atoi(strItem1);}
		if(!strItem2.IsEmpty())  {iSnapshotSpan=atoi(strItem2);}
	}
	else
	{
		int propType,propIndex;
		getPropTypeByName(strPropName,propType,propIndex);
		if(propType==1)
		{
			if(!strItem1.IsEmpty())  {strVarArr.SetAtGrow(propIndex,strItem1);}
			if(!strItem2.IsEmpty())  {strPortArr.SetAtGrow(propIndex,strItem2);}
			if(!strItem3.IsEmpty())  {strTagDespArr.SetAtGrow(propIndex,strItem3);}
			if(!strItem4.IsEmpty())  {strUnitArr.SetAtGrow(propIndex,strItem4);}
		}
	}
}
void CRTDBHistSeriesBatWP::getPropTypeByName(CString strPropName,int &propType,int &propIndex)
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
void CRTDBHistSeriesBatWP::ShowPropertyDialog( CWnd* parent, BOOL show )
{
	PropertyDlgRTDBHistSeriesBatWP dlg;

	int portnumLinkBehind=this->GetOutputNum();
	CStringArray  IDArrRightOut;//�����ԭ�е�����˿�����
	CStringArray  IDArrRightIn;//������ڲ�������˿�����

	for(int r=0; r<portnumLinkBehind;r++)
	{
		CString strPortID;
		strPortID=this->GetOutputPortObj(r)->GetPortID();
		IDArrRightOut.Add(strPortID);
	}

	dlg.strVarArr.Copy(strVarArr);
	dlg.strPortArr.Copy(strPortArr);
	dlg.strTagDespArr.Copy(strTagDespArr);
	dlg.strUnitArr.Copy(strUnitArr);
	dlg.iRawOrSnapshot=iRawOrSnapshot;
	dlg.iSnapshotSpan=iSnapshotSpan;
	if(dlg.DoModal()==IDOK)
	{
		strVarArr.Copy(dlg.strVarArr);
		strPortArr.Copy(dlg.strPortArr);
		strTagDespArr.Copy(dlg.strTagDespArr);
		strUnitArr.Copy(dlg.strUnitArr);
		iRawOrSnapshot=dlg.iRawOrSnapshot;
		iSnapshotSpan=dlg.iSnapshotSpan;
		
		//������˿ڵ��޸�
		for(int i=0;i<strPortArr.GetSize();i++)
		{
			IDArrRightIn.Add(strPortArr[i]);
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
				InSertOutputPortBefore(mm,str, VALUE_RTVALUE_SERIES, _T(str));
			}
		}
		parent->RedrawWindow();
	}

}
//�����Խ���ģ��д�뵽���ݿ���
void CRTDBHistSeriesBatWP::PutPropertyToRecord(_RecordsetPtr& pRecord)
{
	CString strFieldValue;
	CString strField;
	for(int i=0;i<strPortArr.GetSize();i++)//���
	{
		CString strInPut=strPortArr[i];
		if(strInPut=="")
			break;
		pRecord->AddNew();
		pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
		strFieldValue.Format("input.%d",i);
		pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������

		strFieldValue.Format("%s",strVarArr[i]); //������
		pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",strPortArr[i]); //������
		pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",strTagDespArr[i]); //������
		pRecord->PutCollect(_T("value_item3"),_variant_t(strFieldValue) ); //������	
		strFieldValue.Format("%s",strUnitArr[i]); //������
		pRecord->PutCollect(_T("value_item4"),_variant_t(strFieldValue) ); //������
	}
	pRecord->AddNew();
	pRecord->PutCollect(_T("block_name"),_variant_t(GetBlockName()) ); //��Name
	strFieldValue.Format("readfun",i);
	pRecord->PutCollect(_T("prop_name"),_variant_t(strFieldValue) ); //������
	
	strFieldValue.Format("%d",iRawOrSnapshot); //������
	pRecord->PutCollect(_T("value_item1"),_variant_t(strFieldValue) ); //������	
	strFieldValue.Format("%d",iSnapshotSpan); //������
	pRecord->PutCollect(_T("value_item2"),_variant_t(strFieldValue) ); //������	
	pRecord->Update();
}
//����ͼ��ʱ����Ҫ�������ԡ�
void CRTDBHistSeriesBatWP::CopyPropertyData(CCalcBlock* pBlock)
{
	this->strVarArr.Copy(((CRTDBHistSeriesBatWP*)pBlock)->strVarArr);
	this->strPortArr.Copy(((CRTDBHistSeriesBatWP*)pBlock)->strPortArr);
	this->strTagDespArr.Copy(((CRTDBHistSeriesBatWP*)pBlock)->strTagDespArr);
	this->strUnitArr.Copy(((CRTDBHistSeriesBatWP*)pBlock)->strUnitArr);

	this->iRawOrSnapshot=((CRTDBHistSeriesBatWP*)pBlock)->iRawOrSnapshot;//0:ԭʼ��1:����
	this->iSnapshotSpan=((CRTDBHistSeriesBatWP*)pBlock)->iSnapshotSpan;//���ռ��
}
//�����Է��ô���������
void CRTDBHistSeriesBatWP::SetBlockPropertyToTransferData(
			CArray<CBlockPropertyTransferData*,CBlockPropertyTransferData*&> &dataArr)
{
}
