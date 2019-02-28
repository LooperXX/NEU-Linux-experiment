//
// Created by looper on 18-9-29.
//

# include<stdio.h>
#include <zconf.h>
#include <wait.h>

int main() {
    int p1, p2, p3;
    while ((p1 = fork()) == -1);
    if (0 == p1){
        // 我是子进程
        while ((p2 = fork()) == -1);
        if (0 == p2){
            // 我是子进程
            while ((p3 = fork()) == -1);
            if (0 == p3){
                // 我是子进程
                printf("I am child_D. My PID is %d. My PPID is %d.\n", getpid(), getppid());
            }else{
                wait(NULL);
                printf("I am child_C. My PID is %d. My PPID is %d.\n", getpid(), getppid());
            }
        }else{
            wait(NULL);
            printf("I am child_B. My PID is %d. My PPID is %d.\n", getpid(), getppid());
        }
    } else {
        // 我是父进程
        wait(NULL);
        printf("I am father_A. My PID is %d. My PPID is %d.\n", getpid(), getppid());
    }
}