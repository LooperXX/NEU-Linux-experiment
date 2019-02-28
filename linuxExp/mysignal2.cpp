//
// Created by looper on 18-9-29.
//


#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int pid1, pid2;

void IntDelete(int) {
    kill(pid1, 10);
    kill(pid2, 12);

}

void Int1(int) {
    printf("child process 1 is killed by parent!\n");
    exit(0);
}

void Int2(int) {
    printf("child process 2 is killed by parent !\n");
    exit(0);
}

int main() {
    int exitcode;
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    while ((pid1 = fork()) == -1);
    if (pid1 == 0) {
        signal(SIGUSR1, Int1);
        signal(SIGINT, SIG_IGN);
        pause();
        exit(0);
    } else {
        while ((pid2 = fork()) == -1);
        if (pid2 == 0) {
            signal(SIGUSR2, Int2);
            signal(SIGINT, SIG_IGN);
            pause();
            exit(0);
        } else {
            signal(SIGINT, IntDelete);
            /*等待任何子进程中断或结束*/
            waitpid(-1, &exitcode, 0); //-1     meaning wait for any child process.
            printf("parent process is killed!\n");
            exit(0);
        }
    }
}