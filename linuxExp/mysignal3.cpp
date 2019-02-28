//
// Created by looper on 18-9-30.
//
// 编程用 fork()创建一个子进程代表售票员，司机在父进程中，再用系统调用 signal()
// 让父进程（司机）捕捉来自子进程（售票员）发出的中断信号，让子进程（售票员）捕捉来自（司机）发
// 出的中断信号，以实现进程间的同步运行。
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int flag = 1, pid;

void waiting() {
    while (flag > 0);
}

void sync(int){
    flag = 0;
}

void receive(int){
    flag = -1;
}

void say(int flag, int type,int pid){ //flag : -1 receive 0 sync type : 0 child 1 parent
    switch (type){
        case 0:
            if(-1 == flag){
                printf("[Info] Receive sync from my parent.\n");
            }else{
                printf("[Info] Sync to my parent.\n");
                kill(pid,SIGUSR2);
            }
            break;
        case 1:
            if(-1 == flag){
                printf("[Info] Receive sync from my child.\n");
            }else{
                printf("[Info] Sync to my child.\n");
                kill(pid,SIGUSR2);
            }
            break;
        default:
            break;
    }
}

int main() {
    while ((pid = fork()) == -1);
    if (pid != 0) { // 父进程
        printf("[Info] Parent ID: %d,Child ID: %d.\n",getpid(),pid);
        signal(SIGUSR1,sync);
        signal(SIGUSR2,receive);
        while(1){
            flag = 1;
            waiting();
            lockf(1,1,0);
            say(flag,1,pid);
            lockf(1,0,0);
        }
    } else { // 子进程
        signal(SIGUSR1,sync);
        signal(SIGUSR2,receive);
        while(1){
            flag = 1;
            waiting();
            lockf(1,1,0);
            say(flag,0,getppid());
            lockf(1,0,0);
        }
    }
}