#include <cstdio>
#include <iostream>
using namespace std;

#define N 10

int main()
{
    char str[256], *p;
    int a[N] = { 0 }, *pnum, *pa;
    bool is_num;

    cout << "请输入字符串" << endl;
    if (fgets(str, 256, stdin) == NULL)
        *str = '\0';

    p = str;
    pnum = a;
    is_num = false;

    while (*p != '\0') {
        if (*p >= '0' && *p <= '9') {
            if (!is_num && pnum < a + N)
                *pnum = 0;
            if (pnum < a + N)
                *pnum = *pnum * 10 + (*p - '0');
            is_num = true;
        }
        else {
            if (is_num && pnum < a + N)
                pnum++;
            is_num = false;
        }
        p++;
    }

    if (is_num && pnum < a + N)
        pnum++;

    cout << "共有" << int(pnum - a) << "个整数" << endl;
    cout << "依次输出的" << int(pnum - a) << "个整数";
    if (pnum != a)
        cout << " ";

    pa = a;
    while (pa < pnum) {
        if (pa != a)
            cout << " ";
        cout << *pa;
        pa++;
    }
    cout << endl;

    return 0;
}
