/* class student name */
#include "color_linez.h"

void linez_clear_board(LinezGame &game)
{
    for (int r = 0; r < LINEZ_MAX_SIZE; r++)
        for (int c = 0; c < LINEZ_MAX_SIZE; c++)
            game.board[r][c] = 0;
}

void linez_set_size(LinezGame &game, int rows, int cols, unsigned int seed)
{
    game.rows = rows;
    game.cols = cols;
    game.score = 0;
    game.preview = 0;
    game.stats = 0;
    game.random.value = seed;
    if (game.random.value == 0)
        linez_init_random(game.random);
    linez_clear_board(game);
    for (int i = 0; i <= LINEZ_COLOR_COUNT; i++)
        game.eliminated[i] = 0;
    for (int i = 0; i < LINEZ_NEXT_BALLS; i++)
        game.next_colors[i] = 0;
}

void linez_prepare_next(LinezGame &game)
{
    for (int i = 0; i < LINEZ_NEXT_BALLS; i++)
        game.next_colors[i] = linez_random_color(game.random);
}

void linez_init_game(LinezGame &game, int rows, int cols, unsigned int seed)
{
    linez_set_size(game, rows, cols, seed);
    int first[LINEZ_INIT_BALLS];
    for (int i = 0; i < LINEZ_INIT_BALLS; i++)
        first[i] = linez_random_color(game.random);
    linez_place_random_balls(game, LINEZ_INIT_BALLS, first);
    linez_prepare_next(game);
}

int linez_inside(const LinezGame &game, int row, int col)
{
    return row >= 0 && row < game.rows && col >= 0 && col < game.cols;
}

int linez_count_empty(const LinezGame &game)
{
    int count = 0;
    for (int r = 0; r < game.rows; r++)
        for (int c = 0; c < game.cols; c++)
            if (game.board[r][c] == 0)
                count++;
    return count;
}

int linez_place_random_balls(LinezGame &game, int count, const int colors[])
{
    int placed = 0;
    while (placed < count) {
        int empty = linez_count_empty(game);
        if (empty <= 0)
            break;
        int target = linez_random_int(game.random, empty);
        int seen = 0;
        int done = 0;
        for (int r = 0; r < game.rows && !done; r++) {
            for (int c = 0; c < game.cols && !done; c++) {
                if (game.board[r][c] == 0) {
                    if (seen == target) {
                        game.board[r][c] = (colors == NULL) ? linez_random_color(game.random) : colors[placed];
                        placed++;
                        done = 1;
                    }
                    seen++;
                }
            }
        }
    }
    return placed;
}

void linez_fill_percent(LinezGame &game, int percent)
{
    int total = game.rows * game.cols;
    int need = total * percent / 100;
    linez_clear_board(game);
    linez_place_random_balls(game, need, NULL);
}

static void clear_search_arrays(int prev_r[][LINEZ_MAX_SIZE], int prev_c[][LINEZ_MAX_SIZE], int visited[][LINEZ_MAX_SIZE])
{
    for (int r = 0; r < LINEZ_MAX_SIZE; r++) {
        for (int c = 0; c < LINEZ_MAX_SIZE; c++) {
            prev_r[r][c] = -1;
            prev_c[r][c] = -1;
            visited[r][c] = 0;
        }
    }
}

static void build_path(int sr, int sc, int dr, int dc,
                       int prev_r[][LINEZ_MAX_SIZE], int prev_c[][LINEZ_MAX_SIZE], LinezPath &path)
{
    int rev_r[LINEZ_MAX_CELLS];
    int rev_c[LINEZ_MAX_CELLS];
    int count = 0;
    int r = dr;
    int c = dc;
    while (!(r == sr && c == sc) && count < LINEZ_MAX_CELLS) {
        rev_r[count] = r;
        rev_c[count] = c;
        int nr = prev_r[r][c];
        int nc = prev_c[r][c];
        r = nr;
        c = nc;
        count++;
    }
    rev_r[count] = sr;
    rev_c[count] = sc;
    count++;
    path.count = count;
    for (int i = 0; i < count; i++) {
        path.row[i] = rev_r[count - 1 - i];
        path.col[i] = rev_c[count - 1 - i];
    }
}

