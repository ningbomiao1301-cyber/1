/* class student name */
#include "color_linez.h"

void linez_graph_setup(void)
{
    cct_setfontsize("SimSun-ExtB", 28);
    cct_setconsoleborder(80, 28, 80, 200);
    cct_setcursor(CCT_CURSOR_INVISIBLE);
}

static int cell_center_x(int col)
{
    return LINEZ_GRAPH_X + 2 + col * LINEZ_CELL_W;
}

static int cell_center_y(int row)
{
    return LINEZ_GRAPH_Y + 1 + row * LINEZ_CELL_H;
}

static void draw_hline(int x, int y, int cells, int left, int mid, int right)
{
    cct_showstr(x, y, linez_frame_symbol(left), COLOR_BLACK, COLOR_HWHITE);
    x += 2;
    for (int c = 0; c < cells; c++) {
        cct_showstr(x, y, linez_frame_symbol(4), COLOR_BLACK, COLOR_HWHITE, 2);
        x += LINEZ_CELL_W;
        if (c == cells - 1)
            cct_showstr(x, y, linez_frame_symbol(right), COLOR_BLACK, COLOR_HWHITE);
        else
            cct_showstr(x, y, linez_frame_symbol(mid), COLOR_BLACK, COLOR_HWHITE);
    }
}

static void draw_no_grid_frame(const LinezGame &game)
{
    int width = game.cols * LINEZ_CELL_W;
    int height = game.rows * LINEZ_CELL_H;
    draw_hline(LINEZ_GRAPH_X, LINEZ_GRAPH_Y, game.cols, 0, 4, 1);
    for (int y = LINEZ_GRAPH_Y + 1; y < LINEZ_GRAPH_Y + height; y++) {
        cct_showstr(LINEZ_GRAPH_X, y, linez_frame_symbol(5), COLOR_BLACK, COLOR_HWHITE);
        cct_showstr(LINEZ_GRAPH_X + width + 2, y, linez_frame_symbol(5), COLOR_BLACK, COLOR_HWHITE);
    }
    draw_hline(LINEZ_GRAPH_X, LINEZ_GRAPH_Y + height, game.cols, 2, 4, 3);
}

static void draw_grid_frame(const LinezGame &game)
{
    int bottom = LINEZ_GRAPH_Y + game.rows * LINEZ_CELL_H;
    for (int r = 0; r <= game.rows; r++) {
        int y = LINEZ_GRAPH_Y + r * LINEZ_CELL_H;
        if (r == 0)
            draw_hline(LINEZ_GRAPH_X, y, game.cols, 0, 6, 1);
        else if (r == game.rows)
            draw_hline(LINEZ_GRAPH_X, bottom, game.cols, 2, 7, 3);
        else
            draw_hline(LINEZ_GRAPH_X, y, game.cols, 8, 10, 9);
        if (r < game.rows) {
            int content_y = y + 1;
            for (int c = 0; c <= game.cols; c++)
                cct_showstr(LINEZ_GRAPH_X + c * LINEZ_CELL_W, content_y,
                            linez_frame_symbol(5), COLOR_BLACK, COLOR_HWHITE);
        }
    }
}

void linez_draw_frame(const LinezGame &game, int with_grid)
{
    int cols;
    int lines;
    int buffer_cols;
    int buffer_lines;
    cct_cls();
    cct_getconsoleborder(cols, lines, buffer_cols, buffer_lines);
    cct_showstr(0, 0, "屏幕:", COLOR_BLACK, COLOR_HWHITE);
    cct_showint(8, 0, lines, COLOR_BLACK, COLOR_HWHITE);
    cct_showstr(12, 0, "行", COLOR_BLACK, COLOR_HWHITE);
    cct_showint(15, 0, cols, COLOR_BLACK, COLOR_HWHITE);
    cct_showstr(19, 0, "列", COLOR_BLACK, COLOR_HWHITE);
    if (with_grid)
        draw_grid_frame(game);
    else
        draw_no_grid_frame(game);
}

void linez_draw_cell(const LinezGame &game, int row, int col, int selected, int with_grid)
{
    int x = cell_center_x(col);
    int y = cell_center_y(row);
    int color = game.board[row][col];
    const char *text = "  ";
    int fg = COLOR_WHITE;
    (void)with_grid;
    if (color != 0) {
        text = selected ? "<>" : "()";
        fg = linez_color_to_fg(color);
    }
    cct_showstr(x, y, text, selected ? COLOR_HBLACK : COLOR_BLACK, fg, 1, 2);
}

