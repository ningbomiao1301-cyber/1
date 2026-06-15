/* 7-b1.c：秒数转换为日期时间 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <conio.h>
#else
static int _getch(void)
{
    return getchar();
}
#endif

struct tj_time {
    int tj_year;    /* 年 */
    int tj_month;   /* 月，1-12 */
    int tj_day;     /* 日，1-28/29/30/31 */
    int tj_hour;    /* 时，0-23 */
    int tj_minute;  /* 分，0-59 */
    int tj_second;  /* 秒，0-59 */
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
  函数名称：wait_for_enter
  功    能：显示提示并等待回车
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
  函数名称：system_time_output
  功    能：调用系统函数完成转换并输出
***************************************************************************/
void system_time_output(const time_t input_time)
{
    struct tm *tt;

    tt = localtime(&input_time);
    if (tt != NULL)
        printf("%04d-%02d-%02d %02d:%02d:%02d\n", tt->tm_year + 1900, tt->tm_mon + 1, tt->tm_mday, tt->tm_hour, tt->tm_min, tt->tm_sec);

    return;
}

/***************************************************************************
  函数名称：tj_time_output
  功    能：按指定格式输出自定义时间结构体
***************************************************************************/
void tj_time_output(const struct tj_time *const tp)
{
    printf("%04d-%02d-%02d %02d:%02d:%02d\n", tp->tj_year, tp->tj_month, tp->tj_day, tp->tj_hour, tp->tj_minute, tp->tj_second);
}

/***************************************************************************
  函数名称：tj_time_convert
  功    能：将 1970-01-01 00:00:00 起经过的秒数转换为本地日期时间
***************************************************************************/
struct tj_time *tj_time_convert(int input_time)
{
    static struct tj_time result;
    long long total_seconds;
    long long days;
    int seconds_of_day;
    int year;
    int month;

    /*
       题目中的参考程序使用本地时间输出。中国时区相对于 UTC 固定多 8 小时，
       本题不考虑闰秒，因此先补上该固定偏移后再拆分日期和时间。
    */
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

    return &result;
}

/***************************************************************************
  函数名称：main
***************************************************************************/
int main(void)
{
    int read_time;
    struct tj_time *tp;

    for (;;) {
        int ret = scanf("%d", &read_time);

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
        int t = (int)time(0);

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
