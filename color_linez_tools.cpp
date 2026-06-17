/* class student name */
#include "color_linez.h"

void linez_init_random(LinezRandom &random)
{
    unsigned int seed = (unsigned int)time(0);
    if (seed == 0)
        seed = 2463534242U;
    random.value = seed ^ 0x9E3779B9U;
}

int linez_random_int(LinezRandom &random, int limit)
{
    if (limit <= 0)
        return 0;
    random.value = random.value * 1103515245U + 12345U;
    return int((random.value / 65536U) % (unsigned int)limit);
}

int linez_random_color(LinezRandom &random)
{
    return linez_random_int(random, LINEZ_COLOR_COUNT) + 1;
}

int linez_color_to_fg(int color)
{
    switch (color) {
        case 1:
            return COLOR_HRED;
        case 2:
            return COLOR_HGREEN;
        case 3:
            return COLOR_HBLUE;
        case 4:
            return COLOR_HYELLOW;
        case 5:
            return COLOR_HPINK;
        case 6:
            return COLOR_HCYAN;
        case 7:
            return COLOR_WHITE;
        default:
            return COLOR_WHITE;
    }
}

const char *linez_color_name(int color)
{
    switch (color) {
        case 1:
            return "Red";
        case 2:
            return "Green";
        case 3:
            return "Blue";
        case 4:
            return "Yellow";
        case 5:
            return "Pink";
        case 6:
            return "Cyan";
        case 7:
            return "White";
        default:
            return "None";
    }
}

const char *linez_frame_symbol(int index)
{
    switch (index) {
        case 0:
            return "\xA9\xB0"; /* top-left */
        case 1:
            return "\xA9\xB4"; /* top-right */
        case 2:
            return "\xA9\xB8"; /* bottom-left */
        case 3:
            return "\xA9\xBC"; /* bottom-right */
        case 4:
            return "\xA9\xA4"; /* horizontal */
        case 5:
            return "\xA9\xA6"; /* vertical */
        case 6:
            return "\xA9\xD0"; /* top joint */
        case 7:
            return "\xA9\xD8"; /* bottom joint */
        case 8:
            return "\xA9\xC0"; /* left joint */
        case 9:
            return "\xA9\xC8"; /* right joint */
        case 10:
            return "\xA9\xE0"; /* cross */
        default:
            return " ";
    }
}

void linez_pause(const char *prompt)
{
    cct_setcolor();
    if (prompt != NULL)
        cout << prompt;
    cout << " Press Enter to continue...";
    while (_getch() != '\r')
        ;
    cout << endl;
}

void linez_wait_end(void)
{
    char word[32];
    cout << endl << "Input End to return menu: ";
    do {
        cin >> setw(31) >> word;
    } while (strcmp(word, "End") != 0);
}

int linez_input_size(int &rows, int &cols)
{
    cout << "Input rows and columns (7-9): ";
    cin >> rows >> cols;
    if (!cin) {
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "Invalid input." << endl;
        return 0;
    }
    if (rows < LINEZ_MIN_SIZE || rows > LINEZ_MAX_SIZE || cols < LINEZ_MIN_SIZE || cols > LINEZ_MAX_SIZE) {
        cout << "Rows and columns must be in [7,9]." << endl;
        return 0;
    }
    return 1;
}

static int parse_row_char(char ch)
{
    if (ch >= 'a' && ch <= 'z')
        ch = char(ch - 'a' + 'A');
    if (ch < 'A' || ch > 'I')
        return -1;
    return ch - 'A';
}

int linez_read_position(const char *prompt, int rows, int cols, int &row, int &col)
{
    char row_ch;
    int col_no;
    cout << prompt << " (row A-I, col 1-9): ";
    cin >> row_ch >> col_no;
    if (!cin) {
        cin.clear();
        cin.ignore(1024, '\n');
        return 0;
    }
    row = parse_row_char(row_ch);
    col = col_no - 1;
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        cout << "Position out of board." << endl;
        return 0;
    }
    return 1;
}

int linez_read_position_or_quit(const char *prompt, int rows, int cols, int &row, int &col)
{
    char row_ch;
    int col_no;
    cout << prompt << " (Q 0 to quit): ";
    cin >> row_ch >> col_no;
    if (!cin) {
        cin.clear();
        cin.ignore(1024, '\n');
        return -1;
    }
    if (row_ch == 'Q' || row_ch == 'q')
        return 0;
    row = parse_row_char(row_ch);
    col = col_no - 1;
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        cout << "Position out of board." << endl;
        return -1;
    }
    return 1;
}

void linez_print_coord(int row, int col)
{
    cout << char('A' + row) << (col + 1);
}

void linez_print_board(const LinezGame &game)
{
    cout << "    ";
    for (int c = 0; c < game.cols; c++)
        cout << setw(3) << (c + 1);
    cout << endl;
    for (int r = 0; r < game.rows; r++) {
        cout << " " << char('A' + r) << "  ";
        for (int c = 0; c < game.cols; c++) {
            if (game.board[r][c] == 0) {
                cct_setcolor();
                cout << setw(3) << '.';
            }
            else {
                cct_setcolor(COLOR_BLACK, linez_color_to_fg(game.board[r][c]));
                cout << setw(3) << game.board[r][c];
                cct_setcolor();
            }
        }
        cout << endl;
    }
    cct_setcolor();
}

void linez_print_path(const LinezPath &path)
{
    if (path.count <= 0) {
        cout << "No path." << endl;
        return;
    }
    cout << "Path: ";
    for (int i = 0; i < path.count; i++) {
        linez_print_coord(path.row[i], path.col[i]);
        if (i != path.count - 1)
            cout << " -> ";
    }
    cout << endl;
}

void linez_show_message(int x, int y, const char *message)
{
    cct_showstr(x, y, "", COLOR_BLACK, COLOR_WHITE, 1, 32);
    cct_showstr(x, y, message, COLOR_BLACK, COLOR_HWHITE, 1, 32);
}
