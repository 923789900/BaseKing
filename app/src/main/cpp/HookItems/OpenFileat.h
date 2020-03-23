//
// Created by win10 on 2020/3/17.
//

#ifndef BASEKING_OPENFILEAT_H
#define BASEKING_OPENFILEAT_H

#include "AndHook.h"
#include <vector>
#include "androidLog.h"
#include <string>
#include "k_Log.h"

typedef int(*openfile)(int dirfd, const char *path, int flags, int mode);

class OpenFileat {
public:
    OpenFileat();
private:
    void loadHookStart();
    static int Call_OpenAt(int dirfd, const char *path, int flags, int mode);
};


#endif //BASEKING_OPENFILEAT_H
