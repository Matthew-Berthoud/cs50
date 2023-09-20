# Checks for valid credit card number

from cs50 import get_string  # so I can slice it
from re import match  # so I can check for correct length numeric input


def main():
    pattern = r"^\d{13}|\d{15,16}$"
    card = input('Number: ')
    if not match(pattern, card) or not valid(card):
        print('INVALID')


def valid(card):
    x = 0
    s = ''
    for i in range(-1, -1 * (len(card)) - 1, -1):  # go backwards through indicies
        if i % 2 == 0:
            s += str(2 * int(card[i]))  # add to string so we can go digit by digit
        else:
            x += int(card[i])
    for i in s:
        x += int(i)  # add up digits
    comp = company(card)
    if x % 10 == 0 and comp[0]:
        print(comp[1])
        return True
    else:
        return False


def company(card):
    if card[0:2] in ['34', '37']:
        return [True, 'AMEX']
    elif int(card[0:2]) in range(51, 56):
        return [True, 'MASTERCARD']
    elif card[0] == '4':
        return [True, 'VISA']
    else:
        return [False]


main()