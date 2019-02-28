//
// Created by looper on 18-9-28.
//
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <pwd.h>
#include <grp.h>
#include <ctime>

void initmodestr(mode_t mode, char modestr[]){
    strcpy(modestr, "----------");
    if(01 & mode) modestr[9]='x';
    if(02 & mode) modestr[8]='w';
    if(04 & mode) modestr[7]='r';

    if(010 & mode) modestr[6]='x';
    if(020 & mode) modestr[5]='w';
    if(040 & mode) modestr[4]='r';

    if(0100 & mode) modestr[3]='x';
    if(0200 & mode) modestr[2]='w';
    if(0400 & mode) modestr[1]='r';

    if(S_ISDIR(mode)) modestr[0]='d';
}

int main(int ac,char* av[]){
    int rv;
    if(1 == ac){
        printf("[Info]## Current Directory ##\n");
    }else if(2 == ac){
        if((rv = chdir(av[1])) == -1){
            printf("[Error]Cannot change to the path you entered. :<\n");
            exit(0);
        }else{
            printf("[Info]## Directory：%s ##\n",av[1]); // test: /home/looper/linuxNotes
        }
    }else{
        printf("[Error]Your command has too many arguments. :<\n");
        exit(0);
    }

    DIR *dp;
    struct dirent *p;
    struct stat buf{};
    char modestr[11];

    dp=opendir(".");
    while((p=readdir(dp))!= nullptr){
        if(!strcmp(p->d_name,".") || !strcmp(p->d_name,"..")) continue;
        stat(p->d_name, &buf);
        initmodestr(buf.st_mode, modestr);
        printf("%s\t %ld\t %s\t %s\t %-8ld\t %.19s\t %s\n", modestr,
                buf.st_nlink , getpwuid(buf.st_uid)->pw_name, getgrgid(buf.st_gid)->gr_name,buf.st_size, ctime(&buf.st_atime) , p->d_name);
    }

    closedir(dp);
}

//struct dirent {
//    ino_t          d_ino;       /* Inode number */
//    off_t          d_off;       /* Not an offset; see below */
//    unsigned short d_reclen;    /* Length of this record */
//    unsigned char  d_type;      /* Type of file; not supported
//                                              by all filesystem types */
//    char           d_name[256]; /* Null-terminated filename */
//};

//struct stat {
//    dev_t     st_dev;         /* ID of device containing file */
//    ino_t     st_ino;         /* Inode number */
//    mode_t    st_mode;        /* File type and mode */
//    nlink_t   st_nlink;       /* Number of hard links */
//    uid_t     st_uid;         /* User ID of owner */
//    gid_t     st_gid;         /* Group ID of owner */
//    dev_t     st_rdev;        /* Device ID (if special file) */
//    off_t     st_size;        /* Total size, in bytes */
//    blksize_t st_blksize;     /* Block size for filesystem I/O */
//    blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */
//
//    struct timespec st_atim;  /* Time of last access */
//    struct timespec st_mtim;  /* Time of last modification */
//    struct timespec st_ctim;  /* Time of last status change */
//
//#define st_atime st_atim.tv_sec      /* Backward compatibility */
//#define st_mtime st_mtim.tv_sec
//#define st_ctime st_ctim.tv_sec
//};

