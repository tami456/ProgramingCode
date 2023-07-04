
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
	int num;
	num = 0;

	printf("数字を入力してください\n");
	scanf_s("%d", &num);
	
	/*for(現在の回数を入れる変数を書く;規定回数のチェックを行う;現在の回数をカウントアップする)*/
	/*int j= 0*/

	/*j = j + 1
	j += 1
	j++*/
	/*++j*/
	
	for (int j = 0;j < 3; j++)
	{
		printf("入力した数字は%dです\n", num);
	}





	/*if (num >= 10 & num < 100)
	{
		printf("その数字は10以上、100未満です\n");
	}
	if (num >= 100)
	{
		printf("その数字は100以上です\n");
	}
	if (num < 10)
	{
		printf("その数字は10未満です\n");
	}*/

	/*if (num >= 10)
	{
		if (num >= 100)
		{
			printf("その数字は100以上です\n");
		}
		else
		{
			printf("その数字は10以上、100未満です\n");
		}
	}
	else
	{
		printf("numは10未満\n");
	}*/



	/*if (num >= 100)
	{
		printf("その数字は100以上です\n");
	}
	else if (num >= 10)
	{
		printf("その数字は10以上、100未満です\n");
	}
	else
	{
		printf("その数字は10未満です\n");
	}*/

	/*printf("その数字は奇数です\n");
	printf("その数字は偶数です\n");


	printf("数字は%dです\n",num);

	num = num + 1;
	printf("計算結果は%dです\n", num);

	num = num * 5;
	printf("計算結果は%dです\n", num);

	num = num - 3;
	printf("計算結果は%dです\n", num);*/

	system("pause");
	return 0;
}

