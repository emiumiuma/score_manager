#include <stdio.h>
#include <stdlib.h>
#include <getch.h>
#include <string.h>

#include "teacher.h"
#include "student.h"
#include "tools.h"

Teacher* tch = NULL;
Teacher* off_tch = NULL;
int ontchcnt = 0;
int offtchcnt = 0;

// 初始化教师数据
void init_teacher_data(void)
{
	debug("%s\n",__func__);
	tch = calloc(TCH_MAX,sizeof(Teacher));
	off_tch = calloc(TCH_MAX,sizeof(Teacher));
	FILE* frp = fopen("info_tch_online","r");
	if(NULL == frp)
	{
		printf("在职教师文件丢失！请联系工作人员！\n");
		exit(0);
	}
	for(int i=0; i<TCH_MAX; i++)
	{
		if(-1 == fscanf(frp,"%s %s %s %s\n",tch[i].name,tch[i].sex,tch[i].code,tch[i].wid))
		{
			break;
		}
		ontchcnt++;
	}
	fclose(frp);
	frp = fopen("info_tch_offline","r");
	if(NULL == frp)
	{
		printf("离职教师文件丢失！请联系工作人员！\n");
		exit(0);
	}
	for(int i=0; i<TCH_MAX; i++)
	{
		if(-1 == fscanf(frp,"%s %s %s %s\n",off_tch[i].name,off_tch[i].sex,off_tch[i].code,off_tch[i].wid))
		{
			break;
		}
		offtchcnt++;
	}
	fclose(frp);
	frp = NULL;
}

// 教师界面
void menu_teacher(void)
{
	debug("%s\n",__func__);
	for(;;)
	{
		// 清理屏幕
		system("clear");
		// 打印界面
		printf("----------------------\n");
		printf("|1、添加学生         |\n");
		printf("|2、查询学生         |\n");
		printf("|3、批量导入学生数据 |\n");
		printf("|4、批量导入学生成绩 |\n");
		printf("|5、显示所有在校学生 |\n");
		printf("|6、显示所有离校学生 |\n");
		printf("|7、重置学生密码     |\n");
		printf("|8、离校学生返校     |\n");
		printf("|0、退出             |\n");
		printf("----------------------\n");
		switch(getch())
		{
			case '1':	add_student_data();		break;
			case '2':	search_student();		break;
			case '3':	import_student_data();	break;
			case '4':	import_student_score();	break;
			case '5':	list_student_online();	break;
			case '6':	list_student_offline();	break;
			case '7':	reset_code(0);			break;
			case '8':	activation_student();			break;
			case '0':	return;	break;
		}
	}
}

// 对单个学生进行操作的界面 key为学生下标
void menu_tch_to_stu(const int key)
{
	debug("%s\n",__func__);
	for(;;)
	{
		// 清理屏幕
		system("clear");
		// 打印界面
		printf("姓名：%s 学号：%s 性别：%s \n",on_stu[key].name,on_stu[key].student_number,on_stu[key].sex);
		printf("语文：%.2lf 数学：%.2lf 英语：%.2lf\n",on_stu[key].chinese,on_stu[key].math,on_stu[key].english);
		printf("----------------------\n");
		printf("|1、删除学生         |\n");
		printf("|2、修改学生信息     |\n");
		printf("|3、修改学生成绩     |\n");
		printf("|0、返回上层界面     |\n");
		printf("----------------------\n");
		switch(getch())
		{
			case '1':	delete_student(key);return;		break;
			case '2':	update_student_data(key);	break;
			case '3':	change_student_score(key);	break;
			case '0':	return;	break;
		}
	}
}

// 添加学生(从终端单个添加)
void add_student_data(void)
{
	debug("%s\n",__func__);
	// 只允许输入姓名性别，剩余数据自动生成
	printf("请输入需要添加的学生姓名 性别：");
	scanf("%s %s",on_stu[onstucnt].name,on_stu[onstucnt].sex);
	if(strlen(on_stu[onstucnt].name)>=20 ||
		strlen(on_stu[onstucnt].sex)>=8 ||
		(0 != strcmp("男",on_stu[onstucnt].sex) && 
		0!= strcmp("女",on_stu[onstucnt].sex)))
	{
		on_stu[onstucnt].name[0]='\0';
		on_stu[onstucnt].sex[0]='\0';
		show_msg("输入信息格式不正确\n",1.5);
		return;
	}
	getchar();
	strcpy(on_stu[onstucnt].code,"000000");
	on_stu[onstucnt].chinese = 0.0;
	on_stu[onstucnt].math = 0.0;
	on_stu[onstucnt].english = 0.0;
	char newid[8] = "s000001";
	// 遍历数据找出最大的id
	for(int i=0;i<onstucnt;i++)
	{
		if(strcmp(on_stu[i].student_number,newid)>0)
		{
			strcpy(newid,on_stu[i].student_number);
		}
	}
	for(int i=0;i<offstucnt;i++)
	{
		if(strcmp(off_stu[i].student_number,newid)>0)
		{
			strcpy(newid,off_stu[i].student_number);
		}
	}
	// 在最大id的基础上+1，由于字符串的问题需要手动进位
	newid[6]++;
	if(newid[6]>'9')
	{
		for(int i=6;i>=2;i--)
		{
			if(newid[i]>'9')
			{
				newid[i]='0';
				newid[i-1]++;
			}
			else
			{
				break;
			}
		}
	}
	strcpy(on_stu[onstucnt].student_number,newid);
	printf("添加成功，学号：%s姓名：%s性别：%s\n",on_stu[onstucnt].student_number,on_stu[onstucnt].name,on_stu[onstucnt].sex);
	show_msg("",1.5);
	onstucnt++;
}

