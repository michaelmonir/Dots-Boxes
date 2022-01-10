
#include "board.h"

int n;
Player A, B;
char undo[60][60] = {{0}}, undo_index = 0, redo_counter = 0; // Undo could be 60 * 3
char horizontal[60][60] = {{0}}, vertical[60][60] = {{0}},boxes[60][60]={{0}}; // 10 doestn't matter here but we will deal with only a part of it
char Aturn = 1;
int numEdges = 0;
int playerindex = 0; // 0 for human vs human       1 human first           2 computer first
int sec,minutes;
Top top10arr[25];
int timenull;


void clearBuffer()
{
	char c;
	do
    {
		c = getchar();
	}
    while (c != '\n' && c != EOF);
}

void settime()
{
    int total = time(NULL) - timenull;
    minutes = total / 60;
    sec = total % 60;
}

int checkinterval(int x, int start, int end)
{
    if (x < start || x > end)
        return 0;
    return 1;
}

int min(int first, int second)
{
    if (first < second)
        return first;
    return second;
}

int increament(int i, int isforward)
{
    if (!isforward)
        i *= -1;
    if (Aturn)
        A.score += i;
    else
        B.score += i;
}

int increamentedges(int isforward)
{
    int i = 1;
    if (!isforward)
        i *= -1;
    if (Aturn)
        A.edges += i;
    else
        B.edges += i;
}

int increamentBoxes(int i, int j, int isforward)
{
    if (isforward)
    {
        if (Aturn)
            boxes[i][j] = 1;
        else
            boxes[i][j] = 2;
    }
    else
        boxes[i][j] = 0;

}


void initialize(char arr[60][60])
{
    for (int i = 0; i < 60; i++)
    {
        for (int j = 0; j < 60; j++)
        {
            arr[i][j] = 0;
        }
    }
}

Player setplayer(char *n, int s, int e)
{
    Player p;
    p.edges = e;
    p.score = s;
    strcpy(p.name, n);
    return p;
}

char iscomputer()
{
    return (Aturn + 1 == playerindex);
}

void swap_str(char *str1, char *str2)
{
  char *temp = str1;
  str1 = str2;
  str2 = temp;
}

void top10(){

    FILE *topfile;
    topfile=fopen("Top10.txt","r");
    int c=0;
    while (c<25)
    {
      fscanf(topfile,"%s %d\n",&top10arr[c].name,&top10arr[c].score);
      c++;
    }
    fclose(topfile);
    for (int i=0;i<c;i++)
    {
        for(int j=i+1;j<c;j++)
        {
                if (top10arr[i].score<top10arr[j].score)
                {
                        top10arr[24]=top10arr[i];
                        top10arr[i]=top10arr[j];
                        top10arr[j]=top10arr[24];
                }
        }
    }
    for(int i=0;i<10;i++)
        printf(BROWN"%d %d %s\n"RESET,i+1,top10arr[i].score,top10arr[i].name);
}

void undofunc()
{
    if (!undo_index)
        return;
    if (undo[undo_index - 1][2])
        vertical[undo[undo_index - 1][0]][undo[undo_index - 1][1]] = 0;
    else
        horizontal[undo[undo_index - 1][0]][undo[undo_index - 1][1]] = 0;

    undo_index--;
    numEdges --;
    checkBoxes(undo[undo_index][0], undo[undo_index][1], undo[undo_index][2], 0);

    increamentedges(0);
    redo_counter++;
    if (iscomputer())
        undofunc();
}

void redofunc(){
    if(redo_counter>0){

        int i;
        if(Aturn)
            i=1;
        else
            i=2;

        increamentedges(1);
        checkBoxes(undo[undo_index][0], undo[undo_index][1], undo[undo_index][2], 1);
        if (!undo_index)
            return;
        if (undo[undo_index - 1][2])
            vertical[undo[undo_index - 1][0]][undo[undo_index - 1][1]] = i;
        else
            horizontal[undo[undo_index - 1][0]][undo[undo_index - 1][1]] = i;
        numEdges++;
        redo_counter--;
    }
}

char save(int k)
{
    FILE *file;
    char filename[100];
    if (k == 1)
        strcpy(filename, "game1.txt");
    else if (k == 2)
        strcpy(filename, "game2.txt");
    else if (k == 3)
        strcpy(filename, "game3.txt");
    else
        return 0;

    file = fopen(filename, "w");
    fprintf(file, "%d\n", n);

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            fprintf(file, "%d ",horizontal[i][j]);
    fprintf(file, "\n");
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            fprintf(file, "%d ",vertical[i][j]);
    fprintf(file, "\n");
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            fprintf(file, "%d ",boxes[i][j]);
    fprintf(file, "\n%d %d %d %d %d %d %s %s %d", Aturn, numEdges, A.score, B.score, A.edges, B.edges, A.name, B.name, playerindex);
    fclose(file);
    mainMenu();
    return 1;
}

