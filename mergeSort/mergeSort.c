#include <cs50.h>
#include <stdio.h>

int currentLength = 10;

int main(void)
{
    int numbers[] = {5, 1, 2, 4, 3, 9, 0, 8, 6, 7};
}

void sort(int numbers[])
{
    int currentSize = sizeof numbers / sizeof numbers[0];
    if (currentSize = 1)
    {
        return;
    }
    int leftSide[currentSize / 2];
    int rightSide[currentSize - (currentSize / 2)];
    int 
    for (int i = 0; i < currentSize/2; i++)
    {
        leftSide[i] = numbers[i];
    }
    for (int i = currentSize - (currentSize / 2); i < currnetSize; i++)
    {
        rightSide[i] = numbers[i];
    }
    for(int i = 0; i < currnetSize/2; i++)
    {

    }
    sort(int leftSide[]);
    sort(int rightSide[]);

    merge(int leftSide[], int rightSide[]);
}




