#pragma once
class Vector2D
{
public:
	Vector2D();
	Vector2D(int x, int y);
	~Vector2D();
	int x;
	int y;

	//代入演算子
	Vector2D& operator=(const Vector2D& vec);

	//添え字演算子
	//intで返す	[]の中身がj
	int& operator[](int j);

	//単項演算子
	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const Vector2D& vec);
	Vector2D& operator/=(const Vector2D& vec);
	Vector2D& operator%=(const Vector2D& vec);
	Vector2D operator+()const;
	Vector2D operator-()const;

	//二項演算子 A+Bなど 参照ではなくコピーで返す
	//+の前→自分自身
	//自分自身に符号を付与して返す
	Vector2D operator+(const Vector2D& vec);
	Vector2D operator-(const Vector2D& vec);
	Vector2D operator*(const Vector2D& vec);
	//Vector2D operator/(const Vector2D& vec);
	Vector2D operator%(const Vector2D& vec);

	//比較演算子
	bool operator==(const Vector2D vec) const;
	bool operator!=(const Vector2D vec) const;
	bool operator<(const Vector2D vec) const;
	bool operator<=(const Vector2D vec) const;
	bool operator>(const Vector2D vec) const;
	bool operator>=(const Vector2D vec) const;


	/*参照の場合は名前.の形
	* ポインタ→*vec
	* ドット→vec->x
	* .：（実体そのものにアクセス）
	* ->：（何か一段ジャンプしてジャンプした先）、メンバー
	* 自分自身の参照を返す時、クラスそのもの
	*→自分自身のポインタ(thisポインタ)
	*thisポインタを値の状態にして返すには＊をつける
	* ＊をつけると中身にアクセスできる
	* ですポインター＝＊
	*/

	/*
	ポインタ→渡された情報に対して＊をつけてアクセスして書き換えたときコピー元の情報は書き換わる。
	↑に対して参照も同じように動く。
	constは書き換え可能な変数に対して不可能に変える。
	オペレータのオーバーロードしたいときoperatorを=の前につける。
	↑宣言の形としては関数
	受取型(今まではポインタ(場所の住所を教える)・コピー(変数そのものを返す))
	今回(参照(値そのものを返す時にポインタのような住所))
	参照は実体保証がある形でポインタのようにコピー作らず渡せる
	＆をつけると参照
	*/
};

//Vectorとintを使う関数
//Vector2D×intのオーバーロード
Vector2D operator*(const Vector2D& vec1, const Vector2D& vec2);
Vector2D operator*(const Vector2D vec, const int num);
Vector2D operator*(const int num, const Vector2D vec);

Vector2D operator*(const Vector2D vec, const float num);
Vector2D operator*(const float num, const Vector2D vec);

Vector2D operator/(const Vector2D vec, const float num);
Vector2D operator/(const Vector2D num, Vector2D vec2);
Vector2D operator/(const float num, const Vector2D vec);