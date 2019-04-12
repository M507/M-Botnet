
# Time in mins
REFRESH_INTERVAL = 1

"""
Push updates to a file
"""
def refreshBotsStatus():
    f = open("refreshBotsStatus.txt","w")
    for bot in bots:
        state = "offline"
        if bot.getDifference() <= 5:
            state = "online"
        f.write(bot.getip()+" at "+bot.getPort()+" is "+state+"\n")
