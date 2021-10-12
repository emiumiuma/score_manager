#ifndef STUDENT_H
#define STUDENT_H

#define STU_MAX 100

//学生信息 
typedef struct Student
{
    char name[20];
    char sex[5];
    char student_number[8];
    double chinese;
    double math;
    double english;
    char code[7];
}Student;

extern Student* on_stu;
extern Student* off_stu;

extern int onstucnt;
extern int offstucnt;

//初始化
void init_student_data(void);
//界面
void menu_student(int senum);
//查询个人信息
void seek_information(int senum);
//查询个人成绩
void seek_score_personal(int senum);
//查询全体成绩
void seek_score_all();
//写入数据
void write_student_data(void);
// 修改密码 senum为登录的student的下标
void change_code(int senum);

#endif 
