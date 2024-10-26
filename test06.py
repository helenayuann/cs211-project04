## test06.py ##
print()
print("this program has nested while loops")
print()

i = 0
j = 0
N = 2

print("Outer Loop")
while i <= N:
{
    print(i)
    print()

    j = 0
    print("Inner loop")
    while j < N:
    {
        print(j)
        j = j + 1
    }
    print()

    i = i + 1
}

print('Outer Loop Done')
print()
