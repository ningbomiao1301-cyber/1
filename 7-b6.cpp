/* student id  name  class */
#include <iostream>
#include "7-b6.h"
using namespace std;

bool Date::is_valid_year(int y) const
{
	return y >= 1900 && y <= 2099;
}

bool Date::is_leap_year(int y) const
{
	return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int Date::days_of_month(int y, int m) const
{
	if (m == 2) {
		return is_leap_year(y) ? 29 : 28;
	}
	if (m == 4 || m == 6 || m == 9 || m == 11) {
		return 30;
	}
	return 31;
}

int Date::days_of_year(int y) const
{
	return is_leap_year(y) ? 366 : 365;
}

void Date::set_from_day_count(int count)
{
	int y;
	int m;

	if (count < 1) {
		count = 1;
	}
	else if (count > 73049) {
		count = 73049;
	}

	y = 1900;
	while (count > days_of_year(y)) {
		count -= days_of_year(y);
		y++;
	}

	m = 1;
	while (count > days_of_month(y, m)) {
		count -= days_of_month(y, m);
		m++;
	}

	year = y;
	month = m;
	day = count;
}

Date::Date()
{
	year = 2000;
	month = 1;
	day = 1;
}

Date::Date(int y, int m, int d)
{
	year = 2000;
	month = 1;
	day = 1;
	set(y, m, d);
}

Date::Date(int count)
{
	set_from_day_count(count);
}

void Date::set(int y, int m, int d)
{
	int new_year = year;
	int new_month = month;
	int new_day = day;

	if (y != 0) {
		if (is_valid_year(y)) {
			new_year = y;
		}
		else {
			new_year = 2000;
		}
	}

	if (m != 0) {
		if (m >= 1 && m <= 12) {
			new_month = m;
		}
		else {
			new_month = 1;
		}
	}

	if (d != 0) {
		new_day = d;
	}

	if (new_day < 1 || new_day > days_of_month(new_year, new_month)) {
		new_day = 1;
	}

	year = new_year;
	month = new_month;
	day = new_day;
}

void Date::get(int& y, int& m, int& d) const
{
	y = year;
	m = month;
	d = day;
}

void Date::show() const
{
	cout << year << "." << month << "." << day << endl;
}
