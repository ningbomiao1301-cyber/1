/* student id  name  class */
#include <iostream>
using namespace std;

class Days {
private:
	int year;
	int month;
	int day;

	bool is_leap_year() const
	{
		return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	}

	int month_days(int m) const
	{
		if (m == 2) {
			return is_leap_year() ? 29 : 28;
		}
		if (m == 4 || m == 6 || m == 9 || m == 11) {
			return 30;
		}
		return 31;
	}

public:
	Days(int y, int m, int d)
	{
		year = y;
		month = m;
		day = d;
	}

	int calc_days() const
	{
		int sum = 0;
		int i;

		if (month < 1 || month > 12) {
			return -1;
		}
		if (day < 1 || day > month_days(month)) {
			return -1;
		}

		for (i = 1; i < month; i++) {
			sum += month_days(i);
		}

		return sum + day;
	}
};

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：main函数标准
 ***************************************************************************/
int main()
{
	if (1) {
		Days d1(2024, 3, 18);
		cout << "应该是78， 实际是：" << d1.calc_days() << endl;
	}

	if (1) {
		Days d1(2026, 3, 18);
		cout << "应该是77， 实际是：" << d1.calc_days() << endl;
	}

	if (1) {
		Days d1(2024, 12, 31);
		cout << "应该是366，实际是：" << d1.calc_days() << endl;
	}

	if (1) {
		Days d1(2026, 12, 31);
		cout << "应该是365，实际是：" << d1.calc_days() << endl;
	}

	if (1) {
		Days d1(2024, 2, 29);
		cout << "应该是60， 实际是：" << d1.calc_days() << endl;
	}

	if (1) {
		Days d1(2026, 2, 29);
		cout << "应该是-1， 实际是：" << d1.calc_days() << endl;
	}

	return 0;
}
