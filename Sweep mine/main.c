#include"chess.h"
void menu()
{
	printf(" \n                                        ************É¨À×Ó¢ĞÛ°Ô************\n");
	printf("                                           1--->Play ^^^^^^^^^^0---->Exit\n");
	printf("                                        ~~~~~~~~~Come to Play Game~~~~~~~~\n");
}
int main()
{
	int input=0;
	char mine[rows][lows]={0};
	char show[rows][lows]={0};
	srand((unsigned)time( NULL));
	menu();
	printf("ÇëÑ¡Ôñ->:");
	scanf("%d",&input);
    switch(input)
		{
		case 1:
			init(mine,show);
			break;
		case 0:
			break;
        default:printf("ÊäÈëÓĞÎó!");
			break;
		}
	system("pause");
	return 0;
}