#include "snake.h"

//����ȫ�ֱ���
Snake snake;
Food food[5];
Block block[15];
const char ICO[5] = { '$','+','*','t','p' };
const int SCORE[5] = { 1,2,3,4,5 };
int RankList[5], cnt = 0;//���а�
int hash[2000] = { 1 };//hash table �洢ֵ����֤ÿ������ֻ����һ��
char now_Dir = RIGHT;//��ǰ��ͷ����
char pre_Dir = RIGHT;//ǰһ��ʱ�����ͷ����
char direction = RIGHT;//Ԥ����ͷ����
int blockCount = 10;//�ϰ�������

int possess[SKILLNUM] = { 0 };
const char skill_string[SKILLNUM][10] = { "����","����" };
const int skill_value[SKILLNUM] = { 40,80 };
const char skill_control[SKILLNUM] = { 'j','k' };
int vis[SKILLNUM] = {0};//��������,ǰһ������,��һ����
int skill_cnt[SKILLNUM] = {0};
int vis_cnt[SKILLNUM] = {SPEEDFOOT,DUNFOOT};//�������ܸ��Ա����ƵĲ���
int score = 0;

//���˵�ʵ��
int Menu() {
	GotoXY(40, 12);
	printf("��ӭ����̰����С��Ϸ");
	GotoXY(43, 14);
	printf("1.��ʼ��Ϸ");
	GotoXY(43, 16);
	printf("2.����");
	GotoXY(43, 18);
	printf("3.����");
	GotoXY(43, 20);
	printf("4.�̵�");
	GotoXY(43, 22);
	printf("������������˳���Ϸ");
	Hide();//���ع��
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

/*��궨λ����*/
void GotoXY(int x, int y) {
	HANDLE	hout;
	COORD cor;
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	cor.X = x;
	cor.Y = y;
	SetConsoleCursorPosition(hout, cor);
}//���ƿ���̨�ϵĹ��

/*���ع��*/
void Hide() {
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO	cor_info = { 1,0 };
	SetConsoleCursorInfo(hout, &cor_info);
}

/*������Ϣ��ʾ*/
void About() {
	GotoXY(32, 12);
	printf("���ݵ��ӿƼ���ѧ--��������ۺ�ʱ�䰸��");
	GotoXY(43, 14);
	printf("̰����--����̨��Ϸ");
	GotoXY(43, 16);
	printf("������������϶β˵�");
	Hide();
	char ch = _getch();
	system("cls");
}

/*������Ϣ��ʾ*/
void Help() {
	GotoXY(40, 4);
	printf("�ﵽ50�ֻ�ʤ");
	GotoXY(40, 6);
	printf("w ��");
	GotoXY(40, 8);
	printf("s ��");
	GotoXY(40, 10);
	printf("a ��");
	GotoXY(40, 12);
	printf("d ��");
	GotoXY(46, 7);
	printf("j ����");
	GotoXY(46, 11);
	printf("k ���� �����ܿ��������ϰ�����������ӱ߽磩");
	GotoXY(40, 14);
	printf("1.����ײǽ��û�л���ײ���ϰ���ʱ��Ϸ����");
	GotoXY(40, 16);
	printf("2.������ײʱ�����ȥβ��");
	GotoXY(40, 18);
	printf("3.#���ϰ��������ͬ�ķ��ų��˿������Ӳ�ͬ�ĳ���");
	GotoXY(40, 20);
	printf("4.������������Ƕ��ǽ��ʱ�ᱻ�Զ��ض�");
	GotoXY(45, 24);
	printf("������������ϼ��˵�");
	Hide();
	char ch = _getch();
	system("cls");
}

//�̵�
void Shop() {
	int x = 40, y = 8;
	GotoXY(x, y);
	printf("��Ŀǰ���� %d ����    ",score);
	y += 2;
	for (int i = 0;i < SKILLNUM;i++) {
		GotoXY(x, y);
		printf("%s  ��Ҫ%d����  ����%d����  Ŀǰ%s����", skill_string[i],skill_value[i], i,possess[i] ? "��" : "δ");
		y += 2;
	}
	y += 6;
	GotoXY(x, y);
	printf("���������������");
	char ch = _getch();
	while (ch == '0' || ch == '1') {
		int i = ch - '0';
		if (possess[i]==0 && score >= skill_value[i]) {
			score -= skill_value[i];
			possess[i] = 1;
			GotoXY(40, 16);
			printf("  ����%s���ܳɹ�   ", skill_string[i]);
			x = 40, y = 8;
			GotoXY(x, y);
			printf("��Ŀǰ���� %d ����    ", score);
			y += (i + 1) * 2;
			GotoXY(x, y);
			printf("%s  ��Ҫ%d����  ����%d����  Ŀǰ%s����", skill_string[i], skill_value[i], i, possess[i] ? "��" : "δ");
			Sleep(200);
			GotoXY(40, 16);
			printf("                                  ");
		}
		else if (possess[i] == 1) {
			GotoXY(40, 16);
			printf("  �����ظ�����    ");
			Sleep(200);
			GotoXY(40, 16);
			printf("                                  ");
		}
		else if (score < skill_value[i]) {
			GotoXY(40, 16);
			printf("  ��Ļ��ֲ���    ");
			Sleep(100);
			GotoXY(40, 16);
			printf("                                  ");
		}
		ch = _getch();
	}
	system("cls");
	return;
}
//�������а�
void Rank(int u) {
	int i;
	for (i = cnt - 1; i >= 0; i--) {
		if (RankList[i] > u)	break;
		RankList[i + 1] = RankList[i];
	}
	RankList[i + 1] = u;		//�����µ�ֵ���Ӵ�С
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
	printf("@");//��ӡ��ͷ
	snake.length = 3;
	snake.speed = 250;
	now_Dir = RIGHT;
	pre_Dir = RIGHT;
	skill_cnt[0] =skill_cnt[1]= 0;
	vis[0] = vis[1] = 0;
	vis_cnt[0] = SPEEDFOOT;vis_cnt[1] = DUNFOOT;
	for (int i = 1; i < snake.length; i++) {
		snake.snakeNode[i].y = snake.snakeNode[i - 1].y;
		snake.snakeNode[i].x = snake.snakeNode[i - 1].x - 1;
		GotoXY(snake.snakeNode[i].x, snake.snakeNode[i].y);
		printf("o");
	}
	//���ɵ�ͼ���±߽�
	for (int i = 0; i < MAP_WIDTH; i++) {
		GotoXY(i, 0);
		printf("-");
		GotoXY(i, MAP_HEIGHT - 1);
		printf("-");
	}
	//���ɵ�ͼ���ұ߽�
	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		GotoXY(0, i);
		printf("|");
		GotoXY(MAP_WIDTH - 1, i);
		printf("|");
	}

	//�����Ѷ����ɵ��ϰ�����
	if (level == EASY)
	{
		blockCount = 5;
	}
	else if (level == HARD)
	{
		blockCount = 15;
	}
	//��������ʳ��
	PrintFood();
	//�����ϰ�
	PrintBlock(blockCount);
	//�÷�˵��
	GotoXY(60, 5);
	printf("��ǰ�÷�Ϊ0");
	//�������а�
	GotoXY(60, 8);
	printf("���а�ֻȡǰ������");
	Rank(0);
	//���ɼ�����Ϣ
	SKILL_INFORMATION();
	Hide();
}
void SKILL_INFORMATION() {
	int x = 60, y = 16;
	for (int i = 0;i < SKILLNUM;i++) {
		GotoXY(x, y);
		printf("��%sӵ��%s����", possess[i] ? "��" : "δ", skill_string[i]);
		y += 2;
		GotoXY(x, y);
		printf("%s   %c  ʹ�ô�����ʣ%d��", skill_string[i],skill_control[i],possess[i] * (MAX_SKILL_CNT - skill_cnt[i]));
		y += 2;
		GotoXY(x, y);
		printf("%s���ܻ���%d���ͽ���", skill_string[i], vis[i] * vis_cnt[i]);
		y += 2;
	}
	Hide();
}
//��������ʳ��
void PrintFood() {
	for (int i = 0; i < FOODTYPE; i++) {
		int flag = 1;//λ�����غϵı��
		while (flag) {
			flag = 0;
			//��������
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

//���ɱ���ʳ��
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

//�����ϰ�
void PrintBlock(int blockCount) {

	for (int i = 0; i < blockCount; i++) {
		int flag = 1;//λ�����غϵı��
		while (flag) {
			flag = 0;
			//��������
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

//���ɵ����ϰ�
void PrintSingleBlock(int i){
	int flag = 1;
	while (flag) {
		flag = 0;
		block[i].x = rand() % (MAP_WIDTH - 2) + 1;
		block[i].y = rand() % (MAP_HEIGHT - 2) + 1;
		for (int k = 0; k < blockCount; k++) {
			if (i != k && block[i].x == block[k].x && block[i].y == block[k].y) {
				flag = 1;
				break;
			}
		}
		for (int k = 0; k <FOODTYPE; k++) {
			if (block[i].x == food[k].x && block[i].y == food[k].y) {
				flag = 1;
				break;
			}
		}
		for (int k = 0; k < snake.length - 1; k++) {
			if (snake.snakeNode[k].x == block[i].x && snake.snakeNode[k].y == block[i].y) {
				flag = 1;
				break;
			}
		}
	}
	GotoXY(block[i].x, block[i].y);
	printf("#");
}
//���ƶ�(ʵ���Ǹ���ͷ��β),�����ǲ��ϸ��������ƶ�
int MoveSnake(int level) {
	//���¼�����Ϣ
	SKILL_INFORMATION();
	for (int i = 0;i < SKILLNUM;i++) {
		if (vis_cnt[i] == 0)	vis[i] = 0;
		if (vis[i] == 1) {
			vis_cnt[i]--;
		}
	}
	Snakenode temp;
	int flag = 0;//�ж��Ƿ�Ե�ʳ��
	temp = snake.snakeNode[snake.length - 1];
	for (int i = snake.length - 1; i >= 1; i--) {
		snake.snakeNode[i] = snake.snakeNode[i - 1];
	}
	GotoXY(snake.snakeNode[1].x, snake.snakeNode[1].y);
	printf("o");//������������Ȼ�ڣ�ֻҪ����ͷ
	pre_Dir = now_Dir;//��¼ԭ����
	if (_kbhit()) {			//��������Ӧ�������룬�������뷵��1���Ǽ������뷵��0
		direction = _getch();
		switch (direction) {
		case SPEED:
			if (!vis[0] && skill_cnt[0] < MAX_SKILL_CNT) {
				vis[0] = 1, vis_cnt[0] = SPEEDFOOT, skill_cnt[0]++;
				SKILL_INFORMATION();
			}
			break;
		case DUN:
			if (!vis[1] && skill_cnt[1] < MAX_SKILL_CNT) {
				vis[1] = 1, vis_cnt[1] = DUNFOOT, skill_cnt[1]++;
				SKILL_INFORMATION();
			}
			break;
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
	//��ӡ��ͷ
	GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y);
	printf("@");
	//�ж��Ƿ�Ե�ʳ��
	for (int i = 0; i < FOODTYPE; i++) {
		if (snake.snakeNode[0].x == food[i].x && snake.snakeNode[0].y == food[i].y) {
			snake.length++;
			flag = 1;
			snake.snakeNode[snake.length - 1] = temp;//�Ƚ�ԭ����β(���������ӵĳ�����)�ĵ�һ����¼,��һ��һ��������
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
				if (!f)	break;//�������߽��ˣ��򲻿��ټ�������(�ض�)
				snake.length++;
				GotoXY(snake.snakeNode[snake.length - 1].x, snake.snakeNode[snake.length - 1].y);
				printf("o");
			}
		}
		if (!flag) {
			GotoXY(temp.x, temp.y);
			printf(" ");//ɾ��ԭβ��
		}
		else {
			PrintSingleFood(i);//���ɳԵ���ʳ��
			GotoXY(60, 5);
			printf("��ǰ�÷֣�%d        ", snake.length - 3);
			break;
		}
	}
	//�ж��Ƿ񳬳��߽�
	if (!IsBorder()) {
		system("cls");
		GotoXY(45, 16);
		printf("���յ÷֣�%d", snake.length - 3);
		GotoXY(45, 18);
		printf("�����ˣ�");
		score += snake.length - 3;//����

		/*��һ�����а�*/
		GotoXY(45, 8);
		printf("���а�ֻȡǰ������");
		int f = 0;
		if (!hash[snake.length - 3]) {
			cnt++;
			hash[snake.length - 3] = 1;
			if (cnt > 3)	cnt = 3;//ֻ������
			f = 1;
		}
		if (f) {
			int i;
			for (i = cnt - 1; i >= 0; i--) {
				if (RankList[i] > snake.length - 3)	break;
				RankList[i + 1] = RankList[i];
			}
			RankList[i + 1] = snake.length - 3;		//�����µ�ֵ���Ӵ�С
		}
		int x = 45, y = 10;
		for (int i = 0; i < cnt; i++) {
			GotoXY(x, y);
			printf("%d       %d", i + 1, RankList[i]);
			y += 2;
		}
		GotoXY(45, 20);
		printf("��������������˵�");
		char c = _getch();
		system("cls");
		return 0;
	}
	//�ж��Ƿ�ײǽ
	int pos = IsBlock();
	if (~pos) {
		if (vis[1] == 1) {
			PrintSingleBlock(pos);
		}
		else {
			system("cls");
			GotoXY(45, 16);
			printf("���յ÷֣�%d", snake.length - 3);
			GotoXY(45, 18);
			printf("�����ˣ�");
			score += snake.length - 3;//����

			/*��һ�����а�*/
			GotoXY(45, 8);
			printf("���а�ֻȡǰ������");
			int f = 0;
			if (!hash[snake.length - 3]) {
				cnt++;
				hash[snake.length - 3] = 1;
				if (cnt > 3)	cnt = 3;//ֻ������
				f = 1;
			}
			if (f) {
				int i;
				for (i = cnt - 1; i >= 0; i--) {
					if (RankList[i] > snake.length - 3)	break;
					RankList[i + 1] = RankList[i];
				}
				RankList[i + 1] = snake.length - 3;		//�����µ�ֵ���Ӵ�С
			}
			int x = 45, y = 10;
			for (int i = 0; i < cnt; i++) {
				GotoXY(x, y);
				printf("%d       %d", i + 1, RankList[i]);
				y += 2;
			}
			GotoXY(45, 20);
			printf("��������������˵�");
			char c = _getch();
			system("cls");
			return 0;
		}
	}
	//�ж��Ƿ���ײ
    pos = IsSelf();
	if (pos != snake.length) {			//���˻��ܾͲ����ȡ
		for (int i = pos; i < snake.length; i++) {
			GotoXY(snake.snakeNode[i].x, snake.snakeNode[i].y);
			printf(" ");
		}			//�ض�β��
		snake.length = pos;
		GotoXY(60, 5);
		printf("��ǰ�÷֣�%d      ", snake.length - 3);	//Ҫ��ǰ��Ķ�ɾ������Ȼһλ�����ǲ�����λ����������ʱ��"    "
	}
	//�ж��Ƿ�ʤ��
	if (snake.length - 3 >= 50) {
		system("cls");
		GotoXY(45, 14);
		printf("���ʤ��!!!");
		GotoXY(45, 16);
		score += snake.length - 3;
		printf("�����������");
		char ch = _getch();
		system("cls");
		return 0;
	}
	//�����ٶ�
	SpeedControl(level);
	if (vis[0] == 1) { Sleep(snake.speed / 2); return 1; }//���ټ���
	Sleep(snake.speed);
	return 1;
}

//��ײ
int IsSelf() {
	for (int i = 1; i < snake.length; i++) {
		if (snake.snakeNode[0].x == snake.snakeNode[i].x && snake.snakeNode[0].y == snake.snakeNode[i].y)
			return  i;
	}
	return snake.length;
}
//����
int IsBorder() {
	if (snake.snakeNode[0].x == 0 || snake.snakeNode[0].y == 0 || snake.snakeNode[0].x == MAP_WIDTH - 1 || snake.snakeNode[0].y == MAP_HEIGHT - 1)
		return 0;
	return 1;
}
int IsBlock() {
	for (int i = 0; i < blockCount; i++) {
		if ( snake.snakeNode[0].x == block[i].x && snake.snakeNode[0].y == block[i].y)	return i;
	}
	return -1;
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
//�ؿ�
int SelectLevel()
{
	GotoXY(40, 12);
	printf("��ѡ����Ϸ�Ѷȣ�");
	GotoXY(40, 14);
	printf("1. ��");
	GotoXY(40, 16);
	printf("2. ��ͨ");
	GotoXY(40, 18);
	printf("3. ����");

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

