#ifndef TEACHER_H
#define TEACHER_H

#define TCH_MAX 100

typedef struct Teacher
{
	char name[20];
	char sex[5];
	char code[7];
	char wid[8];
}Teacher; 

extern struct Teacher* tch;
extern struct Teacher* off_tch;
extern int ontchcnt;
extern int offtchcnt;

// 初始化教师数据
void init_teacher_data(void);
// 教师界面
void menu_teacher(void);
// 对单个学生进行操作的界面
void menu_tch_to_stu(const int key);
// 添加学生(从终端单个添加)
void add_student_data(void);
// 批量导入学生数据
void import_student_data(void);
// 删除学生
void delete_student(const int key);
// 查找学生
void search_student(void);
// 修改学生信息(只由查找学生调用)
void update_student_data(const int key);
// 录入学生成绩(从终端单个修改)
void change_student_score(const int key); 
// 批量导入学生成绩
void import_student_score(void);
// 显示所有在线学生信息
void list_student_online(void);
// 显示所有离线学生信息
void list_student_offline(void);
// 离校学生返校
void activation_student(void);
// 保存教师数据到文件
void write_teacher(void);

#endif//TEACHER_H
