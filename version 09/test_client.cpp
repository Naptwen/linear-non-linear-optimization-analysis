#include "vsocket.h"

int main(){
    SOCKET_INFO test = {"8080",  "127.0.01", "", ""};
    Socket* A = new ClientSocket<AF_INET>(test);
    A->setting();
    A->run();
    return 0;
}