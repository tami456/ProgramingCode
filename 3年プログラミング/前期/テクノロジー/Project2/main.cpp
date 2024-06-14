#include <iostream>

using namespace std;

//通信の時使う
struct BitField
{
	char a : 1;
	char b : 2;
	char c : 3;
	char d : 2;
};

//共用体
union Data {
	//上の型と下の方は違うが、メモリ領域を共有している
	char c;
	BitField b;
};

int main()
{
	BitField b;
	cout << sizeof(b) << endl;
	cout << sizeof(bool) << endl;
	cout << sizeof(Data) << endl;
	
}