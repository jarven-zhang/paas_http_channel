#ifndef SP_SERVER_H
#define SP_SERVER_H
#include <iostream>

enum UP_TPYE
{
    SEND,
    REPORT,
    UPSms
};

class SP_Server
{
public:
    SP_Server();

    int sendReportInit();

    int init();

    int start();

    int justSendOneReport();

    int responseSms(int client_fd, const UP_TPYE type, const char *send_buf);

    static int sendReportAndUp(int socket);

    static void* sendReportThreadFun(void *arg);

    static int pushReportAndUpsms(int client_fd, const UP_TPYE type, const char *send_buf);

private:
    int server_fd; //服务端

    int report_fd; //连接客户端
};

#endif // SP_SERVER_H
