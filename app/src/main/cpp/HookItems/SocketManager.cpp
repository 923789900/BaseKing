//
// Created by win10 on 2020/3/23.
//
#include "SocketManager.h"

prt_socket sc_socket = nullptr;
ptr_connection p_connection = nullptr;
ptr_addressCast p_inet_pton = nullptr;

SocketManager::SocketManager() {
    //loadHookStart_socket();
    loadHookStart_connection();
    //loadHookStart_inet_phon();
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

    //===========parse socket struct================
    //=====1
    sockaddr_in sin;
    struct sockaddr addr = (*_address);
    struct sockaddr_in *sock = ( struct sockaddr_in*)&addr;
    //=====2
    sockaddr_un *l_bin = ( struct sockaddr_un*)&addr;


    //============socketType=============
    char type[8] = {0};
    sprintf(type, "%d", addr.sa_family);
    keyValue l_socketType = {"socketType", type};
    params->push_back(l_socketType);

    //判断socket类型
    keyValue l_socketRemoteAddress;
    int status;
    char state[15] = {0};
    keyValue l_socketConnectState;
    switch (addr.sa_family)
    {
        case 1:
        {
            //============socketRemoteAddress================
            l_socketRemoteAddress = {"serverAddress", l_bin->sun_path};
            params->push_back(l_socketRemoteAddress);
            //=================call connect=======================
            status = p_connection(_fd, _address, _type);
            //=============socketConnectState============================
            sprintf(state, "%d", status);
            l_socketConnectState = {"connectStatus", state};
            params->push_back(l_socketConnectState);
            break;
        }
        case 2:
        {//============socketRemoteAddress================
            struct in_addr in  = sock->sin_addr;
            char str[INET_ADDRSTRLEN];
            inet_ntop(addr.sa_family,&in, str, sizeof(str));
            l_socketRemoteAddress = {"serverAddress", str};
            params->push_back(l_socketRemoteAddress);

            //============socketRemotePort================
            int port = ntohs(sock->sin_port);
            char L_port[8] = {0};
            sprintf(L_port, "%d", port);
            keyValue l_socketRemotePort = {"serverPort", L_port};
            params->push_back(l_socketRemotePort);

            //=================call connect=======================
            status = p_connection(_fd, _address, _type);

            //=============socketFD============================
            char flag[15] = {0};
            sprintf(flag, "%d", _fd);
            keyValue l_socketFD = {"socketfd", flag};
            params->push_back(l_socketFD);

            //=============socketConnectState============================
            sprintf(state, "%d", status);
            l_socketConnectState = {"connectStatus", state};
            params->push_back(l_socketConnectState);
            k_Log::f_writeLog(logType::socketM, *params);
            break;
        }
        default:
        {
            //=================call connect=======================
            status = p_connection(_fd, _address, _type);
            //=============socketConnectState============================
            sprintf(state, "%d", status);
            l_socketConnectState = {"connectStatus", state};
            params->push_back(l_socketConnectState);
            k_Log::f_writeLog(logType::socketM, *params);
            break;
        }

    }

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

