// stdafx.cpp : source file that includes just the standard includes
//	ePerfManage.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

///////////////////////
/*
��Ƶ�˼·��
��̬--��ģ��
ģ������--����ά�����û���ά�����ڣ���ֱ��ά����㣺��������Ϣ��
���ܽ�����̬����̬�Ĺ�������ǵ��Թ���ʦ���У�����̬�õĶ�����Ҫά������ʱά�����ǣ�
��ͼ�εĻ��淽ʽ��������ģ���е��б�ķ�ʽ���У��û����Դ򿪿�ά����
-----------ʹ�õķ���
����ʦ����̬��ģ�ͣ��ɲ��Ժ�ά��ģ�͵Ĳ��ȣ�
�û���ƽʱ��ά��Ϊ��㡢ú����Ϣ��У��ȹ��ܣ�������,�鿴�������ݡ����������⣬��Ӧ�ý���ά����ú����Ҫά��������¼��Ϣ��������ʷ����Ϣ��
У��Ĺ�������Ҫ�����ķ�ʽ���У����������Ȼ����в��Ժ�У�飩��
:: ��Ʒ���
�����ͼ����ģ���г�ȡԴģ�飬���в�����ԵĽ�����ʾ���������Եı仯�������õ�������У�
��ϵ������ͼ��ú�����ݣ����Կ����ֶ�����飬�����ӹ�ϵ���ȡ�Ĳ��Դ���ݣ��ɿ����޸��������У�
У��Ĺ���ʵ�����ǣ�������ģ�飬Ӧ�����п��ǣ�������Ϊһ������ģ�ͣ���������������С����Թ��ɵ�ģʽ�����ǣ�
--����̬һ��ģ�ͣ���һ������������ģ�ͷ��񣬻�ý���������Լ�������������ʾ���⡣
////////////////////////////////////////////////////////////////////////////////

 Maintenance of  software
 /////////////////////////////////////
 first: the software connects with the current model and load tagpoint source blocks
          maintenance operating including look real-time data vaule, historical data, operator can decide whether the point is wrong or not;
		  if the wrong value of one point is founded, some actions can be done with the tagpoint; the change will be reload to the 
		  calc model.
		  to accomplish the function, the client links with model info database, the model is running, it will detect the change of configration.

����̬ʱ�����Խ��в��ԡ�
�����к��û�ʹ��ʱ����Ҫ����ά���͹����������㡢��������

*/
///////////////////////

