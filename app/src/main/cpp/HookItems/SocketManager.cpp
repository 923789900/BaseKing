//
// Created by win10 on 2020/3/23.
//

#include "SocketManager.h"

connect sc_connect = nullptr;


SocketManager::SocketManager() {
    loadHookStart();
}

void SocketManager::loadHookStart() {
    const void * imageHandle =  AKGetImageByName("libc.so");
    if(imageHandle == NULL)
    {
        king_Log_i("find image Failure -> SystemGetProperty -> libc.so");
        return;
    }

    //__system_property_read_compat
    //__system_property_read
    // __system_property_get
    void* methodHandle =  AKFindSymbol(imageHandle,"__system_property_read");
    if(methodHandle == NULL)
    {
        king_Log_i("find image Method Failure -> SystemGetProperty -> __system_property_read");
        return;
    }

    //AKHookFunction(methodHandle,(void *)&Call_SystemProperty,(void **)&back_SystemGetProperty);
    AKHookFunction(methodHandle,(void *)&Call_SocketConnect,(void **)&sc_connect);
    AKCloseImage(imageHandle);
    king_Log_i("hook->__system_property_read->Success");
}

int SocketManager::Call_SocketConnect(int __af, int __type, int __protocol) {
    int  Result =  sc_connect(__af,__type,__protocol);
    return Result;
}
