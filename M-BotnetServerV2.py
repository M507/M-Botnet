"""
Initial commit, I am still working on it. 
It works fine, but I need to add more functionaries.
"""
from socket import *
import sys
import signal
import threading
print_lock = threading.Lock()
bots = []
#print_lock.acquire()

"""
Receive an output
"""
def w8ing(socket,msg_length):
    sentence = socket.recv(msg_length).decode('UTF-8')
    if len(sentence) <= 0:
        print("Client disconnected.\n")
        socket.close()
        return 0
    # Sometimes the server sends crappy buffers so fix it!
    # ideas: memset()?, lengths,
    try:
        msg_length = int(sentence)
    except:
        msg_length = msg_length
    print("msg_length: ",msg_length," ",sentence)
    sentence = socket.recv(msg_length).decode('UTF-8')
    print(sentence)
    return sentence


"""
Send a command.
"""
def send_something(socket,command):
    command_length = len(command)
    socket.send(bytes(str(command_length), 'UTF-8'))
    socket.send(bytes(command, 'UTF-8'))

"""
Handle SIGINT
"""
def signal_handler_noConn(signal, frame):
    print("\nSignal received, shutting down the server.")
    sys.exit(1)

def signal_handler_Conn(connectionSocket,signal, franme):
    print("\nSignal received, closing connection and shutting down server.")
    connectionSocket.close()
    sys.exit(1)

"""
Interaction implementation
"""
def Interaction(socket, msg_length,name):
    #Handle SIGINT if it occurs while in chat
    #signal.signal(signal.SIGINT,signal_handler_Conn)
    #Stay in Interaction until '\quit' is typed by server or client
    print("Send a SIGINT to shut down.")
    while 1:
        command = ""
        command = input(str(name)+"> ")
        # Switch to another client
        if(command == "back"):
            break
        send_something(socket,command)
        w8ing(socket,msg_length)
    return

def handler(serverSocket,INITIAL_MAX):
    pass

def find_client(name,INITIAL_MAX):
    # Debugging
    print("Debug")
    print("list's length is ", end="")
    print(len(bots))
    print(bots)
    if len(name) > 0:
        print("Loading ...")
        for client in bots:
            if client[0] == name:
                Interaction(client[1],INITIAL_MAX,client[0])
                return
        print("Not found")

"""
The structure is [name,connectionSocket, addr]
"""
def command_line_state(INITIAL_MAX):
    print("command_line_state")
    print(bots)
    command_line = ""
    while 1:
        client = []
        while 1:
            # try:
            print("")
            command_line = input("MB>").strip()
            print(command_line+"\n")
            if command_line == "help" or command_line == "Help":
                print(switch(1,""))
                break
            if command_line == "r" or command_line == "R":
                break
            find_client(command_line,INITIAL_MAX)

            # #command_line = switch(0,command_line)
            # if command_line == "1":
            #     bots_tmp[0] = client
            #     Interaction(client[1],INITIAL_MAX)
            # except:
            #     print_error(2,"For real?")

"""
For debugging
"""
def print_error(Error_number,message):
    if len(message) > 0:
        print(message)
    print("Error: #",Error_number)

def switch(help_me,argument):
    switcher = {
        1: "list",
    }
    if help_me:
        for i in switcher:
            print(i)
    else:
        print(switcher.get(argument, "Invalid month"))

def main():
    # Static values:
    global bots
    INITIAL_MAX = 4094
    PORT = 6060
    print("Port set as", PORT)

    #Create connection (Referenced lecture 15):
    serverSocket = socket(AF_INET, SOCK_STREAM)
    serverSocket.bind(('',PORT))
    serverSocket.listen(1)

    # Set up connectionless signal handler
    signal.signal(signal.SIGINT,signal_handler_noConn)

    print("Ready to receive.\n")

    # console
    A_Thread = threading.Thread(target=command_line_state, args=(INITIAL_MAX,))
    A_Thread.start()
    # dont need it for now
    #A_Thread.join()
    while 1:
        # try:
        #print("")
        # W8ing connections.
        connectionSocket, addr = serverSocket.accept()
        # Handle SIGINT if it occurs while in Interaction
        signal.signal(signal.SIGINT,signal_handler_Conn)
        print("A Bot link established. ")
        # Just get the first name for the bot.
        name = w8ing(connectionSocket,INITIAL_MAX)
        print("My name is ",name)
        print('Connected to :', addr[0], ':', addr[1])
        # Save the address and the connection socket.
        bots.append([name[:-1],connectionSocket, addr])
        # except:
        #     print_error(1,"Go learn Python, your handler sucks!")
        #     print()
        #     print("Error: #1")


if __name__ == '__main__':
    main()
