/*
 * 5-b17.cpp
 * 密码生成程序：按要求生成指定长度的密码（10组）
 * 允许使用 ctime/cstdlib 头文件
 * "其他符号"只能来自指定的静态全局只读数组 other[]
 *
 * 输出格式（供5-b18读取）：
 *   第1行：输入提示
 *   第2行：5个整数（总长 大写最少 小写最少 数字最少 其他最少）
 *   后续10行：10组密码串
 */
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <cstring>
using namespace std;

static const char other[] = "!@#$%^&*-_=+,.?"; // 15个英文符号

/*
 * 安全读取整数：先 cin>>n，再检查下一个字符是否是空白/EOF
 * 若下一个字符是非空白（例如 "16abc" 读完16后遇到'a'），返回 false
 */
bool read_int_safe(int &n) {
    if (!(cin >> n)) return false;
    int c = cin.peek();
    if (c == EOF) return true;
    if (!isspace((unsigned char)c)) return false;
    return true;
}

int main() {
    srand((unsigned int)time(NULL));

    cout << "请输入密码参数（密码长度(12-16) 大写最少(>=2) 小写最少(>=2) 数字最少(>=2) 其他最少(>=2)）：" << endl;

    int total, min_u, min_l, min_d, min_o;

    if (!read_int_safe(total)) {
        cout << "输入错误：请输入有效的整数" << endl;
        return 1;
    }
    if (!read_int_safe(min_u)) {
        cout << "输入错误：请输入有效的整数" << endl;
        return 1;
    }
    if (!read_int_safe(min_l)) {
        cout << "输入错误：请输入有效的整数" << endl;
        return 1;
    }
    if (!read_int_safe(min_d)) {
        cout << "输入错误：请输入有效的整数" << endl;
        return 1;
    }
    if (!read_int_safe(min_o)) {
        cout << "输入错误：请输入有效的整数" << endl;
        return 1;
    }

    if (total < 12 || total > 16) {
        cout << "输入错误：密码长度必须在12到16之间" << endl;
        return 1;
    }
    if (min_u < 2 || min_u > total) {
        cout << "输入错误：大写字母最少数量必须在2到" << total << "之间" << endl;
        return 1;
    }
    if (min_l < 2 || min_l > total) {
        cout << "输入错误：小写字母最少数量必须在2到" << total << "之间" << endl;
        return 1;
    }
    if (min_d < 2 || min_d > total) {
        cout << "输入错误：数字最少数量必须在2到" << total << "之间" << endl;
        return 1;
    }
    if (min_o < 2 || min_o > total) {
        cout << "输入错误：其他符号最少数量必须在2到" << total << "之间" << endl;
        return 1;
    }
    if (min_u + min_l + min_d + min_o > total) {
        cout << "输入错误：各类字符最少数量之和超过密码总长度" << endl;
        return 1;
    }

    /* 输出5个整数（供5-b18读取） */
    cout << total << " " << min_u << " " << min_l << " " << min_d << " " << min_o << endl;

    int other_len = (int)strlen(other);

    for (int k = 0; k < 10; k++) {
        char pwd[17];
        int len = 0;

        /* 先按最少数量填充各类字符 */
        for (int i = 0; i < min_u; i++) pwd[len++] = (char)('A' + rand() % 26);
        for (int i = 0; i < min_l; i++) pwd[len++] = (char)('a' + rand() % 26);
        for (int i = 0; i < min_d; i++) pwd[len++] = (char)('0' + rand() % 10);
        for (int i = 0; i < min_o; i++) pwd[len++] = other[rand() % other_len];

        /* 剩余位置随机填充四类中的任意一类 */
        int remaining = total - len;
        for (int i = 0; i < remaining; i++) {
            int type = rand() % 4;
            if      (type == 0) pwd[len++] = (char)('A' + rand() % 26);
            else if (type == 1) pwd[len++] = (char)('a' + rand() % 26);
            else if (type == 2) pwd[len++] = (char)('0' + rand() % 10);
            else                pwd[len++] = other[rand() % other_len];
        }

        /* Fisher-Yates 随机打乱，避免字符位置固定 */
        for (int i = len - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            char tmp = pwd[i];
            pwd[i] = pwd[j];
            pwd[j] = tmp;
        }

        pwd[len] = '\0';
        cout << pwd << endl;
    }

    return 0;
}
