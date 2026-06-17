/* 编写 学号 姓名 */

#include <iostream>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include "hanoi.h"
#include "cmd_console_tools.h"
#include "cmd_hdc_tools.h"

using namespace std;

// 全局变量声明
int g_move_count = 0;
int g_delay_time = 0;
int g_towers[3][MAX_LAYER] = { 0 };
int g_tower_counts[3] = { 0 };

// --- 辅助输入/工具函数 ---
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
        cout << "输入非法，请输入 A、B 或 C: ";
    }
}

void wait_for_enter()
{
    cout << "\n执行完毕，按回车键[Enter]返回菜单...";
    while (true) {
        int ch = _getch();
        if (ch == '\r' || ch == '\n') {
            break;
        }
    }
}

int get_col_center_x(int col) {
    int left_x = HDC_Start_X + col * (((2 * MAX_LAYER + 1) * HDC_Base_Width) + HDC_Underpan_Distance);
    return left_x + UNDERPAN_WIDTH / 2;
}

// 绘制/擦除单个盘子的辅助函数（擦除时 color_rgb=HDC_COLOR[0]，会自动恢复立柱）
void draw_disk_helper(int disk_id, int x, int y, int color_rgb)
{
    hdc_rectangle(x, y, PAN_WIDTH(disk_id), HDC_Base_High, color_rgb);

    if (color_rgb == HDC_COLOR[0]) {
        for (int t = 0; t < 3; t++) {
            int center_x = get_col_center_x(t);
            int col_left = center_x - HDC_Base_Width / 2;
            int col_height = (MAX_LAYER + 2) * HDC_Base_High;
            int col_top = HDC_Start_Y - col_height;

            if (x <= col_left + HDC_Base_Width && x + PAN_WIDTH(disk_id) >= col_left) {
                int overlap_top = (y > col_top) ? y : col_top;
                int overlap_bottom = (y + HDC_Base_High < HDC_Start_Y) ? (y + HDC_Base_High) : HDC_Start_Y;
                if (overlap_top < overlap_bottom) {
                    hdc_rectangle(col_left, overlap_top, HDC_Base_Width, overlap_bottom - overlap_top, HDC_COLOR[11]);
                }
            }
        }
    }
}

// 第六题专用：从左到右逐列绘制盘子（拉长效果，仅用于题6初始盘子显示）
void draw_disk_animated_lr(int disk_id, int x, int y, int color_rgb)
{
    int total_w = PAN_WIDTH(disk_id);
    for (int offset = 0; offset < total_w; offset++) {
        hdc_rectangle(x + offset, y, 1, HDC_Base_High, color_rgb);
        Sleep(5);
    }
}

// 画三根柱子（含底盘）：底盘从左到右依次出现，柱子从左到右依次出现，均非拉伸效果
void draw_all_towers()
{
    hdc_init(HDC_COLOR[0], HDC_COLOR[11], 120 * 8, 40 * 16);
    hdc_cls();

    // 底盘从左到右依次出现（每个底盘整体直接显示，不拉伸）
    for (int t = 0; t < 3; t++) {
        int left_x = HDC_Start_X + t * (UNDERPAN_WIDTH + HDC_Underpan_Distance);
        hdc_rectangle(left_x, HDC_Start_Y, UNDERPAN_WIDTH, HDC_Base_High, HDC_COLOR[11]);
        Sleep(HDC_Init_Delay / 3);
    }

    // 立柱从左到右依次出现（每根立柱整体直接显示，不拉伸）
    for (int t = 0; t < 3; t++) {
        int center_x = get_col_center_x(t);
        int col_left = center_x - HDC_Base_Width / 2;
        int col_height = (MAX_LAYER + 2) * HDC_Base_High;
        int col_top = HDC_Start_Y - col_height;
        hdc_rectangle(col_left, col_top, HDC_Base_Width, col_height, HDC_COLOR[11]);
        Sleep(HDC_Init_Delay / 3);
    }
}

