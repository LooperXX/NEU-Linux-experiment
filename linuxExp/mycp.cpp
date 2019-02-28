//
// Created by looper on 18-9-28.
//
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <zconf.h>
void error(){
    printf("[Info]Example: ./mycp fileName newFileName\n");
}
int main(int ac, char *av[]){
    int fdin, fdout;
    char buf[512];
    int n;

    //指令格式验证
    if(ac<2){
        error();
        printf("[Error]Please enter the command again along with FileName and newFileName\n");
        exit(0);
    }else if(2 == ac){
        error();
        printf("[Error]Please enter the command again along with newFileName\n");
        exit(0);
    }else if(3 < ac){
        error();
        printf("[Error]Please enter the command again\n");
        exit(0);
    }

    //指令内容验证
    if((fdin=open(av[1], O_RDONLY))==-1){
        perror("cannot open input file");
        exit(0);
    }
    if((fdout=open(av[2], O_WRONLY|O_CREAT, 0640))==-1){
        perror("cannot open output file");
        exit(0);
    }
    while((n=read(fdin, buf, sizeof(buf)))>0){
        write(fdout, buf, n);
    }

    close(fdin);
    close(fdout);
}

