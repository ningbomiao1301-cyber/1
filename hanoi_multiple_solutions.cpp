/* =========================================================
   hanoi_multiple_solutions.cpp
   汉诺塔各菜单项的实现
   ========================================================= */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include "hanoi.h"
#include "cmd_console_tools.h"
#include "cmd_hdc_tools.h"
using namespace std;

// ----------------------------------------------------------
// 全局状态变量（允许使用的四类全局量）
// ----------------------------------------------------------
int g_move_count = 0;
int g_delay_time = 0;
int g_towers[3][MAX_LAYER] = { 0 };
int g_tower_counts[3] = { 0 };

// ----------------------------------------------------------
// 绘图基准参数（像素坐标，固定偏移）
// ----------------------------------------------------------
const int BASE_Y          = 300;  // 底盘上沿 Y 坐标
const int DISK_HEIGHT     = 15;   // 盘子/底盘的像素高度
const int COLUMN_WIDTH    = 5;    // 立柱像素宽度
const int UNDERPAN_WIDTH_FIX = 80; // 底盘像素宽度
const int COLUMN_SPACING  = 120;  // 相邻立柱中心 X 间距

// ----------------------------------------------------------
// 基础辅助函数
// ----------------------------------------------------------

int input_params_safe()
{
    int n;
    while (true) {
        if (cin >> n && n >= 1 && n <= MAX_LAYER) return n;
        cin.clear();
        while (cin.get() != '\n');
        cout << "输入超出范围，请重新输入 (1-" << MAX_LAYER << "): ";
    }
}

char input_src_safe()
{
    char c;
    while (true) {
        cin >> c;
        c = toupper(c);
        if (c >= 'A' && c <= 'C') return c;
        cout << "请输入有效柱子名称 A、B 或 C: ";
    }
}

void wait_for_enter()
{
    cout << "\n执行完毕，请按 [Enter] 键返回主菜单...";
    while (true) {
        int ch = _getch();
        if (ch == '\r' || ch == '\n') break;
    }
}

// 返回第 col 根柱子（0=A,1=B,2=C）的中心 X 像素坐标
int get_col_center_x_fix(int col)
{
    return 100 + col * COLUMN_SPACING;
}

// ----------------------------------------------------------
// 绘制单个底盘 / 单根立柱
// 菜单 5/6/7/8/9 共用（满足"共用一个函数"要求）
// ----------------------------------------------------------

void draw_underpan(int t)
{
    int cx = get_col_center_x_fix(t);
    hdc_rectangle(cx - UNDERPAN_WIDTH_FIX / 2, BASE_Y,
                  UNDERPAN_WIDTH_FIX, DISK_HEIGHT, HDC_COLOR[11]);
}

void draw_column(int t)
{
    int cx  = get_col_center_x_fix(t);
    int col_top = BASE_Y - (MAX_LAYER + 2) * DISK_HEIGHT;
    hdc_rectangle(cx - COLUMN_WIDTH / 2, col_top,
                  COLUMN_WIDTH, BASE_Y - col_top, HDC_COLOR[11]);
}

// ----------------------------------------------------------
// 重绘所有静止元素（用于动画帧内部刷新）
// moving_disk_id = 正在飞行的盘子编号，动画过程中跳过它的静止位置
// moving_x/y    = 飞行盘子当前位置（-1 表示无飞行盘子）
// ----------------------------------------------------------
void redraw_all_elements(int moving_disk_id = -1,
                         int moving_x = -1, int moving_y = -1)
{
    hdc_cls();

    // 三个底盘
    for (int t = 0; t < 3; t++) draw_underpan(t);
    // 三根立柱
    for (int t = 0; t < 3; t++) draw_column(t);

    // 各柱静止盘子（跳过飞行中的盘子）
    for (int t = 0; t < 3; t++) {
        int cx = get_col_center_x_fix(t);
        for (int i = 0; i < g_tower_counts[t]; i++) {
            int did = g_towers[t][i];
            if (did == moving_disk_id) continue;
            int dw  = 10 + did * 8;
            int dy  = BASE_Y - (i + 1) * DISK_HEIGHT;
            hdc_rectangle(cx - dw / 2, dy, dw, DISK_HEIGHT, HDC_COLOR[did]);
        }
    }

    // 飞行中的盘子
    if (moving_disk_id != -1 && moving_x != -1 && moving_y != -1) {
        int dw = 10 + moving_disk_id * 8;
        hdc_rectangle(moving_x - dw / 2, moving_y,
                      dw, DISK_HEIGHT, HDC_COLOR[moving_disk_id]);
    }
}

