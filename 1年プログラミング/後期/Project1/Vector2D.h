#pragma once
class Vector2D
{
public:
	Vector2D();
	Vector2D(int x,int y);
	~Vector2D();
	int x;
	int y;

	//‘ã“ü‰‰Zq
	//‘‚«Š·‚¦‚½‚­‚È‚¢ê‡const‚ğ‘‚­
	Vector2D& operator=(const Vector2D& vec);

	//“Y‚¦š‰‰Zq
	int& operator[](int j);

	//’P€‰‰Zq
	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const Vector2D& vec);
	Vector2D& operator/=(const Vector2D& vec);
	Vector2D& operator%=(const Vector2D& vec);
	Vector2D operator+()const;
	Vector2D operator-()const;

	//“ñ€‰‰Zq
	Vector2D operator+(const Vector2D& vec);
	Vector2D operator-(const Vector2D vec);
	Vector2D operator*(const Vector2D vec);
	Vector2D operator/(const Vector2D vec);
	Vector2D operator%(const Vector2D vec);
	//”äŠr‰‰Zq
	bool operator==(const Vector2D& vec) const;
	bool operator!=(const Vector2D& vec) const;
	bool operator<(const Vector2D& vec) const;
	bool operator<=(const Vector2D& vec) const;
	bool operator>(const Vector2D& vec) const;
	bool operator>=(const Vector2D& vec) const;
};

Vector2D operator*(const Vector2D vec,const int num);
Vector2D operator*( const int num,const Vector2D vec);

Vector2D operator*(const Vector2D vec, const float num);
Vector2D operator*(const float num, const Vector2D vec);

Vector2D operator/(const Vector2D vec, const int num);