void reset()
{
    timenull = time(NULL);

    undo_index = 0;
    redo_counter = 0;
    initialize(vertical);
    initialize(undo);
    initialize(horizontal);
    Aturn = 1; numEdges = 0; A.score = 0; B.score = 0; A.edges = 0; B.edges = 0;
}

char load(int k) // This function must return char yesno whether the chosen game is empty or not
{
    FILE *file;
    char filename[100];
    if (k == 1)
        strcpy(filename, "game1.txt");
    else if (k == 2)
        strcpy(filename, "game2.txt");
    else if (k == 3)
        strcpy(filename, "game3.txt");
    else
        return 0;
    file = fopen(filename, "r");
    fscanf(file, "%d", &n);

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            fscanf(file, "%d ", &horizontal[i][j]);
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            fscanf(file, "%d ", &vertical[i][j]);
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            fscanf(file, "%d ", &boxes[i][j]);
    int ascore, bscore, aedges, bedges;
    char aname[20], bname[20];
    fscanf(file, "%d %d %d %d %d %d %s %s %d", &Aturn, &numEdges, &ascore, &bscore, &aedges, &bedges, aname, bname, &playerindex);
    A = setplayer(aname, ascore, aedges);
    B = setplayer(bname, bscore, bedges);
    fclose(file);
    return 1;
}
// Undo -> isforward = 2
// Move -> isforward = 1
int checkBoxes(int i, int j, char isvertical, char isforward)
{
    if (isforward)
    {
        undo[undo_index][0] = i;
        undo[undo_index][1] = j;
        undo[undo_index][2] = isvertical;
        undo_index++;
    }

    int ret = 0;
    if (isvertical)
    {
        if (j - 1 >= 0 && vertical[i][j - 1] && horizontal[i][j - 1] && horizontal[i + 1][j - 1]){
            ret += 1;
            increamentBoxes(i, j - 1, isforward);
        }
        if ( j + 1 < n+1 && vertical[i][j + 1] && horizontal[i][j] && horizontal[i + 1][j]){
            ret += 1;
            increamentBoxes(i, j, isforward);
        }
    }
    else
    {
        if (i - 1 >= 0 && vertical[i - 1][j] && vertical[i - 1][j + 1] && horizontal[i - 1][j]){
            ret += 1;
            increamentBoxes(i - 1, j, isforward);
        }
        if (i + 1 < n + 1 && vertical[i][j] && vertical[i][j + 1] && horizontal[i + 1][j]){
            ret += 1;
            increamentBoxes(i, j, isforward);
        }
    }
    if (!ret)
        changeturn();
    increament(ret, isforward);
    return ret;
}

void changeturn()
{
    if(Aturn)
        Aturn = 0;
    else
        Aturn = 1;
}


char checkEnded()
{
    return (numEdges == 2 * n * (n + 1)); // This is total number of vertical and horizontal edges
}

void printBoard()
{
    system("cls");
    printf("\t\t\t\t\t\t  ");
    for (int i = 1; i <= n + 1; i++)
        printf("%d   ", i);
    printf("\n\n");
    for (int i = 0; i < n + 1; i++)
    {

        printf("\t\t\t\t\t\t%d ", i + 1);
        // For Rows
        for (int j = 0; j < n; j++)
        {
            printf("%c",254);
            if (horizontal[i][j] == 1)
                printf( RED "---" RESET);
            else if(horizontal[i][j] == 2)
                printf( BLUE "---" RESET);
            else
                printf("   ");
        }
        printf("%c\n",254);

        // For Columns


        if(i == n)
            continue;
        for (int rep = 0; rep < 2; rep ++)
        {
            printf("\t\t\t\t\t\t  ");
            //printf("%d ", i);
            for (int j = 0; j < n + 1; j++)
            {
                if (vertical[i][j] == 0)
                    printf(" ");
                else if(vertical[i][j] == 1)
                    printf(RED"|" RESET);
                else
                    printf(BLUE"|" RESET);
                if(boxes[i][j]==1){
                    printf(RED"%c%c%c"RESET,178,178,178);
                }
                else if(boxes[i][j]==2){
                    printf(BLUE"%c%c%c"RESET,178,178,178);
                }
                else{
                    printf("   ");
                }
            }

            printf("\n");

        }

    }
    printf(RED"score of %s : %d"BLUE"\t\t\t\t\t\t\t\t score of %s : %d\n"RESET,A.name,A.score,B.name,B.score);
    printf(RED"%s played %d times"BLUE" \t\t\t\t\t\t\t %s played %d times\n",A.name,A.edges,B.name,B.edges);
    if(n==2)
        printf(GREEN"the remaining edges is %d\n"RESET,(12-numEdges));
    else
        printf(GREEN"the remaining edges is %d\n"RESET,(60-numEdges));
    settime();
    printf(GREEN"time %d:%d\n"RESET,minutes,sec);
}

