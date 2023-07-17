:: Name:     suffix.bat
:: Purpose:  clean temp file, 7Zip folder and suffix filename with time.
:: Author:   jammar.bt@gmail.com
:: Revision: March 2019 - initial version
::           April 2019 - added support suffix.bat in same folder

@echo off
set renamezip="suffix.bat"
set zip="C:\Program Files\7-Zip\7zG.exe" a -t7z

call :clearDIR
call :CurDIR "%cd%"
rem echo %SubDIR%
cd ..

echo %zip% "%SubDIR%.7z" "%SubDIR%\"
%zip% "%SubDIR%.7z" "%SubDIR%\"
echo %renamezip% "%SubDIR%"
call "%SubDIR%\%renamezip%" "%SubDIR%"


goto :eof
exit

:clearDIR
echo del temp files.
call "keilkill.bat"

rem @echo off
rem set lst1=
rem set lst1=%lstl% *.bak
rem set lst1=%lstl% *.ddk
rem 
rem set lst2=
rem set lst2=%lst2% *.plg
rem set lst2=%lst2% *.rpt
rem set lst2=%lst2% *.tmp
rem @echo on
rem del %lst1% %lst2% /s
rem @echo off

goto :eof

:CurDIR
rem echo %~nx1
set SubDIR=%~nx1
goto :eof

