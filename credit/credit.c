#include <cs50.h>
#include <stdio.h>

int getNumLength(long cardNum);
int getDigit(long cardNum, int index);
int addDigits(int num);
bool isEven(long num);
bool isValid(long cardNum);
string getCardType(long cardNum);


int main(void)
{
    long cardNum = get_long("What is your card number? ");
    if (isValid(cardNum))
    {
        printf("%s\n", getCardType(cardNum));
    }
    else
    {
        printf("INVALID\n");
    }


}


bool isValid(long cardNum)
{
    // if the card length is even you double the even indexes
    // if the card length is odd  you double the odd  indexes
    int cardNumLength = getNumLength(cardNum);
    int totalDoubledNum = 0;
    int totalNonDoubleNum = 0;
    int startIndex;
    int nextIndex;
    if (isEven(cardNumLength))
    {
        startIndex = 0;
        nextIndex = 1;
    }
    else
    {
        startIndex = 1;
        nextIndex = 0;
    }
    for (int i = startIndex; i < cardNumLength; i += 2)
    {
        totalDoubledNum += addDigits((getDigit(cardNum, i) * 2));
    }
    for (int i = nextIndex; i < cardNumLength; i += 2)
    {
        totalNonDoubleNum += getDigit(cardNum, i);
    }
    if ((totalDoubledNum + totalNonDoubleNum) % 10 == 0)
    {
        return true;
    }
    return false;
}

string getCardType(long cardNum)
{
    if (getNumLength(cardNum) == 15 && getDigit(cardNum, 0) == 3 && (getDigit(cardNum, 1) == 4 || getDigit(cardNum, 1) == 7))
    {
        return "AMEX";
    }
    else if (getNumLength(cardNum) == 16 && getDigit(cardNum, 0) == 5)
    {
        for (int i = 1; i < 6; i++)
        {
            if (getDigit(cardNum, 1) == i)
            {
                return "MASTERCARD";
            }
        }
    }
    else if ((getNumLength(cardNum) == 13 || getNumLength(cardNum) == 16) && getDigit(cardNum, 0) == 4)
    {
        return "VISA";
    }
    return "INVALID";
}

bool isEven(long num)
{
    if (num % 2 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// get length of the card number
int getNumLength(long cardNum)
{
    long currentNum = cardNum;
    int i = 1;
    while (currentNum > 9)
    {
        currentNum = currentNum / 10;
        i++;
    }
    return i;
}


// get number at specific index of the card nubmer
int getDigit(long cardNum, int index)
{
    // loop until you hit the digit you are looking for
    int cardNumLength = getNumLength(cardNum);
    int runNum = cardNumLength - 1 - index;
    for (int i = 0; i < runNum; i++)
    {
        cardNum /= 10;
    }
    if (cardNumLength == 1)
    {
        return (int)cardNum;
    }
    return cardNum % 10;
}

int addDigits(int num)
{
    int numLength = getNumLength(num);
    int returnNum = 0;
    for (int i = 0; i < numLength; i++)
    {
        returnNum += getDigit(num, i);
    }
    return returnNum;
}