//
// Created by win10 on 2020/3/16.
//

#include <androidLog.h>
#include "RegisterNatives.h"
#include "AndHook.h"

PRegisterNatives RegisterNatives::targetBack_RegisterNatives = nullptr;

RegisterNatives::RegisterNatives(JNIEnv *env) {
    this->functions = *(*env).functions;
    loadHookStart();
}

void RegisterNatives::loadHookStart() {
    //获取jnienv的指针
    //this->internalEnv->RegisterNatives();
    //target address

    const JNINativeInterface *NativeInterface = &this->functions;
    void *targetAddress = (void *) NativeInterface->RegisterNatives;
    //king_Log_i("targetAddress %p : ", targetAddress);
    void *callAddress = (void *) &this->back_RegisterNatives;
    //king_Log_i("callAddress %p : ", callAddress);
    //call address
    void **targetBack = (void **) &targetBack_RegisterNatives;
    AKHookFunction(targetAddress, callAddress, targetBack);
    //king_Log_i("targetBack_RegisterNatives Ptr_ :  %p ", targetBack);
}

jint
RegisterNatives::back_RegisterNatives(JNIEnv *env, jclass clazz, const JNINativeMethod *methods,
                                      jint nMethods) {

   if(nMethods < 0)
   {
       k_Log::f_writeLog(logType::registerNative,"RegisterNative 0");
       return nMethods;
   }

    char* name =  getLibraryName(methods[0].fnPtr);
    vector<keyValue> *params = new vector<keyValue>();

    for (int i = 0; i < nMethods; i++) {


        string *str = new string(methods[i].name);
        string *str2 = new string(methods[i].signature);
        string result = (*str) + (*str2);

        keyValue param = {"MethodName", result.c_str()};
        params->push_back(param);

        char classPtr[100] = {0};
        sprintf(classPtr, "%p", methods[i].fnPtr);
        keyValue param1 = {"ClassPtr", classPtr};
        params->push_back(param1);

        delete str;
        delete str2;
    }

    k_Log::f_writeLog(logType::registerNative,name, *params);
    delete params;
    return targetBack_RegisterNatives(env, clazz, methods, nMethods);
}

/**
 * get context
 * @param env
 * @return
 */
jobject RegisterNatives::getApplication(JNIEnv *env) {
    jobject application = NULL;
    jclass activity_thread_clz = env->FindClass("android/app/ActivityThread");
    king_Log_i("getApplication 1");
    if (activity_thread_clz != NULL) {
        jmethodID get_Application = env->GetStaticMethodID(activity_thread_clz,
                                                           "currentActivityThread",
                                                           "()Landroid/app/ActivityThread;");
        king_Log_i("getApplication 2");
        if (get_Application != NULL) {
            jobject currentActivityThread = env->CallStaticObjectMethod(activity_thread_clz,
                                                                        get_Application);
            jmethodID getal = env->GetMethodID(activity_thread_clz, "getApplication",
                                               "()Landroid/app/Application;");
            application = env->CallObjectMethod(currentActivityThread, getal);
            king_Log_i("getApplication 3");
        }
    }

    if (application == NULL) {
        application = getApplication2(env);
    }

    return application;


}

/**
 * get application Package Name
 * @param env
 * @return
 */
char *RegisterNatives::getPackageName(JNIEnv *env) {

    jobject obj = getApplication(env);
    if (obj == NULL) {
        king_Log_i("get application Failure ");
        return const_cast<char *>("king");
    }
    jclass applicationCls = env->GetObjectClass(obj);
    jmethodID getpackageName = env->GetMethodID(applicationCls, "getPackageName",
                                                "()Ljava/lang/String;");
    jstring packageName = (jstring) env->CallObjectMethod(obj, getpackageName);
    char *pName = jstringToChar(env, packageName);
    return pName;
}

/**
 * jstring to char *
 * @param env
 * @param data
 * @return
 */
char *RegisterNatives::jstringToChar(JNIEnv *env, jstring data) {
    char *rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(data, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

jobject RegisterNatives::getApplication2(JNIEnv *env) {
    jobject application = NULL;
    jclass activity_thread_clz = env->FindClass("cn/king/admin/Manager");
    if (activity_thread_clz != NULL) {
        /*jmethodID getApplication = env->GetMethodID(activity_thread_clz, "getApplication",
                                                    "()Landroid/app/Application;");
        king_Log_i("getApplication 2 : %p",getApplication);
        application = env->CallStaticObjectMethod(activity_thread_clz, getApplication);*/
        jfieldID app = env->GetStaticFieldID(activity_thread_clz, "app", "android/app/Application");
        application = env->GetStaticObjectField(activity_thread_clz, app);
        return application;
    }

    return application;
}

char *RegisterNatives::getLibraryName(void *ftr) {
    Dl_info info;

    int status = dladdr(ftr, &info);
    if (status != 0) {
        vector<keyValue> *params = new vector<keyValue>();
        const char *libName = info.dli_fname;
        return const_cast<char *>(libName);
    }

    return nullptr;
}

