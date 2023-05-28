# Include cs50 library
from cs50 import get_string

# Prompt the user for input, trying first with a get_string
number = get_string("Number: ")

# Stored the value into a new variable to be used to determine if is valid, and declare variables
sum1 = 0
sum2 = 0
sum3 = 0
invalid = 0


for i in range(len(number)):
    place = len(number) - i
    digit = int(number[place - 1])
    temp = digit * 2
    if (i + 1) % 2 != 0:
        sum1 = sum1 + digit
    elif temp < 10:
        sum2 = sum2 + temp
    else:
        extra = temp % 10
        temp = (temp - extra) / 10
        sum2 = sum2 + extra + temp

sum3 = sum1 + sum2

if sum3 % 10 == 0 and len(number) < 17 and len(number) > 12:
    digit_1 = int(number[0])
    digit_2 = int(number[1])

    # Check for VISA
    if digit_1 == 4:
        print("VISA")
    else:
        invalid += 1

    # Check for MASTERCARD
    if digit_1 == 5 and digit_2 > 0 and digit_2 < 6 and len(number) == 16:
        print("MASTERCARD")
    else:
        invalid += 1

    # Check for AMEX
    if digit_1 == 3 and (digit_2 == 4 or digit_2 == 7) and len(number) == 15:
        print("AMEX")
    else:
        invalid += 1
else:
    print("INVALID")
if invalid == 3:
    print("INVALID")