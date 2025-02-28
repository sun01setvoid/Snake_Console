#include "snake.h"
int main() {
	srand((unsigned int)time(0));
	int end = 1, result, level = MEDIUM;   //默认中等
	while (end) {
		result = Menu();
		switch (result) {
		case 1:
			level = SelectLevel();
			//选项1，开始贪吃蛇游戏
			InitMap(level);
			while (MoveSnake(level));
			break;
		case 2:
			Help();
			break;
		case 3:
			About();
			break;
		case 4:
			end = 0;
			break;
		}
	}
}