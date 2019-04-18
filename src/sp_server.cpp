#include "sp_server.h"
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdexcept>
#include <algorithm>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "define.h"

using namespace  std;


SP_Server::SP_Server():server_fd(-1)
{
}

int SP_Server::init()
{
    struct sockaddr_in server_addr;

    /* 创建socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        cout << "socket error!" << endl;
        exit(-1);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    /* 设置端口，IP，和TCP/IP协议族 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(HTTP_PORT) ;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;

    /*绑定*/
    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr))  < 0)
    {
        cout << "bind error!" << endl;
        close(server_fd);
        server_fd = -1;
        exit(-1) ;
    }

    /* 监听*/
    if (listen(server_fd, 5) < 0)
    {
        cout << "listen error!" << endl;
        close(server_fd);
        server_fd = -1;
        exit(-1) ;
    }

    cout << "http server running on port :" << HTTP_PORT << endl;

    return 0;
}


int SP_Server::responseSms(int client_fd, const UP_TPYE type, const char *send_buf)
{
    string tmpHead = "";
    string headhost = HTTP_HEAD_TYPE;
    if (SEND == type)
    {
        cout << ">>>>>>>>>>>>>>>>>>>>>>> respone to client >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
    }
    else if (REPORT == type)
    {
        cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>> Report >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
    }
    else if (UPSms == type)
    {
        cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>> UP SMS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
    }
    else
    {
        cout << "Bad type!" << endl;
        exit(-1) ;
    }

    int lenth = strlen(send_buf);

    char tmpArray[1024];
    sprintf(tmpArray, "%d", lenth);
    string tmpContent(tmpArray);
    string contentLenth = HTTP_CONENT_LENTH + tmpContent + "\r\n";

    tmpHead += HTTP_HEAD_REPN_STATE;
    send(client_fd, tmpHead.c_str(), tmpHead.length(), 0);
    send(client_fd, contentLenth.c_str(), contentLenth.length(),0);
    send(client_fd, headhost.c_str(), headhost.length(), 0);
    send(client_fd, "\r\n", strlen("\r\n"), 0);

    send(client_fd, send_buf, lenth, 0);

    cout << tmpHead + contentLenth + headhost + "\r\n" + send_buf << endl;
    cout << "--------------------------- over ----------------------------------" << endl << endl << endl;

    return 0;
}

int SP_Server::pushReportAndUpsms(int client_fd, const UP_TPYE type, const char *send_buf)
{
    cout << endl;
    string tmpHead("POST ");
    if (SEND == type)
    {
        cout << "Bad type: SEND!" << endl;
        exit(-1) ;
    }
    else if (REPORT == type)
    {
        tmpHead += LIB_NAME;
        tmpHead += UP_REPORT_TYPE;
        cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>> Report >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
    }
    else if (UPSms == type)
    {
        tmpHead += LIB_NAME;
        tmpHead += UP_REPORT_TYPE;
        cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>> UP SMS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
    }
    else
    {
        cout << "Bad type!" << endl;
        exit(-1) ;
    }

    int lenth = strlen(send_buf);

    char tmpArray[1024];
    sprintf(tmpArray, "%d", lenth);
    string tmpContent(tmpArray);
    string contentLenth = HTTP_CONENT_LENTH + tmpContent + "\r\n";

    string headhost = HTTP_HEAD_TYPE;
    tmpHead += HTTP_HEAD_REQ_STATE;

    send(client_fd, tmpHead.c_str(), tmpHead.length(), 0);
    send(client_fd, contentLenth.c_str(), contentLenth.length(),0);
    send(client_fd, headhost.c_str(), headhost.length(), 0);
    send(client_fd, "\r\n", strlen("\r\n"), 0);

    send(client_fd, send_buf, lenth, 0);

    cout << tmpHead + contentLenth + headhost + "\r\n" + send_buf << endl;
    cout << "--------------------------- over ----------------------------------" << endl << endl << endl;

    return 0;
}

int SP_Server::sendReportInit()
{
    int sockfd = -1;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd)
    {
        perror("sock created");
        exit(-1);
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family      = AF_INET;
    server.sin_port        = htons(SERVER_PORT) ;
    server.sin_addr.s_addr = inet_addr(SERVER_IP) ;

    int res = -1;
    res = connect(sockfd, (struct sockaddr*)&server, sizeof( sockaddr ));
    if(-1 == res)
    {
        perror("sock connect");
        exit(-1) ;
    }

    return sockfd;
}

int SP_Server::start()
{
    int client_fd = -1;
    struct sockaddr_in client_addr;

    socklen_t client_addr_len = sizeof(client_addr) ;

    char recv_buf[BUFF_SIZE];
    memset(recv_buf, 0, BUFF_SIZE);

    char *send_buf   = NULL;
    bool send_report = false;
    bool send_up     = false;

    while (1)
    {
        /* 调用了accept函数，阻塞了程序，直到接收到客户端的请求 */
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len) ;
        if (client_fd < 0)
        {
            cout << "accept error!";
            continue;
        }

        // string client_address = inet_ntoa(client_addr.sin_addr);

        if (0 >= recv(client_fd, recv_buf, BUFF_SIZE, 0))
        {
            cout << "receive error! Maybe the connect is off!";
            close(client_fd);
            continue;
        }

        cout << "<<<<<<<<<<<<<<<<<<<<<<<    recv_buf    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl << recv_buf << endl;
        cout << "----------------------------------------------------------------------" << endl;

        string recv_src(recv_buf);
        string send_src(XHR_Respone);

        if (std::string::npos != recv_src.find("report"))
        {
            if(send_report)
            {
                responseSms(client_fd, REPORT, XHR_REPORT.c_str());
                send_report = false;
            }
        }
        else if(std::string::npos != recv_src.find("upUP"))
        {
            if(send_up)
            {
                responseSms(client_fd, UPSms, XHR_UP.c_str());
                send_up = false;
            }
        }
        else
        {
            send_up = true;
            send_report = true;
            //短信回复
            responseSms(client_fd, SEND, send_src.c_str());
        }

        //状态报告和上行短信
//        sleep(1);

        // sendReportAndUp();
//        justSendOneReport();

        memset(recv_buf, 0, BUFF_SIZE);
        free(send_buf);
        send_buf = NULL;
    }

    return 0;
}

int SP_Server::justSendOneReport()
{
    report_fd = sendReportInit();
    pushReportAndUpsms(report_fd, REPORT, TX_REPORT.c_str());
    sleep(1);

    report_fd = sendReportInit();
    pushReportAndUpsms(report_fd, UPSms, TX_UP.c_str());

    return 0;
}

void* SP_Server::sendReportThreadFun(void *arg)
{
    int fd = *(int *)(arg);

    while (1)
    {
#if 1
        pushReportAndUpsms(fd, REPORT, TX_REPORT.c_str() );
        sleep(1);
        pushReportAndUpsms(fd, UPSms, TX_UP.c_str() );
        sleep(10);
#else
        cout << "this is report!" << endl;
        sleep(1);
        cout << "this is up sms!" << endl;
        sleep(100);
#endif
    }

    return NULL;
}

int SP_Server::sendReportAndUp(int server_fd)
{
    pthread_t t;

    pthread_create(&t, NULL, sendReportThreadFun, &server_fd);

    pthread_detach(t);

    return 0;
}
