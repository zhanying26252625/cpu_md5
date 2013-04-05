#include "buffer.h"
#include <iostream>

using namespace std;

const int Buffer::BatchSize = 1024*2;

const int Buffer::QueueSize = 512;

Buffer::Buffer(){

    pthread_mutex_init(&mutex,NULL);

    sem_init(&sema_empty,0,QueueSize);

    sem_init(&sema_full,0,0);
}

Buffer::~Buffer(){

    pthread_mutex_destroy(&mutex);

    sem_destroy(&sema_empty);

    sem_destroy(&sema_full);
}

void Buffer::addNewPassword(string newPass){

    static int counter = 0;

    static vector<string> tempBuf;

    tempBuf.push_back(newPass);

    //cout<<"( "<<counter<<" ) New password : "<<newPass <<endl;

    counter++;

    if( tempBuf.size() ==  Buffer::BatchSize || !newPass.compare("$") ){

        //cout<<"size is "<<tempBuf.size()<<" BatchSize is "<<BatchSize<<endl;

        produce(tempBuf);

        tempBuf.clear();
    }

    return;
}

void Buffer::produce( vector<string>& passwords){

    sem_wait(&sema_empty);

    pthread_mutex_lock(&mutex);

    buf.push(passwords);

    //cout<<"Produce"<<endl;

    pthread_mutex_unlock(&mutex);

    sem_post(&sema_full);
}


vector<string> Buffer::consume(){

    sem_wait(&sema_full);

    pthread_mutex_lock(&mutex);

    vector<string> passwords = buf.front();

    buf.pop();

    //cout<<"Consume"<<endl;

    pthread_mutex_unlock(&mutex);

    sem_post(&sema_empty);

    return passwords;
}




