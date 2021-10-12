#ifndef PRINCIPAL_H
#define PRINCIPAL_H

/*调试信息*/
#ifdef DEBUG
	#define debug(...) printf(__VA_ARGS__)
#else
	#define debug(...)
#endif

#define code_principal code_principal//校长密码文件
void reset_own_code(void);			//重置自己的密码
void menu_principal(void);			//校长操作界面
void add_teacher_data(void);		//添加教师
void delete_teacher(void);			//删除教师
void show_teacher_online(void);		//显示所有在职教师
void show_teacher_offline(void);	//显示所有离职教师
//离职教师返职
void activation_teacher(void);

#endif //PRINCIPAL_H

