//
// Created by win10 on 2020/3/17.
//

#ifndef BASEKING_SYSTEMGETPROPERTY_H
#define BASEKING_SYSTEMGETPROPERTY_H

#include <androidLog.h>
#include <sys/system_properties.h>
#include <AndHook.h>
#include <vector>
#include "k_Log.h"

typedef int (*systemGetProperty)(const prop_info *_pi, char *name, char *value);
typedef int(*system_Get)(const char* __name, char* __value);
typedef const prop_info *(*findProperty)(const char*);
//new_prop_info


//__system_property_find_compat
struct prop_info_compat {
    char name[PROP_NAME_MAX];
    unsigned volatile serial;
    char value[PROP_VALUE_MAX];
};

struct prop_info {
    volatile uint32_t serial;
    char value[PROP_VALUE_MAX];
    char name[PROP_NAME_MAX];

    prop_info(const char *name, const uint8_t namelen, const char *value,
              const uint8_t valuelen) {
        memcpy(this->name, name, namelen);
        this->name[namelen] = '\0';
        this->serial = (valuelen << 24);
        memcpy(this->value, value, valuelen);
        this->value[valuelen] = '\0';
    }
};

class SystemGetProperty {
public:
    SystemGetProperty();
private:
    void loadHookStart();
    void loadHookStart_find();
    void loadHookStart_newPropInfo();
    static int Call_SystemProperty(const prop_info *_pi, char *name, char *value);
    static int Call_AndroidSystemGet(const char* __name, char* __value);
    static const prop_info *Call_findSystem(const char* name);

};


#endif //BASEKING_SYSTEMGETPROPERTY_H