void linez_draw_board(const LinezGame &game, int with_grid)
{
    for (int r = 0; r < game.rows; r++)
        for (int c = 0; c < game.cols; c++)
            linez_draw_cell(game, r, c, 0, with_grid);
}

void linez_draw_status(const LinezGame &game)
{
    int x = LINEZ_GRAPH_X + game.cols * LINEZ_CELL_W + 8;
    int y = LINEZ_GRAPH_Y;
    cct_showstr(x, y, "得分：", COLOR_BLACK, COLOR_HYELLOW, 1, 20);
    cct_showint(x + 8, y, game.score, COLOR_BLACK, COLOR_HYELLOW);
    y += 2;
    cct_showstr(x, y, "预告：", COLOR_BLACK, COLOR_HWHITE, 1, 20);
    if (game.preview) {
        for (int i = 0; i < LINEZ_NEXT_BALLS; i++)
            cct_showstr(x + i * 4, y + 1, "()", COLOR_BLACK, linez_color_to_fg(game.next_colors[i]), 1, 2);
    }
    else {
        cct_showstr(x, y + 1, "关闭      ", COLOR_BLACK, COLOR_HBLACK);
    }
    y += 4;
    cct_showstr(x, y, "统计：", COLOR_BLACK, COLOR_HWHITE, 1, 20);
    if (game.stats) {
        for (int i = 1; i <= LINEZ_COLOR_COUNT; i++) {
            cct_showstr(x, y + i, linez_color_name(i), COLOR_BLACK, linez_color_to_fg(i), 1, 8);
            cct_showint(x + 9, y + i, game.eliminated[i], COLOR_BLACK, COLOR_HWHITE);
        }
    }
    else {
        cct_showstr(x, y + 1, "关闭      ", COLOR_BLACK, COLOR_HBLACK);
    }
    cct_showstr(x, y + 10, "左键选择  右键退出", COLOR_BLACK, COLOR_HCYAN, 1, 24);
    cct_showstr(x, y + 11, "3键预告  5键统计", COLOR_BLACK, COLOR_HCYAN, 1, 24);
}

void linez_animate_path(const LinezGame &game, const LinezPath &path, int color, int with_grid)
{
    if (path.count <= 0)
        return;
    for (int i = 0; i < path.count; i++) {
        int r = path.row[i];
        int c = path.col[i];
        cct_showstr(cell_center_x(c), cell_center_y(r), "[]", COLOR_BLACK, linez_color_to_fg(color), 1, 2);
        cct_delay(80);
        if (i != path.count - 1)
            linez_draw_cell(game, r, c, 0, with_grid);
    }
}

int linez_mouse_to_cell(const LinezGame &game, int mx, int my, int &row, int &col)
{
    for (int r = 0; r < game.rows; r++) {
        for (int c = 0; c < game.cols; c++) {
            int cx = cell_center_x(c);
            int cy = cell_center_y(r);
            if (my == cy && mx >= cx && mx <= cx + 1) {
                row = r;
                col = c;
                return 1;
            }
        }
    }
    return 0;
}

int linez_get_graphical_selection(const LinezGame &game, int &row, int &col, int &key)
{
    int mx = 0;
    int my = 0;
    int action = 0;
    int key2 = 0;
    key = 0;
    while (1) {
        int ret = cct_read_keyboard_and_mouse(mx, my, action, key, key2);
        if (ret == CCT_KEYBOARD_EVENT) {
            if (key == 'q' || key == 'Q')
                return 0;
            return 2;
        }
        if (ret == CCT_MOUSE_EVENT) {
            int r;
            int c;
            if (linez_mouse_to_cell(game, mx, my, r, c)) {
                cct_showstr(0, 1, "", COLOR_BLACK, COLOR_WHITE, 1, 32);
                cct_showstr(0, 1, "当前格：行", COLOR_BLACK, COLOR_HWHITE, 1, 12);
                cct_showch(12, 1, char('A' + r), COLOR_BLACK, COLOR_HWHITE);
                cct_showstr(14, 1, "列", COLOR_BLACK, COLOR_HWHITE, 1, 4);
                cct_showch(18, 1, char('1' + c), COLOR_BLACK, COLOR_HWHITE);
                if (action == MOUSE_LEFT_BUTTON_CLICK) {
                    row = r;
                    col = c;
                    return 1;
                }
            }
            else {
                linez_show_message(0, LINEZ_GRAPH_Y + game.rows * LINEZ_CELL_H + 2, "[当前光标] 位置非法");
            }
            if (action == MOUSE_RIGHT_BUTTON_CLICK)
                return 0;
        }
    }
}
