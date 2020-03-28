//
// Created by win10 on 2020/3/16.
//

#include <androidLog.h>
#include "RegisterNatives.h"
#include "AndHook.h"

PRegisterNatives RegisterNatives::targetBack_RegisterNatives = nullptr;
static mp_register targetBack_register_class = nullptr;
JNIEnv* RegisterNatives::internalEnv;

RegisterNatives::RegisterNatives(JNIEnv *env) {
    this->functions = *(*env).functions;
    this->internalEnv = env;
    loadHookStart();
    //loadHookStart_ClassRegister();
    /*    (*env).RegisterNatives;*/
}

void RegisterNatives::loadHookStart() {

    const JNINativeInterface *NativeInterface = &this->functions;
    void *targetAddress = (void *) NativeInterface->RegisterNatives;
    void *callAddress = (void *) &this->back_RegisterNatives;
    void **targetBack = (void **) &targetBack_RegisterNatives;
    AKHookFunction(targetAddress, callAddress, targetBack);
}

jint
RegisterNatives::back_RegisterNatives(JNIEnv *env, jclass clazz, const JNINativeMethod *methods,jint nMethods) {

   if(nMethods < 0)
   {
       k_Log::f_writeLog(logType::registerNative,"RegisterNative 0");
       return nMethods;
   }

    char* name = CurrencyTools::getLibraryName(methods[0].fnPtr);
    void *baseAddress = CurrencyTools::getbaseAddress(methods[0].fnPtr);
    vector<keyValue> *params = new vector<keyValue>();
    for (int i = 0; i < nMethods; i++) {


        string *str = new string(methods[i].name);
        string *str2 = new string(methods[i].signature);
        string result = (*str) + (*str2);

        keyValue param = {"MethodName", result.c_str()};
        params->push_back(param);

        char classPtr[100] = {0};
        sprintf(classPtr, "%p", methods[i].fnPtr);
        keyValue param1 = {"MethodPtr", classPtr};
        params->push_back(param1);

        char Address[20] = {0};
        sprintf(Address, "%p", baseAddress);
        keyValue param2 = {"baseAddress", Address};
        params->push_back(param2);



        delete str;
        delete str2;
    }

    k_Log::f_writeLog(logType::registerNative,name, *params);
    delete params;
    return targetBack_RegisterNatives(env, clazz, methods, nMethods);
}

/**
 * hook jni class register method
 * @param clazz
 * @param methods
 * @param nMethods
 * @return
 */
jint RegisterNatives::back_cpRegisterNatives(jclass clazz, const JNINativeMethod *methods,jint nMethods) {

    king_Log_i("lalalala");
    (internalEnv->*targetBack_register_class)(clazz,methods,nMethods);
    return 0;
}

void RegisterNatives::loadHookStart_ClassRegister() {
    mp_register  clsP_register = &JNIEnv::RegisterNatives;
    /*JNIEnv env = (*internalEnv);
    //void * target = static_cast<void *>(env.*clsP_register);
    king_Log_i("lala object method ptr : %p",&target);
    AKHookFunction(target, (void *)&back_cpRegisterNatives, reinterpret_cast<void **>(&targetBack_register_class));
    king_Log_i("lala object call method ptr : %p",&back_cpRegisterNatives);
    king_Log_i("lala object back method ptr : %p",&targetBack_register_class);
    king_Log_i("lala hook state success");*/
}

