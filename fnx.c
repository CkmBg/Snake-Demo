
#include "fnx.h"
#include <windows.h>
#define UP 4
#define SPEED 150
#define SPEEDMIN 90

char c = 77, direction = 77; // pour les touches de direction
char menu[5][20]={"CAMPAIGN", "SURVIVAL", "HELP", "QUIT"};
char camp[4][20]={"NEW GAME", "HIGHSCORES", "BACK"};
int dx =10, dy =5, taille = 2, in = 0, in_c = 0, speed;

// dx dy coordonnee du miam
// pour afficher les scores vide

typedef struct record{
	int length, point;
	char nom[20];
}record;

void gotoxy(int x, int y){ // Deplacement curseur
   HANDLE hConsoleOutput;
   COORD dwCursorPosition;
   dwCursorPosition.X = x;
   dwCursorPosition.Y = y;
   hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleCursorPosition(hConsoleOutput,dwCursorPosition);
}

void color(int t,int f){ // changer la couleur du texte et du fond
        HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(H,f*16+t);
}

void peindre(int debut_x, int fin_x, int debut_y, int fin_y, int font){ // peint un espace donn� avec une couleur precise
	int i, j;
	for(i=debut_x; i<fin_x; i++){
		for(j=debut_y; j<fin_y; j++){
			gotoxy(i,j);
			color(0,font);printf(" ");
		}
	color(5, 15);
	}
}

void clean(int debut_x, int fin_x, int debut_y, int fin_y){ //nettoye l'ecran
	int i, j;
	color(5, 15);
	for(i=debut_x; i<fin_x; i++){
		for(j=debut_y; j<fin_y; j++){
			gotoxy(i,j); printf(" ");
		}
	}
}

int MENU(char Menu[UP][20], int max){ // passer le terminal en mode brut ain que getchar demande uniquement un carractère
	char c; // elle nous permet de recuperer les touches directionnelles
	int i, j=0; // variable servant a� deplacer le curseur
	clean(0, 20, 9, 19);
	do{
		gotoxy(6,11); if(j==0){color(15, 5);printf("%s", Menu[0]);} else {color(5, 15);printf("%s", Menu[0]);}
		gotoxy(6,13); if(j==1){color(15, 5);printf("%s", Menu[1]);} else {color(5, 15);printf("%s", Menu[1]);}
		gotoxy(8,15); if(j==2){color(15, 5);printf("%s", Menu[2]);} else {color(5, 15);printf("%s", Menu[2]);}
		gotoxy(8,17); if(j==3){color(15, 5);printf("%s", Menu[3]);} else {color(5, 15);printf("%s", Menu[3]);}
		do{ c = getch(); }while(c !=80 && c!= 72 && c!=13);// recupere le caractère  de HAUT et BAS.
		if(c == 72) j--; // Si c'est le HAUT, j augemente, la fleche monte
		if(c == 80) j++; // si c'est le BAS, j diminue, la fleche descend
		if(j > max-1) j=0; // Si on depasser la dernière option du menu on remonte au debut
		if(j < 0) j=max-1; // Si on monte au-dessus de la première option du menu on part à la fin
		// entrer = 13 pour sortir du menu deroulant
		color(5, 15);

	}while(c != 13);
	return j;
}

int CAMPAGNE(int max){// passer le terminal en mode brut ain que getchar demande uniquement un carractère
	clean(0, 20, 9, 19);
	char c; // elle nous permet de recuperer les touches directionnelles
	int i, j=0; // variable servant à déplacer le curseur
	do{
		gotoxy(6,11); if(j==0){color(15, 5);printf("%s", camp[0]);} else {color(5, 15);printf("%s", camp[0]);}
		gotoxy(5,13); if(j==1){color(15, 5);printf("%s", camp[1]);} else {color(5, 15);printf("%s", camp[1]);}
		gotoxy(8,15); if(j==2){color(15, 5);printf("%s", camp[2]);} else {color(5, 15);printf("%s", camp[2]);}
		do{ c = getch(); }while(c !=80 && c!= 72 && c!=13);// recupere le caractère  de HAUT et BAS.
		if(c == 72) j--; // Si c'est le HAUT, j augemente, la fleche monte
		if(c == 80) j++; // si c'est le BAS, j diminue, la fleche descend
		if(j > max-1) j=0; // Si on depasser la dernière option du menu on remonte au debut
		if(j < 0) j=max-1; // Si on monte au-dessus de la première option du menu on part à la fin
		// entrer = 13 pour sortir du menu deroulant
		color(5, 15);
	}while(c != 13);
	return j;
}

