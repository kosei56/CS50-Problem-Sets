#include <cs50.h>
#include <stdio.h>


void makePyramid(int height);

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    makePyramid(height);
}

void makePyramid(int height)
{
    for (int i = 0; i < height; i++)
    {
        int number_of_space = height - i - 1;
        int number_of_hash = height - number_of_space;
        for (int j = 0; j < height * 2 + 2; j++)
        {
            if (j < number_of_space || (j > height - 1 && j < height + 2))
            {
                printf(" ");
            }
            else if (j < height + 2 + number_of_hash)
            {
                printf("#");
            }
        }
        printf("\n");
    }
}