/* 学号 班级 姓名*/
#pragma once

/* 定义POP型菜单需要的结构体 */
struct PopMenu {
    const char *title;  /* 标题 */
    int start_x;        /* 左上角x坐标 */
    int start_y;        /* 左上角y坐标 */
    int width;          /* 菜单宽度，包含左右边框 */
    int high;           /* 菜单高度，包含上下边框 */
    int bg_color;       /* 背景色 */
    int fg_color;       /* 前景色 */
};

#define MAX_ITEM_LEN 80

int pop_menu(const char menu[][MAX_ITEM_LEN], const struct PopMenu *para);