void draw_snake(){// dessin le serent
	int i;
	system("mode con: LINES=22 COLS=20");
	peindre(5, 16, 0, 7, 2);
	for(i=0; i<6; i++){
		color(10, 0);
		gotoxy(5,i); printf("%c", 219);
		gotoxy(4,i); printf("%c", 219);
		gotoxy(16,i); printf("%c", 219);
		gotoxy(15,i); printf("%c", 219);
	}
	for(i=5; i<16; i++){
		color(10, 0);
		gotoxy(i,2); printf("%c", 219);
		if(i<7 || i>13)	{	gotoxy(i,6); printf("%c", 219); }
		if(8<i || i<12)	{	gotoxy(i,7); printf("%c", 219); }
	}
	color(15, 0);
	gotoxy(8,5); printf("%c", 219); gotoxy(7,5); printf("%c", 219);
	gotoxy(13,5); printf("%c", 219); gotoxy(12,5); printf("%c", 219);
	color(12, 0); gotoxy(10,8); printf("%c", 219); gotoxy(9,8); printf("%c", 219); gotoxy(11,8); printf("%c", 219); color(15, 0);
}

void init_snake(snake S[2]){ // intitialise les coordonee du serpent
	int i;
	for(i=2; i<4; i++){
		S[i-2].x=i; S[i-2].y=i;
	}
	taille =2;
	c = 77; direction = 77; speed = SPEED;
}

void move(int *x, int *y, snake S[taille]){ // direction du serpent tete serpent
	if(kbhit()) do{	c=getch(); }while(c != 72 && c != 75 && c != 77 && c != 80);
	if( c == 80 && direction!= 72){*y = *y+1; direction =80;}; if( c == 72 && direction!=80 ){ *y = *y-1; direction=72;}
	if( c == 75 && direction!=77){ direction = 75;*x = *x-1;} if( c == 77 && direction!=75) { direction = 77; *x = *x+1;}
	c = direction;
	*x = *x ;
	*y = *y;
}

void crawl(int *x, int *y, int taille, snake S[taille]){ // deplacement du serpent
	int i;
	for(i =0; i<taille-1; i++)
			S[i].body = 219;
	S[taille-1].body = 32;
	move(x, y, S);
		for(i=taille-1; i>0; i--){
			S[i].x = S[i-1].x;
			S[i].y = S[i-1].y;
		}
		S[0].x = *x;
		S[0].y = *y;
	for(i =0 ; i<taille; i++){
		gotoxy( S[i].x,  S[i].y);
		color(11,10);
		if(i == taille-1)color(15,15);
		if(i == 0)color(5,10);
		printf("%c", S[i].body);
		color(5,15);
	}
}

void arena(int debut_x, int fin_x, int debut_y, int fin_y){ // dessin le terrain de jeu
	int i;
	for(i=debut_x; i<=fin_x; i++){
		gotoxy(i, debut_y); printf("%c", 205);
		gotoxy(i, fin_y); printf("%c", 205);
	}
	for(i=debut_y; i<fin_y; i++){
		gotoxy(fin_y, i); printf("%c", 186);
		gotoxy(debut_y, i); printf("%c", 186);
	}
	gotoxy(fin_x,debut_y);	printf("%c", 187); // haut gauche
	gotoxy(fin_x,fin_y);	printf("%c", 188); // bas gauche
	gotoxy(debut_x,debut_y);	printf("%c", 201); // haut droit
	gotoxy(debut_x,fin_y); printf("%c", 200); //bas droit

}

