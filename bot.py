import datetime

class Bot:
    def __init__(self, conn,addr):
        self.connectToMe = 1
        self.conn = conn
        self.addr = addr
        self.port = conn.getpeername()[1]
        self.ip = conn.getpeername()[0]
        self.firstTimeOnline = datetime.datetime.now().strftime("%I%M%S")
        self.lastTimeOnline = datetime.datetime.now().strftime("%I%M%S")
        #port = conn.getpeername()[1]

    def getip(self):
        return self.ip

    def getConn(self):
        return self.conn

    def getAddr(self):
        return self.addr

    def getPort(self):
        return self.port

    def getDifference(self):
        return int(datetime.datetime.now().strftime("%I%M%S")) - int(self.lastTimeOnline)

    def restartTimer(self):
        self.lastTimeOnline = datetime.datetime.now().strftime("%I%M%S")

    def getConnectToME(self):
        return self.connectToMe

    def setConnectToME(self,value):
        self.connectToMe = value
