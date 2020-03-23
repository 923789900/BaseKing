//
// Created by win10 on 2020/3/17.
//

#ifndef BASEKING_ANDROIDLOG_H
#define BASEKING_ANDROIDLOG_H

#include <android/log.h>
#include <vector>
#include <iostream>


using namespace std;
#define  ANDROID_TAG "kingso"
#define king_Log_i(...) __android_log_print(ANDROID_LOG_VERBOSE, ANDROID_TAG, __VA_ARGS__)

struct keyValue
{
    const char* key;
    string value;
};



class androidLog {
public:
    static void Log_vector(vector<keyValue>&);
};


#endif //BASEKING_ANDROIDLOG_H
