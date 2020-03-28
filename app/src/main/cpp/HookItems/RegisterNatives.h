//
// Created by win10 on 2020/3/16.
//

#ifndef BASEKING_REGISTERNATIVES_H
#define BASEKING_REGISTERNATIVES_H

#include <jni.h>
#include "k_Log.h"
#include <dlfcn.h>
#include "CurrencyTools.h"


typedef jint (*PRegisterNatives)(JNIEnv *,jclass clazz, const JNINativeMethod* methods, jint nMethods);
typedef jint (JNIEnv::*mp_register)(jclass clazz, const JNINativeMethod* methods,jint nMethods);


class RegisterNatives {
public:
    RegisterNatives(JNIEnv *);
    /*static jobject getApplication(JNIEnv *);
    static jobject getApplication2(JNIEnv *);
    static char* getPackageName(JNIEnv *);
    static char* jstringToChar(JNIEnv *,jstring);*/
private:
    RegisterNatives();
    static JNIEnv* internalEnv;
    JNINativeInterface functions;
    static PRegisterNatives targetBack_RegisterNatives;
    void loadHookStart();
    void loadHookStart_ClassRegister();
    static jint back_RegisterNatives(JNIEnv *env,jclass clazz, const JNINativeMethod* methods, jint nMethods);
    static jint back_cpRegisterNatives(jclass clazz, const JNINativeMethod* methods,jint nMethods);
};


#endif //BASEKING_REGISTERNATIVES_H