// When a user enters a Horizontal edge, This function modifies the horizontal Array
int modifyHorizontal(int i, int j)
{
    // We are going to modify horizontal[i][j]
    if(horizontal[i][j])
        return 0;

    numEdges++;

    if(Aturn){
        horizontal[i][j] = 1;
        A.edges++;
    }
    else{
        horizontal[i][j] = 2;
        B.edges++;
    }
    //increament(checkBoxes(i, j, 0, 1));
    redo_counter=0;
    checkBoxes(i, j, 0, 1);
    return 1;
}

// This function does the same with the vertical array
int modifyVertical(int i, int j)
{
    if(vertical[i][j])
        return 0;

    numEdges++;
    if(Aturn){
        vertical[i][j] = 1;
        A.edges++;
    }
    else{
        vertical[i][j] = 2;
        B.edges++;
    }
    //increament(checkBoxes(i, j, 1,1));
    redo_counter = 0;
    checkBoxes(i, j, 1, 1);
    return 1;
}


// 0 -> input not valid
// 1 -> 2pts vertical
// 2 -> 2pts horizontal
char checkvalid(int p1x, int p1y, int p2x, int p2y)
{
    if (p1x == 0 && p1y == 0 && p2x == 0 && p2y == 0)
    {
        undofunc();
        return 1;
    }
    else if (p1x==1&&p1y==1&&p2x==1&&p2y==1)
    {
        redofunc();
        return 1;
    }
    if (p1x == 2 && p2x == 2 && p1y == 2 && p2y == 2)
    {
        printf(BROWN"Save in game 1 or 2 or 3"RESET);
        int num;
        while (!getUserInput(&num, NULL, NULL, NULL, 1) || !save(num))
            errormsg();
        return 1;
    }
    if (p1x == 3 && p2x == 3 && p1y == 3 && p2y == 3 && !iscomputer())
    {
        printf(BROWN"Load game 1 or 2 or 3"RESET);
        int num;
        while (!getUserInput(&num, NULL, NULL, NULL, 1) || !load(num))
            errormsg();
        return 1;
    }


    if (  !   (checkinterval(p1x, 0, n) && checkinterval(p2x, 0, n) && checkinterval(p1y, 0, n) && checkinterval(p2y, 0, n)  )  )
        return 0;


    if (p1x == p2x)
    {
        if (abs(p1y - p2y) != 1)
            return 0;

        if (modifyHorizontal(p1x, min(p1y, p2y)) == 0)
            return 0;
        return 2;
    }
    else if (p1y == p2y)
    {
        // We forgot to do this !!!!!!!
        if (abs(p1x - p2x) != 1)
            return 0;

        if (modifyVertical(min(p1x, p2x), p1y) == 0)
            return 0;
        return 1;
    }
    return 0;
}

char validateExcess()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        if(c == ' ' || c == '\n') continue;
        clearBuffer();
        return 0;
    }
    return 1;
}

void getEdge(int player)
{
    int p1x, p1y, p2x, p2y;
    int excess;
    if (iscomputer())
        excess = getComputerInput(&p1x, &p2x, &p1y, &p2y);
    else
        excess = getUserInput(&p1x, &p2x, &p1y, &p2y, 0);
    if (!excess || checkvalid(p1x - 1, p1y - 1, p2x - 1, p2y - 1) == 0)
    {
        if (!iscomputer())
            errormsg();
        getEdge(player);
    }
    printf("\n");
}

char getUserInput(int *p1x, int *p2x, int *p1y, int *p2y, char one)
{
    if (one == 0)
    {
        printf(BROWN "For Undo press 1 1 1 1\t For Redo press 2 2 2 2\nFor save press 3 3 3 3\t For load press 4 4 4 4\nFor new edge betwen points (x1, y1) & (x2, y2), Enter the coordinates as x1 x2 y1 y2"RESET"\n");
        if (Aturn)
            printf(RED "Player A turn"RESET"\n");
        else
            printf(BLUE "Player B turn"RESET"\n");
    }
    if (one)
        scanf("%d", p1x);
    else
        scanf("%d %d %d %d", p1x, p2x, p1y, p2y);
    if (!validateExcess())
        return 0;
}

