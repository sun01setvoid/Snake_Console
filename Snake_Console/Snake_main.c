#include "snake.h"
int main() {
	srand((unsigned int)time(0));
	int end = 1, result, level = MEDIUM;   //Ĭ���е�
	while (end) {
		result = Menu();
		switch (result) {
		case 1:
			level = SelectLevel();
			//ѡ��1����ʼ̰������Ϸ
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