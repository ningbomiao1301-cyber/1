/* 学号 班级 姓名*/

/* 1、如果需要包含头文件
   2、本源文件不允许定义全局变量，包括静态全局，必须使用const和define
   3、如果需要自定义结构体、函数等，应该只被本源文件使用 */

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
#define MAX_LINE_LEN 2208

static int is_gb2312_byte(const unsigned char ch)
{
    return ch >= 0xA1 && ch <= 0xFE;
}

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

static void make_fit_line(char dest[], const char src[], const int width)
{
    int pos = 0;
    int used_width = 0;

    append_text_fit(dest, &pos, &used_width, width, src);
    put_spaces(dest, &pos, width - used_width);
    dest[pos] = '\0';
}

static void append_table_char(char dest[], int *pos, const char table_char[])
{
    dest[*pos] = table_char[0];
    dest[*pos + 1] = table_char[1];
    *pos += 2;
}

static void make_top_line(char dest[], const char title[], const int width)
{
    static const char left_top[] = "\xA9\xB0";
    static const char right_top[] = "\xA9\xB4";
    static const char horizontal[] = "\xA9\xA4";
    char title_part[MAX_LINE_LEN];
    int title_width;
    int title_print_width;
    int left_count;
    int right_count;
    int pos = 0;
    int i;

    append_table_char(dest, &pos, left_top);

    title_width = display_width(title);
    title_print_width = title_width;
    if (title_print_width % 2 != 0)
        ++title_print_width;
    if (title_print_width > width)
        title_print_width = width;

    make_fit_line(title_part, title, title_print_width);
    left_count = (width - title_print_width) / 2;
    right_count = width - title_print_width - left_count;

    for (i = 0; i < left_count; ++i)
        append_table_char(dest, &pos, horizontal);

    for (i = 0; title_part[i] != '\0'; ++i) {
        dest[pos] = title_part[i];
        ++pos;
    }

    for (i = 0; i < right_count; ++i)
        append_table_char(dest, &pos, horizontal);

    append_table_char(dest, &pos, right_top);
    dest[pos] = '\0';
}

static void make_bottom_line(char dest[], const int width)
{
    static const char left_bottom[] = "\xA9\xB8";
    static const char right_bottom[] = "\xA9\xBC";
    static const char horizontal[] = "\xA9\xA4";
    int pos = 0;
    int i;

    append_table_char(dest, &pos, left_bottom);
    for (i = 0; i < width; ++i)
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

static int menu_item_count(const char menu[][MAX_ITEM_LEN])
{
    int count = 0;

    while (menu[count][0] != '\0')
        ++count;

    return count;
}

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

static int adjusted_high(const struct PopMenu *para)
{
    if (para->high < 1)
        return 1;

    return para->high;
}

static int display_line_step(const int start_x, const int width)
{
    int cols;
    int lines;
    int buffer_cols;
    int buffer_lines;
    int first_line_cols;
    int rest_cols;
    int step = 1;

    cct_getconsoleborder(cols, lines, buffer_cols, buffer_lines);
    if (buffer_cols <= 0)
        return 1;

    first_line_cols = buffer_cols - start_x;
    if (first_line_cols <= 0)
        first_line_cols = buffer_cols;

    if (width > first_line_cols) {
        rest_cols = width - first_line_cols;
        step += (rest_cols + buffer_cols - 1) / buffer_cols;
    }

    return step;
}

static void draw_pop_menu(const char menu[][MAX_ITEM_LEN], const struct PopMenu *para, const int count, const int top, const int selected)
{
    char line[MAX_LINE_LEN];
    char content[MAX_LINE_LEN];
    int width = adjusted_width(para);
    int item_rows = adjusted_high(para);
    int line_step = display_line_step(para->start_x, width + 2);
    int row;
    int item_index;

    make_top_line(line, line_step == 1 ? para->title : "", width);
    print_at(para->start_x, para->start_y, line, para->bg_color, para->fg_color);

    for (row = 0; row < item_rows; ++row) {
        item_index = top + row;
        if (item_index < count)
            make_fit_line(content, menu[item_index], width);
        else
            make_fit_line(content, "", width);

        print_menu_row(para->start_x, para->start_y + (row + 1) * line_step, content, item_index == selected, para);
    }

    make_bottom_line(line, width);
    print_at(para->start_x, para->start_y + (item_rows + 1) * line_step, line, para->bg_color, para->fg_color);
}

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
  函数名称：
  功    能：供外部直接调用的函数一般放在头文件中
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int pop_menu(const char menu[][MAX_ITEM_LEN], const struct PopMenu *para)
{
    int count;
    int item_rows;
    int selected = 0;
    int top = 0;
    int key;

    if (menu == NULL || para == NULL)
        return 0;

    count = menu_item_count(menu);
    item_rows = adjusted_high(para);

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
                    selected -= item_rows;
                    if (selected < 0)
                        selected = 0;
                    break;
                case KEY_PAGE_DOWN:
                    selected += item_rows;
                    if (selected >= count)
                        selected = count - 1;
                    break;
                default:
                    break;
            }

            keep_selected_visible(&top, selected, item_rows);
            draw_pop_menu(menu, para, count, top, selected);
        }
    }
}
