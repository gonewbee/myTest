import os
import sys

if __name__ == '__main__':
    f1 = "System_Idle_Process_before_openapp.txt"
    f2 = "System_Idle_Process_after_openapp.txt"
    if (not os.path.isfile(f1)) or (not os.path.isfile(f2)):
        print("file not exist!")
        sys.exit(1)

    with open(f1, 'r') as fb:
        processes1 = [line.split()[0] for line in fb.readlines()[:-1]]
        with open(f2, 'r') as fa:
            processes2 = fa.readlines()
            for process in processes2[:-1]:
                if process.split()[0] not in processes1:
                    print(process, end="")
