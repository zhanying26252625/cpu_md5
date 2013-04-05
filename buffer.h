#include <queue>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <string>

using namespace std;

class Buffer{

public:

    Buffer();

    ~Buffer();

    void addNewPassword(string newPass);

    void produce( vector<string>& passwords);

    vector<string> consume();

    int getBufSize(){ return buf.size(); }

private:
    
    sem_t sema_empty;
    
    sem_t sema_full;

    pthread_mutex_t mutex;

    queue< vector<string> > buf;

    static const int BatchSize; 

    static const int QueueSize;
};
