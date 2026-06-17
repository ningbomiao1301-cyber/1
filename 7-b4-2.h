/* student id  name  class */
#pragma once

class triangle;

class point {
private:
	int x;
	int y;

public:
	point()
	{
		x = 0;
		y = 0;
	}

	void set(int new_x, int new_y)
	{
		x = new_x;
		y = new_y;
	}

	friend class triangle;
};

class triangle {
private:
	point p1;
	point p2;
	point p3;

public:
	triangle(int p1_x, int p1_y, int p2_x, int p2_y, int p3_x, int p3_y)
	{
		p1.set(p1_x, p1_y);
		p2.set(p2_x, p2_y);
		p3.set(p3_x, p3_y);
	}

	double area() const
	{
		double value;

		value = p1.x * (p2.y - p3.y)
			+ p2.x * (p3.y - p1.y)
			+ p3.x * (p1.y - p2.y);
		if (value < 0) {
			value = -value;
		}
		if (value == 0) {
			return -1;
		}

		return value / 2.0;
	}
};
