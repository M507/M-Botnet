
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
