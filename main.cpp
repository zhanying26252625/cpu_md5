#include "md5.h"
#include "cpu_md5.h"
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <stdlib.h>
using namespace std;

void PrintMD5(const string &str, const string & md5) {
	cout << "MD5(\"" << str << "\") = " << md5  << endl;
}

void test(){

    cout<<"Size ulong is "<<sizeof(ulong)<<endl;
	
    cout<<"Size uInt is "<<sizeof(uInt)<<endl;

    MD5 md5;

    string str = md5.calMD5FromString("Z");
	PrintMD5("Z", str);

	str = md5.calMD5FromFile("./Makefile");
	PrintMD5("./Makefile", str);
 
}

int main(int argc, char* argv[]) {

    //test();

    //return 1;

    if(argc < 3){
      
        cout<<"cpu_md5 maxPassLen md5"<<endl;
            
        return 0;
    }

    string md5(argv[2]);
 
    cout<<"MD5 is :"<<md5<<endl;
   
    int maxPassLen = atoi(argv[1]);

    if( maxPassLen < 1 || maxPassLen > 12){
    
        maxPassLen = 12;
    }

    CPU_MD5 cpu_md5_cracker(maxPassLen);

    string password = cpu_md5_cracker.crackMD5(md5);

	double seconds = cpu_md5_cracker.getTimeSpent();
 
	double tests = cpu_md5_cracker.getNumOfTests();

    cout<<seconds<<" seconds have been used"<<endl;

    cout<<tests<<" passwords has been tested"<<endl;

	int throughput = (int)(tests/seconds);

	cout<<throughput<<" md5_hash/per_second"<<endl;

    cout<<"MD5 is :"<<md5<<endl;

    cout<<"Password is :"<<password<<endl<<endl;

	cout.flush();

	usleep(100);

	exit(1);

    return 0;
}
