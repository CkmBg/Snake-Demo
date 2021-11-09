#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#define UP 4

typedef struct snake{
		char body;
		int x, y;
}snake;

typedef struct miam{
		int x;
		int y;
}miam;

void gotoxy(int x, int y);

void color(int t,int f);

void peindre(int debut_x, int fin_x, int debut_y, int fin_y, int font);

void clean(int debut_x, int fin_x, int debut_y, int fin_y);

int MENU(char Menu[UP][20], int max);

int CAMPAGNE(int max);

int SURVIVAL(int max);

void draw_snake();

void move(int *x, int *y, snake *S);

int verif(int x, int y, miam M[5]);
	
void generate(int x, int y, miam M[5]);

void init_snake(snake S[2]);

void crawl(int *x, int *y, int taille, snake *S);

int verif_miam(snake *S);

void food(snake *S);

int mort(snake *S);

void game();

