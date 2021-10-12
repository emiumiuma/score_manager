#include <stdio.h>
#include <getch.h>
#include <stdlib.h>
#include <string.h>

#include "student.h"
#include "tools.h"

//导入的信息个数
int onstucnt=0;
int offstucnt=0;

//结构体变量
Student* on_stu=NULL;
Student* off_stu=NULL;

//打印菜单 senum为登录的student的下标
void menu_student(int senum)
{
	debug("%s\n",__func__);
	for(;;)
	{
		// 清理屏幕
		system("clear");
		// 打印界面
		printf("----------------------\n");
		printf("|1、查询个人成绩     |\n");
		printf("|2、查询全体成绩     |\n");
		printf("|3、查看个人信息     |\n");
		printf("|4、修改密码         |\n");
		printf("|0、退出             |\n");
		printf("----------------------\n");
		switch(getch())
		{
			case '1':	seek_score_personal(senum);		break;
			case '2':	seek_score_all();		break;
			case '3':	seek_information(senum);	break;
			case '4':	change_code(senum);	break;
			case '0':	return;	break;
		}
	}
}

// 修改密码
void change_code(int senum)
{
	char tmpcode[256]={};
	debug("%s\n",__func__);
	// 首先输入旧密码
	printf("请输入旧密码：");
	scanf("%s",tmpcode);
	getchar();
	// 旧密码正确进行密码修改
	if(0 == strcmp(tmpcode,on_stu[senum].code))
	{
		printf("请输入新的密码：");
		scanf("%s",tmpcode);
		getchar();
		// 判断密码长度是否正确
		if(6 != strlen(tmpcode) || strcmp(tmpcode,"000000") == 0)
		{
			show_msg("密码格式不正确\n",1.5);
			return;
		}
		// 重复输入密码进行确认
		printf("请再次输入新的密码：");
		char tmpcode1[256]={};
		scanf("%s",tmpcode1);
		getchar();
		if(0 == strcmp(tmpcode,tmpcode1))
		{
			strcpy(on_stu[senum].code,tmpcode);
			printf("修改密码成功！");
			anykey();
		}
		else
		{
			show_msg("两次密码输入不一致\n",1.5);
			return;
		}
	}
	else
	{
		show_msg("密码输入错误\n",1.5);
	}
}

//学生信息初始化
void init_student_data(void)
{
	debug("%s\n",__func__);
	on_stu=calloc(STU_MAX,sizeof(Student));
	off_stu=calloc(STU_MAX,sizeof(Student));
	//从文件导入在线学生信息
	FILE *frp = fopen("info_stu_online", "r");
	if (NULL == frp)
	{
		perror("fopen");
		return;
	}
	for(int i=0;i<STU_MAX;i++)
	{
		if(0<fscanf(frp,"%s %s %s %lf %lf %lf %s\n",on_stu[i].name,on_stu[i].sex,on_stu[i].student_number,&on_stu[i].chinese,&on_stu[i].math,&on_stu[i].english,on_stu[i].code))
		{
			onstucnt++;
		}
		else
		{
			break;
		}
	}
	//从文件导入离线学生信息
	frp = fopen("info_stu_offline", "r");
	if (NULL == frp)
	{
		perror("fopen");
		return;
	}
	for(int i=0;i<STU_MAX;i++)
	{
		if(0<fscanf(frp,"%s %s %s %lf %lf %lf %s\n",off_stu[i].name,off_stu[i].sex,off_stu[i].student_number,&off_stu[i].chinese,&off_stu[i].math,&off_stu[i].english,off_stu[i].code))
		{
			offstucnt++;
		}
		else
		{
			break;
		}
	}
	fclose(frp);
	frp=NULL;
}

