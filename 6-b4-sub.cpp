/* student name class */

#include <cstdio>

static char tj_to_lower_char(const char ch)
{
    if (ch >= 'A' && ch <= 'Z')
        return char(ch + ('a' - 'A'));

    return ch;
}

int tj_strlen(const char *str)
{
    const char *p;

    if (str == NULL)
        return 0;

    p = str;
    while (*p != '\0')
        p++;

    return int(p - str);
}

char *tj_strcat(char *s1, const char *s2)
{
    char *p1;
    const char *p2;

    if (s1 == NULL)
        return NULL;
    if (s2 == NULL)
        return s1;

    p1 = s1;
    while (*p1 != '\0')
        p1++;

    p2 = s2;
    while (*p2 != '\0') {
        *p1 = *p2;
        p1++;
        p2++;
    }
    *p1 = '\0';

    return s1;
}

char *tj_strncat(char *s1, const char *s2, const int len)
{
    char *p1;
    const char *p2;
    int count;

    if (s1 == NULL)
        return NULL;
    if (s2 == NULL || len <= 0)
        return s1;

    p1 = s1;
    while (*p1 != '\0')
        p1++;

    p2 = s2;
    count = 0;
    while (*p2 != '\0' && count < len) {
        *p1 = *p2;
        p1++;
        p2++;
        count++;
    }
    *p1 = '\0';

    return s1;
}

char *tj_strcpy(char *s1, const char *s2)
{
    char *p1;
    const char *p2;

    if (s1 == NULL)
        return NULL;

    p1 = s1;
    if (s2 == NULL) {
        *p1 = '\0';
        return s1;
    }

    p2 = s2;
    while (*p2 != '\0') {
        *p1 = *p2;
        p1++;
        p2++;
    }
    *p1 = '\0';

    return s1;
}

char *tj_strncpy(char *s1, const char *s2, const int len)
{
    char *p1;
    const char *p2;
    int count;

    if (s1 == NULL)
        return NULL;
    if (s2 == NULL || len <= 0)
        return s1;

    p1 = s1;
    p2 = s2;
    count = 0;
    while (*p2 != '\0' && count < len) {
        *p1 = *p2;
        p1++;
        p2++;
        count++;
    }

    return s1;
}

int tj_strcmp(const char *s1, const char *s2)
{
    const char *p1;
    const char *p2;

    if (s1 == NULL && s2 == NULL)
        return 0;
    if (s1 == NULL)
        return -1;
    if (s2 == NULL)
        return 1;

    p1 = s1;
    p2 = s2;
    while (*p1 != '\0' && *p2 != '\0' && *p1 == *p2) {
        p1++;
        p2++;
    }

    return int(*p1) - int(*p2);
}

int tj_strcasecmp(const char *s1, const char *s2)
{
    const char *p1;
    const char *p2;
    char ch1;
    char ch2;

    if (s1 == NULL && s2 == NULL)
        return 0;
    if (s1 == NULL)
        return -1;
    if (s2 == NULL)
        return 1;

    p1 = s1;
    p2 = s2;
    ch1 = tj_to_lower_char(*p1);
    ch2 = tj_to_lower_char(*p2);
    while (*p1 != '\0' && *p2 != '\0' && ch1 == ch2) {
        p1++;
        p2++;
        ch1 = tj_to_lower_char(*p1);
        ch2 = tj_to_lower_char(*p2);
    }

    return int(ch1) - int(ch2);
}

int tj_strncmp(const char *s1, const char *s2, const int len)
{
    const char *p1;
    const char *p2;
    int count;

    if (len <= 0)
        return 0;
    if (s1 == NULL && s2 == NULL)
        return 0;
    if (s1 == NULL)
        return -1;
    if (s2 == NULL)
        return 1;

    p1 = s1;
    p2 = s2;
    count = 0;
    while (count < len && *p1 != '\0' && *p2 != '\0' && *p1 == *p2) {
        p1++;
        p2++;
        count++;
    }
    if (count == len)
        return 0;

    return int(*p1) - int(*p2);
}

int tj_strcasencmp(const char *s1, const char *s2, const int len)
{
    const char *p1;
    const char *p2;
    char ch1;
    char ch2;
    int count;

    if (len <= 0)
        return 0;
    if (s1 == NULL && s2 == NULL)
        return 0;
    if (s1 == NULL)
        return -1;
    if (s2 == NULL)
        return 1;

    p1 = s1;
    p2 = s2;
    count = 0;
    ch1 = tj_to_lower_char(*p1);
    ch2 = tj_to_lower_char(*p2);
    while (count < len && *p1 != '\0' && *p2 != '\0' && ch1 == ch2) {
        p1++;
        p2++;
        count++;
        ch1 = tj_to_lower_char(*p1);
        ch2 = tj_to_lower_char(*p2);
    }
    if (count == len)
        return 0;

    return int(ch1) - int(ch2);
}

char *tj_strupr(char *str)
{
    char *p;

    if (str == NULL)
        return NULL;

    p = str;
    while (*p != '\0') {
        if (*p >= 'a' && *p <= 'z')
            *p = char(*p - ('a' - 'A'));
        p++;
    }

    return str;
}

char *tj_strlwr(char *str)
{
    char *p;

    if (str == NULL)
        return NULL;

    p = str;
    while (*p != '\0') {
        if (*p >= 'A' && *p <= 'Z')
            *p = char(*p + ('a' - 'A'));
        p++;
    }

    return str;
}

int tj_strchr(const char *str, const char ch)
{
    const char *p;
    int pos;

    if (str == NULL)
        return 0;

    p = str;
    pos = 1;
    while (*p != '\0') {
        if (*p == ch)
            return pos;
        p++;
        pos++;
    }
    if (ch == '\0')
        return pos;

    return 0;
}

int tj_strstr(const char *str, const char *substr)
{
    const char *p;
    const char *s;
    const char *t;
    int pos;

    if (str == NULL || substr == NULL)
        return 0;
    if (*substr == '\0')
        return 1;

    p = str;
    pos = 1;
    while (*p != '\0') {
        s = p;
        t = substr;
        while (*s != '\0' && *t != '\0' && *s == *t) {
            s++;
            t++;
        }
        if (*t == '\0')
            return pos;
        p++;
        pos++;
    }

    return 0;
}

int tj_strrchr(const char *str, const char ch)
{
    const char *p;
    int pos;
    int result;

    if (str == NULL)
        return 0;

    p = str;
    pos = 1;
    result = 0;
    while (*p != '\0') {
        if (*p == ch)
            result = pos;
        p++;
        pos++;
    }
    if (ch == '\0')
        result = pos;

    return result;
}

int tj_strrstr(const char *str, const char *substr)
{
    const char *p;
    const char *s;
    const char *t;
    int pos;
    int result;

    if (str == NULL || substr == NULL)
        return 0;
    if (*substr == '\0')
        return tj_strlen(str) + 1;

    p = str;
    pos = 1;
    result = 0;
    while (*p != '\0') {
        s = p;
        t = substr;
        while (*s != '\0' && *t != '\0' && *s == *t) {
            s++;
            t++;
        }
        if (*t == '\0')
            result = pos;
        p++;
        pos++;
    }

    return result;
}

char *tj_strrev(char *str)
{
    char *left;
    char *right;
    char temp;

    if (str == NULL)
        return NULL;

    left = str;
    right = str;
    while (*right != '\0')
        right++;

    if (right == str)
        return str;

    right--;
    while (left < right) {
        temp = *left;
        *left = *right;
        *right = temp;
        left++;
        right--;
    }

    return str;
}
