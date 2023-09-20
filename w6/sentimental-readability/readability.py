# Determines reading level of input text

from cs50 import get_string


def main():
    text = get_string('Text: ')

    l = 0
    s = 0
    w = 1  # words = num of spaces + 1, also no divide by 0 error in C-L func
    for char in text:  # increment different counts
        if char in ['.', '!', '?']:
            s += 1
        elif char.isalpha():
            l += 1
        elif char == ' ':
            w += 1

    grade = round(coleman_liau(l, s, w))

    if grade < 1:
        print('Before Grade 1')
    elif grade >= 16:
        print('Grade 16+')
    else:  # all special cases handled
        print('Grade', grade)


def coleman_liau(letters, sentences, words):  # not necessary, just nice to be modular
    l = letters / words * 100
    s = sentences / words * 100
    return 0.0588 * l - 0.296 * s - 15.8


main()