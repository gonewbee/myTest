import sys

if len(sys.argv) > 1:
    with open(sys.argv[1], 'rb') as fi:
        data = fi.read()
        for d in data:
            print(hex(d), end=', ')
        print()