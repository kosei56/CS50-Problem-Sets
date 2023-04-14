# TODO
from cs50 import get_string

text = get_string("Text: ")
wordNum = 1
letterNum = 0
sentNum = 0

for c in text:
    if c.isalpha():
        letterNum += 1
    if c == " ":
        wordNum += 1
    if c == "." or c == "?" or c == "!":
        sentNum += 1

L = letterNum * (100/wordNum)
S = sentNum * (100/wordNum)

index = int(round(0.0588 * L - 0.296 * S - 15.8, 0))

if index > 15:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print(f"Grade {index}")