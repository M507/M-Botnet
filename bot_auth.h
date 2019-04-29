//
// Created by M
//

#include <winsock2.h>
#include <stdio.h>
#include <time.h>
#pragma comment(lib, "w2_32")


char *getmyip(char *IP){
#define TMP_FILE_NAME "TMP_IP.txt"

    system("ipconfig | findstr IPv4 > TMP_IP.txt");
    FILE * fdir = fopen(TMP_FILE_NAME, "r");
    char buff[500];
    if (fdir){
        while (1) {
            if (fgets(buff, 300, fdir) == NULL) break;
            IP = strcat(IP,buff);
            //printf("%s", buff);
        }
    }
    fclose(fdir);
    remove(TMP_FILE_NAME);
    return IP;
}

