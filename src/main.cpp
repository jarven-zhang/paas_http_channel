#include <iostream>
#include <vector>
#include <map>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <cstdio>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include "sp_server.h"

using namespace std;


int signatureMoveToTail(string& destSrc)
{
    if(destSrc.empty())
    {
        return -1;
    }

    unsigned int pos = destSrc.find("]");
    if(pos == std::string::npos)
    {
        pos = destSrc.find("】");
        if(pos == std::string::npos)
        {
            return -1;
        }
        pos += 3;
    }
    else
    {
        pos++;
    }

    string src_sign    = destSrc.substr(0, pos);
    string src_content = destSrc.substr(pos);

    destSrc = src_content + src_sign;

    return 0;
}

const string REPORT_HEAD = "report=";

int foo()
{
    string respone = "report=13480690519,UNDELIV,2019-04-15 18:08,F7E49FF6965D4ACAB18D0C70FC54EA9CortSocketHa";
    string report_respone = respone;
    size_t pos = respone.find(REPORT_HEAD);
    pos += strlen(REPORT_HEAD.data());

    report_respone = respone.substr(pos);
    cout << "rst:" << report_respone << endl;
    return 0;
}

int main()
{
#if 1
    sigset_t signal_mask;
    sigemptyset (&signal_mask);
    sigaddset (&signal_mask, SIGPIPE);

    int rc = pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);
    if (rc != 0)
    {
        printf("block sigpipe error\n");
    }
	
	SP_Server one;
	one.init();
	one.start();
#endif
//    string src = "【测试】这个是测试短信";.


    foo();
    return 0;
}



