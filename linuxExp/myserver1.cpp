//
// Created by looper on 18-10-2.
//
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <cstdlib>
#include <cstdio>

#define MSGKEY 75
struct msgform {
    long mtype;
    char mtext[1000];
} msg;
int msgqid;

void server() {
    msgqid = msgget(MSGKEY, 0777 | IPC_CREAT); /*创建 75#消息队列*/
    do {
        msgrcv(msgqid, &msg, 1030, 0, 0); /*接收消息*/
        printf("(server)received\n");
    } while (msg.mtype != 1);
    msgctl(msgqid, IPC_RMID, nullptr); /*删除消息队列，归还资源*/
    exit(0);
}

int main() {
    server();
}

// msgqid=msgget(key,flag)
// msgsnd(msgqid,msgp,size,flag)
// msgrcv(msgqid,msgp,size,type,flag)
// msgctl(msgqid,cmd,buf);