//查询个人信息
void seek_information(int senum)
{
	debug("%s\n",__func__);
	printf("学号：%s \n姓名：%s \n性别：%s \n",on_stu[senum].student_number,on_stu[senum].name,on_stu[senum].sex);
	anykey();
}
//查询个人成绩
void seek_score_personal(int senum)
{
	debug("%s\n",__func__);
	printf("语文：%.2lf \n数学：%.2lf \n英语：%.2lf\n",on_stu[senum].chinese,on_stu[senum].math,on_stu[senum].english);
	double sum=on_stu[senum].chinese+on_stu[senum].math+on_stu[senum].english;
	printf("总分：%.2lf\n",sum);
	int rank=1;
	for(int i=0;i<onstucnt;i++)
	{
		double sum1=on_stu[i].chinese+on_stu[i].math+on_stu[i].english;
		if(sum<sum1)
		{
			rank++;
		}
	}
	printf("排名：%d\n",rank);
	anykey();
}


//查询全体成绩
void seek_score_all()
{
	debug("%s\n",__func__);
	// 设置临时结构体数组
	Student tmpstu[onstucnt];
	for(int i=0;i<onstucnt;i++)
	{
		tmpstu[i]=on_stu[i];
	}
	// 设置最大值最小值
	double cmax=0,mmax=0,emax=0,cmin=100,mmin=100,emin=100;
	for(int i=0;i<onstucnt;i++)
	{
		// 对结构体进行排序，根据总分进行从大到小排序
		int max=i;
		double sum1=tmpstu[i].chinese+tmpstu[i].math+tmpstu[i].english;
		for(int j=i;j<onstucnt;j++)
		{
			double sum2=tmpstu[j].chinese+tmpstu[j].math+tmpstu[j].english;
			if(sum2>sum1)
			{
				max=j;
				sum1=sum2;
			}
		}
		Student tmp = tmpstu[max];
		tmpstu[max] = tmpstu[i];
		tmpstu[i] = tmp;
		// 判断最大值最小值
		if(tmpstu[i].chinese>cmax)
		{
			cmax=tmpstu[i].chinese;
		}
		if(tmpstu[i].chinese<cmin)
		{
			cmin=tmpstu[i].chinese;
		}
		if(tmpstu[i].math>mmax)
		{
			mmax=tmpstu[i].math;
		}
		if(tmpstu[i].math<mmin)
		{
			mmin=tmpstu[i].math;
		}
		if(tmpstu[i].english>emax)
		{
			emax=tmpstu[i].english;
		}
		if(tmpstu[i].english<emin)
		{
			emin=tmpstu[i].english;
		}
	}
	// 打印全体分数排名
	printf("总分   语文  数学  英语  排名    \n");
	for(int i=0;i<onstucnt;i++)
	{
		printf("%.2f %.2f %.2f %.2f  %d\n",tmpstu[i].chinese+tmpstu[i].math+tmpstu[i].english,tmpstu[i].chinese,tmpstu[i].math,tmpstu[i].english,i+1);
	}
	printf("\n");
	printf("语文最高分：%.2f \n数学最高分：%.2f \n英语最高分：%.2f \n",cmax,mmax,emax);
	printf("语文最低分：%.2f \n数学最低分：%.2f \n英语最低分：%.2f \n",cmin,mmin,emin);
	anykey();
}

//写入数据
void write_student_data(void)
{
	debug("%s\n",__func__);
	FILE *fwp = fopen("info_stu_online", "w");
	if (NULL == fwp)
	{
		perror("fopen");
		return;
	}
	for(int i=0;i<onstucnt;i++)
	{
		if(0>fprintf(fwp,"%s %s %s %lf %lf %lf %s\n",on_stu[i].name,on_stu[i].sex,on_stu[i].student_number,on_stu[i].chinese,on_stu[i].math,on_stu[i].english,on_stu[i].code))
		{
			break;
		}
	}
	fwp = fopen("info_stu_offline", "w");
	if (NULL == fwp)
	{
		perror("fopen");
		return;
	}
	for(int i=0;i<offstucnt;i++)
	{
		if(0>fprintf(fwp,"%s %s %s %lf %lf %lf %s\n",off_stu[i].name,off_stu[i].sex,off_stu[i].student_number,off_stu[i].chinese,off_stu[i].math,off_stu[i].english,off_stu[i].code))
		{
			break;
		}
	}
	fclose(fwp);
	fwp=NULL;
}

