// stdafx.cpp : source file that includes just the standard includes
//	ePerfManage.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

///////////////////////
/*
设计的思路：
组态--》模型
模型运行--在线维护（用户打开维护窗口，可直接维护测点：看测点的信息）
不能进行组态，组态的工作最好是调试工程师进行，但组态好的东西需要维护，此时维护的是：
以图形的画面方式，还是以模块中的列表的方式进行；用户可以打开开维护。
-----------使用的方法
工程师：组态好模型，可测试和维护模型的测点等；
用户：平时的维护为测点、煤质信息、校验等功能（开测点表,查看测点的数据、趋势有问题，则应该进行维护；煤质需要维护，并记录信息，包括历史的信息）
校验的工作：需要互动的方式进行，检查条件，然后进行测试和校验）。
:: 设计方法
测点视图：从模型中抽取源模块，具有测点属性的进行显示，测点的属性的变化可以作用到计算块中；
关系库测点视图：煤质数据，可以看到手动输入块，关联从关系表获取的测点源数据，可看和修改数据序列；
校验的工作实际上是：独立的模块，应该另行考虑；可以作为一个计算模型，计算后输出到表格中。所以构成的模式可以是：
--》组态一个模型；用一个程序调用这个模型服务，获得结果，程序自己处理其他的显示问题。
////////////////////////////////////////////////////////////////////////////////

 Maintenance of  software
 /////////////////////////////////////
 first: the software connects with the current model and load tagpoint source blocks
          maintenance operating including look real-time data vaule, historical data, operator can decide whether the point is wrong or not;
		  if the wrong value of one point is founded, some actions can be done with the tagpoint; the change will be reload to the 
		  calc model.
		  to accomplish the function, the client links with model info database, the model is running, it will detect the change of configration.

在组态时，可以进行测试。
在运行和用户使用时，需要进行维护和管理。【管理测点、管理结果】

*/
///////////////////////

