#include "../Vector2D.h"
//templateを使うには.hに実態を移動させるor
//.hと.cppを分けてtemplateを実装するためには.cppを.hにする必要がある
//cpp側では↓とする
template <class T> 
Vector2DTmp<T>::Vector2DTmp()
{
	x = 0;
	y = 0;
}

template <class T>
Vector2DTmp<T>::Vector2DTmp(T x, T y)
{
	Vector2DTmp<T>::x = x;
	Vector2DTmp<T>::y = y;
}

template <class T>
Vector2DTmp<T>::~Vector2DTmp()
{

}

template <class T>
Vector2DTmp<T>& Vector2DTmp<T>::operator=(const Vector2DTmp<T>& vec)
{
	x = vec.x;
	y = vec.y;

	return *this;
}

template <class T>
T& Vector2DTmp<T>::operator[](int j)
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

template <class T>
Vector2DTmp<T>& Vector2DTmp<T>::operator+=(const Vector2DTmp<T>& vec)
{
	x += vec.x;
	y += vec.y;

	return *this;
}

template <class T>
Vector2DTmp<T>& Vector2DTmp<T>::operator-=(const Vector2DTmp<T>& vec)
{
	x -= vec.x;
	y -= vec.y;

	return *this;
}

template <class T>
Vector2DTmp<T>& Vector2DTmp<T>::operator*=(const Vector2DTmp<T>& vec)
{
	x *= vec.x;
	y *= vec.y;

	return *this;
}

template <class T>
Vector2DTmp<T>& Vector2DTmp<T>::operator/=(const Vector2DTmp<T>& vec)
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

template <class T>
Vector2DTmp<T>& Vector2DTmp<T>::operator%=(const Vector2DTmp<T>& vec)
{
	//%はfloatやdoubleでは使えないのでstatic_castを使う
	if (vec.x)
	{
		x = static_cast<int>(x) % static_cast<int>(vec.x);
	}
	else
	{
		x = 0;
	}
	if (vec.y)
	{
		y = static_cast<int>(y) % static_cast<int>(vec.y);
	}
	else
	{
		y = 0;
	}

	return *this;
}

template <class T>
Vector2DTmp<T> Vector2DTmp<T>::operator+() const
{
	return *this;
}

template <class T>
Vector2DTmp<T> Vector2DTmp<T>::operator-() const
{
	return { -x,-y };
}

template <class T>
Vector2DTmp<T> Vector2DTmp<T>::operator+(const Vector2DTmp<T>& vec)
{
	/*Vector2D val;
	val.x = x + vec.x;
	val.y = y + vec.y;
	return val;*/
	return { x + vec.x,y + vec.y };
}

template <class T>
Vector2DTmp<T> Vector2DTmp<T>::operator-(const Vector2DTmp<T> vec)
{
	return { x - vec.x,y - vec.y };
}

template <class T>
Vector2DTmp<T> Vector2DTmp<T>::operator*(const Vector2DTmp<T> vec)
{
	return { x * vec.x,y * vec.y };
}

//Vector2D Vector2D::operator/(const Vector2D vec)
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
//	return val;
//	
//}

template <class T>
Vector2DTmp<T> Vector2DTmp<T>::operator%(const Vector2DTmp<T> vec)
{
	Vector2DTmp<T> val;
	if (vec.x)
	{
		val.x = static_cast<int>(x) % static_cast<int>(vec.x);
	}
	else
	{
		val.x = 0;
	}
	if (vec.y)
	{
		val.y = static_cast<int>(y) % static_cast<int>(vec.y);
	}
	else
	{
		val.y = 0;
	}
	return val;
}

template <class T>
bool Vector2DTmp<T>::operator==(const Vector2DTmp<T>& vec) const
{
	//if (x == vec.x && y == vec.y)
	//{
	//	return true;
	//}
	//return false;

	return (x == vec.x && y == vec.y);
}

template <class T>
bool Vector2DTmp<T>::operator!=(const Vector2DTmp<T>& vec) const
{
	return !(x == vec.x && y == vec.y);
}

template <class T>
bool Vector2DTmp<T>::operator<(const Vector2DTmp<T>& vec) const
{
	return (x < vec.x && y < vec.y);
}

template <class T>
bool Vector2DTmp<T>::operator<=(const Vector2DTmp<T>& vec) const
{
	return (x <= vec.x && y <= vec.y);
}

template <class T>
bool Vector2DTmp<T>::operator>(const Vector2DTmp<T>& vec) const
{
	return (x > vec.x && y > vec.y);
}

template <class T>
bool Vector2DTmp<T>::operator>=(const Vector2DTmp<T>& vec) const
{
	return (x >= vec.x && y >= vec.y);
}

template <class T>
Vector2DTmp<T> operator*(const Vector2DTmp<T>& vec1, const Vector2DTmp<T>& vec2)
{
	return {vec1.x * vec2.x,vec1.y * vec2.y};
}

template <class T>
Vector2DTmp<T> operator*(const Vector2DTmp<T> vec, const int num)
{
	return { vec.x * num,vec.y * num };
}

template <class T>
Vector2DTmp<T> operator*(const int num, const Vector2DTmp<T> vec)
{
	return { vec.y * num , vec.x * num};
}

template <class T>
Vector2DTmp<T> operator*(const Vector2DTmp<T> vec, const float num)
{
	return { static_cast<int>(vec.x * num),static_cast<int>(vec.y * num) };
}

template <class T>
Vector2DTmp<T> operator*(const float num, const Vector2DTmp<T> vec)
{
	return { static_cast<int>(vec.y * num) , static_cast<int>(vec.x * num) };
}

template <class T>
Vector2DTmp<T> operator/(const Vector2DTmp<T> vec, const int num)
{
	return { vec.y / num , vec.x / num }; return Vector2DTmp<T>();
}

template <class T>
Vector2DTmp<T> operator/(const Vector2DTmp<T> vec1, Vector2DTmp<T> vec2)
{
	Vector2DTmp<T> val;
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
