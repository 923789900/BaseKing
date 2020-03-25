//
// Created by win10 on 2020/3/21.
//

#ifndef BASEKING_K_LOG_H
#define BASEKING_K_LOG_H

#include <iostream>
#include <stdio.h>
#include "androidLog.h"
#include "RegisterNatives.h"
#include <dirent.h>


#include <sys/stat.h>
#include<time.h>
#include<sys/types.h>


using namespace std;
enum logType{
    property,
    openFile,
    registerNative,
    total,
    socketM
};
static class k_Log {
public:
    static bool init(const char *fileName,JNIEnv *env);
    static bool f_writeLog(logType,vector<keyValue>& params);
    static bool f_writeLog(logType,string message);
private:
    static char* getDirPath(char* pName);
    static FILE*& getLogtype(logType);
    static bool openFile(string& dirPath);
};


#endif //BASEKING_K_LOG_H
