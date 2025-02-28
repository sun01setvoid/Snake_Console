#include "snake.h"

//定义全局变量
Snake snake;
Food food[5];
Block block[15];
const char ICO[5] = { '$','+','*','t','p' };
const int SCORE[5] = { 1,2,3,4,5 };
int RankList[5], cnt = 0;//排行榜
int hash[2000] = { 1 };//hash table 存储值，保证每个分数只出现一次
char now_Dir = RIGHT;//当前谁头方向
char pre_Dir = RIGHT;//前一个时间的蛇头方向
char direction = RIGHT;//预期蛇头方向
int blockCount = 10;//障碍物数量

//主菜单实现
int Menu() {
	GotoXY(40, 12);
	printf("欢迎来到贪吃蛇小游戏");
	GotoXY(43, 14);
	printf("1.开始游戏");
	GotoXY(43, 16);
	printf("2.帮助");
	GotoXY(43, 18);
	printf("3.关于");
	GotoXY(43, 20);
	printf("按其他任意键退出游戏");
	Hide();//隐藏光标
	char ch;
	int result = 0;
	ch = _getch();
	switch (ch) {
	case '1': result = 1; break;
	case '2': result = 2; break;
	case '3': result = 3; break;
	case '4': result = 4; break;
	}
	system("cls");
	return result;
}

/*光标定位函数*/
void GotoXY(int x, int y) {
	HANDLE	hout;
	COORD cor;
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	cor.X = x;
	cor.Y = y;
	SetConsoleCursorPosition(hout, cor);
}//控制控制台上的光标

/*隐藏光标*/
void Hide() {
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO	cor_info = { 1,0 };
	SetConsoleCursorInfo(hout, &cor_info);
}

/*关于信息显示*/
void About() {
	GotoXY(32, 12);
	printf("杭州电子科技大学--程序设计综合时间案例");
	GotoXY(43, 14);
	printf("贪吃蛇--控制台游戏");
	GotoXY(43, 16);
	printf("按任意键返回上段菜单");
	Hide();
	char ch = _getch();
	system("cls");
}

/*帮助信息显示*/
void Help() {
	GotoXY(40, 4);
	printf("达到50分获胜");
	GotoXY(40, 6);
	printf("w 上");
	GotoXY(40, 8);
	printf("s 下");
	GotoXY(40, 10);
	printf("a 左");
	GotoXY(40, 12);
	printf("d 右");
	GotoXY(40, 14);
	printf("1.当蛇撞墙或撞到障碍物时游戏结束");
	GotoXY(40, 16);
	printf("2.当蛇自撞时，会截去尾部");
	GotoXY(40, 18);
	printf("3.#是障碍物，其他不同的符号吃了可以增加不同的长度");
	GotoXY(40, 20);
	printf("4.当增长的蛇身嵌入墙体时会被自动截断");
	GotoXY(45, 24);
	printf("按任意键返回上级菜单");
	Hide();
	char ch = _getch();
	system("cls");
}

//更新排行榜
void Rank(int u) {
	int i;
	for (i = cnt - 1; i >= 0; i--) {
		if (RankList[i] > u)	break;
		RankList[i + 1] = RankList[i];
	}
	RankList[i + 1] = u;		//插入新的值，从大到小
	int x = 60, y = 10;
	for (i = 0; i < cnt; i++) {
		GotoXY(x, y);
		printf("%d       %d", i + 1, RankList[i]);
		y += 2;
	}
}
void InitMap(int level) {
	Hide();
	snake.snakeNode[0].x = MAP_WIDTH / 2 - 1;
	snake.snakeNode[0].y = MAP_HEIGHT / 2 - 1;
	GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y);
	printf("@");//打印蛇头
	snake.length = 3;
	snake.speed = 250;
	now_Dir = RIGHT;
	pre_Dir = RIGHT;
	for (int i = 1; i < snake.length; i++) {
		snake.snakeNode[i].y = snake.snakeNode[i - 1].y;
		snake.snakeNode[i].x = snake.snakeNode[i - 1].x - 1;
		GotoXY(snake.snakeNode[i].x, snake.snakeNode[i].y);
		printf("o");
	}
	//生成地图上下边界
	for (int i = 0; i < MAP_WIDTH; i++) {
		GotoXY(i, 0);
		printf("-");
		GotoXY(i, MAP_HEIGHT - 1);
		printf("-");
	}
	//生成地图左右边界
	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		GotoXY(0, i);
		printf("|");
		GotoXY(MAP_WIDTH - 1, i);
		printf("|");
	}

	//根据难度生成的障碍数量
	if (level == EASY)
	{
		blockCount = 5;
	}
	else if (level == HARD)
	{
		blockCount = 15;
	}
	//生成所有食物
	PrintFood();
	//生成障碍
	PrintBlock(blockCount);
	//得分说明
	GotoXY(60, 5);
	printf("当前得分为0");
	//生成排行榜
	GotoXY(60, 8);
	printf("排行榜（只取前三名）");
	Rank(0);
}

