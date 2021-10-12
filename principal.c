#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getch.h>

#include "principal.h"
#include "teacher.h"
#include "tools.h"


//重置密码，以写入形式打开，如果打开成功则直接覆盖写入000000
void reset_own_code(void)
{
	FILE* fwp = fopen("code_principal","w");
	if(NULL == fwp)
	{
		printf("出错了，请检查路径\n");
		return ;
	}
	// 需要输入y以确认
	char x;
	printf("\n确认重置？是则输入y：");
	x=getch();
	if('y' == x || 'Y' == x)
	{
		//以二进制方式写入
		fwrite("000000",8,1,fwp);
		show_msg("重置密码成功！\n",1.5);
		fclose(fwp);
		fwp = NULL;	
	}
	else
	{
		return;
	}
	
}

void add_teacher_data(void)
{
	Teacher on_tch = {};	//添加的教师结构体
	printf("请输入添加的教师信息（姓名/性别）\n");
	printf("姓名：");
	scanf("%s",on_tch.name);
	printf("性别：");
	scanf("%s",on_tch.sex);
	strcpy(on_tch.code,"000000");
	if(strlen(on_tch.name)>=20 ||
		strlen(on_tch.sex)>=5 ||
		(0 != strcmp("男",on_tch.sex) && 
		0!= strcmp("女",on_tch.sex)))
	{
		show_msg("输入信息格式不正确\n",1.5);
		return;
	}
	
	//新添加教师工号，为防止重复，遍历在线和离线，新工号在最大工号上加一
	int on_id_end = 1;
	char id_begin = 't';
	for(int i=0;i<ontchcnt;i++)
	{
		if(strcmp(tch[i].wid,on_tch.wid)>0)
		{
			strcpy(on_tch.wid,tch[i].wid);
		}
	}
	for(int i=0;i<offtchcnt;i++)
	{
		if(strcmp(off_tch[i].wid,on_tch.wid)>0)
		{
			strcpy(on_tch.wid,off_tch[i].wid);
		}
	}
	
	//先拆解字符串到内存，再拆解，工号末尾加一
	sscanf(on_tch.wid,"%c%06d",&id_begin,&on_id_end);
	sprintf(on_tch.wid,"%c%06d",id_begin,on_id_end+1);
	
	printf("姓名：%s  性别：%s  工号：%s\n添加成功\n"
		,on_tch.name,on_tch.sex,on_tch.wid);
	//将添加教师加在在线列表最后
	tch[ontchcnt] = on_tch;
	ontchcnt++;
	anykey();
}

void delete_teacher(void)
{
	char flag = 0;//作为是否找到删除教师的标志
	
	Teacher del_tch = {};
	Teacher del_tch2 = {};
	printf("请输入删除的教师工号：");
	scanf("%s",del_tch.wid);
	printf("请确认删除的教师工号：");
	scanf("%s",del_tch2.wid);
	
	//确认工号一致
	if(strcmp(del_tch.wid,del_tch2.wid) != 0)
	{	
		printf("工号不一致，请重试\n");
		return;
	}
	
	for(int i=0; i<ontchcnt; i++)
	{
		if(0 == strcmp(del_tch.wid,tch[i].wid))
		{
			//将删除教师加入离线名单
			off_tch[offtchcnt] = tch[i];
			//删除教师后将最后一名在线教师填补空位
			tch[i] = tch[ontchcnt-1];
			flag = 1;
			break;
		}
	}
	
	//判断是否找到删除的教师
	if(flag)
	{
		printf("姓名：%s  性别：%s  工号：%s\n删除成功\n",
			off_tch[offtchcnt].name,off_tch[offtchcnt].sex,off_tch[offtchcnt].wid);
		ontchcnt--;
		offtchcnt++;
		anykey();
	}
	else
	{
		printf("查无此人，请检查输入信息\n");
		anykey();
	}

}


void show_teacher_online(void)
{
	//遍历在线教师堆内存并打印
	for(int i=0; i<ontchcnt; i++)
	{
			printf("姓名：%s  性别：%s  工号：%s\n",
				tch[i].name,tch[i].sex,tch[i].wid);
	}
	anykey();
}

void show_teacher_offline(void)
{
	//遍历离线教师堆内存并打印
	for(int i=0; i<offtchcnt; i++)
	{
			printf("姓名：%s  性别：%s  工号：%s\n",
				off_tch[i].name,off_tch[i].sex,off_tch[i].wid);
	}
	anykey();
}

//离职教师返职
void activation_teacher(void)
{
	debug("%s\n",__func__);
	printf("请输入需要激活的工号：");
	char id[8]={};
	scanf("%s",id);
	getchar();
	for(int i=0;i<offtchcnt;i++)
	{
		if(0 == strcmp(id,off_tch[i].wid))
		{
			tch[ontchcnt] = off_tch[i];
			off_tch[i] = off_tch[offtchcnt-1];
			ontchcnt++;
			offtchcnt--;//激活成功则离线列表-1,在线列表+1
			printf("工号%s姓名%s激活成功\n",id,tch[ontchcnt-1].name);
			show_msg("",1.5);
			return;
		}
	}
	show_msg("未找到该教师，请检查工号输入是否正确\n",1.5);
	return;
}

void menu_principal(void)
{
	for(;;)
	{
		
		system("clear");
		printf("---------------------------\n");
		printf("|1、重置本人密码          |\n");	
		printf("|2、重置教师密码          |\n");
		printf("|3、添加教师              |\n");
		printf("|4、删除教师              |\n");
		printf("|5、显示所有在职教师      |\n");
		printf("|6、显示所有离职教师      |\n");
		printf("|7、离职教师返职          |\n");
		printf("|0、退出                  |\n");
		printf("---------------------------\n");
		
		switch(getch())
		{
			case '1':  reset_own_code();		break;  
			case '2':  reset_code(1);			break;
			case '3':  add_teacher_data();  	break;
			case '4':  delete_teacher();  		break;
			case '5':  show_teacher_online();	break;
			case '6':  show_teacher_offline();	break;
			case '7':  activation_teacher();	break;
			case '0':  printf("##已退出##\n");return ;
		}
	}
}

