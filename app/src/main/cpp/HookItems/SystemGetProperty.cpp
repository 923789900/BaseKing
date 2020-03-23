//
// Created by win10 on 2020/3/17.
//

#include "SystemGetProperty.h"

static systemGetProperty back_SystemGetProperty = nullptr;
static system_Get  android_property_Get = nullptr;
static findProperty find_SystemProperty = nullptr;

SystemGetProperty::SystemGetProperty()
{
    //loadHookStart();
    loadHookStart_find();
}

void SystemGetProperty::loadHookStart() {
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
    AKHookFunction(methodHandle,(void *)&Call_SystemProperty,(void **)&back_SystemGetProperty);
    AKCloseImage(imageHandle);
    king_Log_i("hook->__system_property_read->Success");
}

void SystemGetProperty::loadHookStart_find() {
    const void * imageHandle =  AKGetImageByName("libc.so");
    if(imageHandle == NULL)
    {
        king_Log_i("find image Failure -> SystemGetProperty -> libc.so");
        return;
    }

    //__system_property_find_compat
    void* methodHandle =  AKFindSymbol(imageHandle,"__system_property_find");
    if(methodHandle == NULL)
    {
        king_Log_i("find image Method Failure -> SystemGetProperty -> __system_property_find_compat");
        return;
    }

    //AKHookFunction(methodHandle,(void *)&Call_SystemProperty,(void **)&back_SystemGetProperty);
    AKHookFunction(methodHandle,(void *)&Call_findSystem,(void **)&find_SystemProperty);
    AKCloseImage(imageHandle);
    king_Log_i("hook->__system_property_find_compat->Success");
}


int SystemGetProperty::Call_SystemProperty(const prop_info *_pi, char *name, char *value) {



    int status = back_SystemGetProperty(_pi,name,value);
    const prop_info_compat *pic = (const prop_info_compat *)_pi;
    vector<keyValue> *params = new vector<keyValue>();
    char* newName = nullptr;
    if(name == NULL)
    {
        newName = const_cast<char *>("nullName");
    } else{
        newName = name;
    }
    keyValue param1 = {const_cast<char *>("name"), newName};
    params->push_back(param1);
    char* newValue = nullptr;
    if(value == NULL)
    {
        newValue = const_cast<char *>("nullValue");
    } else{
        newValue = value;
    }
    keyValue param2 = {"value", newValue};
    params->push_back(param2);
    androidLog::Log_vector(*params);
    delete params;
    return status;
}

int SystemGetProperty::Call_AndroidSystemGet(const char *__name, char *__value) {
    int status = android_property_Get(__name,__value);
    king_Log_i("inline : 0.1");
    vector<keyValue> *params = new vector<keyValue>();
    if(__name == NULL || __value == NULL)
    {
        return status;
    }
    keyValue param2 = {"name", __name};
    params->push_back(param2);
    king_Log_i("inline : 0.2");
    keyValue param = {"value", __value};
    params->push_back(param);
    king_Log_i("inline : 0.3");
    keyValue param1 = {"Result", to_string(status).c_str()};
    params->push_back(param1);
    king_Log_i("inline : 0.4");
    androidLog::Log_vector(*params);
    delete params;
    king_Log_i("inline : 0.5");
    return status;
}

const prop_info *SystemGetProperty::Call_findSystem(const char *name) {
    vector<keyValue> *params = new vector<keyValue>();
    keyValue Method = {"Method","Call_findSystem"};
    params->push_back(Method);
    const prop_info *prop = find_SystemProperty(name);
    const char *value ;
    if(prop == NULL)
    {
        value = const_cast<char *>("null");
    } else{
        char* local_value = (char *)prop->value;
        value = local_value;
    }

    const char * m_name = name;
    keyValue param1 = {"name",m_name};
    params->push_back(param1);

    keyValue param2 = {"value",value};
    params->push_back(param2);
    k_Log::f_writeLog(logType::property,*params);
    //androidLog::Log_vector(*params);
    delete params;
    return prop;
}
