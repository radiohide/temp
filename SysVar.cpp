// SysVar.cpp: implementation of the SysVar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "SysVar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SysVar::SysVar()
{

}

SysVar::~SysVar()
{

}
int SysVar::UpDateDB()
{
	/*
	_ConnectionPtr m_pCon;
	_RecordsetPtr m_pRec;
	int unitIndex;
	CString tableName,strSQL;
	CEO3MinnerDoc* pDoc = theApp.m_pDoc;
		
	pDoc->GetRDBConnectPtr(m_pCon);
	unitIndex=pDoc->GetUnitIndex();
	int sets=pDoc->GetSetsIndex();
	tableName.Format("t_u%ds%d_sysvars",unitIndex,sets);
	
	_variant_t RecordsAffected;
	
	try
	{
		strSQL.Format("UPDATE `%s`SET category_endpoint='%f',fixed_interval='%f' ,relative_interval='%f',cluster_source='%s' WHERE var_name='%s'",//table
			tableName,category_endpoint,fixed_interval,relative_interval,cluster_source,Name);
		m_pCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
	}
	catch(_com_error  *e)   
	{   
		AfxMessageBox(e->ErrorMessage());
		m_pCon->Close();
		return 0;
	}
	m_pCon->Close();
	*/
	return 1;
	
}
int SysVar::GetSelfDisInterval(int iLeftNum,//����ɢ������
							   int iRightNum,//����ɢ������
							   CArray<int,int> &disValueArr,
							   CArray<double,double> &sValueArr,
							   CArray<double,double> &eValueArr)
{
	/*
	if(this->cluster_source=="ABS_STEP")//ABS_STEP:���Լ��
	{
		return GetDisInterval(cluster_source,category_endpoint,fixed_interval,
			iLeftNum,iRightNum,disValueArr,sValueArr,eValueArr);
	}
	else if(cluster_source=="REL_STEP")//REL_STEP:��Լ��
	{
		return GetDisInterval(cluster_source,category_endpoint,relative_interval,
			iLeftNum,iRightNum,disValueArr,sValueArr,eValueArr);
	}
	else 
	*/
		return 0;
}

int SysVar::GetSelfDisInterval(double dSValue,//��ɢ����Χ�Ŀ�ʼ
							   double dEValue,//��ɢ����Χ�Ľ���
							   CArray<int,int> &disValueArr,
							   CArray<double,double> &sValueArr,
							   CArray<double,double> &eValueArr)
{
	/*
	if(this->cluster_source=="ABS_STEP")//ABS_STEP:���Լ��
	{
		return GetDisInterval(cluster_source,category_endpoint,fixed_interval,
			dSValue,dEValue,disValueArr,sValueArr,eValueArr);
	}
	else if(cluster_source=="REL_STEP")//REL_STEP:��Լ��
	{
		return GetDisInterval(cluster_source,category_endpoint,relative_interval,
			dSValue,dEValue,disValueArr,sValueArr,eValueArr);
	}
	else 
	*/
		return 0;
}

int SysVar::GetDisInterval(	CString strComFun, 
						   double dPointValue,
						   double dStepValue,	
						   int iLeftNum,
						   int iRightNum,
						   CArray<int,int> &disValueArr,
						   CArray<double,double> &sValueArr,
						   CArray<double,double> &eValueArr)
{
	/*
	if(strComFun=="TABLE")//TABLE:�������ݿ��
	{
		return 0;
	}
	else if(strComFun=="ABS_STEP")//ABS_STEP:���Լ��
	{
		for(int i=-iLeftNum;i<=-1;i++)
		{
			//[a1+(-N)*d��a1+(-N+1)*d);
			int N=i;
			double sValue=dPointValue+(N)*dStepValue;
			double eValue=dPointValue+(N+1)*dStepValue;
			disValueArr.Add(N);
			sValueArr.Add(sValue);
			eValueArr.Add(eValue);
		}
		for(i=1;i<=iRightNum;i++)
		{
			//[a1+(N-1)*d��a1+N*d)
			int N=i;
			double sValue=dPointValue+(N-1)*dStepValue;
			double eValue=dPointValue+N*dStepValue;
			disValueArr.Add(N);
			sValueArr.Add(sValue);
			eValueArr.Add(eValue);
		}
	}
	else if(strComFun=="REL_STEP")//REL_STEP:��Լ��
	{
		for(int i=-iLeftNum;i<=-1;i++)
		{
			int N=i;
			double sValue=dPointValue*(2-pow(1+dStepValue,(-1)*N));
			double eValue=dPointValue*(2-pow(1+dStepValue,(-1)*(N+1)));
			disValueArr.Add(N);
			sValueArr.Add(sValue);
			eValueArr.Add(eValue);
		}
		for(i=1;i<=iRightNum;i++)
		{
			int N=i;
			double sValue=dPointValue*pow(1+dStepValue,N-1);
			double eValue=dPointValue*pow(1+dStepValue,N);
			disValueArr.Add(N);
			sValueArr.Add(sValue);
			eValueArr.Add(eValue);
		}
	}
	*/
	return 1;
}

