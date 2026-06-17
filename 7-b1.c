/* 学号 姓名 班级 */
#define _CRT_SECURE_NO_WARNINGS        /* 使用被VS认为unsafe的函数 */

#include <stdio.h>
#include <time.h>       /* 系统时间函数对应的头文件，自定义函数中禁止使用 */

#ifdef _WIN32
#include <conio.h>      /* _getch()函数对应的头文件 */
#else
static int _getch(void)
{
    return getchar();
}
#endif

/* 如果有需要，可在此处定义自定义需要的函数 */

struct tj_time {
    int tj_year;    /* 表示年 */
    int tj_month;   /* 表示月(1-12) */
    int tj_day;     /* 表示日(1-28/29/30/31) */
    int tj_hour;    /* 表示小时(0-23) */
    int tj_minute;  /* 表示分(0-59) */
    int tj_second;  /* 表示秒(0-59) */
};

static int tj_is_leap_year(const int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static int tj_days_of_year(const int year)
{
    return tj_is_leap_year(year) ? 366 : 365;
}

static int tj_days_of_month(const int year, const int month)
{
    int days;

    switch (month) {
        case 2:
            days = tj_is_leap_year(year) ? 29 : 28;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            days = 30;
            break;
        default:
            days = 31;
            break;
    }

    return days;
}

/***************************************************************************
  函数名称：
  功    能：显示提示并等待回车
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void wait_for_enter(void)
{
    printf("\n" "\xB0\xB4\xBB\xD8\xB3\xB5\xBC\xFC\xBC\xCC\xD0\xF8");

#ifdef _WIN32
    while (_getch() != '\r')
        ;
#else
    {
        int ch;

        do {
            ch = _getch();
        } while (ch != '\n' && ch != EOF);
    }
#endif

    printf("\n\n");
}

/***************************************************************************
  函数名称：
  功    能：调用系统函数完成转换，将输入秒数转换为与本机时区对应的结构体并输出
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void system_time_output(const time_t input_time)
{
    struct tm *tt;  /* struct tm为系统定义的结构体 */

    tt = localtime(&input_time);    /* localtime为系统函数 */
    if (tt != NULL)
        /* tm_*** 为struct tm中的成员，和本题自定义的struct tj_time中的数据不完全吻合，具体含义自己查阅相关资料 */
        printf("%04d-%02d-%02d %02d:%02d:%02d\n", tt->tm_year + 1900, tt->tm_mon + 1, tt->tm_mday, tt->tm_hour, tt->tm_min, tt->tm_sec);

    return;
}

/***************************************************************************
  函数名称：
  功    能：自定义结构体输出函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void tj_time_output(const struct tj_time *const tp)
{
    printf("%04d-%02d-%02d %02d:%02d:%02d\n", tp->tj_year, tp->tj_month, tp->tj_day, tp->tj_hour, tp->tj_minute, tp->tj_second);
}

/***************************************************************************
  函数名称：
  功    能：自定义转换函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
struct tj_time *tj_time_convert(int input_time)
{
    static struct tj_time result;   /* 定义静态局部变量，保证返回值有效 */
    long long total_seconds;
    long long days;
    int seconds_of_day;
    int year;
    int month;

    /* 实现过程开始，可自行增加需要的定义及执行语句即可 */

    total_seconds = (long long)input_time + 8LL * 60 * 60;

    days = total_seconds / (24LL * 60 * 60);
    seconds_of_day = (int)(total_seconds % (24LL * 60 * 60));
    if (seconds_of_day < 0) {
        seconds_of_day += 24 * 60 * 60;
        --days;
    }

    result.tj_hour = seconds_of_day / (60 * 60);
    seconds_of_day %= 60 * 60;
    result.tj_minute = seconds_of_day / 60;
    result.tj_second = seconds_of_day % 60;

    year = 1970;
    while (days >= tj_days_of_year(year)) {
        days -= tj_days_of_year(year);
        ++year;
    }
    while (days < 0) {
        --year;
        days += tj_days_of_year(year);
    }
    result.tj_year = year;

    month = 1;
    while (days >= tj_days_of_month(year, month)) {
        days -= tj_days_of_month(year, month);
        ++month;
    }
    result.tj_month = month;
    result.tj_day = (int)days + 1;

    /* 实现过程结束 */

    return &result; /* 注意，返回的是静态局部变量的地址，此语句不准改 */
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int main(void)
{
    int read_time;
    struct tj_time *tp;

    for (;;) {
        int ret = scanf("%d", &read_time); /* 因为采用输入重定向，此处不加任何提示 */

        /* 输入错误或者<0则退出循环 */
        if (ret != 1 || read_time < 0)
            break;

        printf("\xC3\xEB\xCA\xFD" "             : %d\n", read_time);
        printf("\xCF\xB5\xCD\xB3\xD7\xAA\xBB\xBB\xB5\xC4\xBD\xE1\xB9\xFB" "   : ");
        system_time_output((time_t)read_time);

        printf("\xD7\xD4\xB6\xA8\xD2\xE5\xD7\xAA\xBB\xBB\xB5\xC4\xBD\xE1\xB9\xFB" " : ");
        tp = tj_time_convert(read_time);
        tj_time_output(tp);

        wait_for_enter();
    }

    if (1) {
        int t = (int)time(0);       /* 系统函数，取当前系统时间（从1970-01-01 00:00:00开始的秒数） */

        printf("\xB5\xB1\xC7\xB0\xCF\xB5\xCD\xB3\xCA\xB1\xBC\xE4" "     : %d\n", t);
        printf("\xCF\xB5\xCD\xB3\xD7\xAA\xBB\xBB\xB5\xC4\xBD\xE1\xB9\xFB" "   : ");
        system_time_output((time_t)t);

        printf("\xD7\xD4\xB6\xA8\xD2\xE5\xD7\xAA\xBB\xBB\xB5\xC4\xBD\xE1\xB9\xFB" " : ");
        tp = tj_time_convert(t);
        tj_time_output(tp);

        wait_for_enter();
    }

    return 0;
}
