//
// Created by looper on 18-10-2.
//

# include<unistd.h>
# include<signal.h>
# include<stdio.h>
# include <wait.h>
# include <cstdlib>

int pid1, pid2;

int main() {
    int fd[2];
    int exitcode;
    char OutPipe[100], InPipe[100];
    pipe(fd);
    while ((pid1 = fork()) == -1);
    if (pid1 == 0) {// 子进程 1
        lockf(fd[0], 1, 0);
        read(fd[0],InPipe,50);
        printf("%s I am child process 1\n", InPipe);
        lockf(fd[0], 0, 0);
        exit(0);
    } else {
        while ((pid2 = fork()) == -1);
        if (pid2 == 0) {// 子进程 2
            lockf(fd[0], 1, 0);
            read(fd[0],InPipe,50);
            printf("%s I am child process 2\n", InPipe);
            lockf(fd[0], 0, 0);
            exit(0);
        } else {// 父进程
            lockf(fd[1], 1, 0);
            sprintf(OutPipe, "Parent process is sending message!");
            write(fd[1], OutPipe, 50);
            write(fd[1], OutPipe, 50);
            lockf(fd[1], 0, 0);
            waitpid(-1, &exitcode, 0);// 回收所有子进程
            exit(0);
        }
    }
}
