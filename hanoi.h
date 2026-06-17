#pragma once

/* 包含const常量定义的头文件，不能改 */
#include "hanoi_const_value.h"

/* ------------------------------------------------------------------------------------------------------

	 本文件功能：
	1、为了保证 hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp 能相互访问和引用的函数的声明
	2、一些公用的宏定义（#define）和全局只读（const）和全局变量声明
	3、可以参考 cmd_console_tools.h 的写法（后续更新的同学请注意多次声明）
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
void move_disk_animation(int disk_id, int from_col, int to_col);
void print_arrays_horizontal(int start_x, int start_y);
void print_towers_vertical(int start_x, int start_y);
void print_towers_vertical_no_line(int start_x, int start_y);
void draw_disk_helper(int disk_id, int x, int y, int color_rgb);
