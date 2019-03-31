touch /tmp/index.html
PWD2=$(pwd)
cd /tmp
wget https://eternallybored.org/misc/wget/1.20/32/wget.exe
cp $PWD2/Scripts/DP.ps1 .
cp $PWD2/Scripts/1.ps1 .
cp $PWD2/Scripts/getIPs.py .
cp $PWD2/Scripts/scan.py .
python -m SimpleHTTPServer 80