//生成所有食物
void PrintFood() {
	for (int i = 0; i < FOODTYPE; i++) {
		int flag = 1;//位置有重合的标记
		while (flag) {
			flag = 0;
			//设置坐标
			food[i].x = rand() % (MAP_WIDTH - 2) + 1;
			food[i].y = rand() % (MAP_HEIGHT - 2) + 1;
			for (int k = 0; k < snake.length - 1; k++) {
				if (snake.snakeNode[k].x == food[i].x && snake.snakeNode[k].y == food[i].y) {
					flag = 1;
					break;
				}
			}
			for (int k = 0; k < i; k++) {
				if (food[i].x == food[k].x && food[i].y == food[k].y) {
					flag = 1;
					break;
				}
			}
			for (int k = 0; k < blockCount; k++) {
				if (food[i].x == block[k].x && food[i].y == block[k].y) {
					flag = 1;
					break;
				}
			}
		}
		GotoXY(food[i].x, food[i].y);
		printf("%c", ICO[i]);
	}
}

//生成被吃食物
void PrintSingleFood(int i) {
	int flag = 1;
	while (flag) {
		flag = 0;
		food[i].x = rand() % (MAP_WIDTH - 2) + 1;
		food[i].y = rand() % (MAP_HEIGHT - 2) + 1;
		for (int k = 0; k < FOODTYPE; k++) {
			if (i != k && food[i].x == food[k].x && food[i].y == food[k].y) {
				flag = 1;
				break;
			}
		}
		for (int k = 0; k < blockCount; k++) {
			if (food[i].x == block[k].x && food[i].y == block[k].y) {
				flag = 1;
				break;
			}
		}
		for (int k = 0; k < snake.length - 1; k++) {
			if (snake.snakeNode[k].x == food[i].x && snake.snakeNode[k].y == food[i].y) {
				flag = 1;
				break;
			}
		}
	}
	GotoXY(food[i].x, food[i].y);
	printf("%c", ICO[i]);
}

