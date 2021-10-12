#ifndef LOGIN_H
#define LOGIN_H

#include <stdbool.h>
#include "student.h"

// 冻结名单
extern char frid[STU_MAX][8];
// 冻结名单长度
extern int lenfr;

// 检测身份标识
void login_identify(char identify);
// 校长登录界面
void login_principal(void);
// 学生老师身份选择并登录
void login_stu_tea(void);
// 判断账号密码是否匹配
bool match_code(char std_code[]);

// 冻结账号函数,id统一规格不传长度
void frozen_code(char id[]);
// 解冻账号
void thaw_code(char id[]);
// 初始化冻结名单
void init_freeze_list(void);
// 写入冻结名单
void write_freeze_list(void);
// 判断是否冻结
bool is_frozen(char id[]);

#endif//LOGIN_H
