#include "Snake.h"
//初始化
void GameInit()
{
	srand((unsigned int)time(NULL));
	//打开一个窗口
	initgraph(WND_WIDTH, WND_HEIGHT); //像素点 拼图 对图片操作 图形
	setbkcolor(BGCOLOR);   //设置好背景颜色
	cleardevice();     //使用设置好的背景颜色刷新背景

	mciSendString(L"open music.mp3 alias back", 0, 0, 0);
	mciSendString(L"play back repeat", 0, 0, 0);

	//初始化一条小蛇
	rectangle(0,0,SNAKESIZE,SNAKESIZE);

	snake.sCoor[0].x = 0; //0-640	
	snake.sCoor[0].y = 0;
	snake.n = 1;
	snake.ch = right;

	food.flag = 0;  //食物是被吃掉的
}

//界面  界面图形图片 看不见的东西联系在一起
//动起来
void GameMove()
{
	//移动的过程
	for (int i = snake.n; i > 0; i--)
	{
		snake.sCoor[i].x = snake.sCoor[i - 1].x;
		snake.sCoor[i].y = snake.sCoor[i - 1].y;
	}

	//根据不同的方向 确定蛇头的移动
	switch (snake.ch)
	{
	case up:snake.sCoor[0].y -= SNAKESIZE; break;
	case down:snake.sCoor[0].y += SNAKESIZE; break;
	case left:snake.sCoor[0].x -= SNAKESIZE; break;
	case right:snake.sCoor[0].x += SNAKESIZE; break;
	}
}

//画蛇
void GameDraw()
{
	for (int i = snake.n - 1; i >= 0; i--)
	{
		rectangle(snake.sCoor[i].x, snake.sCoor[i].y,
			snake.sCoor[i].x + SNAKESIZE,
			snake.sCoor[i].y + SNAKESIZE);
	}
}

//通过按键实现蛇的方向的改变
void ChangeSnake()
{
	int move;			//ascii
	move = getch();		//按键直接出效果 (回车)  getch()
	switch (move)
	{
	case up:if(snake.ch!=down)snake.ch = up; break;
	case down:if(snake.ch!=up) snake.ch = down; break;
	case left:if(snake.ch!=right) snake.ch = left; break;
	case right:if(snake.ch!=left) snake.ch = right; break;
	}
}

//得到食物的坐标
void FoodCoor()
{
	food.fCoor.x = rand() % (WND_WIDTH / SNAKESIZE)*SNAKESIZE;  //0 10 20 30 40 50 60 70 80 ... 630
	food.fCoor.y = rand() % (WND_HEIGHT / SNAKESIZE)*SNAKESIZE;

	food.flag = 1;  //出现了新的食物 等着被吃.
}

//画食物
void DrawFood()
{
	roundrect(food.fCoor.x, food.fCoor.y,
		food.fCoor.x + SNAKESIZE, food.fCoor.y + SNAKESIZE,
		SNAKESIZE, SNAKESIZE);
}

//吃食物
void EatFood()
{
	if (snake.sCoor[0].x == food.fCoor.x&&snake.sCoor[0].y == food.fCoor.y)
	{
		snake.n++;
		food.flag = 0;
	}
}

void BreakSnake()
{
	//1蛇头不能撞墙
	if (snake.sCoor[0].x < 0 || snake.sCoor[0].x>640 || snake.sCoor[0].y < 0 ||
		snake.sCoor[0].y>480)
	{
		//结束游戏
		OverGame();
	}

	//2蛇头不能撞到自己
	for (int i = snake.n - 2; i > 0; i--)
	{
		if (snake.sCoor[0].x == snake.sCoor[i].x&&
			snake.sCoor[0].y == snake.sCoor[i].y)
		{
			//结束
			OverGame();
		}
	}
}

//
void OverGame()
{
	cleardevice();
	setcolor(YELLOW);
	settextstyle(50, 0, L"楷体");
	outtextxy(100, 100, L"10月校招加油！");
	outtextxy(200, 200, L"杭州 网易 我来了！");
	getch();
	closegraph();
}