char getComputerInput(int *p1x, int *p2x, int *p1y, int *p2y)
{
    *p1x = rand() % (n + 1) + 1;
    *p2x = rand() % (n + 1) + 1;
    *p1y = rand() % (n + 1) + 1;
    *p2y = rand() % (n + 1) + 1;

    if ((*p1x == 2 && *p1y == 2 && *p2x == 2 && *p2y == 2)
        || (*p1x == 1 && *p1y == 1&& *p2x == 1 && *p2y==1)
        || (*p1x == 3 && *p1y == 3&& *p2x == 3 && *p2y==3)
        || (*p1x == 4 && *p1y == 4 && *p2x == 4 && *p2y==4))
    {
        getComputerInput(p1x, p1y, p2x, p2y);
    }
    return 1;
}

void errormsg()
{
    printf(BROWN"Sorry but we didn't understand your response\n"RESET);
}

void startgame()
{
    int input = 0;
    while (input != 1 && input != 2)
    {
        printf(BROWN"Please type 1 for Beginner, 2 for Expert"RESET);
        while (!getUserInput(&input, NULL, NULL, NULL, 1))
            errormsg();
        if (input == 1)
            n = 2;
        else if (input == 2)
            n = 5;
        else
        {
            system("cls");
            printf(BROWN"Sorry we didn't understand your option\n"RESET);
        }
    }
    timenull = time(NULL);
    gameflow(1);
    printBoard();
    FILE *file ;
    file = fopen("top10.txt","a");
    if (A.score > B.score)
    {
        printf(RED"\n%s won!!"RESET, A.name);
        if (A.name != "Computer");
            fprintf(file , "%s %d\n",A.name,A.score);
    }
    else if (A.score < B.score)
    {
        printf(BLUE"\n%s won!!"RESET, B.name);
        if (B.name != "Computer");
        fprintf(file , "%s %d\n",B.name,B.score);
    }
    else
        printf("\nDraw");
    fclose(file);

    printf(BROWN"\nFor Undo press 1\nFor Main Menu press 2\n"RESET);

    int choose;
    while (!getUserInput(&choose, NULL, NULL, NULL, 1) || (choose != 1 && choose != 2))
        errormsg();

    if (choose == 1)
    {
        undofunc();
        gameflow(1);
    }
    else
    {
        mainMenu();
    }
}

void gameflow(int neworload)
{
    if (neworload == 0)
        checkvalid(3,3,3,3);
    while (!checkEnded())
    {
        printBoard();
        getEdge(playerindex);
    }
}

void mainMenu()
{
    system("cls");
    timenull = time(NULL);
    reset();
    printf(RED"Dots"RESET"&"BLUE"Boxes\n"RESET);
    printf(BROWN"For play press 1\nFor load game press 2\nFor top player press 3\nFor exit press 4\n "RESET);
    char choose;
    while (!getUserInput(&choose, NULL, NULL, NULL, 1) || (choose != 1 && choose != 2 && choose!=3 && choose !=4))
        errormsg();
    system("cls");
    if (choose == 1){
        char ishuman;
        printf(BROWN"for play against human press 1\nfor play against computer press 2 \n "RESET);
        while (!getUserInput(&ishuman, NULL, NULL, NULL, 1) || (ishuman != 1 && ishuman != 2 && ishuman!=3 && ishuman !=4))
            errormsg();
        if(ishuman==1){
            playerindex = 0;
            printf(BROWN"what's the name of the first player?"RESET);
            char n1[20] = "";
//            while (!strcmp(n1, ""))
//                gets(n1);
            scanf("%s", n1);
            A = setplayer(n1, 0, 0);
            printf(BROWN"what's the name of the second player?"RESET);
            char n2[20] = "";
//            while (!strcmp(n2, ""))
//                gets(n2);
            scanf("%s", n2);
            B = setplayer(n2, 0, 0);
            startgame();
        }
        else if(ishuman==2)
        {
            printf(BROWN"For first player press 1\nFor second player press 2\n"RESET);
            int isfirst;
            while (!getUserInput(&isfirst, NULL, NULL, NULL, 1) || (isfirst != 1 && isfirst != 2))
                errormsg();
            printf(BROWN"what's your name?"RESET);
            char n[20] = "";
//            while (!strcmp(n, ""))
//                gets(n);
            scanf("%s", n);
            if (isfirst)
            {
                A = setplayer(n, 0, 0);
                B = setplayer("Computer", 0, 0);
            }
            else
            {
                B = setplayer(n, 0, 0);
                A = setplayer("Computer", 0, 0);
            }
            playerindex = isfirst;
            startgame();
        }
    }
    else if(choose==2)
        gameflow(0);
    else if(choose==3)
    {
        top10();
        exit(0);
    }
    else
        exit(0);
}
