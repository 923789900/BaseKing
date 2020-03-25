//
// Created by win10 on 2020/3/23.
//

#include <sys/socket.h>
#include "SocketManager.h"

prt_socket sc_socket = nullptr;
ptr_connection p_connection = nullptr;

SocketManager::SocketManager() {
    loadHookStart_socket();
    loadHookStart_connection();
}

void SocketManager::loadHookStart_socket() {
    const void * imageHandle =  AKGetImageByName("libc.so");
    if(imageHandle == NULL)
    {
        king_Log_i("find image Failure -> SystemGetProperty -> libc.so");
        return;
    }

    //__system_property_read_compat
    //__system_property_read
    // __system_property_get
    void* methodHandle =  AKFindSymbol(imageHandle,"socket");
    if(methodHandle == NULL)
    {
        king_Log_i("find image Method Failure -> SystemGetProperty -> __system_property_read");
        return;
    }

    //AKHookFunction(methodHandle,(void *)&Call_SystemProperty,(void **)&back_SystemGetProperty);
    AKHookFunction(methodHandle,(void *)&Call_socket,(void **)&sc_socket);
    AKCloseImage(imageHandle);
    king_Log_i("hook->__system_property_read->Success");
}

void SocketManager::loadHookStart_connection()  {
    const void * imageHandle =  AKGetImageByName("libc.so");
    if(imageHandle == NULL)
    {
        king_Log_i("find image Failure -> SystemGetProperty -> libc.so");
        return;
    }

    //__system_property_read_compat
    //__system_property_read
    // __system_property_get
    void* methodHandle =  AKFindSymbol(imageHandle,"connect");
    if(methodHandle == NULL)
    {
        king_Log_i("find image Method Failure -> SystemGetProperty -> __system_property_read");
        return;
    }

    //AKHookFunction(methodHandle,(void *)&Call_SystemProperty,(void **)&back_SystemGetProperty);
    AKHookFunction(methodHandle,(void *)&Call_Connection,(void **)&p_connection);
    AKCloseImage(imageHandle);
    king_Log_i("hook->__system_property_read->Success");
}


int SocketManager::Call_socket(int __af, int __type, int __protocol) {
    int  Result =  sc_socket(__af,__type,__protocol);
    vector<keyValue> *params = new vector<keyValue>();

    keyValue param = {"addrType",to_string(__af).c_str()};
    params->push_back(param);

    keyValue param1 = {"SocketType",to_string(__type).c_str()};
    params->push_back(param);

    keyValue param2 = {"protocolType",to_string(__protocol).c_str()};
    params->push_back(param);

    k_Log::f_writeLog(logType::socketM,*params);

    delete params;
    return Result;
}

int SocketManager::Call_Connection(int _fd, const struct sockaddr * _address, socklen_t _type) {

    int status = p_connection(_fd,_address,_type);

    vector<keyValue> *params = new vector<keyValue>();

    keyValue param = {"socketFlag",to_string(_fd).c_str()};
    params->push_back(param);

    //keyValue param1 = {"Socketaddr",_address->sa_data};
    //params->push_back(param);

    k_Log::f_writeLog(logType::socketM,*params);

    delete params;

    return status;
}

