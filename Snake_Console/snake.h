//头文件
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
//宏定义
#define MAP_HEIGHT 25
#define MAP_WIDTH 50
#define FOODTYPE 3

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

#define EASY 1
#define MEDIUM 2
#define HARD 3

#define SPEED 'j'
#define DUN 'k'
#define SPEEDFOOT 30
#define DUNFOOT 40
#define MAX_SKILL_CNT 3
#define SKILLNUM 2
//结构体(点，蛇)
typedef struct {
	int x, y;
}Food, Snakenode, Block;

typedef struct {
	Snakenode snakeNode[1000];
	int length;
	int speed;
}Snake;
void GotoXY(int, int);//光标定位
void Hide();//隐藏光标
int Menu();//主菜单
void Help();//帮助信息
void About();//关于信息
void InitMap(int);//初始地图
void PrintFood();//生成所有食物
void PrintSingleFood(int);//生成单个食物
void PrintBlock(int);//生成障碍物
void PrintSingleBlock(int);//生成单个障碍物
int MoveSnake(int);//蛇移动
int IsBorder();//撞墙提示
int IsBlock();//撞障碍物提示
int IsSelf();//自撞提示
void SpeedControl(int level);//速度控制
void Rank(int);//更新排行
void SKILL_INFORMATION();//技能信息
void Shop();//商店