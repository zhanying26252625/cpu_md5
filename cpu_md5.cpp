#include "cpu_md5.h"
#include "md5.h"
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <sys/time.h>

using namespace std;

CPU_MD5::CPU_MD5(int passLen){

    this->numOfTests = 0;

    this->timeSpent = 0.0;

    this->generatorDone = false;
    
    this->found = false;

    this->maxPassLen = passLen;
}

int CPU_MD5::getNumberOfCores(){

    int cores = sysconf(_SC_NPROCESSORS_ONLN);

    this->num_cores = cores;

    return cores;
}

string CPU_MD5::crackMD5(const string& md5){

    this->target_md5 = md5;

    int cores = getNumberOfCores();

    cout<<"Number of Cores :" << cores << endl;

    pthread_t* workThreads = new pthread_t[cores];

    pthread_t generatorThread;

    //create generator thread 
    int rc = pthread_create(&generatorThread,NULL,CPU_MD5::generatorThreadFunc,(void*)this);

    if( -1 == rc ){
            cout<<"Can't create thread"<<endl;
            exit(1);
    }

    //create crack threads
    for(int i=0; i<cores; i++){
         
        usleep(100);

        int rc= pthread_create(&workThreads[i],NULL,CPU_MD5::crackThreadFunc,(void*)this);
        
        if( -1 == rc){
            cout<<"Can't create thread"<<endl;
            exit(1);
        }
    }

    /*
    for(int i=0; i< cores; i++){

        pthread_join( workThreads[i], NULL);
    }

    pthread_join(generatorThread, NULL);
    */

    struct timeval start, end;

    gettimeofday(&start,NULL);

    while(1){
       
        usleep(200);

        if( found || !isThereMore() ){
        
            break;
        }
    }

    gettimeofday(&end,NULL);

    double span = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;

    this->timeSpent = span;

/*
    //stop
    pthread_cancel(generatorThread);

    for(int i=0; i< cores; i++)
        pthread_cancel(workThreads[i]);
	//join
	for(int i=0; i< cores; i++){

        pthread_join( workThreads[i], NULL);
    }

    pthread_join(generatorThread, NULL);
*/
 
    delete[] workThreads;

	usleep(200);

    if(!found){

        cout<<endl<<"Can't find password!"<<endl;
        
        return "";
    }
    
	cout<<endl<<"Good news, we found the password!"<<endl;

    return this->password;
}

/*
 *Assume Password only contains
 A-Z
 a-z
 0-9
 * */

void* CPU_MD5::generatorThreadFunc(void* arg){

	pthread_setcancelstate( PTHREAD_CANCEL_ENABLE,NULL);

    CPU_MD5* cpu_md5 = (CPU_MD5*)arg;

    int maxPassLen = cpu_md5->maxPassLen;

    cout<<"Generator Thread is working...... Max password length is "<<maxPassLen<<endl;

    //init character array
    vector<char> charArr;

    for(char c = 'A'; c <= 'Z'; c++)
        charArr.push_back(c);

    for(char c = 'a'; c <= 'z'; c++)
        charArr.push_back(c);

    for(char c = '0'; c <= '9'; c++)
        charArr.push_back(c);


    for(int len = 1; len <= maxPassLen; len++){

        generatePassword(cpu_md5,len,charArr);
    }
        
    //teminator
    if(! cpu_md5->found)
        cpu_md5->buf.addNewPassword(string("$"));

    cout<<"Generator thread Done!"<<endl;

    cpu_md5->generatorDone = true;

    return (void*)1;
}

void CPU_MD5::_generatePassword(CPU_MD5* cpu_md5, int len, vector<char>& charArr, string& newPass, int level){

    //if already found, no need to cal anymore!
    if( cpu_md5->found )
        return;

    //new pass is generated!
    if( level == len ){

        //cout<<"New generated password is "<<newPass<<endl;
        cpu_md5->buf.addNewPassword(newPass);

        return;
    }

    for(int i = 0; i < charArr.size(); i++ ){

        newPass.push_back( charArr[i] );

        _generatePassword(cpu_md5, len,charArr,newPass,level+1);

        newPass.erase(level,1);
    }

    return;
}

void CPU_MD5::generatePassword(CPU_MD5* cpu_md5, int len, vector<char>& charArr){

    if( cpu_md5->found )
        return;

    string newPass;

    newPass.reserve(len);

    _generatePassword(cpu_md5, len, charArr, newPass, 0); 
}

bool CPU_MD5::isThereMore(){

    if( generatorDone && buf.getBufSize() == 0)
        return false;

    return true;
}

void* CPU_MD5::crackThreadFunc(void* arg){

	pthread_setcancelstate( PTHREAD_CANCEL_ENABLE,NULL);

    CPU_MD5* cpu_md5 = (CPU_MD5*)arg;

    cout<<"A new CrackThread is working......"<<endl;

    while( 1 ){
        
        if( cpu_md5->found )
            break;

        if( ! cpu_md5->isThereMore() )
            break;

        vector<string> passwords = cpu_md5->buf.consume();

        for(int i=0; i< passwords.size(); i++){

            string pass = passwords[i];

            //cout<<"No."<<cpu_md5->numOfTests++<<" Test password :"<<pass<<endl;

            cpu_md5->numOfTests++;

            MD5 md5_engine;

            string str = md5_engine.calMD5FromString(pass);

            //found
            if( !str.compare(cpu_md5->target_md5) ){

                cout<<endl<<"Found the password for this md5"<<endl;

                cpu_md5->found = true;

                cpu_md5->password = pass;

                break;
            }
        }

    }

    cout<<"One cracking thread is done!"<<endl;

    return (void*)0;
}
