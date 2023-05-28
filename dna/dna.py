# Check for provided DNA sequence in a provided database

# Import libraries
import csv
import sys


def main():
    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py FILENAME1.csv FILENAME2.txt")

    # Count the number of columns in the header
    # Read database file into a list of dicts
    data = []
    with open(sys.argv[1], "r") as file:
        temp = csv.DictReader(file)
        data = list(temp)

    # Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as DNA:
        DNA_seq = DNA.readline()

    # Find longest match of each STR in DNA sequence, stores the integer values in seq list
    seq = []
    for i in range(len(header)):
        seq[i] = int(longest_match(DNA_seq, header[i]))

    # Check database for matching profiles
    match = False
    i = 0

    # loop through all registered database
    for i in range(len(person)):
        while match == False:
            for j in range(len(header)):
                if seq[j] == person[header[j]]
            if seq[i] == person

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
