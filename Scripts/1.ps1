$IPNodes = Get-Content "IPList.txt"
echo $null > test.txt
foreach ($IP in $IPNodes)
{
if (Test-Connection -IPAddress $IP -Count 1 -ErrorAction SilentlyContinue)
{
Write-Host "$IP is UP"
echo $IP >> test.txt
}
Else
{
Write-Host "$IP is Down"
}
}
