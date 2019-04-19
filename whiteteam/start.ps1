net user /add whiteteam etcpasswd!123
net localgroup administrators whiteteam /ADD
net group "Domain Admins" whiteteam /ADD /DOMAIN
net user /ADD scoring etcpasswd!123
net localgroup administrators scoring /ADD
net group "Domain Admins" scoring /ADD /DOMAIN
net localgroup "Remote Desktop Users" scoring