void crawl_campagne(int *x, int *y, int taille, snake S[taille]){ // deplacement du srpent
	int i;
	for(i =0; i<taille-1; i++)
			S[i].body = 219;
	S[taille-1].body = 32;
	move(x, y, S);
	if(*x>18) {*x = 1;}
	if(*x<1)  {*x = 18;}
	if(*y<1)  {*y= 18;}
	if(*y>18) {*y = 1;}
		for(i=taille-1; i>0; i--){
			S[i].x = S[i-1].x;
			S[i].y = S[i-1].y;
		}
		S[0].x = *x;
		S[0].y = *y;
	for(i =0 ; i<taille; i++){
		gotoxy( S[i].x,  S[i].y);
		color(11,10);
		if(i == taille-1)color(15,15);
		if(i == 0)color(5,10);
		printf("%c", S[i].body);
		color(5,15);
	}
}

int verification_miam(snake S[taille]){ // verifie que le miam n'apparait pas sur le corps du serpent
	int i;
	for(i=0; i<taille; i++)
		if(dx==S[i].x && dy==S[i].y) return 1;
	return 0;
}

void food(snake S[taille]){ // produit le miam qui est la nourriture du serpent
	gotoxy(dx, dy);
	color(4, 15); printf("%c", 254); color(5, 15);
	if(S[0].x == dx && S[0].y == dy){
		gotoxy( S[taille-1].x,  S[taille-1].y); printf("%c", 219);
		do{
			dx = rand()%(18 - 2)+2;
			dy = rand()%(18 - 2)+2;
		}while(verification_miam(S));
		taille += 1;
		if((taille-2)%2 == 0 && speed>SPEEDMIN)
			speed = speed - 20;
	}
}

void food_c(snake S[taille]){ // idem que food()
	gotoxy(dx, dy);
	color(4, 15); printf("%c", 254); color(5, 15);
	if(S[0].x == dx && S[0].y == dy){
		gotoxy( S[taille-1].x,  S[taille-1].y); printf("%c", 219);
		do{
			dx = rand()%(18 - 2)+2;
			dy = rand()%(18 - 2)+2;
		}while(verification_miam(S));
		taille += 1;
		if((taille-2)%2 == 0 && speed!=SPEEDMIN)
			speed = speed - 10;
	}
}

int mort(snake S[taille]){
	int i;
	if(S[0].x>18 || S[0].x<1 || S[0].y<1 || S[0].y>18) return 0;
    for(i=2; i<taille-1; i++)
        if(S[0].x==S[i].x && S[0].y==S[i].y) return 0;
	return 1;
}

int mort_campagne(snake S[taille]){
	int i;
    for(i=2; i<taille-1; i++)
        if(S[0].x==S[i].x && S[0].y==S[i].y) return 0;
	return 1;
}

void score_campagne(int Pts, int x, int y, char *nom){
    FILE *g=fopen("scores_campagne.ini", "a+");
    record R;
    if(g==NULL){
      gotoxy(x,y);  printf("Impossible de charger le meilleur score"); getch();
    }
    else {
        strcpy(R.nom,nom);
        R.point = Pts;
        R.length=strlen(nom);
        fwrite(&R,sizeof(R),1,g);
    }
    fclose(g);
}

void score(int Pts, int x, int y, char *nom){
    FILE *f=fopen("scores.ini","a+");
    record R;
    if(f==NULL){
      gotoxy(x,y);  printf("Impossible de charger le meilleur score"); getch();
    }
    else {
        strcpy(R.nom,nom);
        R.point = Pts;
        R.length=strlen(nom);
	    fwrite(&R,sizeof(R),1,f);
    }
    fclose(f);
}

void aff_score_campagne(int x, int y){
    FILE *g=fopen("scores_campagne.ini", "r+");
    record R;
    int max = -1;
    if(g==NULL){
      gotoxy(x,y);  printf("Impossible de charger le meilleur score"); getch();
    }
    else {
        rewind(g);
        while(!EOF || fread(&R,sizeof(R),1,g)){
            max = (R.point <= max)? max : R.point;
        }
        rewind(g);
        while(!EOF || fread(&R,sizeof(R),1,g)){
            if( max == R.point) break;
        }
      	gotoxy(1,10); printf("%s : %d",R.nom,R.point);
    }
    fclose(g);
}

