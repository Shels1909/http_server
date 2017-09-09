#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define BUFFSIZE 32

int main(int argc, char** argv){

    int mysocket, conn, clilen, n; 
    struct sockaddr_in serv_addr, client_addr;
    char buffer[BUFFSIZE];
    pid_t pid;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3000);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    mysocket = socket(AF_INET, SOCK_STREAM, 0);

    bind(mysocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    listen(mysocket, 5);

    clilen = sizeof(client_addr);

    for(;;){
        conn = accept(mysocket, (struct sockaddr*) &client_addr, &clilen);

        if(conn < 0){
            printf("ERROR ACCEPTING CONNECTION");
        }
        else{
            printf("%d\n", conn);
            printf("CONNTECTION ACCEPTED\n");
        }

        if((pid == fork()) == 0){


            close(mysocket);
            bzero(buffer, BUFFSIZE);
            n = read(conn, buffer, BUFFSIZE);
            printf("buffer:%s\n", buffer);

            n = send(conn,"I got your message\n",18, 0);

            exit(0);
        }

        close(conn);
    }

        
    /*#return 0;*/

}

