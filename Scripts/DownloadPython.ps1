# TODO
# Add x84 and x64 options

# Setup
$save_dir = $env:temp
#$save_path = '' + $save_dir + '\' + $filename;


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


$wgetBinLocation = get_wget

1..1 | % {
# Downlaod
$filename = "$_" + ".msi"
$Location = '' + $save_dir + '\' + $filename;
Start-Process -FilePath $wgetBinLocation -Args " https://www.python.org/ftp/python/2.7/python-2.7.msi -O $Location" -passthru -N
oNewWindow -Wait
# Install
Start-Process -FilePath "msiexec.exe"  -Args "/a $Location /qb TARGETDIR=C:\python27" -passthru -NoNewWindow -Wait
}



#$filename = "wget.exe"
#$Location = '' + $save_dir + '\' + $filename;
#Start-Process -FilePath $wgetBinLocation  -Args " https://www.python.org/ftp/python/2.7/python-2.7.msi -O C:\Windows\Temp\2.msi
" -passthru -NoNewWindow -Wait -WindowStyle hidden
#Start-Process -FilePath $wgetBinLocation  -Args " https://www.python.org/ftp/python/2.7/python-2.7.msi -O C:\Windows\Temp\3.msi
" -passthru -NoNewWindow -Wait -WindowStyle hidden

#Start-Process -FilePath "msiexec.exe"  -Args "/a $Location /qb TARGETDIR=C:\python27" -passthru -NoNewWindow -Wait
#Start-Process -FilePath "msiexec.exe"  -Args "/a C:\Windows\Temp\2.msi /qb TARGETDIR=C:\python27" -passthru -NoNewWindow -Wait
#Start-Process -FilePath "msiexec.exe"  -Args "/a C:\Windows\Temp\3.msi /qb TARGETDIR=C:\python27" -passthru -NoNewWindow -Wait

#iex "msiexec /a C:\Windows\Temp\1.msi /qb TARGETDIR=C:\python27"
