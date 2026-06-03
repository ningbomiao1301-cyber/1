#include <cstdio>
#include <iostream>
using namespace std;

bool is_palindrome(const char *str)
{
    const char *left;
    const char *right;

    if (str == NULL)
        return true;

    left = str;
    right = str;
    while (*right != '\0')
        right++;

    if (right == str)
        return true;

    right--;
    while (left < right) {
        if (*left != *right)
            return false;
        left++;
        right--;
    }

    return true;
}

int main()
{
    char str[82], *p;

    cout << "ÇëÊäÈë×Ö·û´®" << endl;
    if (fgets(str, 82, stdin) == NULL)
        *str = '\0';

    p = str;
    while (*p != '\0' && *p != '\n')
        p++;
    if (*p == '\n')
        *p = '\0';
    if (p != str) {
        p--;
        if (*p == '\r')
            *p = '\0';
    }

    if (is_palindrome(str))
        cout << "yes" << endl;
    else
        cout << "no" << endl;

    return 0;
}
