#pragma once
//template機能を使うにはclassの前にtemplate<class T>class クラス名　と書く
//Tはintやfloat、doubleの役割を果たす
//Tの中身が確定してないとエラーも出ない
template <class T>class Vector2DTmp
{
public:
	Vector2DTmp();
	Vector2DTmp(T x,T y);
	~Vector2DTmp();
	T x;
	T y;

	//代入演算子
	//書き換えたくない場合constを書く
	Vector2DTmp& operator=(const Vector2DTmp& vec);

	//添え字演算子
	T& operator[](int j);

	//単項演算子
	Vector2DTmp& operator+=(const Vector2DTmp& vec);
	Vector2DTmp& operator-=(const Vector2DTmp& vec);
	Vector2DTmp& operator*=(const Vector2DTmp& vec);
	Vector2DTmp& operator/=(const Vector2DTmp& vec);
	Vector2DTmp& operator%=(const Vector2DTmp& vec);
	Vector2DTmp operator+()const;
	Vector2DTmp operator-()const;

	//二項演算子
	Vector2DTmp operator+(const Vector2DTmp& vec);
	Vector2DTmp operator-(const Vector2DTmp vec);
	Vector2DTmp operator*(const Vector2DTmp vec);
	//Vector2D operator/(const Vector2D vec);
	Vector2DTmp operator%(const Vector2DTmp vec);
	//比較演算子
	bool operator==(const Vector2DTmp& vec) const;
	bool operator!=(const Vector2DTmp& vec) const;
	bool operator<(const Vector2DTmp& vec) const;
	bool operator<=(const Vector2DTmp& vec) const;
	bool operator>(const Vector2DTmp& vec) const;
	bool operator>=(const Vector2DTmp& vec) const;
};

//{}の外に書いている場合は<T>を書く
template <class T>
Vector2DTmp<T> operator*(const Vector2DTmp<T>& vec1, const Vector2DTmp<T>& vec2);

template <class T>
Vector2DTmp<T> operator*(const Vector2DTmp<T> vec, const int num);

template <class T>
Vector2DTmp<T> operator*( const int num,const Vector2DTmp<T> vec);

template <class T>
Vector2DTmp<T> operator*(const Vector2DTmp<T> vec, const float num);

template <class T>
Vector2DTmp<T> operator*(const float num, const Vector2DTmp<T> vec);

template <class T>
Vector2DTmp<T> operator/(const Vector2DTmp<T> vec, const int num);

template <class T>
Vector2DTmp<T> operator/(const Vector2DTmp<T> vec1, Vector2DTmp<T> vec2);

//仮にVector2Dが出てきたとしてもtemplateとして代えてくれる
using Vector2D = Vector2DTmp<int>;
using Vector2DFlot = Vector2DTmp<float>;
using Vector2DDouble = Vector2DTmp<double>;


#include "detail/Vector2D.h"



