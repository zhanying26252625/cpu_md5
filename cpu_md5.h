#ifndef CPU_MD5_H
#define CPU_MD5_H

#include "md5.h"
#include "buffer.h"
#include <string>
#include <vector>

using namespace std;

class CPU_MD5{

public:

    CPU_MD5(int passLen);

    void setMaxPassLen(int num){ this->maxPassLen = num; }

    string crackMD5(const string& md5);

    bool isThereMore();

    void setMaxCores(int num){ this->max_cores = num; }

    double getTimeSpent(){ return this->timeSpent; }

    int getNumOfTests(){ return this->numOfTests; }
private:

    static void* generatorThreadFunc(void* arg);
    
    static void* crackThreadFunc(void* arg);

    static void generatePassword(CPU_MD5* cpu_md5, int len, vector<char>& charArr);

    static void _generatePassword(CPU_MD5* cpu_md5,int len, vector<char>& charArr, string& newPass, int level);

    int getNumberOfCores();

private:

    int num_cores;

    int max_cores;

    int maxPassLen;

    string target_md5;

    string password;

    bool found;

    bool generatorDone;

    int numOfTests;

    double timeSpent;

    Buffer buf;
};


#endif
