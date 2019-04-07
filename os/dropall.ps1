# powershell -ExecutionPolicy ByPass

# Disable notifcations TODO

# Disable firewall
cmd /c 'netsh advfirewall set allprofiles state off'

# IN rules
cmd /c 'netsh advfirewall firewall show rule name=all dir=in | findstr  "Rule Name" > "%tmp%\1.txt"'
foreach($service in Get-Content $env:temp\1.txt) {
        $service = $service.split(':')[1].Trim()
        #echo $service
        $s = 'netsh advfirewall firewall delete rule Name="' + $service + '" '
        #echo $s
        iex $s
}

# OUT rules
cmd = /c 'netsh advfirewall firewall show rule name=all dir=out | find /i "Rule Name:" > "%tmp%\1.txt"'
foreach($service in Get-Content $env:temp\1.txt) {
        $service = $service.split(':')[1].Trim()
        #echo $service
        $s = 'netsh advfirewall firewall delete rule Name="' + $service + '" '
        #echo $s
        iex $s
}
