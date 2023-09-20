def main():
    jar = Jar()
    print(jar)
    jar.deposit(5)
    print(jar)
    jar.withdraw(3)
    print(jar)
    print(jar.size)
    print(jar.capacity)

    try:
        jar.withdraw(20)
    except ValueError:
        print('oops')

    try:
        jar.deposit(20)
    except ValueError:
        print('oops')

class Jar:
    def __init__(self, capacity=12):
        self.capacity = capacity
        if self.capacity < 1 or not isinstance(self.capacity, int):
            raise ValueError
        self.size = 0

    def __str__(self):
        return '🍪' * self.size

    def deposit(self, n):
        new = self.size + n
        if new > self.capacity:
            raise ValueError
        self.size = new

    def withdraw(self, n):
        new = self.size - n
        if new < 0:
            raise ValueError
        self.size = new

    # @property
    def capacity(self):
        return self.capacity

    # @property
    def size(self):
        return self.size

main()