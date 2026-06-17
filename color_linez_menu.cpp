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
        cout << "是否重新输入？输入 Y 继续，其他返回: ";
        cin >> setw(7) >> again;
        if (again[0] != 'Y' && again[0] != 'y')
            return 0;
    }
    return 1;
}

void linez_menu_a(void)
{
    LinezGame game;
    int rows;
    int cols;
    if (!ask_size_or_return(rows, cols))
        return;
    make_game(game, rows, cols);
    cout << endl << "初始五个彩球:" << endl;
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
    linez_print_board(game);
    if (!linez_read_position("起点", rows, cols, sr, sc) ||
        !linez_read_position("终点", rows, cols, dr, dc)) {
        linez_wait_end();
        return;
    }
    if (linez_find_path(game, sr, sc, dr, dc, path))
        linez_print_path(path);
    else
        cout << "没有找到可达路径。" << endl;
    linez_wait_end();
}

static void after_successful_move(LinezGame &game)
{
    int score_gain;
    int removed = linez_remove_lines(game, score_gain);
    if (removed > 0) {
        cout << "消除了 " << removed << " 个彩球，得分 +" << score_gain << "。" << endl;
        return;
    }
    linez_place_random_balls(game, LINEZ_NEXT_BALLS, game.next_colors);
    linez_remove_lines(game, score_gain);
    linez_prepare_next(game);
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
        cout << endl << "当前得分: " << game.score << endl;
        linez_print_board(game);
        int ret = linez_read_position_or_quit("起点", rows, cols, sr, sc);
        if (ret == 0)
            break;
        if (ret < 0)
            continue;
        ret = linez_read_position_or_quit("终点", rows, cols, dr, dc);
        if (ret == 0)
            break;
        if (ret < 0)
            continue;
        if (!linez_move_ball(game, sr, sc, dr, dc, path)) {
            cout << "移动无效。" << endl;
            continue;
        }
        linez_print_path(path);
        after_successful_move(game);
    }
    if (linez_is_game_over(game))
        cout << "游戏结束。最终得分: " << game.score << endl;
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
    linez_graph_setup();
    linez_draw_frame(game, 0);
    linez_draw_board(game, 0);
    linez_pause("");
}

void linez_menu_e(void)
{
    LinezGame game;
    int rows;
    int cols;
    if (!ask_size_or_return(rows, cols))
        return;
    make_game(game, rows, cols);
    linez_graph_setup();
    linez_draw_frame(game, 1);
    linez_draw_board(game, 1);
    linez_pause("");
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
    cout << "游戏结束或已退出。得分: " << game.score << endl;
    linez_wait_end();
}

static void show_menu(void)
{
    cct_setcolor();
    cout << endl;
    cout << "================ 彩球游戏 ================" << endl;
    cout << "A. 随机生成五个彩球并打印内部数组" << endl;
    cout << "B. 生成百分之六十的彩球并查找路径" << endl;
    cout << "C. 内部数组方式的完整游戏" << endl;
    cout << "D. 伪图形界面框架（无分隔线）" << endl;
    cout << "E. 伪图形界面框架（有分隔线）" << endl;
    cout << "F. 伪图形界面生成百分之六十彩球并移动一次" << endl;
    cout << "G. 伪图形界面完整游戏" << endl;
    cout << "Q. 退出" << endl;
    cout << "请选择: ";
}

void linez_run_menu(void)
{
    char choice;
    do {
        show_menu();
        cin >> choice;
        choice = char(toupper((unsigned char)choice));
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
