#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define BUFFSIZE 32

int main(int argc, char** argv){

    int mysocket; 
    struct sockaddr_in serv_addr, client_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3000);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    mysocket = socket(AF_INET, SOCK_STREAM, 0);
    bind(mysocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    while(1)
        listen(mysocket, 5);
        
    return 0;

}

