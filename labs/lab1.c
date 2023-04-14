#include <stdio.h>
#include <cs50.h>

int get_start(void);
int get_end(int start);
int get_year(int size, int year);

int main(void)
{
    int start_size = get_start();
    int end_size = get_end(start_size);
    int year = get_year(start_size, end_size);
    printf("Year: %d\n", year);

}

int get_start(void)
{
    int size;
    do
    {
        size = get_int("Start Size: ");
    }
    while (size < 9);
    return size;
}

int get_end(int start)
{
    int size;
    do
    {
        size = get_int("End Size: ");
    }
    while (size < start);
    return size;
}

int get_year(int start, int end)
{
    int current = start;
    int addPop;
    int year = 0;
    while (current < end)
    {
        addPop = current / 3 - current / 4;
        current += addPop;
        year++;
    }
    return year;
}