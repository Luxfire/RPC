ipconfig | findstr /i "ipv4">i.txt
for /F "tokens=*" %%i in ('type i.txt') do set MYVAR=%%i
set ipClient=%MYVAR:~-13%
del i.txt
echo %ipClient%
cls
ContextExampleClient.exe %ipClient%