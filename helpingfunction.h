#ifndef HELPINGFUNCTIONS_H_INCLUDED
#define HELPINGFUNCTIONS_H_INCLUDED


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
int max(int first, int second)
{
    if (first > second)
        return first;
    return second;
}

int increament(int i, int isforward)
{
    if (!isforward)
        i *= -1;
    if (Aturn)
        Ascore += i;
    else
        Bscore += i;
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

#endif // HELPINGFUNCTIONS_H_INCLUDED
