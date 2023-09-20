# Prints symmetrical pyramids of height from user input

from cs50 import get_int

h = 0
while h not in range(1, 9):
    h = get_int('Height: ')

for i in range(1, h + 1):
    s = ' ' * (h - i)  # spaces
    b = '#' * i        # blocks (#)
    print(s + b + '  ' + b)