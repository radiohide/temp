// wMnStmCalcBlock.cpp: implementation of the CwMnStmCalcBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ePerfManage.h"
#include "wMnStmCalcBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CwMnStmCalcBlock::CwMnStmCalcBlock()
{
	SetBlockType(_T( "CwMainStream" ) );//设置类型
	m_VarArr.RemoveAll();
	for(int i=0;i<maxFormula;i++)
		m_VarArr.Add("");
	m_FormulaArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		m_FormulaArr.Add("");
	m_TypeArr.RemoveAll();
	for(i=0;i<maxFormula;i++)
		m_TypeArr.Add(0);
	ConstructBlockInputOutput ();

}

CwMnStmCalcBlock::~CwMnStmCalcBlock()
{
	
}

//根据不同的对象进行构造固定的输入输出,默认的输入输出
void CwMnStmCalcBlock::ConstructBlockInputOutput ()
{
	//输入
	inPutArr[0]="D0r";inPutTypeArr[0]=0;inPutCanBeEdit[0]=0;
	inPutArr[1]="Sigma";inPutTypeArr[1]=0;inPutCanBeEdit[1]=0;

	inPutArr[2]="hW1";inPutTypeArr[2]=0;inPutCanBeEdit[2]=0;
	inPutArr[3]="hW2";inPutTypeArr[3]=0;inPutCanBeEdit[3]=0;
	inPutArr[4]="hW3";inPutTypeArr[4]=0;inPutCanBeEdit[4]=0;
	inPutArr[5]="hW4";inPutTypeArr[5]=0;inPutCanBeEdit[5]=0;
	inPutArr[6]="hW5";inPutTypeArr[6]=0;inPutCanBeEdit[6]=0;
	inPutArr[7]="hW6";inPutTypeArr[7]=0;inPutCanBeEdit[7]=0;
	inPutArr[8]="hW7";inPutTypeArr[8]=0;inPutCanBeEdit[8]=0;
	inPutArr[9]="hW8";inPutTypeArr[9]=0;inPutCanBeEdit[9]=0;

	inPutArr[10]="wAuxLoss";inPutTypeArr[10]=0;inPutCanBeEdit[10]=0;
	inPutArr[11]="pwGen";inPutTypeArr[11]=0;inPutCanBeEdit[11]=0;
	inPutArr[12]="effjx";inPutTypeArr[12]=0;inPutCanBeEdit[12]=0;
	inPutArr[13]="effGen";inPutTypeArr[13]=0;inPutCanBeEdit[13]=0;
	inPutArr[14]="hLPTExtrStm";inPutTypeArr[14]=0;inPutCanBeEdit[14]=0;
	inPutArr[15]="h0";inPutTypeArr[15]=0;inPutCanBeEdit[15]=0;

	//输出
	outPutArr[0]="D0";outPutTypeArr[0]=0;
	outPutArr[1]="D1";outPutTypeArr[1]=0;
	outPutArr[2]="D2";outPutTypeArr[2]=0;
	outPutArr[3]="D3";outPutTypeArr[3]=0;
	outPutArr[4]="D4";outPutTypeArr[4]=0;
	outPutArr[5]="D5";outPutTypeArr[5]=0;
	outPutArr[6]="D6";outPutTypeArr[6]=0;
	outPutArr[7]="D7";outPutTypeArr[7]=0;
	outPutArr[8]="D8";outPutTypeArr[8]=0;

	//循环前
	IterFVarArr[0]="D0";IterFFormulaArr[0]="[D0r]";IterFVarTypeArr[0]=0;

	//循环条件
	conVarArr[0]="con1";conFormulaArr[0]="ABS([D0]-[D0r])<0.001";conVarTypeArr[0]=0;

	//迭代内容
	IterVarArr[0]="D0r";IterFormulaArr[0]="[D0]";IterVarTypeArr[0]=0;
	//迭代内容//A矩阵
	IterVarArr[1]="A11";IterFormulaArr[1]="";IterVarTypeArr[0]=0;
	IterVarArr[2]="A21";IterFormulaArr[2]="";IterVarTypeArr[1]=0;
	IterVarArr[3]="A22";IterFormulaArr[3]="";IterVarTypeArr[2]=0;
	IterVarArr[4]="A31";IterFormulaArr[4]="";IterVarTypeArr[3]=0;
	IterVarArr[5]="A32";IterFormulaArr[5]="";IterVarTypeArr[4]=0;
	IterVarArr[6]="A33";IterFormulaArr[6]="";IterVarTypeArr[5]=0;
	IterVarArr[7]="A41";IterFormulaArr[7]="";IterVarTypeArr[6]=0;
	IterVarArr[8]="A42";IterFormulaArr[8]="";IterVarTypeArr[7]=0;
	IterVarArr[9]="A43";IterFormulaArr[9]="";IterVarTypeArr[8]=0;
	IterVarArr[10]="A44";IterFormulaArr[10]="";IterVarTypeArr[10]=0;
	IterVarArr[11]="A51";IterFormulaArr[11]="";IterVarTypeArr[11]=0;
	IterVarArr[12]="A52";IterFormulaArr[12]="";IterVarTypeArr[12]=0;
	IterVarArr[13]="A53";IterFormulaArr[13]="";IterVarTypeArr[13]=0;
	IterVarArr[14]="A54";IterFormulaArr[14]="";IterVarTypeArr[14]=0;
	IterVarArr[15]="A55";IterFormulaArr[15]="";IterVarTypeArr[15]=0;
	IterVarArr[16]="A61";IterFormulaArr[16]="";IterVarTypeArr[16]=0;
	IterVarArr[17]="A62";IterFormulaArr[17]="";IterVarTypeArr[17]=0;
	IterVarArr[18]="A63";IterFormulaArr[18]="";IterVarTypeArr[18]=0;
	IterVarArr[19]="A64";IterFormulaArr[19]="";IterVarTypeArr[19]=0;
	IterVarArr[20]="A65";IterFormulaArr[20]="";IterVarTypeArr[20]=0;
	IterVarArr[21]="A66";IterFormulaArr[21]="";IterVarTypeArr[21]=0;
	IterVarArr[22]="A71";IterFormulaArr[22]="";IterVarTypeArr[22]=0;
	IterVarArr[23]="A72";IterFormulaArr[23]="";IterVarTypeArr[23]=0;
	IterVarArr[24]="A73";IterFormulaArr[24]="";IterVarTypeArr[24]=0;
	IterVarArr[25]="A74";IterFormulaArr[25]="";IterVarTypeArr[25]=0;
	IterVarArr[26]="A75";IterFormulaArr[26]="";IterVarTypeArr[26]=0;
	IterVarArr[27]="A76";IterFormulaArr[27]="";IterVarTypeArr[27]=0;
	IterVarArr[28]="A77";IterFormulaArr[28]="";IterVarTypeArr[28]=0;
	IterVarArr[29]="A81";IterFormulaArr[29]="";IterVarTypeArr[29]=0;
	IterVarArr[30]="A82";IterFormulaArr[30]="";IterVarTypeArr[30]=0;
	IterVarArr[31]="A83";IterFormulaArr[31]="";IterVarTypeArr[31]=0;
	IterVarArr[32]="A84";IterFormulaArr[32]="";IterVarTypeArr[32]=0;
	IterVarArr[33]="A85";IterFormulaArr[33]="";IterVarTypeArr[33]=0;
	IterVarArr[34]="A86";IterFormulaArr[34]="";IterVarTypeArr[34]=0;
	IterVarArr[35]="A87";IterFormulaArr[35]="";IterVarTypeArr[35]=0;
	IterVarArr[36]="A88";IterFormulaArr[36]="";IterVarTypeArr[36]=0;

	//迭代内容//WAuxi
	IterVarArr[37]="WAux1";IterFormulaArr[37]="";IterVarTypeArr[37]=0;
	IterVarArr[38]="WAux2";IterFormulaArr[38]="";IterVarTypeArr[38]=0;
	IterVarArr[39]="WAux3";IterFormulaArr[39]="";IterVarTypeArr[39]=0;
	IterVarArr[40]="WAux4";IterFormulaArr[40]="";IterVarTypeArr[40]=0;
	IterVarArr[41]="WAux5";IterFormulaArr[41]="";IterVarTypeArr[41]=0;
	IterVarArr[42]="WAux6";IterFormulaArr[42]="";IterVarTypeArr[42]=0;
	IterVarArr[43]="WAux7";IterFormulaArr[43]="";IterVarTypeArr[43]=0;
	IterVarArr[44]="WAux8";IterFormulaArr[44]="";IterVarTypeArr[44]=0;

	//迭代内容//QAuxi
	IterVarArr[45]="QAux1";IterFormulaArr[45]="";IterVarTypeArr[45]=0;
	IterVarArr[46]="QAux2";IterFormulaArr[46]="";IterVarTypeArr[46]=0;
	IterVarArr[47]="QAux3";IterFormulaArr[47]="";IterVarTypeArr[47]=0;
	IterVarArr[48]="QAux4";IterFormulaArr[48]="";IterVarTypeArr[48]=0;
	IterVarArr[49]="QAux5";IterFormulaArr[49]="";IterVarTypeArr[49]=0;
	IterVarArr[50]="QAux6";IterFormulaArr[50]="";IterVarTypeArr[50]=0;
	IterVarArr[51]="QAux7";IterFormulaArr[51]="";IterVarTypeArr[51]=0;
	IterVarArr[52]="QAux8";IterFormulaArr[52]="";IterVarTypeArr[52]=0;

	//迭代内容//Qwfi
	IterVarArr[53]="Qwf1";IterFormulaArr[53]="";IterVarTypeArr[53]=0;
	IterVarArr[54]="Qwf2";IterFormulaArr[54]="";IterVarTypeArr[54]=0;
	IterVarArr[55]="Qwf3";IterFormulaArr[55]="";IterVarTypeArr[55]=0;
	IterVarArr[56]="Qwf4";IterFormulaArr[56]="";IterVarTypeArr[56]=0;
	IterVarArr[57]="Qwf5";IterFormulaArr[57]="";IterVarTypeArr[57]=0;
	IterVarArr[58]="Qwf6";IterFormulaArr[58]="";IterVarTypeArr[58]=0;
	IterVarArr[59]="Qwf7";IterFormulaArr[59]="";IterVarTypeArr[59]=0;
	IterVarArr[60]="Qwf8";IterFormulaArr[60]="";IterVarTypeArr[60]=0;

	//迭代内容//Qfqi
	IterVarArr[61]="Qfq1";IterFormulaArr[61]="";IterVarTypeArr[61]=0;
	IterVarArr[62]="Qfq2";IterFormulaArr[62]="";IterVarTypeArr[62]=0;
	IterVarArr[63]="Qfq3";IterFormulaArr[63]="";IterVarTypeArr[63]=0;
	IterVarArr[64]="Qfq4";IterFormulaArr[64]="";IterVarTypeArr[64]=0;
	IterVarArr[65]="Qfq5";IterFormulaArr[65]="";IterVarTypeArr[65]=0;
	IterVarArr[66]="Qfq6";IterFormulaArr[66]="";IterVarTypeArr[66]=0;
	IterVarArr[67]="Qfq7";IterFormulaArr[67]="";IterVarTypeArr[67]=0;
	IterVarArr[68]="Qfq8";IterFormulaArr[68]="";IterVarTypeArr[68]=0;

	//迭代内容//Qti
	IterVarArr[69]="Qt1";IterFormulaArr[69]="";IterVarTypeArr[69]=0;
	IterVarArr[70]="Qt2";IterFormulaArr[70]="";IterVarTypeArr[70]=0;
	IterVarArr[71]="Qt3";IterFormulaArr[71]="";IterVarTypeArr[71]=0;
	IterVarArr[72]="Qt4";IterFormulaArr[72]="";IterVarTypeArr[72]=0;
	IterVarArr[73]="Qt5";IterFormulaArr[73]="";IterVarTypeArr[73]=0;
	IterVarArr[74]="Qt6";IterFormulaArr[74]="";IterVarTypeArr[74]=0;
	IterVarArr[75]="Qt7";IterFormulaArr[75]="";IterVarTypeArr[75]=0;
	IterVarArr[76]="Qt8";IterFormulaArr[76]="";IterVarTypeArr[76]=0;

	//计算D1-D8
	
	m_VarArr[0]="D1";m_TypeArr[0]=0;
	m_FormulaArr[0]="([Qt1]-[Qfq1]-[Qwf1]-[QAux1])/[A11]-[WAux1]";

	m_VarArr[1]="D2";m_TypeArr[1]=0;
	m_FormulaArr[1]="([Qt2]-[Qfq2]-[Qwf2]-[QAux2]-A21*([D1]+[WAux1]))/[A22]-[WAux2]";

	m_VarArr[2]="D3";m_TypeArr[2]=0;
	m_FormulaArr[2]="([Qt3]-[Qfq3]-[Qwf3]-[QAux3]-A31*([D1]+[WAux1])-A32*([D2]+[WAux2]))/[A33]-[WAux3]";

	m_VarArr[3]="D4";m_TypeArr[3]=0;
	m_FormulaArr[3]="([Qt4]-[Qfq4]-[Qwf4]-[QAux4]-A41*([D1]+[WAux1])-A42*([D2]+[WAux2])-A43*([D3]+[WAux3]))/[A44]-[WAux4]";

	m_VarArr[4]="D5";m_TypeArr[4]=0;
	m_FormulaArr[4]="([Qt5]-[Qfq5]-[Qwf5]-[QAux5]-A51*([D1]+[WAux1])-A52*([D2]+[WAux2])-A53*([D3]+[WAux3])-A54*([D4]+[WAux4]))/[A55]-[WAux5]";

	m_VarArr[5]="D6";m_TypeArr[5]=0;
	m_FormulaArr[5]="([Qt6]-[Qfq6]-[Qwf6]-[QAux6]-A61*([D1]+[WAux1])-A62*([D2]+[WAux2])-A63*([D3]+[WAux3])-A64*([D4]+[WAux4])-A65*([D5]+[WAux5]))/[A66]-[WAux6]";

	m_VarArr[6]="D7";m_TypeArr[6]=0;
	m_FormulaArr[6]="([Qt7]-[Qfq7]-[Qwf7]-[QAux7]-A71*([D1]+[WAux1])-A72*([D2]+[WAux2])-A73*([D3]+[WAux3])-A74*([D4]+[WAux4])-A75*([D5]+[WAux5])-A76*([D6]+[WAux6]))/[A77]-[WAux7]";

	m_VarArr[7]="D8";m_TypeArr[7]=0;
	m_FormulaArr[7]="([Qt8]-[Qfq8]-[Qwf8]-[QAux8]-A81*([D1]+[WAux1])-A82*([D2]+[WAux2])-A83*([D3]+[WAux3])-A84*([D4]+[WAux4])-A85*([D5]+[WAux5])-A86*([D6]+[WAux6])-A87*([D7]+[WAux7]))/[A88]-[WAux8]";

	m_VarArr[8]="D0";m_TypeArr[8]=0;
	m_FormulaArr[8]="[pwGen]/(([effjx]*[effGen])/10000)/([h0]+[Sigma]-[hLPTExtrStm]-(([D1]/[D0r])*[hW1]+([D2]/[D0r])*[hW2]+([D3]/[D0r])*[hW3]+([D4]/[D0r])*[hW4]+([D5]/[D0r])*[hW5]+([D6]/[D0r])*[hW6]+([D7]/[D0r])*[hW7]+([D8]/[D0r])*[hW8])-[wAuxLoss])";

	//端口
	ClearInputPorts();
	ClearOutputPorts();
	for(int i=0;i<inPutArr.GetSize();i++)
	{
		if(inPutArr[i]=="")
			break;
		CString str;
		str.Format("in%d",i+1);
		AddInputPort(inPutArr[i], inPutTypeArr[i],str);
	}
	for(i=0;i<outPutArr.GetSize();i++)
	{
		if(outPutArr[i]=="")
			break;
		CString str;
		str.Format("out%d",i+1);
		AddOutputPort(outPutArr[i], outPutTypeArr[i], str);
	}
}
//根据类型新建对象的函数
CCalcBlock* CwMnStmCalcBlock::CreateFromString( const CString& strType )
{
	CwMnStmCalcBlock* obj = new CwMnStmCalcBlock;
	if(strType !=obj->GetBlockType())//该处在判断自身的类型是否与4.1步骤设定的类型一致
	{
		delete obj;
		obj = NULL;
	}
	return obj;
}
void CwMnStmCalcBlock::buildInsideEle()
{
	CString strVarName;
	CElementInOut *pElement =NULL;
	for(int i=0;i<this->m_VarArr.GetSize();i++)//计算点，迭代变量
	{
		strVarName=m_VarArr[i];
		if(strVarName=="")
			break;
		pElement=new CElementInOut(strVarName,m_TypeArr[i],m_FormulaArr[i]+"@",this);
		block_WorkSpaceList.SetAt(strVarName,pElement);
	}
}
//初始化计算的函数做特殊的初始化
void CwMnStmCalcBlock::InitCalc()
{
	CCalcBlock::InitCalc();
	DelInnerWorkSpace();
	BulidInPutEle();
	BulidConVarEle();
	BulidIterEle();
	BulidIterBEle();
	buildInsideEle();//加入内部变量构建元素
	BulidIterFEle();//要在最后创建，其要给block_WorkSpaceList补充元素
	ElementParseFormula();

}
void CwMnStmCalcBlock::CalcIter() //进行迭代
{
	for(int i=0;i<IterVarArr.GetSize();i++)
	{
		if(IterVarArr[i]=="")
			break;
		CElementInOut *pElement=block_WorkSpaceList[IterVarArr[i]];
		if(pElement!=NULL)
		{
			pElement->ReSet();//复位，使之未计算
			pElement->CalcFormula();
		}
	}
	for(i=0;i<m_VarArr.GetSize();i++)
	{
		if(m_VarArr[i]=="")
			break;
		CElementInOut *pElement=block_WorkSpaceList[m_VarArr[i]];
		if(pElement!=NULL)
		{
			pElement->ReSet();//复位，使之未计算
			pElement->CalcFormula();
		}
	}
}
