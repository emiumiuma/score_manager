#ifndef TOOLS_H
#define TOOLS_H

#include <stdbool.h>

/*调试信息*/
#ifdef DEBUG
	#define debug(...) printf(__VA_ARGS__)
#else
	#define debug(...)
#endif

//任意键继续
void anykey(void);	
//打印提示信息	
void show_msg(const char* msg,double sec);
//若密码为“000000”，则强制修改密码，不能和原密码相同;id:工号/学号;code:密码
void forced_modify(char *code);
//修改密码(不能改为“000000”，否则下次登录将强制修改密码);id:工号/学号;code:密码
void modify_code(void); 
//重置密码,id:工号/学号
void reset_code(char flag);

#endif//TOOLS_H



