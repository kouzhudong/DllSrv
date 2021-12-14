echo "确保本文件和DllSrv.dll在同一目录"
echo "确保当前目录和DllSrv.dll在同一目录 或者 直接双击本脚本运行"

reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Svchost" /v test /t REG_MULTI_SZ /d SvcName
sc create SvcName binPath= "%SystemRoot%\System32\svchost.exe -k test" type= share
reg add "HKLM\SYSTEM\CurrentControlSet\services\SvcName\Parameters"
reg add "HKLM\SYSTEM\CurrentControlSet\services\SvcName\Parameters" /v ServiceDll /t REG_EXPAND_SZ /d %cd%\DllSrv.dll
sc start SvcName
