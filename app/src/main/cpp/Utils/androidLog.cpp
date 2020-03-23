//
// Created by win10 on 2020/3/17.
//

#include "androidLog.h"

 void androidLog::Log_vector(vector< keyValue>& messages) {

    string Result = "\n================================\n";
    for(vector<keyValue>::iterator iter = messages.begin();iter != messages.end();iter++ )
    {

        Result += (*iter).key;
        Result += " : ";
        Result += (*iter).value;
        Result +="\n";
    }
    const char * data = (char *)Result.c_str();
    king_Log_i("%s",data);
}