// 盘子平移动画（先上移到顶部，再水平移动，再下移到目标位置）
void move_disk_animation(int disk_id, int from_col, int to_col)
{
    int from_center_x = get_col_center_x(from_col);
    int start_x = from_center_x - PAN_WIDTH(disk_id) / 2;
    // g_tower_counts[from_col] 已经递减，+1 还原为移动前的高度
    int start_y = HDC_Start_Y - (g_tower_counts[from_col] + 1) * HDC_Base_High;

    int to_center_x = get_col_center_x(to_col);
    int target_x = to_center_x - PAN_WIDTH(disk_id) / 2;
    // g_tower_counts[to_col] 已经递增，即落点高度
    int target_y = HDC_Start_Y - g_tower_counts[to_col] * HDC_Base_High;

    int cur_x = start_x;
    int cur_y = start_y;

    // 上移到顶部
    while (cur_y > HDC_Top_Y) {
        draw_disk_helper(disk_id, cur_x, cur_y, HDC_COLOR[0]);
        cur_y -= HDC_Step_Y * 2;
        if (cur_y < HDC_Top_Y) cur_y = HDC_Top_Y;
        draw_disk_helper(disk_id, cur_x, cur_y, HDC_COLOR[disk_id]);
        Sleep(5);
    }

    // 水平移动
    if (target_x > start_x) {
        while (cur_x < target_x) {
            draw_disk_helper(disk_id, cur_x, cur_y, HDC_COLOR[0]);
            cur_x += HDC_Step_X * 4;
            if (cur_x > target_x) cur_x = target_x;
            draw_disk_helper(disk_id, cur_x, cur_y, HDC_COLOR[disk_id]);
            Sleep(5);
        }
    }
    else {
        while (cur_x > target_x) {
            draw_disk_helper(disk_id, cur_x, cur_y, HDC_COLOR[0]);
            cur_x -= HDC_Step_X * 4;
            if (cur_x < target_x) cur_x = target_x;
            draw_disk_helper(disk_id, cur_x, cur_y, HDC_COLOR[disk_id]);
            Sleep(5);
        }
    }

    // 下移到目标位置
    while (cur_y < target_y) {
        draw_disk_helper(disk_id, cur_x, cur_y, HDC_COLOR[0]);
        cur_y += HDC_Step_Y * 2;
        if (cur_y > target_y) cur_y = target_y;
        draw_disk_helper(disk_id, cur_x, cur_y, HDC_COLOR[disk_id]);
        Sleep(5);
    }
}

// --- 字符渲染辅助函数 ---
void print_towers_vertical(int start_x, int start_y)
{
    for (int h = MAX_LAYER - 1; h >= 0; h--) {
        cct_gotoxy(start_x + Underpan_A_X_OFFSET, start_y + (MAX_LAYER - 1 - h));
        for (int t = 0; t < 3; t++) {
            if (h < g_tower_counts[t]) cout << g_towers[t][h];
            else cout << "|";
            if (t < 2) { for (int s = 0; s < Underpan_Distance - 1; s++) cout << " "; }
        }
    }
    cct_gotoxy(start_x + Underpan_A_X_OFFSET, start_y + MAX_LAYER);
    cout << "A";
    for (int s = 0; s < Underpan_Distance - 1; s++) cout << " ";
    cout << "B";
    for (int s = 0; s < Underpan_Distance - 1; s++) cout << " ";
    cout << "C" << endl;
}

void print_towers_vertical_no_line(int start_x, int start_y)
{
    for (int h = MAX_LAYER - 1; h >= 0; h--) {
        cct_gotoxy(start_x + Underpan_A_X_OFFSET, start_y + (MAX_LAYER - 1 - h));
        for (int t = 0; t < 3; t++) {
            if (h < g_tower_counts[t]) cout << g_towers[t][h];
            else cout << " ";
            if (t < 2) { for (int s = 0; s < Underpan_Distance - 1; s++) cout << " "; }
        }
    }
}

void print_arrays_horizontal(int start_x, int start_y)
{
    if (start_y != -1) cct_gotoxy(start_x, start_y);
    cout << "第" << setw(3) << g_move_count << " 步: ";
    for (int t = 0; t < 3; t++) {
        cout << (char)('A' + t) << ": ";
        for (int i = 0; i < g_tower_counts[t]; i++) cout << g_towers[t][i] << " ";
        for (int i = g_tower_counts[t]; i < 10; i++) cout << "  ";
    }
    cout << endl;
}

// --- 1~4题递归函数 ---
void hanoi_menu1(int n, char src, char tmp, char dst) {
    if (n == 1) { cout << setw(2) << n << "# " << src << "-->" << dst << endl; }
    else {
        hanoi_menu1(n - 1, src, dst, tmp);
        cout << setw(2) << n << "# " << src << "-->" << dst << endl;
        hanoi_menu1(n - 1, tmp, src, dst);
    }
}

void hanoi_menu2(int n, char src, char tmp, char dst, int& m) {
    if (n == 1) {
        m++;
        cout << setw(5) << m << ":" << setw(3) << n << "# " << src << "-->" << dst << endl;
    }
    else {
        hanoi_menu2(n - 1, src, dst, tmp, m);
        m++;
        cout << setw(5) << m << ":" << setw(3) << n << "# " << src << "-->" << dst << endl;
        hanoi_menu2(n - 1, tmp, src, dst, m);
    }
}

