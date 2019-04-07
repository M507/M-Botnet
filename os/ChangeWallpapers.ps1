# Not hard to understand .. Just read it.

$FIP = ''


function updateWall($FIP){
	$url = 'http://$FIP/whiteteam/new.html';
	$response = (Invoke-RestMethod -Uri $url).Trim();
	return $response;
}


#This function does not need $FIP
function get_wget {
        $filename = "wget.exe"
        $wgetBinLocation = '' + $save_dir + '\' + $filename;
        if ( Test-Path -Path $wgetBinLocation){
            return $wgetBinLocation
        } Else {
            #$client = New-Object System.Net.WebClient
            #https://eternallybored.org/misc/wget/
            write-host "Downloading Wget!"
            $url = "https://eternallybored.org/misc/wget/1.20/32/wget.exe"
            (New-Object System.Net.WebClient).DownloadFile($url, $wgetBinLocation)
			    }
        return $wgetBinLocation
}


#Get file from -whiteteam- I mean from the dir whiteteam
function download_file_from_whiteteam($wgetBinLocation , $fileName) {
    $Location = '' + $save_dir + '\' + $fileName;
    Start-Process -FilePath $wgetBinLocation -Args " https://$FIP/whiteteam/$fileName -O $Location" -passthru -NoNewWindow -Wait
    return $Location
}

# Get the new name for the the wallpaper.
$picName = updateWall $FIP
# Download wget.exe if it does not exist, return the location of wget.exe
$wgetBinLocation get_wget
# Download the $picName from /var/www/html/whiteteam/
$LocationOfPic = download_file_from_whiteteam $wgetBinLocation $picName

# Setup the command
$command = 'reg add "HKEY_CURRENT_USER\Control Panel\Desktop" /v Wallpaper /t REG_SZ /d '+$LocationOfPic+' /f'
# Execute the command
cmd /c $command


