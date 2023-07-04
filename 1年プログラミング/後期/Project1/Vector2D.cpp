#include "Vector2D.h"

Vector2D::Vector2D()
{
	x = 0;
	y = 0;
}

Vector2D::Vector2D(int x, int y)
{
	Vector2D::x = x;
	Vector2D::y = y;
}

Vector2D::~Vector2D()
{

}

Vector2D& Vector2D::operator=(const Vector2D& vec)
{
	x = vec.x;
	y = vec.y;

	return *this;
}

int& Vector2D::operator[](int j)
{
	switch (j)
	{
	case 0:
		return x;
		//break;
	case 1:
		return y;
		//break;
	default:
		return x;
		//break;
	}
}

Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
	x += vec.x;
	y += vec.y;

	return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
	x -= vec.x;
	y -= vec.y;

	return *this;
}

Vector2D& Vector2D::operator*=(const Vector2D& vec)
{
	x *= vec.x;
	y *= vec.y;

	return *this;
}

Vector2D& Vector2D::operator/=(const Vector2D& vec)
{
	if (vec.x)
	{
		x /= vec.x;
	}
	else
	{
		x = 0;
	}
	if (vec.y)
	{
		y /= vec.y;
	}
	else
	{
		y = 0;
	}
	return *this;
}

Vector2D& Vector2D::operator%=(const Vector2D& vec)
{
	if (vec.x)
	{
		x %= vec.x;
	}
	else
	{
		x = 0;
	}
	if (vec.y)
	{
		y %= vec.y;
	}
	else
	{
		y = 0;
	}

	return *this;
}

Vector2D Vector2D::operator+() const
{
	return *this;
}

Vector2D Vector2D::operator-() const
{
	return { -x,-y };
}

Vector2D Vector2D::operator+(const Vector2D& vec)
{
	/*Vector2D val;
	val.x = x + vec.x;
	val.y = y + vec.y;
	return val;*/
	return { x + vec.x,y + vec.y };
}

Vector2D Vector2D::operator-(const Vector2D vec)
{
	return { x - vec.x,y - vec.y };
}

Vector2D Vector2D::operator*(const Vector2D vec)
{
	return { x * vec.x,y * vec.y };
}

Vector2D Vector2D::operator/(const Vector2D vec)
{
	Vector2D val;
	if (vec.x)
	{
		val.x = x / vec.x;
	}
	else
	{
		val.x = 0;
	}
	if (vec.y)
	{
		val.y = y / vec.y;
	}
	else
	{
		val.y = 0;
	}
	return val;
	
}

Vector2D Vector2D::operator%(const Vector2D vec)
{
	Vector2D val;
	if (vec.x)
	{
		val.x = x % vec.x;
	}
	else
	{
		val.x = 0;
	}
	if (vec.y)
	{
		val.y = y % vec.y;
	}
	else
	{
		val.y = 0;
	}
	return val;
}

bool Vector2D::operator==(const Vector2D& vec) const
{
	//if (x == vec.x && y == vec.y)
	//{
	//	return true;
	//}
	//return false;

	return (x == vec.x && y == vec.y);
}

bool Vector2D::operator!=(const Vector2D& vec) const
{
	return !(x == vec.x && y == vec.y);
}

bool Vector2D::operator<(const Vector2D& vec) const
{
	return (x < vec.x && y < vec.y);
}

bool Vector2D::operator<=(const Vector2D& vec) const
{
	return (x <= vec.x && y <= vec.y);
}

bool Vector2D::operator>(const Vector2D& vec) const
{
	return (x > vec.x && y > vec.y);
}

bool Vector2D::operator>=(const Vector2D& vec) const
{
	return (x >= vec.x && y >= vec.y);
}

Vector2D operator*(const Vector2D vec, const int num)
{
	return { vec.x * num,vec.y * num };
}

Vector2D operator*(const int num, const Vector2D vec)
{
	return { vec.y * num , vec.x * num};
}

Vector2D operator*(const Vector2D vec, const float num)
{
	return { static_cast<int>(vec.x * num),static_cast<int>(vec.y * num) };
}

Vector2D operator*(const float num, const Vector2D vec)
{
	return { static_cast<int>(vec.y * num) , static_cast<int>(vec.x * num) };
}

Vector2D operator/(const Vector2D vec, const int num)
{
	return { vec.y / num , vec.x / num }; return Vector2D();
}
