/*
 * 5-b18.cpp
 * 互验名单：无
 * 从5-b17的输出重定向文件中读取随机生成的密码串，验证5-b17的答案是否正确
 *
 * 读取方法：
 *   第1行：上一题的输入提示（含空格），读取后忽略
 *   第2行：5个整数，依次为密码总长、大写/小写/数字/其它的最小个数
 *   后续10行：10组密码串，每行为指定长度个字符
 *
 * 检查条件（十组密码整体检查，结果只有一个）：
 *   1. 密码的总长是否符合要求（每行实际字符数 == 声明的总长；总长须在[12..16]）
 *   2. 各种类型字符的数量是否符合各自的最小要求（限制要求见上题：各类最少>=2）
 *   3. 各种类型字符数量的总和是否等于总长度（确保无非法字符）
 *
 * 输出：正确 / 错误（最后加换行符，程序不输出任何输入提示）
 */
#include <iostream>
#include <cctype>
#include <cstring>
using namespace std;

static const char other[] = "!@#$%^&*-_=+,.?"; /* 与5-b17保持一致 */

bool is_other_char(char c) {
    for (int i = 0; other[i] != '\0'; i++) {
        if (other[i] == c) return true;
    }
    return false;
}

int main() {
    /* 步骤1：读第一行（上一题的输入提示，含空格），忽略 */
    char buf[512];
    cin.getline(buf, 512);

    /* 步骤2：读5个整数 */
    int total, min_u, min_l, min_d, min_o;
    if (!(cin >> total >> min_u >> min_l >> min_d >> min_o)) {
        cout << "错误" << endl;
        return 0;
    }

    /*
     * 检查5个整数本身的合法性（限制要求见上题）：
     *   总长须在[12..16]；各类最小个数须在[2..总长]；各类最小之和不超过总长
     */
    if (total < 12 || total > 16 ||
        min_u < 2 || min_u > total ||
        min_l < 2 || min_l > total ||
        min_d < 2 || min_d > total ||
        min_o < 2 || min_o > total ||
        min_u + min_l + min_d + min_o > total) {
        cout << "错误" << endl;
        return 0;
    }

    /* 跳过第2行行尾（含可能的 \r\n） */
    cin.ignore(1000, '\n');

    /* 步骤3：读后续10行密码，逐条检查 */
    for (int k = 0; k < 10; k++) {
        char pwd[512];
        if (!cin.getline(pwd, 512)) {
            cout << "错误" << endl;
            return 0;
        }

        /* 去除 Windows 行尾 \r */
        int len = (int)strlen(pwd);
        if (len > 0 && pwd[len - 1] == '\r') pwd[--len] = '\0';

        /* 检查条件1：密码的总长是否符合要求（实际长度 == 声明的总长） */
        if (len != total) {
            cout << "错误" << endl;
            return 0;
        }

        /* 统计各类字符数量 */
        int cnt_u = 0, cnt_l = 0, cnt_d = 0, cnt_o = 0;
        for (int i = 0; i < len; i++) {
            unsigned char c = (unsigned char)pwd[i];
            if      (isupper(c))          cnt_u++;
            else if (islower(c))          cnt_l++;
            else if (isdigit(c))          cnt_d++;
            else if (is_other_char(pwd[i])) cnt_o++;
            /* 不属于四类的字符不计入任何统计，导致条件3不满足 */
        }

        /* 检查条件3：各种类型字符数量的总和是否等于总长度（无非法字符） */
        if (cnt_u + cnt_l + cnt_d + cnt_o != len) {
            cout << "错误" << endl;
            return 0;
        }

        /* 检查条件2：各种类型字符的数量是否符合各自的最小要求 */
        if (cnt_u < min_u || cnt_l < min_l || cnt_d < min_d || cnt_o < min_o) {
            cout << "错误" << endl;
            return 0;
        }
    }

    cout << "正确" << endl;
    return 0;
}
