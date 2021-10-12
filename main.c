#include <stdio.h>
#include <string.h>

#include "login.h"
#include "tools.h"

int main(int argc,const char* argv[])
{
	debug("%s\n",__func__);
	if(1 == argc)	//无参数运行默认进入学生教师登录端
	{
		login_identify(0);
	}
	else if(2 == argc)
	{
		if(0 == strcmp(argv[1],"admin"))	//运行时加入admin参数可进入校长登录界面
		{
			login_identify(1);
		}
		else
		{
			printf("参数不匹配！\n");
		}
	}
	else
	{
		printf("参数过多！\n");
		return 0;
	}
}