void aff_score(int x, int y){
    FILE *f=fopen("scores.ini","r+");
    record R;
    int max = -1;
    if(f==NULL){
      gotoxy(x,y);  printf("Impossible de charger le meilleur score"); getch();
    }
    else {
        rewind(f);
        while(!EOF || fread(&R,sizeof(R),1,f)){
            max = (R.point <= max)? max : R.point;
        }
        rewind(f);
        while(!EOF || fread(&R,sizeof(R),1,f)){
            if( max == R.point) break;
        }
      	gotoxy(1,10); printf("%s : %d",R.nom,R.point);
    }
    fclose(f);
}

void game(char *nom){
	srand((int)time(NULL));
	snake S[400];
	int x=2, y=2;
	miam M, super_M[5];
	char c;
	init_snake(S);
	while(mort(S)){
		arena(0, 19, 0, 19);
		food(S);
		crawl(&x, &y, taille, S);
		gotoxy(1, 20); printf("%s | SCORE : %d", nom, taille-2);
		Sleep(120);
	}
    score(taille-2, 10, 10, nom);
}

void game_campagne(char *nom){
	srand((int)time(NULL));
	snake S[400];
	int x=2, y=2;
	miam M, super_M[5];
	char c;
	init_snake(S);
	do{
		arena(0, 19, 0, 19);
		food_c(S);
		crawl_campagne(&x, &y, taille, S);
		gotoxy(1, 20); printf("%s | SCORE : %d", nom, taille-2);
		Sleep(speed);
	}while(mort_campagne(S));
	score_campagne(taille-2, 1, 10, nom);

}

void loading(int debut_x, int fin_x, int debut_y, int fin_y, int font){
	int i, j;
	for(i=debut_x; i<fin_x; i++){
		for(j=debut_y; j<fin_y; j++){
			gotoxy(i,j);
			color(0,font);printf(" "); //vert
		}
	Sleep(90);
	}
	color(5, 15);
	Sleep(1500);
	clean(debut_x, fin_x, debut_y, fin_y);
}

void wait(){
	char c;
	gotoxy(30, 50); printf("PRESS ANY KEY");
	c=getch();
}

void loading_game(){
	color(0,0);
	system("mode con: LINES=52 COLS=79");
	clean(0, 79, 0, 52);
	peindre(0, 100, 0, 53, 15);
	peindre(10, 70, 3, 33, 0);
	peindre(10, 18, 0, 21, 15); peindre(10, 14, 21, 23, 15); peindre(18, 22, 15, 19, 15); peindre(10, 22, 3, 9, 15);peindre(10, 26, 3, 5, 15); peindre(22, 26, 17, 19, 15);
	peindre(10, 14, 29, 33, 15); peindre(10, 18, 31, 33, 15); peindre(62, 70, 21, 23, 15); peindre(66, 70, 23, 25, 15); peindre(58, 70, 0, 21, 15); peindre(66, 70, 31, 33, 15);
	peindre(54, 62, 31, 33, 15); peindre(54, 58, 15, 19, 15); peindre(54, 58, 0, 11, 15); peindre(50, 54, 0, 7, 15); peindre(46, 50, 0, 5, 15);// peindre(66, 70, 31, 33, 15);

	peindre(26, 46, 5, 15, 10); peindre(22, 50, 9, 13, 10); peindre(50, 54, 12, 15, 2); peindre(46, 50, 7, 9, 10); peindre(26, 50, 15, 17, 2); //t�te
	peindre(26, 30, 9, 13, 15); peindre(42, 46, 9, 13, 15); // yeux
	peindre(22, 26, 13, 15, 13); peindre(46, 50, 13, 15, 13); // rougeur
	peindre(26, 34, 19, 21, 10); peindre(38, 46, 19, 23, 10); peindre(34, 38, 21, 23, 10); peindre(18, 20, 21, 13, 10); peindre(54, 58, 21, 29, 2); peindre(46, 50, 19, 23, 2);
	peindre(14, 22, 23, 25, 10); peindre(50, 54, 23, 25, 10); peindre(26, 46, 23, 25, 2); peindre(54, 62, 23, 25, 2); peindre(62, 66, 25, 31, 2); peindre(18, 54, 29, 31, 2);
	peindre(46, 54, 25, 29, 10); peindre(18, 26, 25, 29, 10); peindre(14, 18, 25, 29, 2); peindre(26, 46, 27, 29, 10); peindre(18, 30, 21, 23, 10);

	loading(10, 70, 40, 42, 5);
}

