#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
//Data structures

typedef struct
{
    char name[20];
    int score;
    int edges;
}Player;


typedef struct {
    char name[100];
    int score;
}Top;

#define RED     "\x1b[31m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"
#define BROWN   "\x1b[33m"
#define GREEN   "\x1b[32m"
// All the functions


void clearBuffer();
char checkvalid(int p1x, int p1y, int p2x, int p2y);
void getEdge();
int getHorizontalIndex(pnt1, pnt2);
int getVerticalIndex(pnt1, pnt2);
void printBoard(); // And Scores
char checkEnded();
void changeturn();
void startgame();
void debugundo();
int checkBoxes(int i, int j, char isvertical, char isforward);
void redofunc();
char iscomputer();
char save(int num);
char load (int num);
void top10();
char validateExcess();
char getComputerInput(int *p1x, int *p1y, int *p2x, int *p2y);
char getUserInput(int *p1x, int *p1y, int *p2x, int *p2y, char one);
Player setplayer(char *n, int s, int e);
void reset();
void initialize(char arr[60][60]);
int checkinterval(int x, int start, int end);
int min(int first, int second);
int increament(int i, int isforward);
int increamentBoxes(int i, int j, int isforward);
#endif
