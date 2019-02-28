//
// Created by looper on 18-9-29.
//
// refer : https://blog.csdn.net/feng964497595/article/details/79672214

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    const int targetLayer = 4;// 目标层数
    const int nodes = 1;
    pid_t pid[nodes + 1];
    int layer = 1;

    printf("[Info]Root pid is %d.\n", getpid());
    for (int i = 0; i < nodes;) {
        if (i == 0) layer++;
        pid[i] = fork();

        if (pid[i] == 0) {
            // 子进程
            printf("[Info]My parent is %d, my pid is %d.\n", getppid(), getpid());
            if (layer >= targetLayer) {
                // 层数够了
                printf("[Info]Process %d had exited.\n", getpid());
                exit(0);
            } else {
                i = 0;
            }
        } else if (pid[i] != -1) {
            // 父进程
            printf("[Info]Parent process %d create child process %d.\n", getpid(), pid[i++]);//下标+1
        } else {
            printf("[Info]Error: when %d fork()!\n", getpid());
        }
    }

    for (int i = 0; i < nodes; ++i) {
        waitpid(pid[i], NULL, 0);
    }

    printf("[Info]Process %d had exited.\n", getpid());

    exit(0);
}