// ----------------------------------------------------------
// 盘子移动动画（菜单 7/8/9 共用）
//
// 【重要】调用本函数前，必须已完成 g_towers / g_tower_counts 的更新：
//   即已从 from_col 弹出该盘、已压入 to_col。
//   因此修正公式为：
//     start_y  = BASE_Y - (g_tower_counts[from_col] + 1) * DISK_HEIGHT
//     target_y = BASE_Y -  g_tower_counts[to_col]        * DISK_HEIGHT
// ----------------------------------------------------------
void move_disk_animation_fix(int disk_id, int from_col, int to_col)
{
    int from_cx = get_col_center_x_fix(from_col);
    int to_cx   = get_col_center_x_fix(to_col);

    // 数据已更新：from_col 计数已 -1，故 +1 还原盘子移前所在高度
    int start_y  = BASE_Y - (g_tower_counts[from_col] + 1) * DISK_HEIGHT;
    // 数据已更新：to_col 计数已 +1，直接乘即为盘子目标上沿高度
    int target_y = BASE_Y - g_tower_counts[to_col] * DISK_HEIGHT;
    int top_y    = BASE_Y - (MAX_LAYER + 3) * DISK_HEIGHT; // 飞越最高点

    // 阶段 1：向上飞
    for (int y = start_y; y >= top_y; y -= HDC_Step_Y) {
        redraw_all_elements(disk_id, from_cx, y);
        Sleep(1);
    }

    // 阶段 2：水平飞
    int step_x = (to_cx > from_cx) ? HDC_Step_X : -HDC_Step_X;
    for (int x = from_cx;
         (step_x > 0 ? x < to_cx : x > to_cx);
         x += step_x) {
        redraw_all_elements(disk_id, x, top_y);
        Sleep(1);
    }

    // 阶段 3：向下落
    for (int y = top_y; y <= target_y; y += HDC_Step_Y) {
        redraw_all_elements(disk_id, to_cx, y);
        Sleep(1);
    }

    // 最终静止重绘（落稳后去掉飞行态）
    redraw_all_elements();
}

// ----------------------------------------------------------
// 初始化绘制塔架与初始盘子
// 效果：底盘 A→B→C 依次出现，立柱 A→B→C 依次出现，
//       再将 src_col 上的盘子从底到顶依次出现。
// 均为"瞬间出现"（每个元素一次 hdc_rectangle 完成），不拉长。
// 菜单 5/6/7/8/9 共用本函数。
//
// 【前提】调用前 g_towers / g_tower_counts 必须由 menu_handler 已初始化；
//         本函数不修改这两个全局变量。
// ----------------------------------------------------------
void init_draw_towers_and_disks(int n, int src_col)
{
    hdc_init(HDC_COLOR[0], HDC_COLOR[11], 800, 600);

    // 底盘从左到右依次出现
    for (int t = 0; t < 3; t++) {
        draw_underpan(t);
        Sleep(HDC_Init_Delay / 3);
    }

    // 立柱从左到右依次出现
    for (int t = 0; t < 3; t++) {
        draw_column(t);
        Sleep(HDC_Init_Delay / 3);
    }

    // 盘子从底到顶依次出现（使用已初始化的 g_towers 数据）
    for (int i = 0; i < n; i++) {
        int did = g_towers[src_col][i];   // 底部 i=0 最大，顶部 i=n-1 最小
        int cx  = get_col_center_x_fix(src_col);
        int dw  = 10 + did * 8;
        int dy  = BASE_Y - (i + 1) * DISK_HEIGHT;
        hdc_rectangle(cx - dw / 2, dy, dw, DISK_HEIGHT, HDC_COLOR[did]);
        Sleep(HDC_Init_Delay / 5);
    }
}

// ----------------------------------------------------------
// 字符串打印辅助（菜单 4/8/9 共用）
// ----------------------------------------------------------

