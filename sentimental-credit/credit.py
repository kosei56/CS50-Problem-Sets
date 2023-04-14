# TODO
from cs50 import get_int
import re


def main():
    cardNum = input("Number: ")
    if valid(cardNum):
        if re.match("35", cardNum) != None or re.match("37", cardNum):
            print("AMEX")
        elif int(cardNum[:2]) in range(51, 56):
            print("MASTERCARD")
        elif cardNum[0] == "4":
            print("VISA")
    else:
        print("INVALID")


def valid(cardNum):
    sum1 = 0
    sum2 = 0
    startNum = 0
    if len(cardNum) % 2 == 0:
        startNum = 0
    else:
        startNum = 1
    for i in cardNum[startNum::2]:
        num = int(i)*2
        if(num < 10):
            sum1 += num
        else:
            for j in str(num):
                sum1 += int(j)
    for i in cardNum[startNum::2]:
        sum2 += int(i)
    total = sum1 + sum2
    if total % 10 == 0:
        return True
    else:
        return False


main()