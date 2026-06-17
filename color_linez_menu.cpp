/* class student name */
#include "color_linez.h"

static void make_game(LinezGame &game, int rows, int cols)
{
    LinezRandom random;
    linez_init_random(random);
    linez_init_game(game, rows, cols, random.value);
}

static int ask_size_or_return(int &rows, int &cols)
{
    while (!linez_input_size(rows, cols)) {
        char again[8];
        cout << "输入错误，是否重新输入？输入1重新输入，输入其它字符返回菜单：";
        cin >> setw(7) >> again;
        if (again[0] != '1' && again[0] != 'Y' && again[0] != 'y')
            return 0;
    }
    return 1;
}

static char read_menu_choice(void)
{
    int ch;
    do {
        ch = _getch();
    } while (ch == '\r' || ch == '\n');
    if (ch >= 'a' && ch <= 'z')
        ch = ch - 'a' + 'A';
    cout << char(ch) << endl;
    return char(ch);
}

static void print_next_colors(const LinezGame &game)
{
    cout << endl << "下3个彩球的颜色分别是：";
    for (int i = 0; i < LINEZ_NEXT_BALLS; i++)
        cout << setw(3) << game.next_colors[i];
    cout << endl;
}

void linez_menu_a(void)
{
    LinezGame game;
    int rows;
    int cols;
    if (!ask_size_or_return(rows, cols))
        return;
    make_game(game, rows, cols);
    cout << endl << endl << "初始数组：" << endl;
    linez_print_board(game);
    linez_wait_end();
}

void linez_menu_b(void)
{
    LinezGame game;
    LinezPath path;
    int rows;
    int cols;
    int sr;
    int sc;
    int dr;
    int dc;
    if (!ask_size_or_return(rows, cols))
        return;
    make_game(game, rows, cols);
    linez_fill_percent(game, 60);
    cout << endl << endl << "当前数组：" << endl;
    linez_print_board(game);
    print_next_colors(game);
    if (!linez_read_position("请以字母+数字形式[例： c2]输入要移动球的矩阵坐标：", rows, cols, sr, sc) ||
        !linez_read_position("请以字母+数字形式[例： c2]输入要移动球的目的坐标：", rows, cols, dr, dc)) {
        linez_wait_end();
        return;
    }
    cout << "输入为" << char('A' + sr) << "行" << (sc + 1) << "列" << endl;
    cout << "目标为" << char('A' + dr) << "行" << (dc + 1) << "列" << endl;
    if (linez_find_path(game, sr, sc, dr, dc, path)) {
        linez_print_board_with_path(game, path, "查找结果数组：");
        linez_print_board_with_path(game, path, "移动路径（不同色标识）：");
        linez_print_path(path);
    }
    else
        cout << "没有找到可达路径。" << endl;
    linez_wait_end();
}

static int after_successful_move(LinezGame &game)
{
    int score_gain;
    int removed = linez_remove_lines(game, score_gain);
    if (removed > 0) {
        cout << "消除了 " << removed << " 个彩球，得分 +" << score_gain << "。" << endl;
        return score_gain;
    }
    linez_place_random_balls(game, LINEZ_NEXT_BALLS, game.next_colors);
    linez_remove_lines(game, score_gain);
    linez_prepare_next(game);
    return score_gain;
}

void linez_menu_c(void)
{
    LinezGame game;
    int rows;
    int cols;
    if (!ask_size_or_return(rows, cols))
        return;
    make_game(game, rows, cols);
    while (!linez_is_game_over(game)) {
        int sr;
        int sc;
        int dr;
        int dc;
        LinezPath path;
        cout << endl << "当前数组：" << endl;
        linez_print_board(game);
        print_next_colors(game);
        int ret = linez_read_position_or_quit("请以字母+数字形式[例： c2]输入要移动球的矩阵坐标：", rows, cols, sr, sc);
        if (ret == 0)
            break;
        if (ret < 0)
            continue;
        cout << "输入为" << char('A' + sr) << "行" << (sc + 1) << "列" << endl;
        ret = linez_read_position_or_quit("请以字母+数字形式[例： c2]输入要移动球的目的坐标：", rows, cols, dr, dc);
        if (ret == 0)
            break;
        if (ret < 0)
            continue;
        cout << "输入为" << char('A' + dr) << "行" << (dc + 1) << "列" << endl;
        if (!linez_move_ball(game, sr, sc, dr, dc, path)) {
            cout << "移动无效。" << endl;
            continue;
        }
        cout << endl << "移动后的数组（不同色标识）：" << endl;
        linez_print_board(game);
        int score_gain = after_successful_move(game);
        cout << "本次得分：" << score_gain << "  总得分：" << game.score << endl;
    }
    if (linez_is_game_over(game))
        cout << "游戏结束。最终得分：" << game.score << endl;
    linez_wait_end();
}

void linez_menu_d(void)
{
    LinezGame game;
    int rows;
    int cols;
    if (!ask_size_or_return(rows, cols))
        return;
    make_game(game, rows, cols);
    cout << endl << endl << "初始数组：" << endl;
    linez_print_board(game);
    cout << endl << "按回车键显示图形...";
    while (_getch() != '\r')
        ;
    linez_graph_setup();
    linez_draw_frame(game, 0);
    linez_draw_board(game, 0);
    cct_gotoxy(0, 24);
    linez_wait_end();
}

