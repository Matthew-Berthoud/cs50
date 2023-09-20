import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print('Usage: python dna.py data.csv sequence.txt')
        return

    # TODO: Read database file into a variable
    file1 = open(sys.argv[1])
    keys = file1.readline().rstrip().split(',')[1:]
    file1.close()

    file1 = open(sys.argv[1])
    database = csv.DictReader(file1)

    # TODO: Read DNA sequence file into a variable
    file2 = open(sys.argv[2])
    seq = file2.readline()

    # TODO: Find longest match of each STR in DNA sequence
    person = {}
    for sub in keys:
        person[sub] = longest_match(seq, sub)

    # TODO: Check database for matching profiles
    name = 'No match'
    for profile in database:
        match = True
        for str_name, str_count in person.items():
            if int(profile[str_name]) != str_count:
                match = False
                break
        if match:
            name = profile['name']
            break

    file1.close()
    file2.close()
    print(name)


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