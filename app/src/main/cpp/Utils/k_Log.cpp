//
// Created by win10 on 2020/3/21.
//

#include "k_Log.h"

static bool m_canWrite;
static FILE *m_property;
static FILE *m_openfile;
static FILE *m_register;
static FILE *m_total;
static FILE *m_socket;

/**
 * write log to file
 * @param params
 */
bool k_Log::f_writeLog(logType type, vector<keyValue> &params) {
    if (!m_canWrite) {
        king_Log_i("open log file is Failure");
        return false;
    }
    FILE *loac = getLogtype(type);
    string Result = "\n================================\n";
    string splitCharactor = " : ";
    string lineFeed = "\n";
    const char *header = Result.c_str();


    fwrite(header, sizeof(char), strlen(header), loac);
    fflush(loac);
    //m_logFile<<Result.c_str()<<endl;
    for (vector<keyValue>::iterator iter = params.begin(); iter != params.end(); iter++) {
        string item = (*iter).key + splitCharactor + (*iter).value + lineFeed;
        fwrite(item.c_str(), sizeof(char), strlen(item.c_str()), loac);
        fflush(loac);
    }
    return true;
}

/**
 * initialize log object
 * @param fileName
 */
bool k_Log::init(const char *fileName, JNIEnv *env) {
    char *pName = RegisterNatives::getPackageName(env);
    king_Log_i("native PackageName : %s", pName);
    char *dir_Path = getDirPath(pName);
    king_Log_i("native Log dirPath : %s", dir_Path);
    DIR *dir = opendir(dir_Path);
    if (NULL == dir) {
        king_Log_i("Create native_log Dir : %s", dir_Path);
        int status = mkdir(dir_Path, S_IRWXU);
        if (status == 0) {
            king_Log_i("native_log Dir : Success");
            //关闭dir
            closedir(dir);
        } else {
            king_Log_i("Create native_log Dir Status : %d", status);
        }
    } else {
        king_Log_i("native_log Dir : Success");
        //关闭dir
        closedir(dir);
    }
    string dirPath = dir_Path;
    return openFile(dirPath);
}

FILE *&k_Log::getLogtype(logType type) {
    switch (type) {
        case logType::openFile :
            return m_openfile;
            break;
        case logType::property :
            return m_property;
            break;
        case logType::registerNative :
            return m_register;
            break;
        case logType::socketM:
            return m_socket;
        default:
            return m_total;
    }

}

/**
 *  out put single message
 * @param message
 */
bool k_Log::f_writeLog(logType type, string message) {
    if (m_canWrite) {
        const char *mess = message.c_str();
        size_t count = strlen(mess);
        FILE *loac = getLogtype(type);
        size_t status = fwrite(mess, sizeof(char), count, loac);
        if (status > 0) {
            fflush(loac);
            return true;
        }

    }
    return false;

}

/**
 * Get Current Dirpath
 * @param pName
 * @return
 */
char *k_Log::getDirPath(char *pName) {
    string befor, middle, fileName, result;
    befor = "data/data/";
    middle = pName;
    fileName = "/native_log";
    result = befor + middle + fileName;
    /*int dir_Len = strlen("data/data/") + strlen(pName) + strlen("/native_log");
    char *dir_Path = (char *) malloc(dir_Len);
    memset(dir_Path, 0, dir_Len);
    dir_Path = strcpy(dir_Path, "data/data/");
    dir_Path = strcat(dir_Path, pName);
    dir_Path = strcat(dir_Path, "/native_log");*/
    return const_cast<char *>(result.c_str());
}

bool k_Log::openFile(string &dirPath) {
    string openFileName = "/openFile.txt";
    string property = "/property.txt";
    string registerNatives = "/registerNatives.txt";
    string total = "/total.txt";
    string socketFile = "/socket.txt";
    //---------------------------打开所有文件------------------
    string openFile = dirPath + openFileName;
    char *packageName = const_cast<char *>(openFile.c_str());
    m_openfile = fopen(packageName, "w");
    //=====================================
    openFile = dirPath + property;
    packageName = const_cast<char *>(openFile.c_str());
    m_property = fopen(packageName, "w");
    //=====================================
    openFile = dirPath + registerNatives;
    packageName = const_cast<char *>(openFile.c_str());
    m_register = fopen(packageName, "w");
    //=====================================
    openFile = dirPath + total;
    packageName = const_cast<char *>(openFile.c_str());
    m_total = fopen(packageName, "w");
    //=====================================
    openFile = dirPath + socketFile;
    packageName = const_cast<char *>(openFile.c_str());
    m_socket = fopen(packageName, "w");

    if (NULL != m_openfile && NULL != m_property && NULL != m_register && NULL != m_total) {
        m_canWrite = true;
        return true;
    }
    m_canWrite = false;
    return false;
}

