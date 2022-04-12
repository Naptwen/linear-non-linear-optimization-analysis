
#ifndef VSOCKET_H__
#define VSOCKET_H__

#include <iostream>
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <atomic>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sstream>

#include <sys/types.h>      //for socket type
#include <sys/types.h>      //connection type TCP or UDP
#include <netinet/in.h>     //for internet 
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>         //for close


using namespace std;
#define BUFFERSIZE 1024
//reference from http://www.zedwood.com/article/cpp-winsock-basic-http-connection
//reference from https://www.linuxhowtos.org/C_C++/socket.htm
//reference from https://www.cplusplus.com/forum/windows/109180/
//reference from https://www.delftstack.com/ko/howto/c/getaddrinfo-in-c/

typedef struct socket{
    string port = "";
    string ip_addr = "";
    string request = "";
    string response = "";
}SOCKET_INFO;

class Socket{
protected:
    struct sockaddr_in addr;
    char buffer[BUFFERSIZE] = {0};
    int resp_len = BUFFERSIZE;
    int mysocket = 0;
    int mysocket_type = 0;
    int myseockt_set = 0;

    string web = "";
    string protocol = "";
    string hostname = "";
    string path = "";

    string port = "";
    string ip_addr = "";
public:
    string request = "";
    string response = "";

    Socket(SOCKET_INFO A){
        this->port = A.port;
        this->ip_addr = A.ip_addr;
        this->request = A.request;
        this->response = A.response;
    }
    ~Socket(){}
    virtual void setting() = 0;
    virtual void run() = 0;
    friend istream& operator >> (istream& cs, Socket& ss){
        string temp;
        cs >> temp;
        ss.request += temp;
        return cs;
    }
};


template<int type, int type2>
class ServerSocket : public Socket{
public:
    ServerSocket(SOCKET_INFO info):Socket(info){
        cout << "SERVER CREATED" <<endl;
    }
    ~ServerSocket(){}
    void setting(){
        cout << "SERVER SETTING" << endl;
        this->port = port;
        this->ip_addr = ip_addr;
        //creating socket
        mysocket = socket(type, SOCK_STREAM, IPPROTO_TCP);
        if(mysocket < 0){  //domain, type, protocol txp socket
            perror("[SOCKET] : SOCKET ");
            exit(-1);
        } 
        //set detail of socket
        setsockopt(mysocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&myseockt_set, sizeof(int));
        if(mysocket < 0){
            perror("[SOCKET] : SOCKET ");
            exit(-1);
        }
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = type2;
        addr.sin_port   = htons(stoi(this->port));
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        memset(&addr.sin_zero, 0, 8);

        if(::bind(mysocket, (sockaddr *)&addr, sizeof(sockaddr)) < 0){
            perror("[SOCKET] : BIND ");
            exit(-1);
        } 
    }
    void run(){
        unsigned int client_len;
        int client_sock;
        struct sockaddr_in clientaddr;
        clock_t s = clock();
        clock_t f = clock();
        while(1){
            listen(mysocket, 5);
            client_len = sizeof(clientaddr);
            if((client_sock = accept(mysocket, (sockaddr *)&addr, &client_len)) < 0){
                perror("[SOCKET] SERVER ACCEPT : ");
                close(client_sock);
            }
            send(client_sock, "HELLO WORLD\n", 13, 0);
            memset(&buffer, 0, BUFFERSIZE);
            if(read(client_sock, buffer, BUFFERSIZE) < 0){
                perror("[SOCKET] SERVER READ : ");
                close(client_sock);
            }
            break;
        }
        
    }
};

