//
// Created by looper on 18-10-2.
//
// 在父进程中创建一个消息队列，用 fork 创建一个子进程，在子进程中将一条消息传送至消息
// 队列，父进程接受队列的消息，并将消息送屏幕显示。
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <cstdlib>
#include <cstdio>
#include <zconf.h>
#include <wait.h>

#define MSGKEY 75
struct msgform {
    long mtype;
    char mtext[1000];
} msg;
int msgqid, pid;

int main() {
    while ((pid = fork()) == -1);
    if (0 == pid) {// 子进程
        int i;
        msgqid = msgget(MSGKEY, 0777); /*打开 75#消息队列*/
        for (i = 10; i >= 1; i--) {
            msg.mtype = i;
            printf("(client)sent\n");
            msgsnd(msgqid, &msg, 1024, 0); /*发送消息*/
        }
        exit(0);
    } else {// 父进程
        msgqid = msgget(MSGKEY, 0777 | IPC_CREAT); /*创建 75#消息队列*/
        wait(nullptr);
        do {
            msgrcv(msgqid, &msg, 1030, 0, 0); /*接收消息*/
            printf("(server)received\n");
        } while (msg.mtype != 1);
        msgctl(msgqid, IPC_RMID, nullptr); /*删除消息队列，归还资源*/
        exit(0);
    }
}

// msgqid=msgget(key,flag)
// msgsnd(msgqid,msgp,size,flag)
// msgrcv(msgqid,msgp,size,type,flag)
// msgctl(msgqid,cmd,buf);