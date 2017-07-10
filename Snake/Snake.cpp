#include "main.h"
#include "Snake.h"
int main()
{
	GameInit();
	while (1)
	{
		while (!kbhit())
		{
			if (food.flag == 0)
			{
				FoodCoor();
			}
			BeginBatchDraw();
			cleardevice();
			DrawFood();
			EatFood();
			GameDraw();
			GameMove();
			Sleep(100);
			EndBatchDraw();
			BreakSnake();
		}
		ChangeSnake();
		
	}
	getchar();  //卡屏 保证打开的窗口不闪退.
	return 0;
}