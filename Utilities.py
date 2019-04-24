
import subprocess
import requests

"""
Stylish input()
"""
def s_input(string):
    return input("["+string+"]>").strip("\n")


"""
Execute command locally
"""
def execute_command(command):
    if len(command) > 0:
        print(command)
        proc = subprocess.Popen(command.split(" "), stdout=subprocess.PIPE, cwd="/tmp")
        return proc

"""
Get the host's IP
"""
def getIP():
    ip = requests.session().get("http://httpbin.org/ip").text.split(":")[1].split('"')[1]
    print(ip)
    return
    pass



"""
Basic python3 function that sends an update to the pwnboard
"""

def sendUpdate(ip, name="python"):
    host = "http://logs.pwnboard.win:8080/generic"
    data = {'ip': ip, 'type': name}
    try:
        req = requests.post(host, json=data, timeout=3)
        print(req.text)
        return True
    except Exception as E:
        print(E)
        return False



"""
Send json to reach
"""


