/* 7-b2.cpp：弹出式菜单函数实现 */
#include <iostream>
#include <conio.h>
using namespace std;

#include "cmd_console_tools.h"
#include "7-b2.h"

#define KEY_ESC 27
#define KEY_ENTER 13
#define KEY_PREFIX_1 0
#define KEY_PREFIX_2 224
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_HOME 71
#define KEY_END 79
#define KEY_PAGE_UP 73
#define KEY_PAGE_DOWN 81
#define MAX_MENU_WIDTH 1000
#define MAX_LINE_LEN 1008

static int is_gb2312_byte(const unsigned char ch)
{
    return ch >= 0xA1 && ch <= 0xFE;
}

/* 计算字符串在控制台中的显示宽度，GB2312 汉字按 2 列处理 */
static int display_width(const char str[])
{
    int width = 0;
    int i = 0;

    if (str == NULL)
        return 0;

    while (str[i] != '\0') {
        unsigned char ch = (unsigned char)str[i];

        if (is_gb2312_byte(ch) && str[i + 1] != '\0' && is_gb2312_byte((unsigned char)str[i + 1])) {
            width += 2;
            i += 2;
        }
        else {
            ++width;
            ++i;
        }
    }

    return width;
}

static void put_spaces(char dest[], int *pos, const int count)
{
    int i;

    for (i = 0; i < count; ++i) {
        dest[*pos] = ' ';
        ++(*pos);
    }
}

/* 将字符串复制到指定宽度内，遇到半个汉字位置时停止复制 */
static void append_text_fit(char dest[], int *pos, int *used_width, const int max_width, const char src[])
{
    int i = 0;

    if (src == NULL)
        return;

    while (src[i] != '\0' && *used_width < max_width) {
        unsigned char ch = (unsigned char)src[i];

        if (is_gb2312_byte(ch) && src[i + 1] != '\0' && is_gb2312_byte((unsigned char)src[i + 1])) {
            if (*used_width + 2 > max_width)
                break;

            dest[*pos] = src[i];
            dest[*pos + 1] = src[i + 1];
            *pos += 2;
            *used_width += 2;
            i += 2;
        }
        else {
            dest[*pos] = src[i];
            ++(*pos);
            ++(*used_width);
            ++i;
        }
    }
}

/* 生成定宽菜单内容行，不足部分补空格 */
static void make_fit_line(char dest[], const char src[], const int width)
{
    int pos = 0;
    int used_width = 0;

    append_text_fit(dest, &pos, &used_width, width, src);
    put_spaces(dest, &pos, width - used_width);
    dest[pos] = '\0';
}

/* 追加一个中文制表符，中文制表符在 GBK/GB2312 下占 2 字节 */
static void append_table_char(char dest[], int *pos, const char table_char[])
{
    dest[*pos] = table_char[0];
    dest[*pos + 1] = table_char[1];
    *pos += 2;
}

/* 生成带居中标题的上边框 */
static void make_top_line(char dest[], const char title[], const int width)
{
    static const char left_top[] = "\xA9\xB0";
    static const char right_top[] = "\xA9\xB4";
    static const char horizontal[] = "\xA9\xA4";
    char middle[MAX_LINE_LEN];
    char title_part[MAX_LINE_LEN];
    int title_width;
    int title_print_width;
    int title_pos;
    int pos = 0;
    int i;

    append_table_char(dest, &pos, left_top);

    for (i = 0; i < width / 2; ++i) {
        middle[i * 2] = horizontal[0];
        middle[i * 2 + 1] = horizontal[1];
    }
    middle[width] = '\0';

    title_width = display_width(title);
    title_print_width = title_width;
    if (title_print_width % 2 != 0)
        ++title_print_width;
    if (title_print_width > width)
        title_print_width = width;

    make_fit_line(title_part, title, title_print_width);
    title_pos = (width - title_print_width) / 2;
    for (i = 0; i < title_print_width; ++i)
        middle[title_pos + i] = title_part[i];

    for (i = 0; i < width; ++i) {
        dest[pos] = middle[i];
        ++pos;
    }

    append_table_char(dest, &pos, right_top);
    dest[pos] = '\0';
}

/* 生成下边框 */
static void make_bottom_line(char dest[], const int width)
{
    static const char left_bottom[] = "\xA9\xB8";
    static const char right_bottom[] = "\xA9\xBC";
    static const char horizontal[] = "\xA9\xA4";
    int pos = 0;
    int i;

    append_table_char(dest, &pos, left_bottom);
    for (i = 0; i < width / 2; ++i)
        append_table_char(dest, &pos, horizontal);
    append_table_char(dest, &pos, right_bottom);
    dest[pos] = '\0';
}