//生成障碍
void PrintBlock(int blockCount) {

	for (int i = 0; i < blockCount; i++) {
		int flag = 1;//位置有重合的标记
		while (flag) {
			flag = 0;
			//设置坐标
			block[i].x = rand() % (MAP_WIDTH - 2) + 1;
			block[i].y = rand() % (MAP_HEIGHT - 2) + 1;
			for (int k = 0; k < snake.length - 1; k++) {
				if (snake.snakeNode[k].x == block[i].x && snake.snakeNode[k].y == block[i].y) {
					flag = 1;
					break;
				}
			}
			for (int k = 0; k < FOODTYPE; k++) {
				if (block[i].x == food[k].x && block[i].y == food[k].y) {
					flag = 1;
					break;
				}
			}
			for (int k = 0; k < i; k++) {
				if (block[i].x == block[k].x && block[i].y == block[k].y) {
					flag = 1;
					break;
				}
			}
		}
		GotoXY(block[i].x, block[i].y);
		printf("#");
	}
}
//蛇移动(实际是更新头和尾),身体是不断跟随他们移动
int MoveSnake(int level) {
	Snakenode temp;
	int flag = 0;//判断是否吃到食物
	temp = snake.snakeNode[snake.length - 1];
	for (int i = snake.length - 1; i >= 1; i--) {
		snake.snakeNode[i] = snake.snakeNode[i - 1];
	}
	GotoXY(snake.snakeNode[1].x, snake.snakeNode[1].y);
	printf("o");//其他的身子仍然在，只要补上头
	pre_Dir = now_Dir;//记录原方向
	if (_kbhit()) {			//非阻塞响应键盘输入，键盘输入返回1，非键盘输入返回0
		direction = _getch();
		switch (direction) {
		case UP:
			if (now_Dir != DOWN)
				now_Dir = direction;
			break;
		case DOWN:
			if (now_Dir != UP)
			{
				now_Dir = direction;
			}
			break;
		case LEFT:
			if (now_Dir != RIGHT)
			{
				now_Dir = direction;
			}
			break;
		case RIGHT:
			if (now_Dir != LEFT)
			{
				now_Dir = direction;
			}
			break;
		}
	}
	switch (now_Dir) {
	case UP:snake.snakeNode[0].y--; break;
	case DOWN:snake.snakeNode[0].y++; break;
	case LEFT:snake.snakeNode[0].x--; break;
	case RIGHT:snake.snakeNode[0].x++; break;
	}
	//打印蛇头
	GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y);
	printf("@");
	//判断是否吃到食物
	for (int i = 0; i < FOODTYPE; i++) {
		if (snake.snakeNode[0].x == food[i].x && snake.snakeNode[0].y == food[i].y) {
			snake.length++;
			flag = 1;
			snake.snakeNode[snake.length - 1] = temp;//先将原来的尾(后来的增加的长度中)的第一个记录,第一个一定能增长
			for (int j = 0; j < SCORE[i] - 1; j++) {
				int f = 1;
				switch (pre_Dir) {
				case UP:
					if (snake.snakeNode[snake.length - 1].y + 1 >= MAP_HEIGHT - 1) {
						f = 0;
						break;
					}
					snake.snakeNode[snake.length].x = snake.snakeNode[snake.length - 1].x;
					snake.snakeNode[snake.length].y = snake.snakeNode[snake.length - 1].y + 1;
					break;
				case DOWN:
					if (snake.snakeNode[snake.length - 1].y - 1 <= 0) {
						f = 0;
						break;
					}
					snake.snakeNode[snake.length].x = snake.snakeNode[snake.length - 1].x;
					snake.snakeNode[snake.length].y = snake.snakeNode[snake.length - 1].y - 1;
					break;
				case LEFT:
					if (snake.snakeNode[snake.length - 1].x + 1 >= MAP_WIDTH - 1) {
						f = 0;
						break;
					}
					snake.snakeNode[snake.length].x = snake.snakeNode[snake.length - 1].x + 1;
					snake.snakeNode[snake.length].y = snake.snakeNode[snake.length - 1].y;
					break;
				case RIGHT:
					if (snake.snakeNode[snake.length - 1].x - 1 <= 0) {
						f = 0;
						break;
					}
					snake.snakeNode[snake.length].x = snake.snakeNode[snake.length - 1].x - 1;
					snake.snakeNode[snake.length].y = snake.snakeNode[snake.length - 1].y;
					break;
				}
				if (!f)	break;//若超出边界了，则不可再继续生长(截断)
				snake.length++;
				GotoXY(snake.snakeNode[snake.length - 1].x, snake.snakeNode[snake.length - 1].y);
				printf("o");
			}
		}
		if (!flag) {
			GotoXY(temp.x, temp.y);
			printf(" ");//删除原尾巴
		}
		else {
			PrintSingleFood(i);//生成吃掉的食物
			GotoXY(60, 5);
			printf("当前得分：%d        ", snake.length - 3);
			break;
		}
	}
	//判断是否死亡
	if (!IsCorrect()) {
		system("cls");
		GotoXY(45, 16);
		printf("最终得分：%d", snake.length - 3);
		GotoXY(45, 18);
		printf("你输了！");

		/*看一下排行榜*/
		GotoXY(45, 8);
		printf("排行榜（只取前三名）");
		int f = 0;
		if (!hash[snake.length - 3]) {
			cnt++;
			hash[snake.length - 3] = 1;
			if (cnt > 3)	cnt = 3;//只存三个
			f = 1;
		}
		if (f) {
			int i;
			for (i = cnt - 1; i >= 0; i--) {
				if (RankList[i] > snake.length - 3)	break;
				RankList[i + 1] = RankList[i];
			}
			RankList[i + 1] = snake.length - 3;		//插入新的值，从大到小
		}
		int x = 45, y = 10;
		for (int i = 0; i < cnt; i++) {
			GotoXY(x, y);
			printf("%d       %d", i + 1, RankList[i]);
			y += 2;
		}
		GotoXY(45, 20);
		printf("按任意键返回主菜单");
		char c = _getch();
		system("cls");
		return 0;
	}
	//判断是否自撞
	int pos = IsSelf();
	if (pos != snake.length) {
		for (int i = pos; i < snake.length; i++) {
			GotoXY(snake.snakeNode[i].x, snake.snakeNode[i].y);
			printf(" ");
		}			//截断尾巴
		snake.length = pos;
		GotoXY(50, 5);
		printf("当前得分：%d      ", snake.length - 3);	//要把前面的都删掉，不然一位数覆盖不了两位数，这里暂时用"    "
	}
	//判断是否胜利
	if (snake.length - 3 >= 50) {
		system("cls");
		GotoXY(45, 14);
		printf("你获胜了!!!");
		GotoXY(45, 16);
		printf("按任意键继续");
		char ch = _getch();
		system("cls");
		return 0;
	}
	//调整速度
	SpeedControl(level);
	Sleep(snake.speed);
	return 1;
}

