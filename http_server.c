#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "http_server.h"
#define BUFFSIZE 512
#define MAX_COMMAND_SIZE 5
int main(int argc, char** argv){

    int mysocket, conn, clilen, n; 
    struct sockaddr_in serv_addr, client_addr;
    char buffer[BUFFSIZE];
    char response[BUFFSIZE];
    char* buf_p;
    pid_t pid;

    signal(SIGCHLD, sig_child_handler);


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
            buf_p = buffer;
            buf_p = process_command(buf_p);
            printf("buffer after process_command: %s\n", buf_p);
            sprintf(response, "HTTP/1.1 200 OK\r\n");
            send(conn, response, strlen(response), 0);
            close(conn);
            exit(0);
        }
        close(conn);
    }
    return 0;
}

char* process_command(char* buffer){
    const char* valid_commands[] = {"GET", "POST", "PUT", "DELETE", NULL};
    char* p1 = buffer; 
    char command[MAX_COMMAND_SIZE];
    int cnt = 0;
    int done = 0;
    int valid = 0;
    
    while(p1 != NULL && !done){
        if( *(p1) == ' '){
            *(p1) = '\0';
            p1  = p1 + 1;
            strcpy(command, buffer); 
            buffer = p1;
            printf("buffer: %s\n", buffer);
            done = 1;
        }
        else{
            p1 = p1 + 1;
        }
    }
    
    while(valid_commands[cnt] != NULL && !valid){
        if(strcmp(command, valid_commands[cnt]) == 0){
            valid = 1;
        }
        cnt++;
    }
    if(!valid){
        perror("invalid command");
    } 
    else{
        return p1;
    } 
}


static void sig_child_handler(int sig){
    while(waitpid((pid_t)(-1), 0, WNOHANG) > 0){} 
}
