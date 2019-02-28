//
// Created by looper on 18-9-29.
//

//编写一段程序，使用系统调用 fork( )创建两个子进程，再用系统调用 signal( )让父进程捕捉
//        键盘上来的中断信号（即按 ctrl+c 键），当捕捉到中断信号后，父进程用系统调用 kill( )向两个
//        子进程发出信号，子进程捕捉到父进程发来的信号后，分别输出下列信息后终止：
//Child process 1 is killed by parent!
//Child process 2 is killed by parent!
//父进程等待两个子进程终止后，输出以下信息后终止：
//Parent process is killed!

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int wait_mark;

void waiting() {
    while (wait_mark != 0);
}

void stop(int) {
    wait_mark = 0;
}

int main() {
    int p1, p2;
    signal(SIGINT, stop);
    while ((p1 = fork()) == -1);
    if (p1 > 0) { /*在父进程中*/   // 1
        while ((p2 = fork()) == -1);
        if (p2 > 0) { /*在父进程中*/  // 2
            wait_mark = 1;
            waiting();
            kill(p1, 10);
            kill(p2, 12);
            wait(nullptr);
            wait(nullptr);
            printf("parent process is killed!\n");
            exit(0);
        } else { /*在子进程 2 中*/
            wait_mark = 1;
            signal(12, stop);
            waiting();
            //lockf(1,1,0)是锁定屏幕输出，不让其他进程可以输出到屏幕，lockf(1,0,0)则是解锁
            lockf(1, 1, 0);
            printf("child process 2 is killed by parent !\n");
            lockf(1, 0, 0);
            exit(0);
        }
    } else { /*在子进程 1 中*/
        wait_mark = 1;
        signal(10, stop);
        waiting();
        lockf(1, 1, 0);
        printf("child process 1 is killed by parent!\n");
        lockf(1, 0, 0);
        exit(0);
    }
}