//自撞
int IsSelf() {
	for (int i = 1; i < snake.length; i++) {
		if (snake.snakeNode[0].x == snake.snakeNode[i].x && snake.snakeNode[0].y == snake.snakeNode[i].y)
			return  i;
	}
	return snake.length;
}
//死亡
int IsCorrect() {
	if (snake.snakeNode[0].x == 0 || snake.snakeNode[0].y == 0 || snake.snakeNode[0].x == MAP_WIDTH - 1 || snake.snakeNode[0].y == MAP_HEIGHT - 1)
		return 0;
	for (int i = 0; i < blockCount; i++) {
		if (snake.snakeNode[0].x == block[i].x && snake.snakeNode[0].y == block[i].y)	return 0;
	}
	return 1;
}

void SpeedControl(int level)
{
	switch (level)
	{
	case EASY:
		switch (snake.length)
		{
		case 7:snake.speed = 240; break;
		case 15:snake.speed = 230; break;
		case 20:snake.speed = 220; break;
		case 25:snake.speed = 210; break;
		case 29:snake.speed = 200; break;
		case 35:snake.speed = 190; break;
		case 39:snake.speed = 180; break;
		case 42:snake.speed = 170; break;
		case 46:snake.speed = 160; break;
		default:break;
		}
	case MEDIUM:
		switch (snake.length)
		{
		case 7:snake.speed = 230; break;
		case 11:snake.speed = 210; break;
		case 15:snake.speed = 190; break;
		case 19:snake.speed = 170; break;
		case 23:snake.speed = 150; break;
		case 27:snake.speed = 130; break;
		case 31:snake.speed = 110; break;
		case 35:snake.speed = 90; break;
		case 39:snake.speed = 70; break;
		default:break;
		}
	case HARD:
		switch (snake.length)
		{
		case 7:snake.speed = 200; break;
		case 11:snake.speed = 180; break;
		case 15:snake.speed = 160; break;
		case 19:snake.speed = 140; break;
		case 23:snake.speed = 120; break;
		case 27:snake.speed = 100; break;
		case 31:snake.speed = 80; break;
		case 35:snake.speed = 60; break;
		case 39:snake.speed = 40; break;
		default:break;
		}
	}
}
//关卡
int SelectLevel()
{
	GotoXY(40, 12);
	printf("请选择游戏难度：");
	GotoXY(40, 14);
	printf("1. 简单");
	GotoXY(40, 16);
	printf("2. 普通");
	GotoXY(40, 18);
	printf("3. 困难");

	int level;
	char ch = _getch();

	switch (ch)
	{
	case'1': level = EASY; break;
	case'2': level = MEDIUM; break;
	case'3': level = HARD; break;
	default: level = MEDIUM; break;
	}

	system("cls");
	return level;
}

