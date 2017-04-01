#pragma once

#include   "stdafx.h"
#include <iostream>
#include <fstream>
using namespace std;

#include "FileOperate.h"

int CopyFileB(char *SourceFile,char *NewFile)
{
    ifstream in;
    ofstream out;
    in.open(SourceFile,ios::binary);//打开源文件
    if(in.fail())//打开源文件失败
	{
       //cout<<"Error 1: Fail to open the source file."<<endl;
       in.close();
       out.close();
       return 0;
	}
    out.open(NewFile,ios::binary);//创建目标文件 
    if(out.fail())//创建文件失败
	{
       //cout<<"Error 2: Fail to create the new file."<<endl;
       out.close();
       in.close();
       return 0;
	}
    else//复制文件
	{
       out<<in.rdbuf();
       out.close();
       in.close();
       return 1;
	}
}