void hanoi_menu3(int n, char src, char tmp, char dst) {
    if (n == 0) return;
    hanoi_menu3(n - 1, src, dst, tmp);

    int from = src - 'A', to = dst - 'A';
    int disk = g_towers[from][--g_tower_counts[from]];
    g_towers[to][g_tower_counts[to]++] = disk;
    g_move_count++;

    cout << "第" << setw(4) << g_move_count << " 步(" << setw(2) << n << "): " << src << "-->" << dst << " ";
    cout << "A:";
    for (int i = 0; i < g_tower_counts[0]; i++) cout << setw(2) << g_towers[0][i];
    for (int i = g_tower_counts[0]; i < 10; i++) cout << "  ";
    cout << " B:";
    for (int i = 0; i < g_tower_counts[1]; i++) cout << setw(2) << g_towers[1][i];
    for (int i = g_tower_counts[1]; i < 10; i++) cout << "  ";
    cout << " C:";
    for (int i = 0; i < g_tower_counts[2]; i++) cout << setw(2) << g_towers[2][i];
    for (int i = g_tower_counts[2]; i < 10; i++) cout << "  ";
    cout << endl;

    hanoi_menu3(n - 1, tmp, src, dst);
}

void hanoi_menu4(int n, char src, char tmp, char dst) {
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

// --- 7~8题图形动画自动移动函数 ---
void hanoi_menu7(int n, char src, char tmp, char dst, int& limit) {
    if (n == 0 || limit <= 0) return;
    hanoi_menu7(n - 1, src, dst, tmp, limit);
    if (limit <= 0) return;

    int from = src - 'A', to = dst - 'A';
    int disk = g_towers[from][--g_tower_counts[from]];
    g_towers[to][g_tower_counts[to]++] = disk;
    g_move_count++;
    limit--;

    move_disk_animation(disk, from, to);
    print_towers_vertical_no_line(MenuItem8_Start_X, MenuItem8_Start_Y);

    hanoi_menu7(n - 1, tmp, src, dst, limit);
}

void hanoi_menu8(int n, char src, char tmp, char dst) {
    if (n == 0) return;
    hanoi_menu8(n - 1, src, dst, tmp);

    int from = src - 'A', to = dst - 'A';
    int disk = g_towers[from][--g_tower_counts[from]];
    g_towers[to][g_tower_counts[to]++] = disk;
    g_move_count++;

    move_disk_animation(disk, from, to);
    print_towers_vertical_no_line(MenuItem8_Start_X, MenuItem8_Start_Y);

    hanoi_menu8(n - 1, tmp, src, dst);
}

// 绘制初始盘子（题7/8/9共用）：整体直接显示，不拉伸，与动画盘子位置一致
void draw_initial_disks(int n, char src)
{
    for (int i = 0; i < n; i++) {
        int disk_id = n - i;
        int center_x = get_col_center_x(src - 'A');
        int disk_x = center_x - PAN_WIDTH(disk_id) / 2;
        int disk_y = HDC_Start_Y - (i + 1) * HDC_Base_High;
        draw_disk_helper(disk_id, disk_x, disk_y, HDC_COLOR[disk_id]);
        Sleep(200);
    }
}

// 打印 A/B/C 柱子标签（题7/8/9共用）
void print_col_labels(int start_x, int start_y)
{
    cct_gotoxy(start_x + Underpan_A_X_OFFSET, start_y + MAX_LAYER);
    cout << "A";
    for (int s = 0; s < Underpan_Distance - 1; s++) cout << " ";
    cout << "B";
    for (int s = 0; s < Underpan_Distance - 1; s++) cout << " ";
    cout << "C";
}

// --- 各题菜单处理函数 ---
void menu_handler(int choice)
{
    if (choice < 1 || choice > 9) return;

    g_move_count = 0;
    g_delay_time = 0;
    for (int i = 0; i < 3; i++) {
        g_tower_counts[i] = 0;
        for (int j = 0; j < MAX_LAYER; j++) g_towers[i][j] = 0;
    }

    cout << "请输入汉诺塔层数 (1-10): ";
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
        cout << "请输入延时时间(毫秒): ";
        while (!(cin >> g_delay_time) || g_delay_time < 0) {
            cin.clear();
            while (cin.get() != '\n');
            cout << "输入有误，请重新输入延时时间: ";
        }
    }

    cct_cls();

    if (choice != 1 && choice != 2 && choice != 5) {
        g_tower_counts[src - 'A'] = n;
        for (int i = 0; i < n; i++) {
            g_towers[src - 'A'][i] = n - i;
        }
    }

    // ---- 分题目执行逻辑 ----

    if (choice == 1) {
        hanoi_menu1(n, src, mid, dst);
        wait_for_enter();
    }
    else if (choice == 2) {
        int m = 0;
        hanoi_menu2(n, src, mid, dst, m);
        wait_for_enter();
    }
    else if (choice == 3) {
        hanoi_menu3(n, src, mid, dst);
        wait_for_enter();
    }
    else if (choice == 4) {
        print_towers_vertical(MenuItem4_Start_X, MenuItem4_Start_Y);
        print_arrays_horizontal(MenuItem4_Start_X, MenuItem4_Start_Y + 12);
        if (g_delay_time > 0) Sleep(g_delay_time);
        hanoi_menu4(n, src, mid, dst);
        wait_for_enter();
    }
    else if (choice == 5) {
        // 题5：仅画三根柱子（底盘+立柱从左到右依次出现）
        draw_all_towers();
        cct_gotoxy(0, 38);
        wait_for_enter();
    }
    else if (choice == 6) {
        // 题6：画柱子后，在起始柱上从下到上依次画n个盘子（盘子保留从左到右拉伸的动画效果）
        draw_all_towers();
        for (int i = 0; i < n; i++) {
            int disk_id = n - i;
            int center_x = get_col_center_x(src - 'A');
            int disk_x = center_x - PAN_WIDTH(disk_id) / 2;
            int disk_y = HDC_Start_Y - (i + 1) * HDC_Base_High;
            draw_disk_animated_lr(disk_id, disk_x, disk_y, HDC_COLOR[disk_id]);
            Sleep(200);
        }
        cct_gotoxy(0, 38);
        wait_for_enter();
    }
    else if (choice == 7) {
        // 题7：底盘/立柱同题6（从左到右依次出现，非拉伸）
        //       初始盘子整体显示（与动画盘子位置一致，无穿模）
        draw_all_towers();
        draw_initial_disks(n, src);
        print_col_labels(MenuItem8_Start_X, MenuItem8_Start_Y);
        print_towers_vertical_no_line(MenuItem8_Start_X, MenuItem8_Start_Y);

        Sleep(600);
        int single_move_limit = 1;
        hanoi_menu7(n, src, mid, dst, single_move_limit);

        cct_gotoxy(0, 38);
        wait_for_enter();
    }
    else if (choice == 8) {
        // 题8：同题7（底盘/立柱从左到右依次出现，初始盘子整体显示，完整自动移动演示）
        draw_all_towers();
        draw_initial_disks(n, src);
        print_col_labels(MenuItem8_Start_X, MenuItem8_Start_Y);
        print_towers_vertical_no_line(MenuItem8_Start_X, MenuItem8_Start_Y);

        Sleep(600);
        hanoi_menu8(n, src, mid, dst);
        cct_gotoxy(0, 38);
        wait_for_enter();
    }
    else if (choice == 9) {
        // 题9：汉诺塔游戏（人工操作），底盘/立柱/初始盘子绘制同题7/8
        draw_all_towers();
        draw_initial_disks(n, src);
        print_col_labels(MenuItem9_Start_X, MenuItem9_Start_Y);
        print_towers_vertical_no_line(MenuItem9_Start_X, MenuItem9_Start_Y);

        // 游戏主循环：逐步输入移动，验证合法性，动画，检查胜利
        while (true) {
            // 提示并读取本次移动
            int from_col, to_col;
            char move_src, move_dst;

            while (true) {
                cct_gotoxy(0, Status_Line_Y);
                cout << "第" << setw(4) << g_move_count
                     << "步 请输入移动(源柱->目标柱，如AC): ";
                move_src = input_src_safe();
                move_dst = input_src_safe();

                from_col = move_src - 'A';
                to_col   = move_dst - 'A';

                if (from_col == to_col) {
                    cct_gotoxy(0, Status_Line_Y);
                    cout << "错误：源柱和目标柱相同，请重新输入！                 ";
                }
                else if (g_tower_counts[from_col] == 0) {
                    cct_gotoxy(0, Status_Line_Y);
                    cout << "错误：源柱为空，请重新输入！                          ";
                }
                else if (g_tower_counts[to_col] > 0 &&
                         g_towers[to_col][g_tower_counts[to_col] - 1] <
                         g_towers[from_col][g_tower_counts[from_col] - 1]) {
                    cct_gotoxy(0, Status_Line_Y);
                    cout << "错误：不能将大盘放到小盘上，请重新输入！              ";
                }
                else {
                    break;  // 合法移动，退出内层验证循环
                }
            }

            // 执行合法移动：先更新状态，再动画（move_disk_animation 依赖更新后的 g_tower_counts）
            int disk = g_towers[from_col][--g_tower_counts[from_col]];
            g_towers[to_col][g_tower_counts[to_col]++] = disk;
            g_move_count++;

            move_disk_animation(disk, from_col, to_col);
            print_towers_vertical_no_line(MenuItem9_Start_X, MenuItem9_Start_Y);

            // 胜利检测：目标柱盘子数等于 n
            if (g_tower_counts[dst - 'A'] == n) {
                cct_gotoxy(0, Status_Line_Y);
                cout << "游戏结束！共用 " << g_move_count
                     << " 步完成！                                    ";
                break;
            }
        }

        wait_for_enter();
    }
}
