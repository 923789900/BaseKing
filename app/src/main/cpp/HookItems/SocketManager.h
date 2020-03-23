//
// Created by win10 on 2020/3/23.
//

#ifndef BASEKING_SOCKETMANAGER_H
#define BASEKING_SOCKETMANAGER_H

#include <AndHook.h>
#include <k_Log.h>

typedef int (*connect)(int __af, int __type, int __protocol);

class SocketManager {

public:
    SocketManager();

private:
   void loadHookStart();
   static int Call_SocketConnect(int __af, int __type, int __protocol);
};


#endif //BASEKING_SOCKETMANAGER_H