int SysVar::GetDisInterval(	CString strComFun, 
						   double dPointValue,
						   double dStepValue,	
						   double dSValue,//��ɢ����Χ�Ŀ�ʼ
						   double dEValue,//��ɢ����Χ�Ľ���
						   CArray<int,int> &disValueArr,
						   CArray<double,double> &sValueArr,
						   CArray<double,double> &eValueArr)
{
/*
	int inDivMinN,iDivMaxN;
	int tagType=-1;
	if(dSValue>dEValue)//����������յ��򽻻�
	{
		double dtemp=dSValue;
		dSValue=dEValue;
		dEValue=dtemp;
	}
	if(strComFun=="TABLE")//TABLE:�������ݿ��
	{
		return 0;
	}
	else if(strComFun=="ABS_STEP")//ABS_STEP:���Լ��
	{
		if(dSValue>=dPointValue)//��������ڶ˵�
		{
			inDivMinN=floor((dSValue-dPointValue)/dStepValue+1);
			iDivMaxN=floor((dEValue-dPointValue)/dStepValue+1);
			tagType=2;//������
		}
		else if((dSValue<dPointValue)&&(dEValue>=dPointValue))//��������֮��
		{
			inDivMinN=(-1)*ceil((dPointValue-dSValue)/dStepValue);
			iDivMaxN=floor((dEValue-dPointValue)/dStepValue+1);
			tagType=1;//��������֮��
		}
		else if(dEValue<dPointValue)//�����С�ڶ˵�
		{
			inDivMinN=(-1)*ceil((dPointValue-dSValue)/dStepValue);
			iDivMaxN=(-1)*ceil((dPointValue-dEValue)/dStepValue);
			tagType=0;//�����С�ڶ˵�
		}
		if(iDivMaxN-inDivMinN>1000)
			return -1;//��������1000;
		if(tagType==2)
		{
			for(int i=inDivMinN;i<=iDivMaxN;i++)
			{
				//[a1+(N-1)*d��a1+N*d)
				int N=i;
				double sValue=dPointValue+(N-1)*dStepValue;
				double eValue=dPointValue+N*dStepValue;
				disValueArr.Add(N);
				sValueArr.Add(sValue);
				eValueArr.Add(eValue);
			}
		}
		else if(tagType==1)
		{
			for(int i=inDivMinN;i<=-1;i++)
			{
				//[a1+(-N)*d��a1+(-N+1)*d);
				int N=i;
				double sValue=dPointValue+(N)*dStepValue;
				double eValue=dPointValue+(N+1)*dStepValue;
				disValueArr.Add(N);
				sValueArr.Add(sValue);
				eValueArr.Add(eValue);
			}
			for(i=1;i<=iDivMaxN;i++)
			{
				//[a1+(N-1)*d��a1+N*d)
				int N=i;
				double sValue=dPointValue+(N-1)*dStepValue;
				double eValue=dPointValue+N*dStepValue;
				disValueArr.Add(N);
				sValueArr.Add(sValue);
				eValueArr.Add(eValue);
			}
		}
		else if(tagType==0)
		{
			for(int i=inDivMinN;i<=iDivMaxN;i++)
			{
				//[a1+(-N)*d��a1+(-N+1)*d);
				int N=i;
				double sValue=dPointValue+(N)*dStepValue;
				double eValue=dPointValue+(N+1)*dStepValue;
				disValueArr.Add(N);
				sValueArr.Add(sValue);
				eValueArr.Add(eValue);
			}
		}
	}
	else if(strComFun=="REL_STEP")//REL_STEP:��Լ��
	{
		if(dSValue>=dPointValue)//��������ڶ˵�
		{
			inDivMinN=floor(1+log(dEValue/dPointValue)/log(1+dStepValue));
			iDivMaxN=floor(1+log(dSValue/dPointValue)/log(1+dStepValue));
			tagType=2;//������
		}
		else if((dSValue<dPointValue)&&(dEValue>=dPointValue))//��������֮��
		{
			inDivMinN=ceil((-1)*(log(2-dSValue/dPointValue)/log(1+dStepValue))-1);
			iDivMaxN=floor(1+log(dEValue/dPointValue)/log(1+dStepValue));
			tagType=1;//��������֮��
		}
		else if(dEValue<dPointValue)//�����С�ڶ˵�
		{
			inDivMinN=ceil((-1)*(log(2-dSValue/dPointValue)/log(1+dStepValue))-1);
			iDivMaxN=ceil((-1)*(log(2-dEValue/dPointValue)/log(1+dStepValue))-1);
			tagType=0;//�����С�ڶ˵�
		}
		if(iDivMaxN-inDivMinN>1000)
			return -1;//��������1000;
		if(tagType==2)
		{	
			for(int i=inDivMinN;i<=iDivMaxN;i++)
			{
				int N=i;
				double sValue=dPointValue*pow(1+dStepValue,N-1);
				double eValue=dPointValue*pow(1+dStepValue,N);
				disValueArr.Add(N);
				sValueArr.Add(sValue);
				eValueArr.Add(eValue);
			}
		}
		else if(tagType==1)
		{
			for(int i=inDivMinN;i<=-1;i++)
			{
				int N=i;
				double sValue=dPointValue*(2-pow(1+dStepValue,(-1)*N));
				double eValue=dPointValue*(2-pow(1+dStepValue,(-1)*(N+1)));
				disValueArr.Add(N);
				sValueArr.Add(sValue);
				eValueArr.Add(eValue);
			}
			for(i=1;i<=iDivMaxN;i++)
			{
				int N=i;
				double sValue=dPointValue*pow(1+dStepValue,N-1);
				double eValue=dPointValue*pow(1+dStepValue,N);
				disValueArr.Add(N);
				sValueArr.Add(sValue);
				eValueArr.Add(eValue);
			}
		}
		else if(tagType==0)
		{
			for(int i=inDivMinN;i<=iDivMaxN;i++)
			{
				int N=i;
				double sValue=dPointValue*(2-pow(1+dStepValue,(-1)*N));
				double eValue=dPointValue*(2-pow(1+dStepValue,(-1)*(N+1)));
				disValueArr.Add(N);
				sValueArr.Add(sValue);
				eValueArr.Add(eValue);
			}
		}
	}
	*/
	return 1;
}