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

class Socket{
private:
    string web = "";
    string protocol = "";
    string hostname = "";
    string path = "";

    uint32_t port;
    addrinfo* host = nullptr;

    char buffer[BUFFERSIZE] = {0};
    struct sockaddr_in addr; //address 
    char serv[NI_MAXSERV] = "";
    
    string request = "";
    string order = "";
    string response;
    
    int resp_len = BUFFERSIZE;
    int mysocket = 0;
    int on = 0;
    
public: 
    Socket(string url ,string port){
        this->web = url;
        char temp[BUFFERSIZE];
        this->protocol =    url.substr(0, url.find("://"));  //protocol
        url.erase(0, this->protocol.length() + 3);
        this->hostname =    url.substr(0, url.find("/"));//host name
        url.erase(0, this->hostname.length() + 1);
        this->path     =    url.substr(0, url.size());   //path
        // transform(protocol.begin(), protocol.end(), protocol.begin(), ::toupper);

        //get ip address from URL domain
        addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM; 
        //
        if(getaddrinfo(this->hostname.c_str(), port.c_str(), &hints, &host ) != 0)
            perror("[SOCKET] : HOST IS WRONG");
        if(getnameinfo(this->host->ai_addr, this->host->ai_addrlen, NULL, 0, this->serv, NI_MAXSERV, 0) != 0)
            perror("[SOCKET] : HOST IS WRONG");
        int i = 0;
    
        //
        cout << "URL NAME   : " << this->web <<endl;    
        for(addrinfo* tmp = this->host; tmp != NULL; tmp = tmp->ai_next)
            cout << "IP ADDRESS : " << inet_ntoa( ((struct sockaddr_in *)tmp->ai_addr)->sin_addr ) <<endl;
        cout << "IP PORT    : " << ((struct sockaddr_in *)host->ai_addr)->sin_port <<endl;
        cout << "IP FAMILY  : " << (int)host->ai_family <<endl;
        cout << "IP TYPE    : " << host->ai_socktype <<endl;
    }
    ~Socket(){
        memset(buffer, 0, BUFFERSIZE);
        if(host != nullptr)
            freeaddrinfo(host);
    }
    void msg(){
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

    bool run(){
        //creating socket
        mysocket = socket(PF_INET, this->host->ai_socktype, IPPROTO_TCP);
        if(mysocket < 0){  //domain, type, protocol txp socket
            perror("[SOCKET] : SOCKET ");
            return false;
        } 
        //set detail of socket
        setsockopt(mysocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
        if(mysocket < 0){
            perror("[SOCKET] : SOCKET ");
            return false;
        }
        //set server
        memset(&addr, 0, sizeof(addr));
        addr.sin_family              = ((struct sockaddr_in *)this->host->ai_addr)->sin_family;     
        addr.sin_port                = ((struct sockaddr_in *)this->host->ai_addr)->sin_port;          
        addr.sin_addr.s_addr         = ((struct sockaddr_in *)this->host->ai_addr)->sin_addr.s_addr;
        memset(&addr.sin_zero, 0, 8);
        //connecting to website
        if(connect(mysocket, (sockaddr *)&addr, sizeof(sockaddr)) < 0){
            perror("[SOCKET] : CONNECT ");
            return false;
        } 
        //send request message
        if(send(mysocket, request.c_str(), request.length(), (int)0) != request.length()){
            perror("[SOCKET] : SEND ");
            return false;
        }  
        //get response from website
        while(resp_len > 0){
            resp_len = recv(mysocket, (char*)&buffer, BUFFERSIZE, 0);
            response += string(buffer).substr(0, resp_len);
        }
        cout << response <<endl;
        close(mysocket);
        return true;
    }
    
    friend istream& operator >> (istream& cin, Socket& A);
};

istream& operator >> (istream& cin, Socket& A){
    string temp;
    cin >> temp;
    A.order += temp;
    return cin;
}


int main(){
    Socket test("http://kostat.go.kr", "80");
    
    test.msg();
    test.run();
    return 0;
}