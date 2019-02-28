//
// Created by looper on 18-9-29.
//
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <zconf.h>

int main(){
    int fd;
    char buf[512];
    int n;

    if((fd = open("/dev/input/mice", O_RDONLY))==-1){
        perror("cannot open");
        exit(0);
    }

    while((n = read(fd, buf, sizeof(buf)))>0){
        printf("%.*s\n", n, buf);
    }

    close(fd);
}
