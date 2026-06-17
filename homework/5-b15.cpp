/*
 * 5-b15.cpp
 * 键盘输入三行文字，统计英文大写字母、小写字母、数字、空格及其它字符的个数
 * 要求：先将三行全部输入完成，再统计各种字符的数量
 *       用 char str[3][128] 存储，读入每行用 str[i] 形式
 *       中文（多字节）按多个单字节判断计数
 */
#include <iostream>
#include <cctype>
using namespace std;

int main() {
    char str[3][128];

    for (int i = 0; i < 3; i++) {
        cout << "请输入第" << (i + 1) << "行文字：" << endl;
        cin.getline(str[i], 128);
    }

    int upper = 0, lower = 0, digit_cnt = 0, space_cnt = 0, other = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; str[i][j] != '\0'; j++) {
            unsigned char c = (unsigned char)str[i][j];
            if (isupper(c))       upper++;
            else if (islower(c))  lower++;
            else if (isdigit(c))  digit_cnt++;
            else if (c == ' ')    space_cnt++;
            else                  other++;
        }
    }

    cout << "大写字母 : " << upper     << endl;
    cout << "小写字母 : " << lower     << endl;
    cout << "数字 : "     << digit_cnt << endl;
    cout << "空格 : "     << space_cnt << endl;
    cout << "其他 : "     << other     << endl;

    return 0;
}
