from flask import Flask
app = Flask(__name__)

# Disabled debug mode
import logging
log = logging.getLogger('werkzeug')
log.disabled = True

@app.route("/")
def runDashboard():
    try:
        with open('refreshBotsStatus.txt', 'r') as myfile:
            whatIwant2append = myfile.read()
    except:
        whatIwant2append = ""
    s = "<!doctype html>"\
        "<html>"\
          "<head>"\
            "<title>Title!</title>"\
          "</head>"\
          "<body>" + whatIwant2append + \
          "<script type='text/javascript'>"\
          "setTimeout(function () { location.reload(true); }, 5000);"\
        "</script>"\
          "</body>"\
        "</html>"
    return s

def startDashboard():
    app.run()
