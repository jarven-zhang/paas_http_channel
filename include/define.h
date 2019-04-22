#ifndef DEFINE_H
#define DEFINE_H

#include <iostream>

#define HTTP_PORT 8888
#define SERVER_IP "172.16.5.51"
#define SERVER_PORT 19527

#define BUFF_SIZE 1024
#define SMALL_BUFF_SIZE 64
#define QUEUE_MAX_COUNT 5

#define HTTP_HEAD_REPN_STATE "HTTP/1.1 200 OK\r\n"
#define LIB_NAME "/XHR"
#define UP_REPORT_TYPE "report.do "
#define UP_MO_TYPE "getmo.do "
#define HTTP_HEAD_REQ_STATE  " HTTP/1.1\r\n"

#define HTTP_CONENT_LENTH "Content-Length: "
#define HTTP_HEAD_TYPE "Content-Type: text/plain; charset=UTF-8\r\n"

const std::string TX_RESPONE ="result=0&balance=1249&linkid=AC082549E06C45B2BA4416DA652988DA&description=%b7%a2%cb%cd%b6%cc%d0%c5%b3%c9%b9%a6";
const std::string TX_REPORT = "report=13456565656,DELIVRD,2019-04-19 11:36,AC082549E06C45B2BA4416DA652988DA;"
           "13456565656,DELIVRD,2029-04-19 12:16,AC082549E06C45B2BA4416DA652988DA;"
           "13456565656,DELIVRD,2029-04-19 13:45,AC082549E06C45B2BA4416DA652988DA;"
           "13456565656,DELIVRD,2029-04-19 14:32,AC082549E06C45B2BA4416DA652988DA";
const std::string TX_UP = "ssss";

const std::string XHR_RESPONE = "041517332783988";//msgid

const std::string XHR_UP =  "<MO>"
        "<MSG mobile=\"8322820698\" spnumber=\"19853020618\" message=\"%e6%94%b6%e5%88%b0\" id=\"214561\" cc=\"1\" time=\"2019-04-19 11:29:55\" codec=\"utf-8\" />"
        "</MO>";

//041710093728136
const std::string XHR_REPORT = "<DR>"
        "<MSG mobile = \"8322820777\""
        "status = \"DELIVRD\""
        "msgid = \"041517332783988\""
        "submit_date = \"1904191733\""
        "done_date = \"1904151733\" />"
        "</DR>";

//const std::string PULL_REPORT = "=============== Pull report! =======================";

//const std::string PULL_UP_SMS = "+++++++++++++++= Pull mo ! =++++++++++++++++++++++";

#endif // DEFINE_H
