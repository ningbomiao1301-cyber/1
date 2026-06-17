/* student id  name  class */
#pragma once

class Date {
private:
	int year;
	int month;
	int day;

	bool is_valid_year(int y) const;
	bool is_leap_year(int y) const;
	int days_of_month(int y, int m) const;
	int days_of_year(int y) const;
	void set_from_day_count(int count);

public:
	Date();
	Date(int y, int m, int d);
	Date(int count);

	void set(int y, int m = 1, int d = 1);
	void get(int& y, int& m, int& d) const;
	void show() const;
};