void print_towers_vertical(int start_x, int start_y)
{
    for (int h = MAX_LAYER - 1; h >= 0; h--) {
        cct_gotoxy(start_x + Underpan_A_X_OFFSET,
                   start_y + (MAX_LAYER - 1 - h));
        for (int t = 0; t < 3; t++) {
            if (h < g_tower_counts[t]) cout << g_towers[t][h];
            else                        cout << "|";
            if (t < 2)
                for (int s = 0; s < Underpan_Distance - 1; s++) cout << " ";
        }
    }
    cct_gotoxy(start_x + Underpan_A_X_OFFSET, start_y + MAX_LAYER);
    cout << "A   B   C" << endl;
}

void print_towers_vertical_no_line(int start_x, int start_y)
{
    for (int h = MAX_LAYER - 1; h >= 0; h--) {
        cct_gotoxy(start_x + Underpan_A_X_OFFSET,
                   start_y + (MAX_LAYER - 1 - h));
        for (int t = 0; t < 3; t++) {
            if (h < g_tower_counts[t]) cout << g_towers[t][h];
            else                        cout << " ";
            if (t < 2)
                for (int s = 0; s < Underpan_Distance - 1; s++) cout << " ";
        }
    }
}

void print_arrays_horizontal(int start_x, int start_y)
{
    if (start_y != -1) cct_gotoxy(start_x, start_y);
    cout << "第" << setw(3) << g_move_count << " 步: ";
    for (int t = 0; t < 3; t++) {
        cout << (char)('A' + t) << ": ";
        for (int i = 0; i < g_tower_counts[t]; i++)  cout << g_towers[t][i] << " ";
        for (int i = g_tower_counts[t]; i < 10; i++) cout << "  ";
    }
    cout << endl;
}

// ----------------------------------------------------------
// 递归演示函数（菜单 1~4/8 复用）
// ----------------------------------------------------------

void hanoi_menu1(int n, char src, char tmp, char dst)
{
    if (n == 1) { cout << setw(2) << n << "# " << src << "-->" << dst << endl; }
    else {
        hanoi_menu1(n - 1, src, dst, tmp);
        cout << setw(2) << n << "# " << src << "-->" << dst << endl;
        hanoi_menu1(n - 1, tmp, src, dst);
    }
}

void hanoi_menu2(int n, char src, char tmp, char dst, int& m)
{
    if (n == 1) {
        m++;
        cout << setw(5) << m << ":" << setw(3) << n << "# "
             << src << "-->" << dst << endl;
    } else {
        hanoi_menu2(n - 1, src, dst, tmp, m);
        m++;
        cout << setw(5) << m << ":" << setw(3) << n << "# "
             << src << "-->" << dst << endl;
        hanoi_menu2(n - 1, tmp, src, dst, m);
    }
}

void hanoi_menu3(int n, char src, char tmp, char dst)
{
    if (n == 0) return;
    hanoi_menu3(n - 1, src, dst, tmp);
    int from = src - 'A', to = dst - 'A';
    int disk = g_towers[from][--g_tower_counts[from]];
    g_towers[to][g_tower_counts[to]++] = disk;
    g_move_count++;
    cout << "第" << setw(4) << g_move_count << " 步("
         << setw(2) << n << "): " << src << "-->" << dst << " ";
    cout << "A:";
    for (int i = 0; i < g_tower_counts[0]; i++) cout << setw(2) << g_towers[0][i];
    for (int i = g_tower_counts[0]; i < 10;  i++) cout << "  ";
    cout << " B:";
    for (int i = 0; i < g_tower_counts[1]; i++) cout << setw(2) << g_towers[1][i];
    for (int i = g_tower_counts[1]; i < 10;  i++) cout << "  ";
    cout << " C:";
    for (int i = 0; i < g_tower_counts[2]; i++) cout << setw(2) << g_towers[2][i];
    for (int i = g_tower_counts[2]; i < 10;  i++) cout << "  ";
    cout << endl;
    hanoi_menu3(n - 1, tmp, src, dst);
}

