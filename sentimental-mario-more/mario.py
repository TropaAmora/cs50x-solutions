# Libraries
from cs50 import get_int

# Declaring variables
height = 0
counter = 0

# Prompt the user for the height input
while height > 8 or height < 1:
    height = get_int("Height: ")

# Runs through each line
for i in range(height):

    # Print the empty lines
    for j in range(height - 1 - counter):
        print(" ", end="")

    # Print the hashes
    for j in range(counter + 1):
        print("#", end="")

    # Print the blank spaces
    print("  ", end="")

    # Return to print the hashes
    for j in range(counter + 1):
        print("#", end="")

    # Print a new line and update the counter
    print("")
    counter += 1
