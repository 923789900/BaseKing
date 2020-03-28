//
// Created by win10 on 2020/3/23.
//

#ifndef BASEKING_SOCKETMANAGER_H
#define BASEKING_SOCKETMANAGER_H

#include <AndHook.h>
#include <k_Log.h>
#include <netinet/in.h>


typedef int (*prt_socket)(int __af, int __type, int __protocol);

typedef int (*ptr_connection)(int __fd, const struct sockaddr *__addr, socklen_t __addr_length);

typedef int (*ptr_addressCast)(int af, const char *src, void *dst);

class SocketManager {

public:
    SocketManager();

private:
    void loadHookStart_socket();

    void loadHookStart_connection();

    void loadHookStart_inet_phon();

    static int Call_socket(int __af, int __type, int __protocol);

    static int Call_Connection(int, const struct sockaddr *, socklen_t);

    static int Call_inet_pton(int af, const char *src, void *dst);
};


#endif //BASEKING_SOCKETMANAGER_H
