# Import lybraries
from cs50 import get_string


def main():

    # Prompt the user for input and run through the counting functions
    text = get_string("Text: ")
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # Coleman-Liau index equation
    L = (letters / words) * 100
    S = (sentences / words) * 100
    index = round(0.0588 * L - 0.296 * S - 15.8)

    # Rating grades
    if index < 1:
        print("Before Grade 1")
    elif index > 15:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


# Define count_letters
def count_letters(text):
    counter = 0
    for i in range(len(text)):
        index = ord(text[i])
        if (index < 91 and index > 64) or (index < 123 and index > 96):
            counter += 1

    return counter


# Define count_words
def count_words(text):
    counter = 0
    previous_char = False
    special_cha = [32, 46, 63, 33, 59, 44]
    for i in range(len(text)):
        index = ord(text[i])

        # Check if it is a special character and if the previous character was a letter
        if (index in special_cha) and previous_char == True:
            counter += 1

        # If it is a letter turn it true
        if (index < 91 and index > 64) or (index < 123 and index > 96):
            previous_char = True
        else:
            previous_char = False

    return counter


# Define count_sentences
def count_sentences(text):
    counter = 0
    special_char = [46, 63, 33]
    for i in range(len(text)):
        index = ord(text[i])
        if index in special_char:
            counter += 1
    return counter


if __name__ == "__main__":
    main()

    # Problems casting int with str text[], try to compare the characters by
    # their string form (not the ascii values)