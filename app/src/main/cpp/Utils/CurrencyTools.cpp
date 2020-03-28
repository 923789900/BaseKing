//
// Created by win10 on 2020/3/27.
//


#include "CurrencyTools.h"

jobject CurrencyTools::getApplication(JNIEnv *env) {
    jobject application = NULL;
    jclass activity_thread_clz = (*env).FindClass("android/app/ActivityThread");
    if (activity_thread_clz != NULL) {
        jmethodID get_Application = (*env).GetStaticMethodID(activity_thread_clz,
                                                              "currentActivityThread",
                                                              "()Landroid/app/ActivityThread;");
        if (get_Application != NULL) {
            jobject currentActivityThread = (*env).CallStaticObjectMethod(activity_thread_clz,
                                                                           get_Application);
            jmethodID getal = (*env).GetMethodID(activity_thread_clz, "getApplication",
                                                  "()Landroid/app/Application;");
            application = (*env).CallObjectMethod(currentActivityThread, getal);
        }
    }

    if (application == NULL) {
        application = getApplication2(env);
    }

    return application;
}

char *CurrencyTools::getPackageName(JNIEnv *env, char *packageNames) {
    jobject obj = getApplication(env);
    if (obj == NULL) {
        return "king";
    }
    // (*env)->GetMethodID(env,
    jclass applicationCls = (*env).GetObjectClass(obj);
    jmethodID getpackageName = (*env).GetMethodID(applicationCls, "getPackageName",
                                                   "()Ljava/lang/String;");
    jstring packageName = (jstring) (*env).CallObjectMethod(obj, getpackageName);
    char *pName = jstringToChar(env, packageName);
    if(pName == NULL)
    {
        return packageNames;
    }else{
        return pName;
    };
}

char *CurrencyTools::jstringToChar(JNIEnv *env, jstring data) {
    char *rtn = NULL;
    jclass clsstring = (*env).FindClass("java/lang/String");
    jstring strencode = (*env).NewStringUTF("utf-8");
    jmethodID mid = (*env).GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) (*env).CallObjectMethod(data, mid, strencode);
    jsize alen = (*env).GetArrayLength(barr);
    jbyte *ba = (*env).GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    (*env).ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

jobject CurrencyTools::getApplication2(JNIEnv *env) {
    jobject application = NULL;
    jclass activity_thread_clz = (*env).FindClass("cn/king/admin/Manager");
    if (activity_thread_clz != NULL) {
        /*jmethodID getApplication = env->GetMethodID(activity_thread_clz, "getApplication",
                                                    "()Landroid/app/Application;");
        king_Log_i("getApplication 2 : %p",getApplication);
        application = env->CallStaticObjectMethod(activity_thread_clz, getApplication);*/
        jfieldID app = (*env).GetStaticFieldID(activity_thread_clz, "app", "android/app/Application");
        application = (*env).GetStaticObjectField(activity_thread_clz, app);
        return application;
    }

    return application;
}

char *CurrencyTools::getLibraryName(void *ftr) {
    Dl_info info;

    int status = dladdr(ftr, &info);
    if (status != 0) {
        const char *libName = info.dli_fname;
        return (char *) libName;
    }

    return "";
}

void *CurrencyTools::getbaseAddress(void *ftr) {
    Dl_info info;

    int status = dladdr(ftr, &info);
    if (status != 0) {
        return info.dli_fbase;
    }

    return nullptr;
}
