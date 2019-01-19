// Author: Mohammed Ali
// Server side C Socket
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 9999

typedef struct Client_S {
    int number;
    char * name;           ///< name of the person.
    int server_fd;
    struct sockaddr_in address_p;
} Client;                         ///< NTree is an alias for the struct.
typedef struct Client_S * Client_P;

typedef struct Data1_S {
    int server_fd;
    struct sockaddr_in address_p;
} Data1;
typedef struct Data1_S * Data1_P;

Client_P array[10];
size_t array_size = 0;

Client_P create_client(char *name,int number,int server_fd,struct sockaddr_in *address_p){
    Client_P newClient = (Client_P) malloc(sizeof(struct Client_S));
    if (newClient != NULL){
        newClient -> name = name;
        newClient -> number = number;
        newClient->server_fd = server_fd;
        newClient->address_p = *address_p;
    }
    return newClient;
}


Data1_P create_data1(int *server_fd,struct sockaddr_in *address_p){
    Data1_P newData = (Data1_P) malloc(sizeof(struct Data1_S));
    if (newData != NULL){
        newData->server_fd = *server_fd;
        newData->address_p = *address_p;
    }
    return newData;
}


int setup(int *server_fd,struct sockaddr_in *address){
    int opt = 1;

    // Creating socket file descriptor
    if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(*server_fd, SOL_SOCKET, SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt");
        return 1;
    }
    (*address).sin_family = AF_INET;
    (*address).sin_addr.s_addr = INADDR_ANY;
    (*address).sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(*server_fd, (struct sockaddr *) &(*address), (sizeof(*address))) < 0) {
        perror("bind failed");
        return 1;
    }
    if (listen(*server_fd, 3) < 0) {
        perror("listen");
        return 1;
    }
    return 0;
}

void clients_alive() {
    char *buffer_to_be_sent = malloc(1024);

    for (int i = 0; i < array_size; ++i) {
        int alive = 1;
        Client_P client = array[i];
        int new_socket = client -> number;
        if (getsockname(client ->server_fd ,
                        (struct sockaddr *) &(client ->address_p) ,
                                ((socklen_t *) &(client ->address_p)) ) != 0){
            printf("Not found!");
        }

    }
    free(buffer_to_be_sent);
}

int find_client(char *buffer){
    int number = atoi(buffer);
    for (int i = 0; i < array_size; ++i) {
        if (array[i] ->number == number){
            return array[i] ->number;
        }
    }
    return -1;
}

void printALL(){
    //clients_alive();
    printf("-------");
    for (int i = 0; i < array_size; ++i) {
        printf("\n");
        if (array[i] != NULL){
            printf("PC name: %s\n",array[i] -> name);
            printf("PC num: %i\n",array[i] -> number);
            printf("-------");
        }
        printf("\n");
    }
}

void work();

int talk(int new_socket){
    ssize_t valread;

    while(1) {
        size_t  buffer_size = 1024;
        char *buffer_to_be_sent = calloc(buffer_size , sizeof(char));
        char buffer[1024];

        memset(buffer_to_be_sent,0,buffer_size);
        printf("> ");
        ssize_t length = getline(&buffer_to_be_sent, &buffer_size, stdin);
        printf("%s",buffer_to_be_sent);
        //length = strlen(buffer_to_be_sent);
        // Remove \n
        //buffer_to_be_sent[length-1] = '\0';

        if (buffer_to_be_sent[0] == '~'){
            work();
        }
        ////////////////////////////////////////////////////////////////////////////////////////////// SEND
        // buffer sends the length of the next buffer.
        memset(buffer, 0, 1024);
        snprintf(buffer, sizeof(buffer), "%zd", length);
        if (send(new_socket,buffer, (size_t) length, 0) < 0 ){
            break;
        }
        if (send(new_socket, buffer_to_be_sent,(size_t) length, 0) < 0 ){
            break;
        }
        ////////////////////////////////////////////////////////////////////////////////////////////// RECV
        // will use buffer_to_be_sent buffer to recive the length of the next buffer and for the next buffer.
        memset(buffer_to_be_sent,0,length);
        valread = recv(new_socket, buffer_to_be_sent, 1024,0);
        if (valread <= 0){
            printf("1 - [%i] \n", (int) valread);
            break;
        }
        int size_of_the_next_buffer = atoi(buffer_to_be_sent);
        printf("size_of_the_next_buffer string: %s int: %i \n",buffer_to_be_sent, size_of_the_next_buffer);

        if (size_of_the_next_buffer <= 0){
            printf("The coming buffer is 0\n");
        } else {
            memset(buffer_to_be_sent,0,buffer_size);
            valread = recv(new_socket, buffer_to_be_sent, (size_t) size_of_the_next_buffer,0);
            if (valread <= 0){
                printf("2 - [%i] \n", (int) valread);
                break;
            }
            printf("From client: %s\n",buffer_to_be_sent);
        }
        free(buffer_to_be_sent);
    }
    return 0;
}

void work() {

    while (1) {
        size_t  buffer_size = 1024;
        char *buffer = malloc(buffer_size * sizeof(char));
        memset(buffer, 0, buffer_size);
        printf("@main> \n");
        ssize_t length = getline(&buffer, &buffer_size, stdin);
        printf("%s",buffer);
        // Remove \n
        buffer[length - 1] = '\0';

        if (buffer[0] == '~'){
            continue;
        }

        if (strlen(buffer) <= 2) {
            int number = find_client(buffer);
            if (number != -1){
                talk(number);
            }

        }
        if (!strcmp(buffer, "users")) {
            printALL();
        } else if (!strcmp(buffer, "exit")) {
            exit(EXIT_SUCCESS);
        }
    }
}


void add_client(int client_number,int server_fd,struct sockaddr_in *address){
    char *buffer = malloc(1024 * sizeof(char));
    memset(buffer,0,1024);
    ssize_t valread = recv(client_number, buffer, 1024,0);
    if (valread <= 0){
        printf(" Error 001 [%i] \n", (int) valread);
        exit(-1);
    } else {
        printf("name recived %s \n",buffer);
    }
    array[array_size] = create_client(buffer,client_number,server_fd,address);
    array_size++;
}


void *handler_baby(void* void_data) {
    Data1_P data = (Data1_P) void_data;
    int server_fd = data->server_fd;
    struct sockaddr_in address = data->address_p;
    int new_socket;
    int addrlen = sizeof(address);
    printf("Start accepting..\n");
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("Client has connected: %i \n",new_socket);
        add_client(new_socket,server_fd,&address);
        //talk(new_socket);
    }
}

void thre(int *server_fd,struct sockaddr_in *address_p) {
    Data1_P data = create_data1(server_fd, address_p);
    void *retval;
    int rc;
    pthread_t thread_1;
    rc = pthread_create(&thread_1, NULL, handler_baby, (void *) data);
    if (rc) {
        printf("ERROR; pthread_create() returned %d\n", rc);
        exit(-1);
    }
    //pthread_join(thread_1, &retval);
}

int main(int argc,char *argv[]) {
    int server_fd;
    int new_socket;
    struct sockaddr_in address;
    ssize_t valread;
    if (setup(&server_fd,&address)) return -1;
    thre(&server_fd,&address);
    //while (1){}
    work();
    return 0;
}
