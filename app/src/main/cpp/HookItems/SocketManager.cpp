//
// Created by win10 on 2020/3/23.
//
#include "SocketManager.h"

prt_socket sc_socket = nullptr;
ptr_connection p_connection = nullptr;
ptr_addressCast p_inet_pton = nullptr;

SocketManager::SocketManager() {
    loadHookStart_socket();
    loadHookStart_connection();
    loadHookStart_inet_phon();
}

void SocketManager::loadHookStart_socket() {
    const void *imageHandle = AKGetImageByName("libc.so");
    if (imageHandle == NULL) {
        king_Log_i("find image Failure -> SystemGetProperty -> libc.so");
        return;
    }

    //__system_property_read_compat
    //__system_property_read
    // __system_property_get
    void *methodHandle = AKFindSymbol(imageHandle, "socket");
    if (methodHandle == NULL) {
        king_Log_i("find image Method Failure -> SystemGetProperty -> __system_property_read");
        return;
    }

    //AKHookFunction(methodHandle,(void *)&Call_SystemProperty,(void **)&back_SystemGetProperty);
    AKHookFunction(methodHandle, (void *) &Call_socket, (void **) &sc_socket);
    AKCloseImage(imageHandle);
    king_Log_i("hook->socket->Success");
}

void SocketManager::loadHookStart_connection() {
    const void *imageHandle = AKGetImageByName("libc.so");
    if (imageHandle == NULL) {
        king_Log_i("find image Failure -> SystemGetProperty -> libc.so");
        return;
    }

    //__system_property_read_compat
    //__system_property_read
    // __system_property_get
    void *methodHandle = AKFindSymbol(imageHandle, "connect");
    if (methodHandle == NULL) {
        king_Log_i("find image Method Failure -> SystemGetProperty -> __system_property_read");
        return;
    }

    //AKHookFunction(methodHandle,(void *)&Call_SystemProperty,(void **)&back_SystemGetProperty);
    AKHookFunction(methodHandle, (void *) &Call_Connection, (void **) &p_connection);
    AKCloseImage(imageHandle);
    king_Log_i("hook->connect->Success");
}

void SocketManager::loadHookStart_inet_phon() {
    const void *imageHandle = AKGetImageByName("libc.so");
    if (imageHandle == NULL) {
        king_Log_i("find image Failure -> SystemGetProperty -> libc.so");
        return;
    }

    //__system_property_read_compat
    //__system_property_read
    // __system_property_get
    void *methodHandle = AKFindSymbol(imageHandle, "inet_pton");
    if (methodHandle == NULL) {
        king_Log_i("find image Method Failure -> SystemGetProperty -> __system_property_read");
        return;
    }

    //AKHookFunction(methodHandle,(void *)&Call_SystemProperty,(void **)&back_SystemGetProperty);
    AKHookFunction(methodHandle, (void *) &Call_inet_pton, (void **) &p_inet_pton);
    AKCloseImage(imageHandle);
    king_Log_i("hook->inet_pton->Success");
}


int SocketManager::Call_socket(int __af, int __type, int __protocol) {
    int Result = sc_socket(__af, __type, __protocol);
    vector<keyValue> *params = new vector<keyValue>();

    keyValue param = {"addrType", to_string(__af).c_str()};
    params->push_back(param);

    keyValue param1 = {"SocketType", to_string(__type).c_str()};
    params->push_back(param1);

    keyValue param2 = {"protocolType", to_string(__protocol).c_str()};
    params->push_back(param2);

    k_Log::f_writeLog(logType::socketM, *params);

    delete params;
    return Result;
}

int SocketManager::Call_Connection(int _fd, const struct sockaddr *_address, socklen_t _type) {


    vector<keyValue> *params = new vector<keyValue>();

    sockaddr_in sin;
    struct sockaddr addr = (*_address);
    struct sockaddr_in *sock = ( struct sockaddr_in*)&addr;

    int port = ntohs(sock->sin_port);

    struct in_addr in  = sock->sin_addr;
    char str[INET_ADDRSTRLEN];   //INET_ADDRSTRLEN这个宏系统默认定义 16
    //成功的话此时IP地址保存在str字符串中。
    inet_ntop(AF_INET,&in, str, sizeof(str));


    keyValue param2 = {"serverAddress", str};
    params->push_back(param2);


    char L_port[8] = {0};
    sprintf(L_port, "%d", port);
    keyValue param4 = {"serverPort", L_port};
    params->push_back(param4);



    int status = p_connection(_fd, _address, _type);


    keyValue param = {"socketFlag", to_string(_fd).c_str()};
    params->push_back(param);

    char state[15] = {0};
    sprintf(state, "%d", status);
    keyValue param1 = {"connectStatus", state};
    params->push_back(param1);

    k_Log::f_writeLog(logType::socketM, *params);

    delete params;

    return status;
}

int SocketManager::Call_inet_pton(int af, const char *src, void *dst) {
    int Result = p_inet_pton(af, src, dst);

    vector<keyValue> *params = new vector<keyValue>();

    keyValue param = {"ServerAddress", src};
    params->push_back(param);

    //取当前的基地址
    void *baseAddress = CurrencyTools::getbaseAddress(&p_inet_pton);
    char base[20] = {0};
    sprintf(base, "%p", baseAddress);
    keyValue param1 = {"baseAddress", base};
    params->push_back(param1);


    k_Log::f_writeLog(logType::socketM, *params);
    delete params;

    return Result;
}