int linez_find_path(const LinezGame &game, int sr, int sc, int dr, int dc, LinezPath &path)
{
    int queue_r[LINEZ_MAX_CELLS];
    int queue_c[LINEZ_MAX_CELLS];
    int prev_r[LINEZ_MAX_SIZE][LINEZ_MAX_SIZE];
    int prev_c[LINEZ_MAX_SIZE][LINEZ_MAX_SIZE];
    int visited[LINEZ_MAX_SIZE][LINEZ_MAX_SIZE];
    const int drow[4] = { -1, 1, 0, 0 };
    const int dcol[4] = { 0, 0, -1, 1 };
    int head = 0;
    int tail = 0;
    path.count = 0;
    if (!linez_inside(game, sr, sc) || !linez_inside(game, dr, dc))
        return 0;
    if (game.board[sr][sc] == 0 || game.board[dr][dc] != 0)
        return 0;
    clear_search_arrays(prev_r, prev_c, visited);
    queue_r[tail] = sr;
    queue_c[tail] = sc;
    tail++;
    visited[sr][sc] = 1;
    while (head < tail) {
        int r = queue_r[head];
        int c = queue_c[head];
        head++;
        if (r == dr && c == dc) {
            build_path(sr, sc, dr, dc, prev_r, prev_c, path);
            return 1;
        }
        for (int i = 0; i < 4; i++) {
            int nr = r + drow[i];
            int nc = c + dcol[i];
            if (linez_inside(game, nr, nc) && !visited[nr][nc] && game.board[nr][nc] == 0) {
                visited[nr][nc] = 1;
                prev_r[nr][nc] = r;
                prev_c[nr][nc] = c;
                queue_r[tail] = nr;
                queue_c[tail] = nc;
                tail++;
            }
        }
    }
    return 0;
}

int linez_move_ball(LinezGame &game, int sr, int sc, int dr, int dc, LinezPath &path)
{
    if (!linez_find_path(game, sr, sc, dr, dc, path))
        return 0;
    game.board[dr][dc] = game.board[sr][sc];
    game.board[sr][sc] = 0;
    return 1;
}

static int previous_same(const LinezGame &game, int row, int col, int drow, int dcol, int color)
{
    int pr = row - drow;
    int pc = col - dcol;
    return linez_inside(game, pr, pc) && game.board[pr][pc] == color;
}

static void mark_run(const LinezGame &game, int row, int col, int drow, int dcol,
                     int length, int mark[][LINEZ_MAX_SIZE])
{
    for (int i = 0; i < length; i++) {
        int r = row + drow * i;
        int c = col + dcol * i;
        if (linez_inside(game, r, c))
            mark[r][c] = 1;
    }
}

int linez_remove_lines(LinezGame &game, int &score_gain)
{
    int mark[LINEZ_MAX_SIZE][LINEZ_MAX_SIZE];
    const int drow[4] = { 0, 1, 1, 1 };
    const int dcol[4] = { 1, 0, 1, -1 };
    score_gain = 0;
    for (int r = 0; r < LINEZ_MAX_SIZE; r++)
        for (int c = 0; c < LINEZ_MAX_SIZE; c++)
            mark[r][c] = 0;
    for (int r = 0; r < game.rows; r++) {
        for (int c = 0; c < game.cols; c++) {
            int color = game.board[r][c];
            if (color == 0)
                continue;
            for (int d = 0; d < 4; d++) {
                if (previous_same(game, r, c, drow[d], dcol[d], color))
                    continue;
                int len = 0;
                while (linez_inside(game, r + drow[d] * len, c + dcol[d] * len) &&
                       game.board[r + drow[d] * len][c + dcol[d] * len] == color) {
                    len++;
                }
                if (len >= LINEZ_WIN_LENGTH) {
                    mark_run(game, r, c, drow[d], dcol[d], len, mark);
                    score_gain += (len - 1) * (len - 2);
                }
            }
        }
    }
    int removed = 0;
    for (int r = 0; r < game.rows; r++) {
        for (int c = 0; c < game.cols; c++) {
            if (mark[r][c]) {
                int color = game.board[r][c];
                if (color >= 1 && color <= LINEZ_COLOR_COUNT)
                    game.eliminated[color]++;
                game.board[r][c] = 0;
                removed++;
            }
        }
    }
    game.score += score_gain;
    return removed;
}

int linez_is_game_over(const LinezGame &game)
{
    return linez_count_empty(game) == 0;
}
