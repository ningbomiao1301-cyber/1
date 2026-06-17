#pragma once

#define MAX_STU_NUM 512
#define MAX_NAME_LEN 32

struct student {
	int no;
	char name[MAX_NAME_LEN];
};

int read_stulist(const char* filename, struct student* stu, const int max_num);
int print_stulist(const char* prompt, const struct student* stu, const int stu_num);
