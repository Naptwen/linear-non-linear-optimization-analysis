#include "vsocket.h"

int main(){
    SOCKET_INFO test = {"8080",  "127.0.01", "", ""};
    Socket* B = new ServerSocket<AF_INET>(test);
    B->setting();
    B->run();
    return 0;
}