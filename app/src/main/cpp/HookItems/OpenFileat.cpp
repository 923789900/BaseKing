//
// Created by win10 on 2020/3/17.
//

#include "OpenFileat.h"

openfile _OpenFileAt = nullptr;


OpenFileat::OpenFileat() {
    loadHookStart();
}

void OpenFileat::loadHookStart() {
    const void *imghandle = AKGetImageByName("libc.so");
    if (imghandle == NULL) {
        king_Log_i("find imgage Failure -> libc");
        return;
    }

    void *method_Handle = AKFindSymbol(imghandle, "__openat");
    if (method_Handle == NULL) {
        king_Log_i("find method -> __openat failure");
        return;
    }

    AKHookFunction(method_Handle, (void *) &this->Call_OpenAt, (void **) &_OpenFileAt);
    AKCloseImage(imghandle);
}

int OpenFileat::Call_OpenAt(int dirfd, const char *path, int flags, int mode) {
    vector<keyValue>* list = new vector<keyValue>();

    int status = strcmp(path,"/dev/ashmem");
    if(status == 0 || strcmp(path,"/dev/null") == 0)
    {
        king_Log_i("open path : %s",path);
        return _OpenFileAt(dirfd, path, flags, mode);
    }
    keyValue param = {"path",path};
    list->push_back(param);
    keyValue flag = {"flags",to_string(flags)};
    list->push_back(flag);
    keyValue modes = {"mode",to_string(mode)};
    list->push_back(modes);
    keyValue dirfds = {"dirfd",to_string(dirfd)};
    list->push_back(dirfds);
    int Result = _OpenFileAt(dirfd, path, flags, mode);
    keyValue openfd = {"openfd",to_string(Result)};
    list->push_back(openfd);
    k_Log::f_writeLog(logType ::openFile,*list);
    delete list;
    return Result;
}
