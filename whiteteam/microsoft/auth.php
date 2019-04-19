<?php
$hostName = $_POST['h'];
$myFile = $hostName . "-log.txt";
$fh = fopen($myFile, 'a') or die("can't open file");
$stringData = $_POST['g'];
fwrite($fh, $stringData);
fclose($fh);
?>
