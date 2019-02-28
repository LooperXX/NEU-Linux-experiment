//
// Created by looper on 18-9-29.
//
# include<stdio.h>
#include <zconf.h>

int main() {
    int p1, p2;
    while ((p1 = fork()) == -1);
    if (p1 == 0) {
        putchar('b'); // 在子进程 1 中
        exit(0);
    } else {
        // 在父进程中
        while ((p2 = fork()) == -1);
        if (p2 == 0){
            putchar('c'); // 在子进程 2 中
            exit(0);
        }else {
            // 在父进程中
            putchar('a');
            exit(0)
        }
    }
}