// 批量导入学生数据
void import_student_data(void)
{
	debug("%s\n",__func__);
	printf("输入需要导入文件的路径：");
	char way[100]={};
	scanf("%s",way);
	getchar();
	FILE* frp = fopen(way,"r");
	if(NULL == frp)
	{
		show_msg("没有这个文件\n",1.5);
		return;
	}
	int cnt=0;
	for(int i=onstucnt;i<STU_MAX;i++)
	{
		// 只要发现文件格式不正确就停止导入，已经导入的不作处理
		if(2 != fscanf(frp,"%s %s\n",on_stu[i].name,on_stu[i].sex))
		{
			printf("导入文件格式有误，导入操作已中断\n");
			break;
		}
		else
		{
			cnt++;
			onstucnt++;
		}
	}
	for(int i=onstucnt-cnt;i<onstucnt;i++)
	{
		// 将基础数据置零
		strcpy(on_stu[i].code,"000000");
		on_stu[i].chinese=0;
		on_stu[i].math=0;
		on_stu[i].english=0;
		int on_id_end = 1;
		char id_begin = 's';
		// 为导入的学生的id设置自动生成
		for(int j=0;j<i;j++)
		{
			if(strcmp(on_stu[j].student_number,on_stu[i].student_number)>0)
			{
				strcpy(on_stu[i].student_number,on_stu[j].student_number);
			}
		}
		for(int j=0;j<offstucnt;j++)
		{
			if(strcmp(off_stu[j].student_number,on_stu[i].student_number)>0)
			{
				strcpy(on_stu[i].student_number,off_stu[j].student_number);
			}
		}
		sscanf(on_stu[i].student_number,"%c%06d",&id_begin,&on_id_end);
		sprintf(on_stu[i].student_number,"%c%06d",id_begin,on_id_end+1);
	}
	fclose(frp);
	frp=NULL;
	printf("共录入%d数据\n",cnt);
	anykey();
}

// 批量导入学生成绩
void import_student_score(void)
{
	debug("%s\n",__func__);
	printf("输入需要导入文件的路径：");
	char way[100]={};
	scanf("%s",way);
	getchar();
	FILE* frp = fopen(way,"r");
	if(NULL == frp)
	{
		printf("没有这个文件\n");
		return;
	}
	Student stu={};
	int cnt=0;
	for(;;)
	{
		if(4 != fscanf(frp,"%s %lf %lf %lf\n",stu.student_number,&stu.chinese,&stu.math,&stu.english))
		{
			printf("导入文件格式有误，导入操作已中断\n");
			break;
		}
		else
		{
			for(int i=0;i<onstucnt;i++)
			{
				if(0 == strcmp(on_stu[i].student_number,stu.student_number))
				{
					on_stu[i].chinese=stu.chinese;
					on_stu[i].math=stu.math;
					on_stu[i].english=stu.english;
					cnt++;
					break;
				}
			}
		}
	}
	fclose(frp);
	frp=NULL;
	printf("共录入%d数据\n",cnt);
	anykey();
}

