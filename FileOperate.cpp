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
    in.open(SourceFile,ios::binary);//��Դ�ļ�
    if(in.fail())//��Դ�ļ�ʧ��
	{
       //cout<<"Error 1: Fail to open the source file."<<endl;
       in.close();
       out.close();
       return 0;
	}
    out.open(NewFile,ios::binary);//����Ŀ���ļ� 
    if(out.fail())//�����ļ�ʧ��
	{
       //cout<<"Error 2: Fail to create the new file."<<endl;
       out.close();
       in.close();
       return 0;
	}
    else//�����ļ�
	{
       out<<in.rdbuf();
       out.close();
       in.close();
       return 1;
	}
}