void hanoi_menu4(int n, char src, char tmp, char dst)
{
    if (n == 0) return;
    hanoi_menu4(n - 1, src, dst, tmp);
    int from = src - 'A', to = dst - 'A';
    int disk = g_towers[from][--g_tower_counts[from]];
    g_towers[to][g_tower_counts[to]++] = disk;
    g_move_count++;
    print_towers_vertical(MenuItem4_Start_X, MenuItem4_Start_Y);
    print_arrays_horizontal(MenuItem4_Start_X, MenuItem4_Start_Y + 12);
    if (g_delay_time > 0) Sleep(g_delay_time);
    hanoi_menu4(n - 1, tmp, src, dst);
}

// 菜单 7：只走第一步（limit 控制）
void hanoi_menu7(int n, char src, char tmp, char dst, int& limit)
{
    if (n == 0 || limit <= 0) return;
    hanoi_menu7(n - 1, src, dst, tmp, limit);
    if (limit <= 0) return;
    int from = src - 'A', to = dst - 'A';
    // 先更新数据，再动画（与 move_disk_animation_fix 约定一致，防止穿模）
    int disk = g_towers[from][--g_tower_counts[from]];
    g_towers[to][g_tower_counts[to]++] = disk;
    g_move_count++;
    limit--;
    move_disk_animation_fix(disk, from, to);
    print_towers_vertical_no_line(MenuItem8_Start_X, MenuItem8_Start_Y);
    hanoi_menu7(n - 1, tmp, src, dst, limit);
}

// 菜单 8：全自动演示
void hanoi_menu8(int n, char src, char tmp, char dst)
{
    if (n == 0) return;
    hanoi_menu8(n - 1, src, dst, tmp);
    int from = src - 'A', to = dst - 'A';
    // 先更新数据，再动画
    int disk = g_towers[from][--g_tower_counts[from]];
    g_towers[to][g_tower_counts[to]++] = disk;
    g_move_count++;
    move_disk_animation_fix(disk, from, to);
    print_towers_vertical_no_line(MenuItem8_Start_X, MenuItem8_Start_Y);
    hanoi_menu8(n - 1, tmp, src, dst);
}

