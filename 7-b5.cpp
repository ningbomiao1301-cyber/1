/* student id  name  class */
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include "read_stulist.h"
using namespace std;

#define MAX_FILENAME_LEN 512

class stu_merge {
private:
	int stu_no;
	char stu_name[MAX_NAME_LEN];
	bool round_1;
	bool round_2;

	friend class stu_list;
};

class stu_list {
private:
	student list_round_1[MAX_STU_NUM];
	int list_num_1;

	student list_round_2[MAX_STU_NUM];
	int list_num_2;

	stu_merge list_merge[MAX_STU_NUM];
	int list_merge_num;

	void clear_merge(stu_merge& stu)
	{
		stu.stu_no = 0;
		stu.stu_name[0] = '\0';
		stu.round_1 = false;
		stu.round_2 = false;
	}

	void copy_name(stu_merge& dst, const char* name)
	{
		int i;

		for (i = 0; i < MAX_NAME_LEN - 1 && name[i] != '\0'; i++) {
			dst.stu_name[i] = name[i];
		}
		dst.stu_name[i] = '\0';
	}

	void set_merge(stu_merge& dst, const student& src)
	{
		dst.stu_no = src.no;
		copy_name(dst, src.name);
		dst.round_1 = false;
		dst.round_2 = false;
	}

	void mark_merge(stu_merge& stu, int round)
	{
		if (round == 1) {
			stu.round_1 = true;
		}
		else if (round == 2) {
			stu.round_2 = true;
		}
	}

	int find_merge(int no) const
	{
		int i;

		for (i = 0; i < list_merge_num; i++) {
			if (list_merge[i].stu_no == no) {
				return i;
			}
		}

		return -1;
	}

	bool exists_in_round(const student list[], int count, int pos) const
	{
		int i;

		for (i = 0; i < pos && i < count; i++) {
			if (list[i].no == list[pos].no) {
				return true;
			}
		}

		return false;
	}

	void add_student(const student& stu, int round)
	{
		int pos;

		pos = find_merge(stu.no);
		if (pos < 0) {
			if (list_merge_num >= MAX_STU_NUM) {
				return;
			}
			set_merge(list_merge[list_merge_num], stu);
			mark_merge(list_merge[list_merge_num], round);
			list_merge_num++;
		}
		else {
			mark_merge(list_merge[pos], round);
		}
	}

	void sort_merge()
	{
		int i;
		int j;
		int min_pos;

		for (i = 0; i < list_merge_num - 1; i++) {
			min_pos = i;
			for (j = i + 1; j < list_merge_num; j++) {
				if (list_merge[j].stu_no < list_merge[min_pos].stu_no) {
					min_pos = j;
				}
			}
			if (min_pos != i) {
				stu_merge tmp = list_merge[i];
				list_merge[i] = list_merge[min_pos];
				list_merge[min_pos] = tmp;
			}
		}
	}

	int max_name_len() const
	{
		int i;
		int max_len = 4;
		int len;

		for (i = 0; i < list_merge_num; i++) {
			len = static_cast<int>(strlen(list_merge[i].stu_name));
			if (len > max_len) {
				max_len = len;
			}
		}

		return max_len;
	}

public:
	stu_list();
	int read(const char* filename, const int round);
	void merge();
	int print(const char* prompt = 0);
};

stu_list::stu_list()
{
	int i;

	list_num_1 = 0;
	list_num_2 = 0;
	list_merge_num = 0;
	for (i = 0; i < MAX_STU_NUM; i++) {
		list_round_1[i].no = 0;
		list_round_1[i].name[0] = '\0';
		list_round_2[i].no = 0;
		list_round_2[i].name[0] = '\0';
		clear_merge(list_merge[i]);
	}
}

int stu_list::read(const char* filename, const int round)
{
	int ret = 0;

	switch (round) {
		case 1:
			list_num_1 = read_stulist(filename, list_round_1, MAX_STU_NUM);
			if (list_num_1 > 0) {
				print_stulist("第一轮选课名单", list_round_1, list_num_1);
			}
			else {
				ret = -1;
			}
			break;
		case 2:
			list_num_2 = read_stulist(filename, list_round_2, MAX_STU_NUM);
			if (list_num_2 > 0) {
				print_stulist("第二轮选课名单", list_round_2, list_num_2);
			}
			else {
				ret = -1;
			}
			break;
		default:
			ret = -1;
			break;
	}

	return ret;
}

void stu_list::merge()
{
	int i;

	list_merge_num = 0;
	for (i = 0; i < list_num_1; i++) {
		if (!exists_in_round(list_round_1, list_num_1, i)) {
			add_student(list_round_1[i], 1);
		}
	}
	for (i = 0; i < list_num_2; i++) {
		if (!exists_in_round(list_round_2, list_num_2, i)) {
			add_student(list_round_2[i], 2);
		}
	}
	sort_merge();
}

int stu_list::print(const char* prompt)
{
	int i;
	int name_width;

	name_width = max_name_len();
	if (prompt != 0) {
		cout << prompt << endl;
	}

	cout << left;
	cout << " " << setw(4) << "序号" << "  "
		<< setw(7) << "学号" << "  "
		<< setw(6) << "第一轮" << "  "
		<< setw(6) << "第二轮" << "  "
		<< setw(name_width) << "姓名" << " " << endl;

	for (i = 0; i < list_merge_num; i++) {
		cout << " " << setw(4) << i + 1 << "  "
			<< setw(7) << list_merge[i].stu_no << "  "
			<< setw(6) << (list_merge[i].round_1 ? "Y" : "N") << "  "
			<< setw(6) << (list_merge[i].round_2 ? "Y" : "N") << "  "
			<< setw(name_width) << list_merge[i].stu_name << " " << endl;
	}

	return 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
int main()
{
	char file1[MAX_FILENAME_LEN];
	char file2[MAX_FILENAME_LEN];

	cout << "请输入前一轮选课的名单文件 : ";
	gets_s(file1);

	cout << "请输入后一轮选课的名单文件 : ";
	gets_s(file2);

	stu_list list;

	if (list.read(file1, 1) < 0) {
		return -1;
	}
	if (list.read(file2, 2) < 0) {
		return -1;
	}

	list.merge();
	list.print("最终选课情况");

	return 0;
}
