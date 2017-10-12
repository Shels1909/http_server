#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/wait.h>
#include "http_server.h"
#define BUFFSIZE 32
#define MAX_COMMAND_SIZE 5
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
            read(conn, buffer, BUFFSIZE);
            printf("buffer:%s\n", buffer);
            process_command(buffer);

            send(conn,"I got your message\n",18, 0);

            close(conn);
            exit(0);
        }
        wait(0);
        close(conn);
    }
    return 0;
}

void process_command(char* buffer){
    const char* valid_commands[] = {"GET", "POST", "PUT", "DELETE", NULL};
    char* p1 = buffer; 
    char* p2 = buffer;
    char command[MAX_COMMAND_SIZE];
    int cnt = 0;
    
    while(p1 != NULL){
        if( *(p1) == ' '){
           *(p1) = '\0';
            p1 ++;
            strcpy(command, p2); 
            p2 = p1;
        }
        else{
            p1++;
        }
    }
    
    while(valid_commands[cnt] != NULL){
        if(strcmp(command, valid_commands[cnt]) == 0){
            printf("valid command"); 
            break;
        }
        cnt++;
    }
    
}