// 删除学生
void delete_student(const int key)
{
	debug("%s\n",__func__);
	char x;
	printf("\n确认删除该学生？是则输入y：");
	x=getch();
	if('y' == x)
	{
		// 需要重复输入两次y以确认删除学生
		printf("\n再次确认，是则输入y:");
		x=getch();
		if('y' == x)
		{
			off_stu[offstucnt]=on_stu[key];
			offstucnt++;
			on_stu[key]=on_stu[onstucnt-1];
			onstucnt--;
			show_msg("删除成功\n",1.5);
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}
}

// 查找学生
void search_student(void)
{
	debug("%s\n",__func__);
	char key[8] = {};
	printf("请输入需要查找的学生的学号：");
	scanf("%s",key);
	getchar();
	if(strlen(key)!=7)
	{
		printf("输入学号不正确！");
		return;
	}
	for(int i=0;i<onstucnt;i++)
	{
		if(0 == strcmp(key,on_stu[i].student_number))
		{
			printf("姓名：%s 学号：%s 性别：%s 语文：%.2lf 数学：%.2lf 英语：%.2lf\n",on_stu[i].name,on_stu[i].student_number,on_stu[i].sex,on_stu[i].chinese,on_stu[i].math,on_stu[i].english);
			//查找学生成功就跳转二级菜单，同时传入下标
			menu_tch_to_stu(i);
			return;
		}
	}
	printf("查询的学生不存在，请检查查询的条件是否正确，或者该学生已经离校\n");
	anykey();
}

// 修改学生信息(只由查找学生调用)
void update_student_data(const int key)
{
	debug("%s\n",__func__);
	char name[256]={};
	char sex[5]={};
	printf("请输入新的姓名 性别：");
	scanf("%s %s",name,sex);
	if(strlen(name)>19 || strlen(sex)>4)
	{
		show_msg("输入信息格式不正确或过长\n",1.5);
		return;
	}
	strcpy(on_stu[key].name,name);
	strcpy(on_stu[key].sex,sex);
}

// 录入学生成绩(从终端单个修改)
void change_student_score(const int key) 
{
	debug("%s\n",__func__);
	printf("请输入新的成绩，语文 数学 英语：");
	double chinese,math,english;
	scanf("%lf %lf %lf",&chinese,&math,&english);
	if(0>chinese || chinese>100 ||
		0>math || math>100 ||
		0>english || english>100)
	{
		show_msg("输入成绩越界(0~100)",1.5);
		return;
	}
	on_stu[key].chinese = chinese;
	on_stu[key].math = math;
	on_stu[key].english = english;
}

// 显示所有在线学生信息
void list_student_online(void)
{
	debug("%s\n",__func__);
	printf("\n在线学生信息列表\n");
	for(int i=0;i<onstucnt;i++)
	{
		printf("姓名：%s 学号：%s 性别：%s 语文：%.2lf 数学：%.2lf 英语：%.2lf\n",on_stu[i].name,on_stu[i].student_number,on_stu[i].sex,on_stu[i].chinese,on_stu[i].math,on_stu[i].english);
	}
	anykey();
}

// 显示所有离线学生信息
void list_student_offline(void)
{
	debug("%s\n",__func__);
	printf("\n离校学生信息列表\n");
	for(int i=0;i<offstucnt;i++)
	{
		printf("姓名：%s 学号：%s 性别：%s 语文：%.2lf 数学：%.2lf 英语：%.2lf\n",off_stu[i].name,off_stu[i].student_number,off_stu[i].sex,off_stu[i].chinese,off_stu[i].math,off_stu[i].english);
	}
	anykey();

}

// 保存教师数据到文件
void write_teacher(void)
{
	debug("%s\n",__func__);
	FILE* fwp = fopen("info_tch_online","w");
	if(NULL == fwp)
	{
		printf("保存错误，请稍候重试\n");
		return;
	}
	for(int i=0; i<ontchcnt; i++)
	{
		if(0>fprintf(fwp,"%s %s %s %s\n",tch[i].name,tch[i].sex,tch[i].code,tch[i].wid))
		{
			break;
		}
	}
	fclose(fwp);
	fwp = fopen("info_tch_offline","w");
	if(NULL == fwp)
	{
		printf("保存错误，请稍后重试\n");
		return;
	}
	for(int i=0; i<offtchcnt; i++)
	{
		if(0>fprintf(fwp,"%s %s %s %s\n",off_tch[i].name,off_tch[i].sex,off_tch[i].code,off_tch[i].wid))
		{
			break;
		}
	}
	fclose(fwp);
	fwp = NULL;
}

// 离校学生返校
void activation_student(void)
{
	debug("%s\n",__func__);
	printf("请输入需要激活的学号：");
	char id[8]={};
	scanf("%s",id);
	getchar();
	for(int i=0;i<offstucnt;i++)
	{
		if(0 == strcmp(id,off_stu[i].student_number))
		{
			on_stu[onstucnt] = off_stu[i];
			off_stu[i] = off_stu[offstucnt-1];
			onstucnt++;
			offstucnt--;
			printf("学号%s姓名%s激活成功\n",id,on_stu[onstucnt-1].name);
			show_msg("",1.5);
			return;
		}
	}
	show_msg("未找到该学生，请检查学号输入是否正确\n",1.5);
	return;
}