void linez_menu_e(void)
{
    LinezGame game;
    int rows;
    int cols;
    if (!ask_size_or_return(rows, cols))
        return;
    make_game(game, rows, cols);
    cout << endl << endl << "初始数组：" << endl;
    linez_print_board(game);
    cout << endl << "按回车键显示图形...";
    while (_getch() != '\r')
        ;
    linez_graph_setup();
    linez_draw_frame(game, 1);
    linez_draw_board(game, 1);
    cct_gotoxy(0, 24);
    linez_wait_end();
}

static void redraw_graph_game(const LinezGame &game)
{
    linez_draw_frame(game, 1);
    linez_draw_board(game, 1);
    linez_draw_status(game);
}

void linez_menu_f(void)
{
    LinezGame game;
    int rows;
    int cols;
    int sr = -1;
    int sc = -1;
    int dr;
    int dc;
    int key;
    LinezPath path;
    if (!ask_size_or_return(rows, cols))
        return;
    make_game(game, rows, cols);
    linez_fill_percent(game, 60);
    linez_graph_setup();
    redraw_graph_game(game);
    cct_enable_mouse();
    while (1) {
        int ret = linez_get_graphical_selection(game, dr, dc, key);
        if (ret == 0)
            break;
        if (ret == 2)
            continue;
        if (sr < 0) {
            if (game.board[dr][dc] == 0) {
                linez_show_message(0, 2, "请先选择一个彩球。");
                continue;
            }
            sr = dr;
            sc = dc;
            linez_draw_cell(game, sr, sc, 1, 1);
        }
        else {
            int color = game.board[sr][sc];
            if (linez_move_ball(game, sr, sc, dr, dc, path)) {
                linez_animate_path(game, path, color, 1);
                redraw_graph_game(game);
                break;
            }
            linez_show_message(0, 2, "无通路或目标无效。");
            linez_draw_cell(game, sr, sc, 0, 1);
            sr = -1;
        }
    }
    cct_disable_mouse();
    cct_setcursor(CCT_CURSOR_VISIBLE_NORMAL);
    cct_gotoxy(0, 24);
    linez_wait_end();
}

static void graph_turn_after_move(LinezGame &game)
{
    int score_gain;
    int removed = linez_remove_lines(game, score_gain);
    if (removed == 0) {
        linez_place_random_balls(game, LINEZ_NEXT_BALLS, game.next_colors);
        linez_remove_lines(game, score_gain);
        linez_prepare_next(game);
    }
}

static void handle_graph_key(LinezGame &game, int key)
{
    if (key == '3' || key == 'p' || key == 'P')
        game.preview = !game.preview;
    if (key == '5' || key == 's' || key == 'S')
        game.stats = !game.stats;
}

void linez_menu_g(void)
{
    LinezGame game;
    int rows;
    int cols;
    int selected_r = -1;
    int selected_c = -1;
    if (!ask_size_or_return(rows, cols))
        return;
    make_game(game, rows, cols);
    linez_graph_setup();
    cct_enable_mouse();
    while (!linez_is_game_over(game)) {
        int row;
        int col;
        int key;
        redraw_graph_game(game);
        int ret = linez_get_graphical_selection(game, row, col, key);
        if (ret == 0)
            break;
        if (ret == 2) {
            handle_graph_key(game, key);
            continue;
        }
        if (selected_r < 0) {
            if (game.board[row][col] == 0) {
                linez_show_message(0, 2, "请单击一个彩球。");
                continue;
            }
            selected_r = row;
            selected_c = col;
            linez_draw_cell(game, selected_r, selected_c, 1, 1);
        }
        else {
            LinezPath path;
            int color = game.board[selected_r][selected_c];
            if (linez_move_ball(game, selected_r, selected_c, row, col, path)) {
                linez_animate_path(game, path, color, 1);
                graph_turn_after_move(game);
            }
            else {
                linez_show_message(0, 2, "移动失败。");
            }
            selected_r = -1;
            selected_c = -1;
        }
    }
    cct_disable_mouse();
    cct_setcursor(CCT_CURSOR_VISIBLE_NORMAL);
    cct_cls();
    cout << "游戏结束或已退出。得分：" << game.score << endl;
    linez_wait_end();
}

static void show_menu(void)
{
    cct_cls();
    cct_setcolor(COLOR_BLACK, COLOR_WHITE);
    cout << "------------------------------------------------" << endl;
    cout << "A.内部数组，随机生成初始5个球" << endl;
    cout << "B.内部数组，随机生成60%的球，寻找移动路径" << endl;
    cout << "C.内部数组，完整版" << endl;
    cout << "D.画出n*n的框架（无分隔线），随机显示5个球" << endl;
    cout << "E.画出n*n的框架（有分隔线），随机显示5个球" << endl;
    cout << "F.n*n的框架，60%的球，支持鼠标，完成一次移动" << endl;
    cout << "G.cmd图形界面完整版" << endl;
    cout << "Q.退出" << endl;
    cout << "------------------------------------------------" << endl;
    cout << "[请选择:] ";
}

void linez_run_menu(void)
{
    char choice;
    do {
        show_menu();
        choice = read_menu_choice();
        switch (choice) {
            case 'A':
                linez_menu_a();
                break;
            case 'B':
                linez_menu_b();
                break;
            case 'C':
                linez_menu_c();
                break;
            case 'D':
                linez_menu_d();
                break;
            case 'E':
                linez_menu_e();
                break;
            case 'F':
                linez_menu_f();
                break;
            case 'G':
                linez_menu_g();
                break;
            case 'Q':
                break;
            default:
                cout << "菜单选择无效。" << endl;
                break;
        }
    } while (choice != 'Q');
}