void cadre_help(void){ // cette fonction permet de construire un cadre pour l'affichage de l'aide
    int i;
	for ( i = 0; i <= 18; ++i){
        gotoxy(5,28+i);printf("%c", 186);
        gotoxy(7,28+i);printf("%c", 186);
        gotoxy(72,28+i);printf("%c", 186);
        gotoxy(74,28+i);printf("%c", 186);
        gotoxy(6,28+i); printf("*\n");
        gotoxy(73,28+i); printf("*\n");
    }
    for ( i = 6; i <=73; ++i){
    	gotoxy(i,27); printf("%c", 205);
        gotoxy(i,28); printf("*\n");
    	if(i!=6 || i!=73){gotoxy(i,29); printf("%c", 205);}
        if(i!=6 || i!=73){gotoxy(i,44);printf("%c", 205);}
        gotoxy(i,45); printf("*\n");
        gotoxy(i,46); printf("%c", 205);
    }
    gotoxy(5,27);	printf("%c", 201); gotoxy(7,29);	printf("%c", 201); // haut gauche
	gotoxy(5,46);	printf("%c", 200); gotoxy(7,44);	printf("%c", 200); // bas gauche
	gotoxy(74,27);	printf("%c", 187); gotoxy(72,29);	printf("%c", 187); // haut droit
	gotoxy(74,46); printf("%c", 188); gotoxy(72,44); printf("%c", 188); //bas droit

	gotoxy(30, 26); color(15, 5); printf("MENU D'AIDE DE SNAKE");color(5, 15);

	gotoxy(36, 30); printf("CAMPAGNE"); gotoxy(11+10, 32); printf("Le serpent mange les 'MIAMS' et grandit");gotoxy(8+10, 33); printf("Il traverse les murs et meurt quand il se mort");
	gotoxy(36, 35); printf("SURVIVAL"); gotoxy(11+10, 37); printf("Le serpent mange les 'MIAMS' et grandit");gotoxy(13, 38); printf("Il meurt quand il touche les murs et  quand il se mort");
	gotoxy(37, 40); printf("ASTUCES"); gotoxy(11+2, 42); printf("Appuie sur 'ANY KEYS' pour Activer/Desactiver la pause");
	gotoxy(31, 50); printf("PRESS Q KEY TO QUIT");

}