static void print_at(const int x, const int y, const char str[], const int bg_color, const int fg_color)
{
    cct_gotoxy(x, y);
    cct_setcolor(bg_color, fg_color);
    cout << str;
}

static void print_menu_row(const int x, const int y, const char content[], const int selected, const struct PopMenu *para)
{
    static const char vertical[] = "\xA9\xA6";

    cct_gotoxy(x, y);
    cct_setcolor(para->bg_color, para->fg_color);
    cout << vertical;

    if (selected)
        cct_setcolor(para->fg_color, para->bg_color);
    else
        cct_setcolor(para->bg_color, para->fg_color);
    cout << content;

    cct_setcolor(para->bg_color, para->fg_color);
    cout << vertical;
}

/* 统计菜单项数量，NULL 和空串都会使首字节为 '\0' */
static int menu_item_count(const char menu[][MAX_ITEM_LEN])
{
    int count = 0;

    while (menu[count][0] != '\0')
        ++count;

    return count;
}

/* 调整菜单宽度：保证偶数、至少能显示标题，并限制在内部缓冲区可承受范围内 */
static int adjusted_width(const struct PopMenu *para)
{
    int width = para->width;
    int title_width = display_width(para->title);

    if (width < title_width)
        width = title_width;
    if (width % 2 != 0)
        ++width;
    if (width < 2)
        width = 2;
    if (width > MAX_MENU_WIDTH)
        width = MAX_MENU_WIDTH;

    return width;
}

/* 调整菜单高度，至少保留一行内容区 */
static int adjusted_high(const struct PopMenu *para)
{
    if (para->high < 1)
        return 1;

    return para->high;
}

/* 按当前滚动位置和选中项重画菜单 */
static void draw_pop_menu(const char menu[][MAX_ITEM_LEN], const struct PopMenu *para, const int count, const int top, const int selected)
{
    char line[MAX_LINE_LEN];
    char content[MAX_LINE_LEN];
    int width = adjusted_width(para);
    int high = adjusted_high(para);
    int row;
    int item_index;

    make_top_line(line, para->title, width);
    print_at(para->start_x, para->start_y, line, para->bg_color, para->fg_color);

    for (row = 0; row < high; ++row) {
        item_index = top + row;
        if (item_index < count)
            make_fit_line(content, menu[item_index], width);
        else
            make_fit_line(content, "", width);

        print_menu_row(para->start_x, para->start_y + row + 1, content, item_index == selected, para);
    }

    make_bottom_line(line, width);
    print_at(para->start_x, para->start_y + high + 1, line, para->bg_color, para->fg_color);
}

/* 保证选中项始终处于当前可见区域内 */
static void keep_selected_visible(int *top, const int selected, const int high)
{
    if (selected < *top)
        *top = selected;
    else if (selected >= *top + high)
        *top = selected - high + 1;

    if (*top < 0)
        *top = 0;
}

/***************************************************************************
  函数名称：pop_menu
  功    能：绘制弹出式菜单并返回选中的菜单项序号，未选择则返回 0
***************************************************************************/
int pop_menu(const char menu[][MAX_ITEM_LEN], const struct PopMenu *para)
{
    int count;
    int high;
    int selected = 0;
    int top = 0;
    int key;

    if (menu == NULL || para == NULL)
        return 0;

    count = menu_item_count(menu);
    high = adjusted_high(para);

    draw_pop_menu(menu, para, count, top, selected);
    if (count <= 0) {
        cct_setcolor();
        return 0;
    }

    for (;;) {
        key = _getch();

        if (key == KEY_ENTER) {
            cct_setcolor();
            return selected + 1;
        }
        if (key == KEY_ESC) {
            cct_setcolor();
            return 0;
        }
        if (key == KEY_PREFIX_1 || key == KEY_PREFIX_2) {
            key = _getch();

            switch (key) {
                case KEY_UP:
                    if (selected > 0)
                        --selected;
                    else
                        selected = count - 1;
                    break;
                case KEY_DOWN:
                    if (selected < count - 1)
                        ++selected;
                    else
                        selected = 0;
                    break;
                case KEY_HOME:
                    selected = 0;
                    break;
                case KEY_END:
                    selected = count - 1;
                    break;
                case KEY_PAGE_UP:
                    selected -= high;
                    if (selected < 0)
                        selected = 0;
                    break;
                case KEY_PAGE_DOWN:
                    selected += high;
                    if (selected >= count)
                        selected = count - 1;
                    break;
                default:
                    break;
            }

            keep_selected_visible(&top, selected, high);
            draw_pop_menu(menu, para, count, top, selected);
        }
    }
}
