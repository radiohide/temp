
#ifndef g_Data_zxg
#define g_Data_zxg

#include <vector>
#define		TAGNAME_MAX				48				// 最长点名
#define		FUNC_MAX				100				// 最大公式个数
typedef struct unitTagData_struct{
	char   id[10];
	char   unit[10];
	char   sets[10];
	char   name[50];
	char   capacity[4];
	char   type[50];
	char   PatLib_StaTime[18];
	char   PatLib_LasTime[18];
	char   PatLib_VerNo[15];
}unitTagData;
typedef struct logTagData_struct{
	char   logID[24];
	char   donetime[48];
	char   logbegintime[48];
	char   logendtime[48];
}logTagData;
typedef struct pointTagData_struct{
	char   varID[20];
	char   varname[50];
	char   vardes[50];
	char   varunit[5];
	char   varformulation[100];
	char   vartype[10];
	std::vector<CString> strings_points;//包含了所用对应数字的索引
    std::vector<CString> pointcycle;//包含了所用对应数字的索引

	char   category_value[50];//	char   classifyattribute[50];
	char   category_endpoint[45];//	char   clusterbegin[45];
	char   direction[45];
	char   fixsep[45];
	char   relationsep[45];
	char   clustersource[45];
	//char   data_source[1024];
	std::vector<CString> data_source;//包含了所用测点的来源
	std::vector<CString> tag_beinvolved;//包含了是否参与清洗的标记 1参与 0 不参与
}pointTagData;
typedef struct ReadHiDataRequest_struct{
	long stTime;				//查询点记录的开始时间，秒
	long enTime;				//查询点记录的结束时间，秒
	long tPeriod;			    //查询点值的时间间隔，秒
	int	 reqType;				//查询的数据类型（原始=0，最大=1，最小=2，平均=3,快照=4）
	char pointName[48];	    //请求的完整点名
}ReadHiDataRequest;

typedef struct TagData_struct{
	double			value;		//返回点的数值
	long    		time;		//返回点的时间，秒
	int 			status;		//返回点的状态
}TagData;

typedef struct fresh_badTagData_struct{
	char		index[10];		//序号
	char		begintime_bad[48];		//坏时间段的开始
	char		endtime_bad[48];		//坏时间段的结束
	char		timeflag[48];           //坏时间年月日
}freshbadTagData; 
typedef struct fresh_goodTagData_struct{
	char		index[10];		//序号
	char		begintime_good[48];		//好时间段的开始
	char		endtime_good[48];		//好时间段的结束
	char		timeflag[48];           //好时间年月日
}freshgoodTagData; 

typedef struct hist_badTagData_struct{
	char		index[10];		//序号
	char		begintime_bad[48];		//坏时间段的开始
	char		endtime_bad[48];		//坏时间段的结束
	char		timeflag[48];           //坏时间年月日
}histbadTagData; 

typedef struct hist_goodTagData_struct{
	char		index[10];		//序号
	char		begintime_good[48];		//好时间段的开始
	char		endtime_good[48];		//好时间段的结束
	char		timeflag[48];           //好时间年月日
}histgoodTagData;

typedef struct ordercorresponding_TagData_struct{
	int		Rnum;		//行号
	int		Ordernum;		//不同测点在相同名称中的顺序值
	int		Listnum;		//列表中的索引
}ordercorrespondingTagData;

typedef struct dataCleanGlobal_TagData_struct{
	int		rate;		//倍率(自然数)
}dataCleanGlobalTagData;

typedef struct seps_TagData_struct{
	int		sep_begin;		//时间段开始
	int		sep_end;		//时间段结束
	int		sep_flag;		//时间（未用）
}jiangesTagData;

typedef struct Group_TagData_struct{
	char		groupID[10];		//组ID
	char		groupname[50];		//组名
	char		groupdes[50];		//组描述
	char		grouptype[50];      //组类型
}GroupTagData; 
typedef struct VarsGroup_TagData_struct{
	char		id[10];		        //序号
	char		groupid[50];		//子组ID
	char		varname[50];		//点名
}VarsGroupTagData; 

