/* 7-b1.c */
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
    int tj_year;
    int tj_month;
    int tj_day;
    int tj_hour;
    int tj_minute;
    int tj_second;
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
  Function: wait_for_enter
  Purpose : show prompt and wait for Enter
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
  Function: system_time_output
  Purpose : convert by system function and print
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
  Function: tj_time_output
  Purpose : print custom time struct
***************************************************************************/
void tj_time_output(const struct tj_time *const tp)
{
    printf("%04d-%02d-%02d %02d:%02d:%02d\n", tp->tj_year, tp->tj_month, tp->tj_day, tp->tj_hour, tp->tj_minute, tp->tj_second);
}

/***************************************************************************
  Function: tj_time_convert
  Purpose : convert seconds from 1970-01-01 00:00:00 to local Beijing time
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
       The demo compares against localtime on a China locale.  The fixed
       difference from UTC is +8 hours; leap seconds are ignored by request.
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
  Function: main
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
