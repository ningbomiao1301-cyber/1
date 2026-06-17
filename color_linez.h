/* class student name */
#pragma once

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cctype>
#include <ctime>
#include <cstdio>

#ifdef _WIN32
#include <conio.h>
#include <Windows.h>
#include "cmd_console_tools.h"
#else
#include <termios.h>
#include <unistd.h>
#define COLOR_BLACK     0
#define COLOR_BLUE      1
#define COLOR_GREEN     2
#define COLOR_CYAN      3
#define COLOR_RED       4
#define COLOR_PINK      5
#define COLOR_YELLOW    6
#define COLOR_WHITE     7
#define COLOR_HBLACK    8
#define COLOR_HBLUE     9
#define COLOR_HGREEN    10
#define COLOR_HCYAN     11
#define COLOR_HRED      12
#define COLOR_HPINK     13
#define COLOR_HYELLOW   14
#define COLOR_HWHITE    15
#define CCT_MOUSE_EVENT 0
#define CCT_KEYBOARD_EVENT 1
#define MOUSE_NO_ACTION 0x0000
#define MOUSE_ONLY_MOVED 0x0001
#define MOUSE_LEFT_BUTTON_CLICK 0x0002
#define MOUSE_RIGHT_BUTTON_CLICK 0x0008
#define CCT_CURSOR_INVISIBLE 3
#define CCT_CURSOR_VISIBLE_NORMAL 2
inline void cct_cls(void) { std::cout << "\033[2J\033[H"; }
inline void cct_setcolor(const int bg_color = COLOR_BLACK, const int fg_color = COLOR_WHITE)
{
    (void)bg_color;
    (void)fg_color;
}
inline void cct_gotoxy(const int X, const int Y) { std::cout << "\033[" << (Y + 1) << ';' << (X + 1) << 'H'; }
inline void cct_setcursor(const int options) { (void)options; }
inline void cct_showch(const int X, const int Y, const char ch,
                       const int bg_color = COLOR_BLACK, const int fg_color = COLOR_WHITE, const int rpt = 1)
{
    cct_gotoxy(X, Y);
    cct_setcolor(bg_color, fg_color);
    for (int i = 0; i < rpt; i++)
        std::cout << ch;
}
inline void cct_showstr(const int X, const int Y, const char *str,
                        const int bg_color = COLOR_BLACK, const int fg_color = COLOR_WHITE,
                        int rpt = 1, int max_len = -1)
{
    cct_gotoxy(X, Y);
    cct_setcolor(bg_color, fg_color);
    if (str == NULL)
        str = "";
    for (int i = 0; i < rpt; i++)
        std::cout << str;
    if (max_len > 0) {
        int len = int(std::strlen(str)) * rpt;
        while (len++ < max_len)
            std::cout << ' ';
    }
}
inline void cct_showint(const int X, const int Y, const int num,
                        const int bg_color = COLOR_BLACK, const int fg_color = COLOR_WHITE, const int rpt = 1)
{
    cct_gotoxy(X, Y);
    cct_setcolor(bg_color, fg_color);
    for (int i = 0; i < rpt; i++)
        std::cout << num;
}
inline void cct_setconsoleborder(int set_cols, int set_lines, int set_buffer_cols = -1, int set_buffer_lines = -1)
{
    (void)set_cols;
    (void)set_lines;
    (void)set_buffer_cols;
    (void)set_buffer_lines;
}
inline void cct_getconsoleborder(int &cols, int &lines, int &buffer_cols, int &buffer_lines)
{
    cols = 80;
    lines = 28;
    buffer_cols = 80;
    buffer_lines = 200;
}
inline void cct_setfontsize(const char *fontname, const int high, const int width = 0)
{
    (void)fontname;
    (void)high;
    (void)width;
}
inline void cct_delay(const int ms) { (void)ms; }
inline void cct_enable_mouse(void) {}
inline void cct_disable_mouse(void) {}
inline int cct_read_keyboard_and_mouse(int &MX, int &MY, int &MAction, int &keycode1, int &keycode2)
{
    (void)MX;
    (void)MY;
    MAction = MOUSE_NO_ACTION;
    keycode1 = std::getchar();
    keycode2 = 0;
    return CCT_KEYBOARD_EVENT;
}
inline int _getch(void)
{
    if (!isatty(STDIN_FILENO)) {
        int ch = std::getchar();
        return (ch == '\n') ? '\r' : ch;
    }
    termios old_term;
    termios new_term;
    int ch;
    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;
    new_term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
    ch = std::getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
    return (ch == '\n') ? '\r' : ch;
}
#endif

