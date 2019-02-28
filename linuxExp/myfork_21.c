//
// Created by looper on 18-10-24.
//
#include <stdio.h>
#include <zconf.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>

void getTime();

int main( void )
{
    int p1, p2;
    while ((p1 = fork()) == -1);
    if (p1 == 0) {
        printf("b");
        getTime();
        exit(0);
    } else {
        // 在父进程中
        while ((p2 = fork()) == -1);
        if (p2 == 0){
            printf("c");
            getTime();
            exit(0);
        }else {
            // 在父进程中
            printf("a");
            getTime();
            exit(0);

        }
    }
}


void getTime(){
    struct timeb timebuffer;
    time_t time1;
    unsigned short millitm1;
    ftime( &timebuffer );
    time1 = timebuffer.time;
    millitm1 = timebuffer.millitm;
    printf( "\n%ld %d\n",time1,millitm1);
}
