import os


def diff_ports(f1, f2):
    if (not os.path.isfile(f1)) or (not os.path.isfile(f2)):
        print("file not exist!")
        return

    print("diff " + f1 + " " + f2)
    with open(f1, 'r') as fb:
        # ports1 = [line.split()[1] for line in fb.readlines()]
        ports1 = fb.readlines()
        with open(f2, 'r') as fa:
            for line in fa.readlines():
                if line not in ports1:
                    print(line, end='')
    print()

if __name__ == '__main__':
    files = [["netstat_before1.txt", "netstat_after1.txt"],
             ["netstat_before2.txt", "netstat_after2.txt"]]
    for f1, f2 in files:
        diff_ports(f1, f2)
