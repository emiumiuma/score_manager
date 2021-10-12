#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "login.h"
#include "principal.h"
#include "teacher.h"
#include "tools.h"

// 冻结名单
char frid[STU_MAX][8]={};
// 冻结名单长度
int lenfr = 0;

// 检测身份标识
void login_identify(char identify)
{ 
	debug("%s\n",__func__);
	if(1 == identify)
	{
		// identify等于1的话跳转校长登录界面
		login_principal();
	}
	else if(0 == identify)
	{
		// identity等于0的话进行学生教师登录
		login_stu_tea();
	}
	else
	{
		printf("返回身份值有误！程序终止！\n");
		return ;
	}
}

// 校长登录界面
void login_principal(void)
{
	debug("%s\n",__func__);
	//清屏打印校长登录界面
	system("clear");
	printf("       ╭════════════╮              \n");
	printf("╭══════╣校长登录界面╠══════╮\n");
	printf("║      ╰════════════╯      ║\n");
	printf("║                          ║\n");
	printf("║        请输入密码        ║\n");
	printf("║                          ║\n");
	printf("║                          ║\n");
	printf("╰══════════════════════════╯\n");
	char code1[10]={};
	// 读取校长密码
	FILE* frp = fopen("code_principal","r");
	if(NULL==frp)
	{
		printf("警告：校长密码丢失!\n");
		return;
	}
	fscanf(frp,"%s",code1);
	fclose(frp);
	frp=NULL;
	int flag = 0;
	char inputcode[256]={};
	printf("请输入密码：");
	scanf("%s",inputcode);
	getchar();
	//输入失败则重复输入直到正确为止
	for(;;)
	{
		if(0 == strcmp(inputcode,code1))
		{
			flag=1;
			break;
		}
		printf("密码错误！请再次输入密码：");
		scanf("%s",inputcode);
		getchar();
	}
	if(flag)
	{
		if(0==strcmp(code1,"000000"))//校长首次登陆修改密码
		{
			printf("检测到您是首次登陆，请修改您的密码\n");
			scanf("%s",code1);
			getchar();
			while(strlen(code1)!=6 || strcmp(code1,"000000") == 0)
			{
				printf("密码格式有误，请输入正确格式的6位密码\n");
				code1[0]='\0';
				scanf("%s",code1);
				getchar();
			}
			FILE* fwp = fopen("code_principal","w");
			fwrite(code1,1,8,fwp);
			fclose(fwp);
			fwp=NULL;
			show_msg("修改密码成功！\n",1.5);
		}
		//正确登录则初始化教师数据，进入校长功能界面
		init_teacher_data();
		menu_principal();
		write_teacher();
	}
}

// 学生老师身份选择并登录
void login_stu_tea(void)
{
	//初始化所有数据，读入学生、教师数据以及冻结名单
	init_student_data();
	init_teacher_data();
	init_freeze_list();
	debug("%s\n",__func__);
	char id[256]={};
	int i=0;
	system("clear");
	printf("****************************\n");
	printf("->请输入您的学号/工号:");
	scanf("%s",id);
	getchar();
	if(strlen(id)>7) 
	{	
		printf("帐号有误，请重试！\n");
		return ;
	}
	if(id[0]=='s')
	{
		if(is_frozen(id))	// 检测此id是否冻结
		{
			printf("该id已被冻结，请联系老师重置密码以解冻\n");
			return;
		}
		while(0!=strcmp(id,on_stu[i].student_number))
		{
			i++;
			if(i>onstucnt)
			{
				printf("查无此人，请输入正确的id\n");
				return;	// 输入失败需要重新进入程序
			}
		}
		if(match_code(on_stu[i].code))
		{
			if(0==strcmp(on_stu[i].code,"000000"))
			{
				forced_modify(on_stu[i].code);
			}
			menu_student(i);
		}
		else
		{
			frozen_code(id);
		}
	}
	if(id[0]=='t')
	{
		if(is_frozen(id))
		{
			printf("该id已被冻结，请联系校长重置密码以解冻\n");
			return;
		}
		while(strcmp(id,tch[i].wid))
		{
			i++;
			if(i>ontchcnt)
			{
				printf("查无此人，请输入正确的id\n");
				return;
			}
		}
		if(match_code(tch[i].code))
		{
			if(0==strcmp(tch[i].code,"000000"))
			{
				forced_modify(tch[i].code);
			}
			menu_teacher();
		}	
		else
		{
			frozen_code(id);
		}
	}
	// 最后统一写入数据
	write_student_data();
	write_teacher();
	write_freeze_list();
}

// 冻结账号函数,id统一规格不传长度
void frozen_code(char id[])
{
	debug("%s\n",__func__);
	// 将此id加入到冻结名单
	strcpy(frid[lenfr],id);
	lenfr++;
	printf("id%s已冻结！\n",id);
}

// 解冻账号
void thaw_code(char id[])
{
	debug("%s\n",__func__);
	for(int i=0;i<lenfr;i++)
	{
		if(0 == strcmp(id,frid[i]))
		{
			// 从冻结名单删除此id
			strcpy(frid[i],frid[lenfr]);
			lenfr--;
			show_msg("解冻成功\n",1.5);
			return;
		}
	}
	show_msg("解冻失败，解冻对象未被冻结\n",1.5);
}

// 判断是否冻结
bool is_frozen(char id[])
{
	debug("%s\n",__func__);
	// 遍历冻结名单判断此id是否与名单中的id匹配
	for(int i=0;i<lenfr;i++)
	{
		if(0 == strcmp(id,frid[i]))
		{
			return true;
		}
	}
	return false;
}

// 初始化冻结名单
void init_freeze_list(void)
{
	debug("%s\n",__func__);
	FILE* frp = fopen("list_frozen","r");
	if(NULL == frp)
	{
		printf("读取冻结名单失败\n");
		return;
	}
	for(int i=0; i<TCH_MAX; i++)
	{
		if(-1 == fscanf(frp,"%s\n",frid[i]))
		{
			break;
		}
		lenfr++;
	}
	fclose(frp);
	frp = NULL;
}

// 写入冻结名单
void write_freeze_list(void)
{
	debug("%s\n",__func__);
	FILE* fwp = fopen("list_frozen","w");
	if(NULL == fwp)
	{
		printf("写入冻结名单失败\n");
		return;
	}
	for(int i=0; i<TCH_MAX; i++)
	{
		if(-1 == fprintf(fwp,"%s\n",frid[i]))
		{
			break;
		}
		lenfr++;
	}
	fclose(fwp);
	fwp = NULL;
}

// 判断账号密码是否匹配
bool match_code(char std_code[])
{
	debug("%s\n",__func__);
	int cnt=0;
	char entered_code[256]={};
	for(;;)
	{
		if(cnt>=3)
		{
			show_msg("您已输错密码三次，账户已冻结!请联系您的上级部门！\n",1.5);
			break;
		}
		printf("请输入密码：");
		scanf("%s",entered_code);
		getchar();
		if(0==strcmp(std_code,entered_code))
		{
			show_msg("密码正确！\n",1.5);
			return true;
		}
		else
		{
			cnt++;
			printf("密码错误,您还有%d次机会\n",3-cnt);
		}
	}
	return false;
}