void help_direction(int i){
	clean(10, 39, 5, 17);
	gotoxy(40, 10); printf("LES TOUCHES DIRECTIONNELLES SERVENT"); gotoxy(44, 12); printf("A SE DEPLACER DANS L'ARENE");
	if(i==0){ //HAUT
		peindre(24, 28, 5, 6, 0); peindre(28, 30, 6, 7, 0); peindre(30, 32, 7, 8, 0); peindre(32, 34, 8, 9, 0); peindre(34, 36, 9, 11, 0); peindre(30, 34, 11, 12, 0);peindre(28, 30, 12, 15, 0);
		peindre(24, 28, 15, 16, 0); peindre(22, 24, 6, 7, 0); peindre(20, 22, 7, 8, 0); peindre(18, 20, 8, 9, 0); peindre(16, 18, 9, 11, 0); peindre(18, 22, 11, 12, 0);peindre(22, 24, 12, 15, 0);
		peindre(24, 28, 6, 11, 9); peindre(28, 30, 7, 10, 9); peindre(30, 32, 9, 10, 9); peindre(30, 32, 8, 9, 1); peindre(32, 34, 9, 11, 1); peindre(28, 34, 10, 11, 1); peindre(26, 30, 11, 12, 1); peindre(24, 28, 12, 15, 1);
		peindre(22, 26, 11, 12, 9); peindre(22, 24, 7, 11, 9); peindre(20, 22, 8, 11, 9); peindre(18, 20, 9, 11, 9); peindre(20, 22, 9, 10, 3); peindre(22, 24, 8, 9, 3); peindre(24, 26, 7, 8, 3);
	}
	if(i==1){ //BAS
		peindre(24, 28, 5, 6, 0); peindre(28, 30, 14, 15, 0); peindre(30, 32, 13, 14, 0); peindre(32, 34, 12, 13, 0); peindre(34, 36, 10, 12, 0); peindre(30, 34, 9, 10, 0);peindre(28, 30, 6, 9, 0);
		peindre(24, 28, 15, 16, 0); peindre(22, 24, 14, 15, 0); peindre(20, 22, 13, 14, 0); peindre(18, 20, 12, 13, 0); peindre(16, 18, 10, 12, 0); peindre(18, 22, 9, 10, 0);peindre(22, 24, 6, 9, 0);
		peindre(24, 28, 7, 15, 9); peindre(28, 30, 11, 14, 9); peindre(30, 32, 11, 12, 9); peindre(30, 32, 12, 13, 1); peindre(32, 34, 11, 12, 1); peindre(28, 34, 10, 11, 1); peindre(26, 30, 9, 10, 1); peindre(24, 28, 6, 9, 1);
		peindre(22, 26, 9, 10, 9); peindre(22, 24, 10, 14, 9); peindre(20, 22, 10, 13, 9); peindre(18, 20, 10, 12, 9); peindre(20, 22, 11, 12, 3); peindre(22, 24, 12, 13, 3); peindre(24, 26, 13, 14, 3);
	}
	if(i==2){ //DROITE
		peindre(10, 12, 9, 11, 0); peindre(12, 18, 8, 9, 0); peindre(18, 20, 6, 8, 0); peindre(20, 24, 5, 6, 0); peindre(24, 26, 6, 7, 0); peindre(26, 28, 7, 8, 0); peindre(28, 30, 8, 9, 0);
		peindre(30, 32, 9, 11, 0); peindre(12, 18, 11, 12, 0); peindre(18, 20, 12, 14, 0); peindre(20, 24, 14, 15, 0); peindre(24, 26, 13, 14, 0); peindre(26, 28, 12, 13, 0); peindre(28, 30, 11, 12, 0);
		peindre(20, 24, 6, 14, 9); peindre(24, 26, 7, 13, 9); peindre(18, 28, 8, 12, 9); peindre(12, 30, 9, 11, 9); peindre(12, 18, 9, 11, 1); peindre(18, 20, 10, 12, 1); peindre(20, 22, 11, 14, 1); peindre(22, 24, 13, 14, 1); peindre(24, 26, 12, 13, 1);
		peindre(22, 24, 7, 8, 3); peindre(24, 26, 8, 9, 3); peindre(26, 28, 9, 10, 3); peindre(16, 18, 9, 10, 9);
	}
	if(i==3){ //GAUCHE
		peindre(10, 12, 9, 11, 0); peindre(12, 14, 8, 9, 0); peindre(14, 16, 7, 8, 0); peindre(16, 18, 6, 7, 0); peindre(18, 22, 5, 6, 0); peindre(22, 24, 6, 8, 0); peindre(24, 30, 8, 9, 0);
		peindre(30, 32, 9, 11, 0); peindre(12, 14, 11, 12, 0); peindre(14, 16, 12, 13, 0); peindre(16, 18, 13, 14, 0); peindre(18, 22, 14, 15, 0); peindre(22, 24, 12, 14, 0); peindre(24, 30, 11, 12, 0);
		peindre(16, 18, 7, 13, 9); peindre(18, 22, 6, 14, 9); peindre(14, 24, 8, 12, 9); peindre(12, 30, 9, 11, 9); peindre(24, 30, 9, 11, 1); peindre(22, 24, 8, 10, 1); peindre(20, 22, 6, 9, 1); peindre(18, 22, 6, 7, 1); peindre(16, 18, 7, 8, 1);
		peindre(14, 16, 10, 11, 3); peindre(16, 18, 11, 12, 3); peindre(18, 20, 12, 13, 3); peindre(24, 26, 10, 11, 9);
	}
}

