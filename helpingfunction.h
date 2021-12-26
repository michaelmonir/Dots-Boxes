#ifndef HELPINGFUNCTION_H_INCLUDED
#define HELPINGFUNCTION_H_INCLUDED


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

int increament(int i)
{
    if (Aturn)
        Ascore += i;
    else
        Bscore += i;
}
#endif // HELPINGFUNCTION_H_INCLUDED
