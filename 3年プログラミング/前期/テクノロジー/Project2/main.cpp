#include <iostream>

using namespace std;

//�ʐM�̎��g��
struct BitField
{
	char a : 1;
	char b : 2;
	char c : 3;
	char d : 2;
};

//���p��
union Data {
	//��̌^�Ɖ��̕��͈Ⴄ���A�������̈�����L���Ă���
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