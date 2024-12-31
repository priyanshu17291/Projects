class Stack:
    def __init__(self):
        self.stack = []

    def push(self, item):
        self.stack.append(item)

    def pop(self):
        if not self.is_empty():
            return self.stack.pop()
        else:
            return 'Error'

    def peek(self):
        if not self.is_empty():
            return self.stack[-1]
        else:
            return 'Error'
    def __str__(self):
        c = ''
        for i in self.stack:
            c += ' '+str(i)
        s = c.strip()[::-1]
        return s


    def is_empty(self):
        return len(self.stack) == 0

    def __len__(self):
        return len(self.stack)
'''a = Stack()
a.push(1)
a.push(2)
a.push(3)
a.push("e")
print(len(a))
print(a)'''