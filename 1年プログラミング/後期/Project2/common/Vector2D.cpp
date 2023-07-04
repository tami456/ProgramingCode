#include "Vector2D.h"

Vector2D::Vector2D()
{
	x = 0;
	y = 0;
}

Vector2D::Vector2D(int x, int y)
{
	//メンバー変数にローカル変数を代入
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

int& Vector2D::operator[](int j)		//添字でアクセスしたいとき
{
	//添字が0だったらｘを返す…など
	switch (j)
	{
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	default:
		return x;
		break;
	}
}

Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
	x += vec.x;
	y += vec.y;

	return *this;		//thisポインタの参照を返す
}

Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;		//thisポインタの参照を返す
}

Vector2D& Vector2D::operator*=(const Vector2D& vec)
{
	x *= vec.x;
	y *= vec.y;
	return *this;		//thisポインタの参照を返す
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
	//分母が０で割ることはNG
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
	//return Vector2D(-x,-y);でも可
	return { -x,-y };
}

//二項演算子
Vector2D Vector2D::operator+(const Vector2D& vec)
{
	//モダンにかくとこう
	return { x + vec.x,y + vec.y };
}

Vector2D Vector2D::operator-(const Vector2D& vec)
{
	return { x - vec.x,y - vec.y };
}

Vector2D Vector2D::operator*(const Vector2D& vec)
{
	return { x * vec.x,y * vec.y };
}

//Vector2D Vector2D::operator/(const Vector2D& vec)
//{
//	Vector2D val;
//	if (vec.x)
//	{
//		val.x = x / vec.x;
//	}
//	else
//	{
//		val.x = 0;
//	}
//	if (vec.y)
//	{
//		val.y = y / vec.y;
//	}
//	else
//	{
//		val.y = 0;
//	}
//
//	return val;
//}

Vector2D Vector2D::operator%(const Vector2D& vec)
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
//比較演算子
bool Vector2D::operator==(const Vector2D vec) const
{
	//丁寧に書いたら
	//if (x == vec.x && y == vec.y)
	//{
	//	return true;
	//}
	//return false;

	//モダンな書き方
	return (x == vec.x && y == vec.y);
}

bool Vector2D::operator!=(const Vector2D vec) const
{
	return !(x == vec.x && y == vec.y);
}

bool Vector2D::operator<(const Vector2D vec) const
{
	return (x < vec.x&& y < vec.y);
}

bool Vector2D::operator<=(const Vector2D vec) const
{
	return (x <= vec.x && y <= vec.y);
}

bool Vector2D::operator>(const Vector2D vec) const
{
	return (x > vec.x && y > vec.y);
}

bool Vector2D::operator>=(const Vector2D vec) const
{
	return (x >= vec.x && y >= vec.y);
}

Vector2D operator*(const Vector2D& vec1, const Vector2D& vec2)
{
	return { vec1.x * vec2.x,vec1.y * vec2.y };
}

Vector2D operator*(const Vector2D vec, const int num)
{

	return { vec.x * num,vec.y * num };
}

Vector2D operator*(const int num, const Vector2D vec)
{
	return { vec.x * num,vec.y * num };
}

Vector2D operator*(const Vector2D vec, const float num)
{
	return { static_cast<int>(vec.x * num),static_cast<int>(vec.y * num) };
}

Vector2D operator*(const float num, const Vector2D vec)
{
	return { static_cast<int>(vec.x * num),static_cast<int>(vec.y * num) };
}

Vector2D operator/(const Vector2D vec, const float num)
{
	return { static_cast<int>(vec.x / num),static_cast<int>(vec.y / num) };
}

Vector2D operator/(const Vector2D vec1, Vector2D vec2)
{
	Vector2D val;
	if (vec2.x)
	{
		val.x = vec1.x / vec2.x;
	}
	else
	{
		val.x = 0;
	}
	if (vec2.y)
	{
		val.y = vec1.y / vec2.y;
	}
	else
	{
		val.y = 0;
	}
	return val;
}

Vector2D operator/(const float num, const Vector2D vec)
{
	return { static_cast<int>(num / vec.x),static_cast<int>(num / vec.y) };
}
