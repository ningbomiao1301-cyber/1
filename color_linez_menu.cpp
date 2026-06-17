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
        cout << "Try again? (Y/N): ";
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
    cout << endl << "Initial five balls:" << endl;
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
    if (!linez_read_position("Source", rows, cols, sr, sc) ||
        !linez_read_position("Destination", rows, cols, dr, dc)) {
        linez_wait_end();
        return;
    }
    if (linez_find_path(game, sr, sc, dr, dc, path))
        linez_print_path(path);
    else
        cout << "No path can be found." << endl;
    linez_wait_end();
}

static void after_successful_move(LinezGame &game)
{
    int score_gain;
    int removed = linez_remove_lines(game, score_gain);
    if (removed > 0) {
        cout << "Removed " << removed << " balls, score +" << score_gain << "." << endl;
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
        cout << endl << "Score: " << game.score << endl;
        linez_print_board(game);
        int ret = linez_read_position_or_quit("Source", rows, cols, sr, sc);
        if (ret == 0)
            break;
        if (ret < 0)
            continue;
        ret = linez_read_position_or_quit("Destination", rows, cols, dr, dc);
        if (ret == 0)
            break;
        if (ret < 0)
            continue;
        if (!linez_move_ball(game, sr, sc, dr, dc, path)) {
            cout << "Invalid move." << endl;
            continue;
        }
        linez_print_path(path);
        after_successful_move(game);
    }
    if (linez_is_game_over(game))
        cout << "Game over. Final score: " << game.score << endl;
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
                linez_show_message(0, 2, "Choose a ball first.");
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
            linez_show_message(0, 2, "No path or invalid target.");
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
                linez_show_message(0, 2, "Click a ball.");
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
                linez_show_message(0, 2, "Move failed.");
            }
            selected_r = -1;
            selected_c = -1;
        }
    }
    cct_disable_mouse();
    cct_setcursor(CCT_CURSOR_VISIBLE_NORMAL);
    cct_cls();
    cout << "Game over or exited. Score: " << game.score << endl;
    linez_wait_end();
}

static void show_menu(void)
{
    cct_setcolor();
    cout << endl;
    cout << "================ Color Linez ================" << endl;
    cout << "A. random five balls and print internal board" << endl;
    cout << "B. generate 60 percent balls and find path" << endl;
    cout << "C. full game by internal board" << endl;
    cout << "D. graph frame without separators" << endl;
    cout << "E. graph frame with separators" << endl;
    cout << "F. graph 60 percent board and one mouse move" << endl;
    cout << "G. full graph game" << endl;
    cout << "Q. quit" << endl;
    cout << "Select: ";
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
                cout << "Invalid choice." << endl;
                break;
        }
    } while (choice != 'Q');
}