using namespace std;

const int LINEZ_MIN_SIZE = 7;
const int LINEZ_MAX_SIZE = 9;
const int LINEZ_COLOR_COUNT = 7;
const int LINEZ_INIT_BALLS = 5;
const int LINEZ_NEXT_BALLS = 3;
const int LINEZ_WIN_LENGTH = 5;
const int LINEZ_MAX_CELLS = LINEZ_MAX_SIZE * LINEZ_MAX_SIZE;
const int LINEZ_GRAPH_X = 2;
const int LINEZ_GRAPH_Y = 3;
const int LINEZ_CELL_W = 4;
const int LINEZ_CELL_H = 2;

struct LinezRandom {
    unsigned int value;
};

struct LinezPath {
    int row[LINEZ_MAX_CELLS];
    int col[LINEZ_MAX_CELLS];
    int count;
};

struct LinezGame {
    int rows;
    int cols;
    int board[LINEZ_MAX_SIZE][LINEZ_MAX_SIZE];
    int score;
    int next_colors[LINEZ_NEXT_BALLS];
    int eliminated[LINEZ_COLOR_COUNT + 1];
    int preview;
    int stats;
    LinezRandom random;
};

/* color_linez_tools.cpp */
void linez_init_random(LinezRandom &random);
int linez_random_int(LinezRandom &random, int limit);
int linez_random_color(LinezRandom &random);
int linez_color_to_fg(int color);
const char *linez_color_name(int color);
const char *linez_frame_symbol(int index);
void linez_pause(const char *prompt);
void linez_wait_end(void);
int linez_input_size(int &rows, int &cols);
int linez_read_position(const char *prompt, int rows, int cols, int &row, int &col);
int linez_read_position_or_quit(const char *prompt, int rows, int cols, int &row, int &col);
void linez_print_coord(int row, int col);
void linez_print_board(const LinezGame &game);
void linez_print_board_with_path(const LinezGame &game, const LinezPath &path, const char *title);
void linez_print_path(const LinezPath &path);
void linez_show_message(int x, int y, const char *message);

/* color_linez_base.cpp */
void linez_set_size(LinezGame &game, int rows, int cols, unsigned int seed);
void linez_clear_board(LinezGame &game);
void linez_prepare_next(LinezGame &game);
void linez_init_game(LinezGame &game, int rows, int cols, unsigned int seed);
int linez_inside(const LinezGame &game, int row, int col);
int linez_count_empty(const LinezGame &game);
int linez_place_random_balls(LinezGame &game, int count, const int colors[]);
void linez_fill_percent(LinezGame &game, int percent);
int linez_find_path(const LinezGame &game, int sr, int sc, int dr, int dc, LinezPath &path);
int linez_move_ball(LinezGame &game, int sr, int sc, int dr, int dc, LinezPath &path);
int linez_remove_lines(LinezGame &game, int &score_gain);
int linez_is_game_over(const LinezGame &game);

/* color_linez_graph.cpp */
void linez_graph_setup(void);
void linez_draw_frame(const LinezGame &game, int with_grid);
void linez_draw_board(const LinezGame &game, int with_grid);
void linez_draw_cell(const LinezGame &game, int row, int col, int selected, int with_grid);
void linez_draw_status(const LinezGame &game);
void linez_animate_path(const LinezGame &game, const LinezPath &path, int color, int with_grid);
int linez_mouse_to_cell(const LinezGame &game, int mx, int my, int &row, int &col);
int linez_get_graphical_selection(const LinezGame &game, int &row, int &col, int &key);

/* color_linez_menu.cpp */
void linez_run_menu(void);
void linez_menu_a(void);
void linez_menu_b(void);
void linez_menu_c(void);
void linez_menu_d(void);
void linez_menu_e(void);
void linez_menu_f(void);
void linez_menu_g(void);
