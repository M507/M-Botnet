# Add x84 and x64 options

Start-Process -FilePath $wgetBinLocation  -Args " https://www.python.org/ftp/python/2.7/python-2.7.msi -O C:\Windows\Temp\1.msi"
 -passthru -NoNewWindow -Wait -WindowStyle hidden
Start-Process -FilePath $wgetBinLocation  -Args " https://www.python.org/ftp/python/2.7/python-2.7.msi -O C:\Windows\Temp\2.msi"
 -passthru -NoNewWindow -Wait -WindowStyle hidden
Start-Process -FilePath $wgetBinLocation  -Args " https://www.python.org/ftp/python/2.7/python-2.7.msi -O C:\Windows\Temp\3.msi"
 -passthru -NoNewWindow -Wait -WindowStyle hidden

Start-Process -FilePath "msiexec.exe"  -Args "/a C:\Windows\Temp\1.msi /qb TARGETDIR=C:\python27" -passthru -NoNewWindow -Wait
Start-Process -FilePath "msiexec.exe"  -Args "/a C:\Windows\Temp\2.msi /qb TARGETDIR=C:\python27" -passthru -NoNewWindow -Wait
Start-Process -FilePath "msiexec.exe"  -Args "/a C:\Windows\Temp\3.msi /qb TARGETDIR=C:\python27" -passthru -NoNewWindow -Wait


#iex "msiexec /a C:\Windows\Temp\1.msi /qb TARGETDIR=C:\python27"