typedef struct ConditionClass_Fresh_TagData_struct{
	char		ID[48];		            //工况ID
	char		Condition_code[128];		//工况代码
	char		Record_num[48];		    //记录个数
	char		Carry_situation[48];    //负荷情况
    char		Coal_situation[48];		//煤质情况
	char		Env_Tem_situation[48];  //环境温度情况
	char        Ear_RID[48];//时间最早的RID
	char        Late_RID[48];//时间最晚的RID
}ConditionClass_FreshTagData; 

typedef struct StableRecord_Fresh_TagData_struct{
	char		RID[48];		            //RID
	char		Stable_begin[48];		 //稳态起始时间标签
	char		Stable_end[48];		    //稳态结束时间标签
	char		Stable_longth[48];     //稳态时间段长度
    char		Stable_flag[48];				//稳态有效标志
	char		Condition_Code[128];				//工况代码
}StableRecord_FreshTagData; 

typedef struct StableRecord_Hist_TagData_struct{
	char		RID[48];		            //RID
	char		Stable_begin[48];		 //稳态起始时间标签
	char		Stable_end[48];		    //稳态结束时间标签
	char		Stable_longth[48];     //稳态时间段长度
    char		Stable_flag[48];				//稳态有效标志
	char		Condition_Code[128];				//工况代码
}StableRecord_HistTagData;

typedef struct Data_TagData_struct{
	char		RID[48];		            //RID
	char		avg[48];			//数字量和模拟量的返回值，对于模拟量返回的是平均值，对于数字量返回的是1或者0来代表其值
	char		mixvalue[48];		//最小值
	char		mixtime[48];		//最小值时间
    char		maxvalue[48];		//最大值
	char		maxtime[48];		//最大值时间
	char		mean_square[48];	//均方差
	char		stablevalue[48];	//稳定度
	
	char        frequency[48];      //数字量频率
}DataTagData; 

typedef struct basicgroupclass_fresh_TagData_struct{
	char		RID[48];		   
    std::vector<CString> SortStr;//包含了所用GroupX_ClassMark
}basicgroupclass_freshTagData;

typedef struct basicgroupclass_hist_TagData_struct{
	char		RID[48];		   
    std::vector<CString> SortStr;//包含了所用GroupX_ClassMark
}basicgroupclass_histTagData;

// T_UnitX_Group[X]_Class_Hist表
typedef struct groupX_class_hist_TagData_struct{
	char		ClassSerial_id[48];	//类别编号ID	   
	char		class_mark[48];     //类别符号
	char		condition_code[128]; //所属工况代码
	char		record_numble[48];  //包含记录个数
    std::vector<CString> point_eigenvalues;//包含了一行中的所有特征值
}groupX_class_histTagData;

typedef struct groupX_class_fresh_TagData_struct{
	char		ClassSerial_id[48];	//类别编号ID	   
	char		class_mark[48];     //类别符号
	char		condition_code[128]; //所属工况代码
	char		record_numble[48];  //包含记录个数
    std::vector<CString> point_eigenvalues;//包含了一行中的所有特征值
}groupX_class_freshTagData;

typedef struct dataloadinfo_TagData_struct{
	char		ID[10];	//索引	   
	char		load_starttime[15];     //加载开始时间
	char		load_endtime[15]; //加载结束时间
	char		load_operatetime[15];     //操作时间
	char		load_operator[8]; //操作者
	char		load_remark[200]; //备注
}dataloadinfoTagData;

typedef struct tagTAGINFO{
	double			dValue;							// 点值
	time_t			tTime;							// 点时标
	DWORD			wState;							// 点状态
	char			cName[TAGNAME_MAX];				// 点名
} TAGINFO, *PTAGINFO;
typedef struct ComGroup_TagData_struct{
	char		ComgroupID[148];		//
	char		Comgroupname[148];	//
	char		Comgroupdes[148];	//
}ComGroupTagData;
typedef struct ComVarsGroup_TagData_struct{
	char		ComID[48];		//
	char		ComgroupID[48];//
	char		Comvarname[48];//
}ComVarsGroupTagData; 
typedef TAGINFO	(*RTFUNC)(TAGINFO* pVar, int);
#endif
