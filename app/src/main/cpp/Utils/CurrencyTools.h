//
// Created by win10 on 2020/3/27.
//

#ifndef BASEKING_CURRENCYTOOLS_H
#define BASEKING_CURRENCYTOOLS_H

#include <jni.h>
#include <memory.h>
#include <stdlib.h>
#include <dlfcn.h>

class CurrencyTools {
public:
    static jobject getApplication(JNIEnv *env);
    static char *getPackageName(JNIEnv *env,char *packageNames);
    static char *jstringToChar(JNIEnv *env, jstring data);
    static jobject getApplication2(JNIEnv *env);
    static char *getLibraryName(void *ftr);
    static void *getbaseAddress(void *ftr);
};


#endif //BASEKING_CURRENCYTOOLS_H
