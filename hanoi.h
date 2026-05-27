#pragma once

/* ������const���������ͷ�ļ��������� */
#include "hanoi_const_value.h"

/* ------------------------------------------------------------------------------------------------------

	 ���ļ����ܣ�
	1��Ϊ�˱�֤ hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp ���໥���ʺ����ĺ�������
	2��һ�����ϵ�cpp���õ��ĺ궨�壨#define����ȫ��ֻ����const����������������
	3�����Բο� cmd_console_tools.h ��д���������Ķ�����ᣩ
   ------------------------------------------------------------------------------------------------------ */

#define PAN_WIDTH(disk_id)  ((2 * (disk_id) + 1) * HDC_Base_Width)
#define UNDERPAN_WIDTH      ((2 * MAX_LAYER + 1) * HDC_Base_Width)

extern int g_move_count;
extern int g_delay_time;
extern int g_towers[3][MAX_LAYER];
extern int g_tower_counts[3];

int display_menu();
void menu_handler(int choice);

int input_params_safe();
char input_src_safe();
void draw_all_towers();
void move_disk_animation(int disk_id, int from_col, int to_col, int from_height, int to_height);
void print_arrays_horizontal(int start_x, int start_y);
void print_towers_vertical(int start_x, int start_y);

void hanoi_rec(int n, char src, char mid, char dst, int menu_type);
void handle_move(char src, char dst, int menu_type);