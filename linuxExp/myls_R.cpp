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
#include <vector>
#include <ctime>
#include <string>

void show(std::string type) {
    DIR *dp;
    struct dirent *p;
    struct stat buf{};
    std::vector<std::string> dir;
    int k = 0;

    printf("%s:\n",type.c_str());
    dp=opendir(type.c_str());
    while((p=readdir(dp))!= nullptr){
        if(!strcmp(p->d_name,".") || !strcmp(p->d_name,"..")) continue;
        k++;
        stat((type + "/" + p->d_name).c_str(),&buf);
        if(S_ISDIR(buf.st_mode))
            dir.push_back(p->d_name);
        printf("%s\t",p->d_name);
        if(k == 10){
            printf("\n");
            k = 0;
        }
    }
    printf("\n\n");
    for(auto &itor : dir){
        show(type + "/" + itor);
    }

    closedir(dp);
}

int main(int ac,char* av[]){
    int rv;
    std::string type = ".";
    if(1 == ac){
        printf("[Info]## Current Directory ##\n");
    }else if(2 == ac){
        if((rv = chdir(av[1])) == -1){
            printf("[Error]Cannot change to the path you entered. :<\n");
            exit(0);
        }else{
            printf("[Info]## Directory：%s ##\n",av[1]); // test: /home/looper/linuxNotes
            type = av[1];
            //if(!strcmp(&type.at(type.length() - 1),"/")) type = type.substr(0,type.length() - 2);
        }
    }else{
        printf("[Error]Your command has too many arguments. :<\n");
        exit(0);
    }

    show(type);

}


