//ͷ�ļ�
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
//�궨��
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
//�ṹ��(�㣬��)
typedef struct {
	int x, y;
}Food, Snakenode, Block;

typedef struct {
	Snakenode snakeNode[1000];
	int length;
	int speed;
}Snake;
void GotoXY(int, int);//��궨λ
void Hide();//���ع��
int Menu();//���˵�
void Help();//������Ϣ
void About();//������Ϣ
void InitMap(int);//��ʼ��ͼ
void PrintFood();//��������ʳ��
void PrintSingleFood(int);//���ɵ���ʳ��
void PrintBlock(int);//�����ϰ���
void PrintSingleBlock(int);//���ɵ����ϰ���
int MoveSnake(int);//���ƶ�
int IsBorder();//ײǽ��ʾ
int IsBlock();//ײ�ϰ�����ʾ
int IsSelf();//��ײ��ʾ
void SpeedControl(int level);//�ٶȿ���
void Rank(int);//��������
void SKILL_INFORMATION();//������Ϣ
void Shop();//�̵