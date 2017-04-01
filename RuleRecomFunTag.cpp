// RuleRecomFunTag.cpp: implementation of the RuleRecomFunTag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eperfmanage.h"
#include "RuleRecomFunTag.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RuleRecomFunTag::RuleRecomFunTag()
{
	CGroup_ID=0;
	Expression="";
	Desp="";

	ConstraintsExp="";
	TargetFunExp="";
	ListID=0;
}

RuleRecomFunTag::~RuleRecomFunTag()
{
	ParaItemArr.RemoveAll();
}
void RuleRecomFunTag::CopySelf(RuleRecomFunTag *OriRuleFun)
{
	this->CGroup_ID=OriRuleFun->CGroup_ID;
	this->ConstraintsExp=OriRuleFun->ConstraintsExp;
	this->Desp=OriRuleFun->Desp;
	this->Expression=OriRuleFun->Desp;
	this->ParaItemArr.Copy(OriRuleFun->ParaItemArr);
	this->TargetFunExp=OriRuleFun->TargetFunExp;
}
void RuleRecomFunTag::AnalyExp()//扩展解析方法，
{
	ConstraintsExp="";
	TargetFunExp="";
	ParaItemArr.RemoveAll();

	ConstraintsExp=Expression.Left(Expression.Find('('));
	if((ConstraintsExp=="Min")||(ConstraintsExp=="Max"))
	{
		CString itemStr=Expression.Mid(Expression.ReverseFind(',')+1,Expression.ReverseFind(')')-Expression.ReverseFind(',')-1);
		TargetFunExp=Expression.Mid(Expression.Find('(')+1,Expression.ReverseFind(',')-Expression.Find('(')-1);
		ParaItemArr.Add(::atof(itemStr));
	}
	else if((ConstraintsExp=="MaxUp")||(ConstraintsExp=="MinUp"))
	{
		int secondComma=Expression.ReverseFind(',');
		CString tempStr=Expression.Left(secondComma);
		int firstComma=tempStr.ReverseFind(',');

		CString itemStr1=Expression.Mid(firstComma+1,secondComma-firstComma-1);
		CString itemStr2=Expression.Mid(secondComma+1,Expression.ReverseFind(')')-secondComma-1);
		TargetFunExp=Expression.Mid(Expression.Find('(')+1,firstComma-Expression.Find('(')-1);
		ParaItemArr.Add(::atof(itemStr1));
		ParaItemArr.Add(::atof(itemStr2));
	}
	else if(ConstraintsExp=="Closest")
	{
		int fourthComma=Expression.ReverseFind(',');
		CString tempStr=Expression.Left(fourthComma);
		int thirdComma=tempStr.ReverseFind(',');
		tempStr=tempStr.Left(thirdComma);
		int secondComma=tempStr.ReverseFind(',');
		tempStr=tempStr.Left(secondComma);
		int firstComma=tempStr.ReverseFind(',');

		TargetFunExp=Expression.Mid(Expression.Find('(')+1,firstComma-Expression.Find('(')-1);

		CString itemStr1=Expression.Mid(firstComma+1,secondComma-firstComma-1);
		CString itemStr2=Expression.Mid(secondComma+1,thirdComma-secondComma-1);
		CString itemStr3=Expression.Mid(thirdComma+1,fourthComma-thirdComma-1);
		CString itemStr4=Expression.Mid(fourthComma+1,Expression.ReverseFind(')')-fourthComma-1);
		ParaItemArr.Add(::atof(itemStr1));
		ParaItemArr.Add(::atof(itemStr2));
		ParaItemArr.Add(::atof(itemStr3));
		ParaItemArr.Add(::atof(itemStr4));
	}
	/*
	else if(ConstraintsExp=="Between")
	{
		int thirdComma=Expression.ReverseFind(',');
		CString tempStr=Expression.Left(thirdComma);
		int secondComma=tempStr.ReverseFind(',');
		tempStr=tempStr.Left(secondComma);
		int firstComma=tempStr.ReverseFind(',');

		TargetFunExp=Expression.Mid(Expression.Find('(')+1,firstComma-Expression.Find('(')-1);

		CString itemStr1=Expression.Mid(firstComma+1,secondComma-firstComma-1);
		CString itemStr2=Expression.Mid(secondComma+1,thirdComma-secondComma-1);
		CString itemStr3=Expression.Mid(thirdComma+1,Expression.ReverseFind(')')-thirdComma-1);	
	}
	*/
}
