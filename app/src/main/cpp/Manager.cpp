//
// Created by win10 on 2020/3/16.
//

#include <jni.h>
#include "Utils/androidLog.h"
#include "HookItems/RegisterNatives.h"
#include "HookItems/OpenFileat.h"
#include "HookItems/SystemGetProperty.h"
#include "Utils/k_Log.h"
#include <sys/socket.h>
#include "SocketManager.h"

static RegisterNatives *registerNatives = nullptr;
static OpenFileat *HK_OpenFile = nullptr;
static SystemGetProperty *systemGet = nullptr;
static SocketManager *socketManager = nullptr;
static bool hookState = false;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm,void *s)
{

    JNIEnv *env;
    jint status =  vm->GetEnv((void **)&env,JNI_VERSION_1_4);
    king_Log_i("get env status : %d",status);
    if(status != JNI_OK)
    {
        return status;
    }

    if(!hookState)
    {
        //注册log method
        king_Log_i("open log File");
        bool status = k_Log::init("native_log.txt",env);
        king_Log_i("init log File %d",status);
        bool writeState =  k_Log::f_writeLog(logType::total,"init Success");
        //注册RegisterNatives hook Method
        registerNatives = new RegisterNatives(env);
        //注册__openat hook Method
        HK_OpenFile = new OpenFileat();
        //注册__system_property_get
        systemGet = new SystemGetProperty();
        //注册socket connect
        socketManager = new SocketManager();

    }





    return JNI_VERSION_1_4;
}