void help(){
	system("mode con: LINES=52 COLS=79");
	int i;
	char ch;
	color(5, 15);
	clean(0, 80, 0, 82);
	i=0;
	do{
	cadre_help();
	help_direction(i);
	i++;
	Sleep(500);
	if(i==4) i=0;
	if(kbhit())ch=getch();
	}while(ch!=113);
}

int snake_game(){
	FILE *f=fopen("scores.ini","r+");
	FILE *g=fopen("scores_campagne.ini", "r+");
    int i, j, choix, choix_0;
	char c, plr[20], plr_c[20];
	clean(0, 80, 0, 52);
	loading_game();
    do{
	draw_snake();
    choix = MENU(menu, 4);
	    switch (choix){
	        case 0:
	            do{
					draw_snake();
	            	choix_0=CAMPAGNE(3);
		            switch (choix_0){
		                case 0:
		                    taille = 2;
		                    clean(3, 20, 9, 19);
		                    gotoxy(7, 10);
		                    printf("LOADING");
		                    Sleep(1000);
		                    clean(0, 22, 0, 22);
		                    gotoxy(1, 10); printf("ENTRER VOTRE NOM"); gotoxy(1, 11); gets(plr_c); in_c =1;
		                    clean(0, 20, 9, 12);
		                    game_campagne(plr_c);
		                    clean(0, 22, 0, 22);
		                    gotoxy(5, 10);
		                    printf("GAME OVER");
		                    Sleep(1000);
		                    break;
		                case 1:
		                    clean(3, 20, 9, 19);
		                    gotoxy(7, 10);
		                    printf("LOADING");
		                    Sleep(1000);
		                    clean(0, 20, 9, 11);
		                    aff_score_campagne(1, 10 );
		                    gotoxy(3, 18); printf("PRESS ANY KEY"); getch();
		                    break;
		                case 2:
		                    clean(3, 20, 9, 19);
		                    gotoxy(7, 10);
		                    break;
	            	}
	            }while(choix_0!=2);
	            break;
	        case 1:
	            do{
					draw_snake();
	            	choix_0=CAMPAGNE(3);
		            switch (choix_0){
		                case 0:
		                    taille = 2;
		                    clean(3, 20, 9, 19);
		                    gotoxy(7, 10);
		                    printf("LOADING");
		                    Sleep(1000);
		                    clean(0, 22, 0, 22);
		                    gotoxy(1, 10); printf("ENTRER VOTRE NOM"); gotoxy(1, 11); gets(plr); in = 1;
		                    clean(0, 20, 9, 12);
		                    game(plr);
		                    clean(0, 22, 0, 22);
		                    gotoxy(5, 10);
		                    printf("GAME OVER");
		                    Sleep(3000);
		                    break;
		                case 1:
		                    clean(3, 20, 9, 19);
		                    gotoxy(7, 10);
		                    printf("LOADING");
		                    Sleep(1000);
		                    clean(0, 20, 9, 11);
		                    aff_score(1, 10);
		                    gotoxy(3, 18); printf("PRESS ANY KEY"); getch();
		                    break;
		                case 2:
		                    clean(3, 20, 9, 19);
		                    break;
		            }
		        }while(choix_0!=2);
	            break;
	        case 2:
				help();
	            break;
	        case 3:
		//		choix_0=QUIT(quit[10], 2);
	            break;
	    }
	}while(choix!=3);
	return choix;
}

int main(){
    system("mode con: LINES=52 COLS=79");
    while(snake_game()!=3){
    }
}

