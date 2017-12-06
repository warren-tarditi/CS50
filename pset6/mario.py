import cs50

while True:
    print("Height: ", end = "")
    Height = cs50.get_int()
    if Height >= 0 and Height <= 23:
        break

for i in range(Height):
    print(' ' * (Height-1-i), end = "")
    print('#' * (i+2))


