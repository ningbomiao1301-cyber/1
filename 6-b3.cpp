#include <iostream>
using namespace std;

unsigned int binary_to_decimal(const char *str)
{
    const char *p;
    unsigned int result;

    result = 0;
    p = str;
    while (*p != '\0') {
        result = result * 2 + unsigned(*p - '0');
        p++;
    }

    return result;
}

int main()
{
    char str[33];

    cout << "ÇëÊäÈë0/1×Ö·û´®" << endl;
    cin >> str;
    cout << binary_to_decimal(str) << endl;

    return 0;
}
