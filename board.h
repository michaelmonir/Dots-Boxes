
#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED



//Data structures
int n;
char undo[60][3] = {{0}}, undo_index = 0,redo_counter=0;

char horizontal[10][10] = {{0}}, vertical[10][10]={{0}},boxes[10][10]={{0}}; // 10 doestn't matter here but we will deal with only a part of it
char Aturn = 1;
int numEdges = 0;
int Ascore = 0, Bscore = 0;
#define RED     "\x1b[31m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"
// All the functions

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

void undofunc()
{
    //debugundo();
    if (!undo_index)
        return;
    if (undo[undo_index - 1][2])
        vertical[undo[undo_index - 1][0]][undo[undo_index - 1][1]] = 0;
    else
        horizontal[undo[undo_index - 1][0]][undo[undo_index - 1][1]] = 0;
    undo_index--;
    checkBoxes(undo[undo_index][0], undo[undo_index][1], undo[undo_index][2], 0);
    redo_counter++;

}

void redofunc(){
    if(redo_counter>0){

        int i;
        if(Aturn)
            i=1;
        else
            i=2;
        checkBoxes(undo[undo_index][0], undo[undo_index][1], undo[undo_index][2], 1);
        if (!undo_index)
            return;
        if (undo[undo_index - 1][2])
            vertical[undo[undo_index - 1][0]][undo[undo_index - 1][1]] = i;
        else
            horizontal[undo[undo_index - 1][0]][undo[undo_index - 1][1]] = i;

        redo_counter--;
    }
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
            printf("%d", undo[undo_index][1]);
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
    return (numEdges == n * (n + 1) * 2); // This is total number of vertical and horizontal edges
}


void printBoard()
{
    system("cls");
    printf("  ");
    for (int i = 1; i <= n + 1; i++)
        printf("%d   ", i);
    printf("\n\n");
    for (int i = 0; i < n + 1; i++)
    {

        printf("%d ", i + 1);
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
            printf("  ");
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
    printf(RED"Score A: %d\t" BLUE"Score B: %d\n"RESET, Ascore, Bscore);
    printf("%d %d %d\n",undo[0][0],undo[0][1],undo[0][2]);
    printf("%d %d %d\n",undo[1][0],undo[1][1],undo[1][2]);
    printf("%d %d %d\n",undo[2][0],undo[2][1],undo[2][2]);
    printf("%d %d %d\n",undo[3][0],undo[3][1],undo[3][2]);
    printf("%d %d \n",undo_index,redo_counter);
}

// When a user enters a Horizontal edge, This function modifies the horizontal Array
int modifyHorizontal(int i, int j)
{
    // We are going to modify horizontal[i][j]
    if(horizontal[i][j] == 1)
        return 0;
    if(Aturn)
        horizontal[i][j] = 1;
    else
        horizontal[i][j] = 2;
    //increament(checkBoxes(i, j, 0, 1));
    redo_counter=0;
    checkBoxes(i, j, 0, 1);
    numEdges++;
    return 1;
}

// This function does the same with the vertical array
int modifyVertical(int i, int j)
{
    if(vertical[i][j] == 1)
        return 0;
    if(Aturn)
        vertical[i][j] = 1;
    else
        vertical[i][j] = 2;
    //increament(checkBoxes(i, j, 1,1));
    redo_counter = 0;
    checkBoxes(i, j, 1, 1);
    numEdges++;
    return 1;
}


// 0 -> input not valid
// 1 -> 2pts vertical
// 2 -> 2pts horizontal
char checkvalid(int p1x, int p1y, int p2x, int p2y)
{
    if (p1x==0&&p1y==0&&p2x==0&&p2y==0)
    {
        undofunc();
        return 1;
    }
    else if (p1x==1&&p1y==1&&p2x==1&&p2y==1){
        redofunc();
        return 1;
    }

    if (  !(checkinterval(p1x, 0, n) && checkinterval(p2x, 0,n) && checkinterval(p1y, 0, n) && checkinterval(p2y, 0, n))  )
        return 0;

    if (p1x == p2x)
    {
        if (modifyHorizontal(p1x, min(p1y, p2y)) == 0)
            return 0;
        return 2;
    }
    else if (p1y == p2y)
    {
        if (modifyVertical(min(p1x, p2x), p1y) == 0)
            return 0;
        return 1;
    }
    return 0;
}


void getEdge()
{

    if (Aturn)
        printf(RED "Player A turn please enter the first and second"RESET"\n");
    else
        printf(BLUE "Player B turn please enter the first and second"RESET"\n");
    int p1, p2, p1x, p1y, p2x, p2y;
    scanf("%d %d %d %d", &p1x, &p1y, &p2x, &p2y);
    p1 = (p1x - 1) * (n + 1) + p1y;
    p2 = (p2x - 1) * (n + 1) + p2y;
    if (checkvalid(p1x - 1, p1y - 1, p2x - 1, p2y - 1) == 0)
    {
        errormsg();
        getEdge();
    }
    printf("\n");
}

void errormsg()
{
    printf("Your input isn't valid \nPlease enter the two points");
}

void startgame()
{
    int input = 0;
    while (input != 1 && input != 2)
    {
        printf("Please type 1 for Beginner, 2 for Expert");
        scanf("%d", &input);
        if (input == 1)
            n = 2;
        else if (input == 2)
            n = 5;
        else
        {
            system("cls");
            printf("Sorry we didn't understand your option\n");
        }

    }

    while (!checkEnded())
    {
        printBoard();
        getEdge();
    }
    if (Ascore > Bscore)
        printf(RED"\nA won!!"RESET);
    else if (Ascore < Bscore)
        printf(BLUE"\nB won!!"RESET);
    else
        printf("\nDraw");
}

#endif // BOARD_H_INCLUDED
