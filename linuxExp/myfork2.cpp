//
// Created by looper on 18-9-29.
//
# include<stdio.h>
#include <zconf.h>

int main() {
    int p1, p2, i;
    while ((p1 = fork()) == -1);
    if (0 == p1){
        for (i = 0; i < 100; i++)
            printf("daughter %d\n", i);
        exit(0);
    } else {
        while ((p2 = fork()) == -1);
        if (0 == p2){
            for (i = 0; i < 100; i++)
                printf("son %d\n", i);
            exit(0);
        } else{
            for (i = 0; i < 100; i++)
                printf("parents %d\n", i);
            exit(0);
        }
    }
}