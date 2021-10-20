# README

## How to Use

Run `./build-and-run.sh` and read the output messages. On my Ubuntu 18.04, the output is:

```
+ mktemp -d
+ TMP_D=/tmp/tmp.K90ft3mOO4
+ trap cleanup EXIT INT TERM
+ : A_subsubfunc1 A_subfunc1 A_subfunc2 A_subfunc3 A_func1 A_func2
+ echo Compiling A_subsubfunc1.c
Compiling A_subsubfunc1.c
+ gcc -c A_subsubfunc1.c -o A_subsubfunc1.o
+ echo Compiling A_subfunc1.c
Compiling A_subfunc1.c
+ gcc -c A_subfunc1.c -o A_subfunc1.o
+ echo Compiling A_subfunc2.c
Compiling A_subfunc2.c
+ gcc -c A_subfunc2.c -o A_subfunc2.o
+ echo Compiling A_subfunc3.c
Compiling A_subfunc3.c
+ gcc -c A_subfunc3.c -o A_subfunc3.o
+ echo Compiling A_func1.c
Compiling A_func1.c
+ gcc -c A_func1.c -o A_func1.o
+ echo Compiling A_func2.c
Compiling A_func2.c
+ gcc -c A_func2.c -o A_func2.o
+ A_OBJ_FILES=/tmp/tmp.K90ft3mOO4/A_obj_files.txt
+ touch /tmp/tmp.K90ft3mOO4/A_obj_files.txt
+ echo A_subsubfunc1.o
+ echo A_subfunc1.o
+ echo A_subfunc2.o
+ echo A_subfunc3.o
+ echo A_func1.o
+ echo A_func2.o
+ cat /tmp/tmp.K90ft3mOO4/A_obj_files.txt
A_subsubfunc1.o
A_subfunc1.o
A_subfunc2.o
A_subfunc3.o
A_func1.o
A_func2.o
+ cat /tmp/tmp.K90ft3mOO4/A_obj_files.txt
+ ar rvs libA.a A_subsubfunc1.o A_subfunc1.o A_subfunc2.o A_subfunc3.o A_func1.o A_func2.o
ar: creating libA.a
a - A_subsubfunc1.o
a - A_subfunc1.o
a - A_subfunc2.o
a - A_subfunc3.o
a - A_func1.o
a - A_func2.o
+ gcc -c B.c -o B.o
+ ar rvs libB.a B.o
ar: creating libB.a
a - B.o
+ gcc -c main.c
+ gcc main.o -o main1.exe -L. -lA -lB
+ gcc main.o -o main2.exe -L. -lB -lA
./libA.a(A_subfunc2.o): In function `subfunc2':
A_subfunc2.c:(.text+0x14): undefined reference to `B_work'
collect2: error: ld returned 1 exit status
+ cleanup
+ rm -fr /tmp/tmp.K90ft3mOO4
```

Note the last two `gcc` command lines: the first one can link the program successfully while the second one fails with "undefined reference to `B_work'". Both the success and failure are expected.