class WebSocket : public Socket{
public:
    WebSocket(SOCKET_INFO info, string url):Socket(info){
        this->web = url;
        cout << "WEBSOCKET CREATED\n" <<endl;
    }
    ~WebSocket(){}
    void web_setting(){
        char temp[BUFFERSIZE];
        this->protocol =    web.substr(0, web.find("://"));  //protocol
        web.erase(0, this->protocol.length() + 3);
        this->hostname =    web.substr(0, web.find("/"));//host name
        web.erase(0, this->hostname.length() + 1);
        this->path     =    web.substr(0, web.size());   //path
        //get ip address from URL domain
        addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM; 
        //
        addrinfo* host = nullptr;
        addrinfo* tmp = nullptr;
        if(getaddrinfo(this->hostname.c_str(), this->port.c_str(), &hints, &host ) != 0)
            perror("[SOCKET] : HOST IS WRONG");
        if(getnameinfo(host->ai_addr, host->ai_addrlen, NULL, 0, NULL, 0, 0) != 0)
            perror("[SOCKET] : HOST IS WRONG");
        int i = 0;
        //
        cout << "URL NAME   : " << this->web <<endl;    
        for(tmp = host; tmp != NULL; tmp = tmp->ai_next)
            cout << "IP ADDRESS : " << inet_ntoa( ((struct sockaddr_in *)tmp->ai_addr)->sin_addr ) <<endl;
        cout << "IP PORT    : " << ((struct sockaddr_in *)host->ai_addr)->sin_port <<endl;
        cout << "IP FAMILY  : " << (int)host->ai_family <<endl;
        cout << "IP TYPE    : " << host->ai_socktype <<endl;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = ((struct sockaddr_in *)host->ai_addr)->sin_family;
        addr.sin_len    = ((struct sockaddr_in *)host->ai_addr)->sin_len;
        addr.sin_port   = ((struct sockaddr_in *)host->ai_addr)->sin_port;
        addr.sin_addr   = ((struct sockaddr_in *)host->ai_addr)->sin_addr;
        mysocket_type = host->ai_socktype;
        memset(&addr.sin_zero, 0, 8);
        freeaddrinfo(host);
        freeaddrinfo(tmp);
    }
    void setting(){
        web_setting();
        //creating socket
        mysocket = socket(PF_INET, mysocket_type, IPPROTO_TCP);
        if(mysocket < 0){  //domain, type, protocol txp socket
            perror("[SOCKET] : SOCKET ");
            exit(-1);
        } 
        //set detail of socket
        setsockopt(mysocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&myseockt_set, sizeof(int));
        if(mysocket < 0){
            perror("[SOCKET] : SOCKET ");
            exit(-1);
        }
        cout << "---REQUEST MESSAGㄸ---" <<endl;
        this->request.clear();
        this->request += "GET /" + this->path + " HTTP/1.1\r\n";
        this->request += "HOST: " + this->hostname + "\r\n";
        this->request += "Accept: text/html, application/xhtml+xml, */*\r\n";
        this->request += "Accept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7\r\n";
        this->request += "Connection: keep-alive\r\n";
        this->request += "Keep-Alive: 115\r\n\r\n";
        cout << this->request <<endl;
        
    }
    void run(){
        //connecting to website
        if(connect(mysocket, (sockaddr *)&addr, sizeof(sockaddr)) < 0){
            perror("[SOCKET] : CONNECT ");
            exit(-1);
        } 
        //send request message
        if(send(mysocket, request.c_str(), request.length(), (int)0) != request.length()){
            perror("[SOCKET] : SEND ");
            exit(-1);
        }  
        //get response from website
        while(resp_len > 0){
            resp_len = recv(mysocket, (char*)&buffer, BUFFERSIZE, 0);
            response += string(buffer).substr(0, resp_len);
        }
        cout << response <<endl;
        close(mysocket);
        exit(-1);
    }
};
template<int type>
class ClientSocket : public Socket{
public:
    ClientSocket(SOCKET_INFO info):Socket(info){
        cout << "CLIENT CREATED\n" <<endl;
    }
    ~ClientSocket(){}
    void setting(){
        //creating socket
        mysocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(mysocket < 0){  //domain, type, protocol txp socket
            perror("[SOCKET] : SOCKET ");
            exit(-1);
        } 
        //set detail of socket
        setsockopt(mysocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&myseockt_set, sizeof(int));
        if(mysocket < 0){
            perror("[SOCKET] : SOCKET ");
            exit(-1);
        }
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = type;
        addr.sin_port   = htons(stoi(this->port));
        inet_pton(type, this->ip_addr.c_str(), &(addr.sin_addr));
        memset(&addr.sin_zero, 0, 8);
        
    }
    void run(){
        //connecting to website
        if(connect(mysocket, (sockaddr *)&addr, sizeof(sockaddr)) < 0){
            perror("[SOCKET] : CONNECT ");
            exit(-1);
        } 
        //send request message
        if(send(mysocket, request.c_str(), request.length(), (int)0) != request.length()){
            perror("[SOCKET] : SEND ");
            exit(-1);
        }  
        //get response from website
        while(resp_len > 0){
            resp_len = recv(mysocket, (char*)&buffer, BUFFERSIZE, 0);
            response += string(buffer).substr(0, resp_len);
        }
        cout << response <<endl;
        close(mysocket);
        
    }
};

#endif