// ----------------------------------------------------------
// 菜单总处理函数
// ----------------------------------------------------------
void menu_handler(int choice)
{
    if (choice < 1 || choice > 9) return;

    // 重置全局状态
    g_move_count = 0;
    g_delay_time = 0;
    for (int i = 0; i < 3; i++) {
        g_tower_counts[i] = 0;
        for (int j = 0; j < MAX_LAYER; j++) g_towers[i][j] = 0;
    }

    // 读入公共参数
    cout << "请输入圆盘的数量 (1-" << MAX_LAYER << "): ";
    int n = input_params_safe();
    cout << "请输入起始柱 (A/B/C): ";
    char src = input_src_safe();

    char dst;
    while (true) {
        cout << "请输入目标柱 (A/B/C): ";
        dst = input_src_safe();
        if (dst != src) break;
        cout << "目标柱不能与起始柱 (" << src << ") 相同，请重新输入。" << endl;
    }
    char mid = (char)('A' + 'B' + 'C' - src - dst);

    if (choice == 4) {
        cout << "请输入延时时间（毫秒，0 表示不延时）: ";
        while (!(cin >> g_delay_time) || g_delay_time < 0) {
            cin.clear();
            while (cin.get() != '\n');
            cout << "请输入非负整数: ";
        }
    }

    cct_cls();
    int src_col = src - 'A';
    int dst_col = dst - 'A';

    // 菜单 3/4/6/7/8/9 需要预先初始化塔状态
    if (choice != 1 && choice != 2 && choice != 5) {
        for (int i = 0; i < n; i++) g_towers[src_col][i] = n - i;
        g_tower_counts[src_col] = n;
    }

    // ---- 菜单 1 ----
    if (choice == 1) {
        hanoi_menu1(n, src, mid, dst);
        wait_for_enter();
    }
    // ---- 菜单 2 ----
    else if (choice == 2) {
        int m = 0;
        hanoi_menu2(n, src, mid, dst, m);
        wait_for_enter();
    }
    // ---- 菜单 3 ----
    else if (choice == 3) {
        hanoi_menu3(n, src, mid, dst);
        wait_for_enter();
    }
    // ---- 菜单 4 ----
    else if (choice == 4) {
        print_towers_vertical(MenuItem4_Start_X, MenuItem4_Start_Y);
        print_arrays_horizontal(MenuItem4_Start_X, MenuItem4_Start_Y + 12);
        if (g_delay_time > 0) Sleep(g_delay_time);
        hanoi_menu4(n, src, mid, dst);
        wait_for_enter();
    }
    // ---- 菜单 5：只画三根柱子（无盘子）----
    else if (choice == 5) {
        // g_tower_counts 均为 0，init 函数画盘子循环不执行
        init_draw_towers_and_disks(0, src_col);
        cct_gotoxy(0, 38);
        wait_for_enter();
    }
    // ---- 菜单 6：画初始盘子 ----
    else if (choice == 6) {
        init_draw_towers_and_disks(n, src_col);
        cct_gotoxy(0, 38);
        wait_for_enter();
    }
    // ---- 菜单 7：只走第一步 ----
    else if (choice == 7) {
        init_draw_towers_and_disks(n, src_col);
        cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET,
                   MenuItem8_Start_Y + MAX_LAYER);
        cout << "A   B   C";
        Sleep(600);
        int limit = 1;
        hanoi_menu7(n, src, mid, dst, limit);
        cct_gotoxy(0, 38);
        wait_for_enter();
    }
    // ---- 菜单 8：全自动演示 ----
    else if (choice == 8) {
        init_draw_towers_and_disks(n, src_col);
        cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET,
                   MenuItem8_Start_Y + MAX_LAYER);
        cout << "A   B   C";
        Sleep(600);
        hanoi_menu8(n, src, mid, dst);
        cct_gotoxy(0, 38);
        wait_for_enter();
    }
    // ---- 菜单 9：游戏模式（人工操作）----
    else if (choice == 9) {
        // 同菜单 7/8：初始化显示（底盘→立柱→盘子，从左到右依次出现）
        init_draw_towers_and_disks(n, src_col);
        cct_gotoxy(MenuItem9_Start_X + Underpan_A_X_OFFSET,
                   MenuItem9_Start_Y + MAX_LAYER);
        cout << "A   B   C";
        Sleep(600);

        // 游戏主循环（不使用递归函数）
        bool game_over = false;
        while (!game_over) {
            // 检查胜利条件
            if (g_tower_counts[dst_col] == n) {
                cct_gotoxy(0, 38);
                cout << "游戏结束！您共移动了 " << g_move_count
                     << " 步。                                   ";
                game_over = true;
                break;
            }

            // 显示输入提示
            cct_gotoxy(0, 38);
            cout << "输入[源->目标](如 AB)，Q 退出: ";

            char input[32] = { 0 };
            cin.width(31);
            cin >> input;

            // 退出
            if (toupper(input[0]) == 'Q') {
                game_over = true;
                break;
            }

            char from_c = toupper(input[0]);
            char to_c   = (input[1] != '\0') ? toupper(input[1]) : '\0';

            // 输入合法性检查
            cct_gotoxy(0, 39);
            if (from_c < 'A' || from_c > 'C' || to_c < 'A' || to_c > 'C') {
                cout << "输入无效，请输入两个合法字母(A/B/C)！         ";
                continue;
            }
            if (from_c == to_c) {
                cout << "源柱与目标柱不能相同！                         ";
                continue;
            }

            int from = from_c - 'A';
            int to   = to_c   - 'A';

            // 规则检查：源柱不为空
            if (g_tower_counts[from] == 0) {
                cout << "源柱为空，无法移动！                           ";
                continue;
            }

            // 规则检查：不允许大盘压小盘
            int top_from = g_towers[from][g_tower_counts[from] - 1];
            if (g_tower_counts[to] > 0 &&
                g_towers[to][g_tower_counts[to] - 1] < top_from) {
                cout << "不能将大盘放在小盘上！                         ";
                continue;
            }

            // 合法移动：先更新数据，再执行动画（与菜单 7/8 保持一致，防止穿模）
            int disk = g_towers[from][--g_tower_counts[from]];
            g_towers[to][g_tower_counts[to]++] = disk;
            g_move_count++;
            move_disk_animation_fix(disk, from, to);
            print_towers_vertical_no_line(MenuItem9_Start_X, MenuItem9_Start_Y);

            cct_gotoxy(0, 39);
            cout << "第" << setw(4) << g_move_count << " 步: "
                 << from_c << " --> " << to_c << "                    ";
        }

        cct_gotoxy(0, 40);
        wait_for_enter();
    }
}
