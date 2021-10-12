#include <stdio.h>

typedef struct Teacher
{
	char name[20];
	char sex[5];
	char code[7];
	char wid[8];
}Teacher;

int main(int argc,const char* argv[])
{
	Teacher tch = {"沈方伟","男","000000","t000002"};
	FILE* fp = fopen("info_tch_offline","w");
	fprintf(fp,"%s %s %s %s",tch.name,tch.sex,tch.code,tch.wid);
}
