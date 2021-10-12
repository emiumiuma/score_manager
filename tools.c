#include <stdio.h>
#include <string.h>
#include <getch.h>
#include "tools.h"
#include "student.h"
#include "teacher.h"
#include "principal.h"
#include "login.h"

//任意键继续
void anykey(void)
{
	puts("任意键继续...");
	stdin->_IO_read_ptr = stdin->_IO_read_end;
	getch();
}

//打印提示信息
void show_msg(const char* msg,double sec)
{
	printf("%s",msg);
	fflush(stdout);
	usleep(sec*1000000);
}

//强制修改密码
void forced_modify(char *code)
{	

	printf("您好，您的密码为初始密码，请输入新的6位密码进行修改！\n");
	char temp[100]={};
		scanf("%s",temp);
		while(strlen(temp)!=6 || strcmp(temp,"000000") == 0)
		{
			printf("请输入正确格式的6位密码\n");
			temp[0]='\0';
			scanf("%s",temp);
			getchar();
		}
		strcpy(code,temp);
		show_msg("修改密码成功！\n",1.5);
}

//修改密码
void modify_code(void)
{
	char id[256]={};//存储输入的id
	int i=0;//计数，用于遍历结构体数组
	printf("请输入修改对象的ID");
	scanf("%s",id);
	getchar();
	
	char temp1[256]={};
	char temp2[256]={};
	if(id[0]=='s')//找出要修改的学生的原密码
	{
		while(strcmp(id,on_stu[i].student_number))
		{
			i++;
			if(i>onstucnt)
			{
				show_msg("查无此人，请输入正确的id\n",1.5);
				return;
			}
		}
		printf("请输入旧密码：");
		scanf("%s",temp1);
		getchar();
		while(0!=strcmp(temp1,on_stu[i].code))
		{
			printf("密码错误，请重新输入！\n");
			temp1[0]='\0';
			scanf("%s",temp1);
			getchar();
		}
	}
	else if(id[0]=='t')//找出要修改的老师的原密码
	{
		while(strcmp(id,tch[i].wid))
		{
			i++;
			if(i>ontchcnt)
			{
				show_msg("查无此人，请输入正确的id",1.5);
				return;
			}
		}
		printf("请输入旧密码：");
		scanf("%s",temp1);
		getchar();
		while(0!=strcmp(temp1,tch[i].code))
		{
			printf("密码错误，请重新输入！\n");
			temp1[0]='\0';
			scanf("%s",temp1);
			getchar();
		}
	}
	else
	{
		show_msg("查无此人，请输入正确的id",1.5);
		return;
	}
	
	for(;;)//修改密码，输入两次并确认
	{
		printf("请输入新密码:");
		scanf("%s",temp1);
		while(strlen(temp1)!=6)
		{
			printf("密码格式有误，请输入正确格式的6位密码\n");
			temp1[0]='\0';
			scanf("%s",temp1);
		}
		printf("请再次输入新密码:");
		scanf("%s",temp2);
		if(0!=strcmp(temp2,temp1))
		{
			printf("两次密码不一致，请重新输入！\n");
			temp1[0]='\0';
			temp2[0]='\0';
		}
		else
		{
			if(id[0]=='s')
			{
				strcpy(on_stu[i].code,temp2);
				printf("密码修改成功！\n");
				break;
			}
			if(id[0]=='t')
			{
				strcpy(tch[i].code,temp2);
				printf("密码修改成功！\n");
				break;
			}
		}
	}
}

//重置密码,id指要重置的对象的工号/学号
void reset_code(char flag)
{
	char id[256]={};
	printf("请输入要重置对象的ID：");
	scanf("%s",id);
	getchar();
	int i=0;
	if(id[0]=='t')
	{
		if(flag==0)
		{
			show_msg("您没有权限重置教师的密码!\n",1.5);
			return;
		}
		while(strcmp(id,tch[i].wid))
		{
			i++;
			if(i>ontchcnt)
			{
				show_msg("请输入正确的id\n",1.5);
				return;
			}
		}
		printf("确认重置该老师的密码？(确认:y)\n");
		char ch=getch();
		if(ch=='y'||ch=='Y')
		{
			sprintf(tch[i].code,"000000");
			show_msg("重置密码成功！\n",1.5);
			if(is_frozen(id))
			{
				thaw_code(id);
			}
		}
		return;
	}
	
	if(id[0]=='s')
	{
		while(strcmp(id,on_stu[i].student_number))
		{
			i++;
			if(i>onstucnt)
			{
				show_msg("请输入正确的id\n",1.5);
				return;
			}
		}
		printf("确认重置该学生的密码？(确认:y)\n");
		char ch=getch();
		if(ch=='y'||ch=='Y')
		{
			sprintf(tch[i].code,"000000");
			show_msg("重置密码成功！\n",1.5);
			if(is_frozen(id))
			{
				thaw_code(id);
			}
		}
		return;
	}
	show_msg("输入无效\n",1.5);
}

