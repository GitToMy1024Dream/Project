#pragma once
#include  <stdio.h>
#include  <time.h>
#include  <conio.h>
#include  <stdlib.h>
#include  <graphics.h>  //图形界面头文件
#include  <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define WND_WIDTH   640   //窗口的宽
#define WND_HEIGHT  480   //高
#define BGCOLOR		RGB(220,120,150)  //背景颜色
#define SNAKESIZE   10
#define SNAKEMAX	100

//枚举方向
typedef enum Ch {up=72,down=80,left=75,right=77}CH;

typedef struct Coor  //坐标
{
	int x;
	int y;
}COOR;

//结构一条蛇
typedef struct Snake
{
	int n;   //有多少节蛇  每一节蛇都是一个小矩形
	COOR sCoor[SNAKEMAX];  //100个坐标组成的数组
	CH ch;
}SNAKE;

typedef struct Food
{
	COOR fCoor;   //坐标

	int flag;    //食物是否被吃掉的标记
}FOOD;

SNAKE snake;
FOOD food;

//初始化
void GameInit();
void GameMove();
void GameDraw();
void ChangeSnake();
void FoodCoor();
void DrawFood();
//吃食物
void EatFood();
void OverGame();
void BreakSnake();



