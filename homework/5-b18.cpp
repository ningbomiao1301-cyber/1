/*
 * 5-b18.cpp
 * 互验名单：无
 * 从5-b17的输出重定向文件中读取密码，验证是否正确
 *
 * 读取格式：
 *   第1行：输入提示（忽略）
 *   第2行：5个整数（总长 大写最少 小写最少 数字最少 其他最少）
 *   后续10行：10组密码串
 *
 * 检查条件：
 *   1. 参数合法（密码长度12-16，各类>=2，总和<=密码长度）
 *   2. 每行密码长度 == 总长
 *   3. 各类字符数量满足最小要求
 *   4. 大写+小写+数字+other字符数量之和 == 总长（无非法字符）
 *
 * 输出：正确 / 错误（整体一个结果）
 */
#include <iostream>
#include <cctype>
#include <cstring>
using namespace std;

static const char other[] = "!@#$%^&*-_=+,.?";

bool is_other_char(char c) {
    for (int i = 0; other[i] != '\0'; i++) {
        if (other[i] == c) return true;
    }
    return false;
}

int main() {
    /* 读第1行（输入提示），忽略 */
    char buf[512];
    cin.getline(buf, 512);
    /* 去除可能的 Windows 行尾 \r */
    int blen = (int)strlen(buf);
    if (blen > 0 && buf[blen - 1] == '\r') buf[blen - 1] = '\0';

    /* 读5个整数 */
    int total, min_u, min_l, min_d, min_o;
    if (!(cin >> total >> min_u >> min_l >> min_d >> min_o)) {
        cout << "错误" << endl;
        return 0;
    }

    /* 验证参数合法性 */
    if (total < 12 || total > 16 ||
        min_u < 2 || min_u > total ||
        min_l < 2 || min_l > total ||
        min_d < 2 || min_d > total ||
        min_o < 2 || min_o > total ||
        min_u + min_l + min_d + min_o > total) {
        cout << "错误" << endl;
        return 0;
    }

    /* 跳过读5个整数后行尾的所有字符（含 \r\n） */
    cin.ignore(1000, '\n');

    /* 读取并验证10行密码 */
    for (int k = 0; k < 10; k++) {
        char pwd[512];
        if (!cin.getline(pwd, 512)) {
            cout << "错误" << endl;
            return 0;
        }

        /* 去除可能的 Windows 行尾 \r */
        int len = (int)strlen(pwd);
        if (len > 0 && pwd[len - 1] == '\r') { pwd[--len] = '\0'; }
        if (len != total) {
            cout << "错误" << endl;
            return 0;
        }

        int cnt_u = 0, cnt_l = 0, cnt_d = 0, cnt_o = 0;
        for (int i = 0; i < len; i++) {
            unsigned char c = (unsigned char)pwd[i];
            if (isupper(c))               cnt_u++;
            else if (islower(c))          cnt_l++;
            else if (isdigit(c))          cnt_d++;
            else if (is_other_char(pwd[i])) cnt_o++;
            /* 其他字符（不属于四类）不累计，导致总和不等于len */
        }

        /* 四类总和须等于总长（确保没有非法字符） */
        if (cnt_u + cnt_l + cnt_d + cnt_o != len) {
            cout << "错误" << endl;
            return 0;
        }

        /* 各类须满足最小要求 */
        if (cnt_u < min_u || cnt_l < min_l || cnt_d < min_d || cnt_o < min_o) {
            cout << "错误" << endl;
            return 0;
        }
    }

    cout << "正确" << endl;
    return 0;
}
