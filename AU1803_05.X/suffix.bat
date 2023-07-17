:: Name:     suffix.bat
:: Purpose:  rename the 7z file as data_time.
:: Author:   jammar.bt@gmail.com
:: Revision: March 2019 - initial version
::           April 2019 - added support 

echo on
set FileT=%1
set suffix=%DATE:~0,4%%DATE:~5,2%%DATE:~8,2%
set time_suffix=%suffix%_%time:~0,2%%time:~3,2%
set time_suffix=%time_suffix: =0%
if exist %FileT%.7z (
   rem file exists
   echo file exists.try to rename.
   call :rename_date_7z
) else (
      rem file doesn't exist
      echo file doesn't exist.
      goto :eof
)

timeout /t 3
goto :eof

:rename_date_7z
rem force time format
rename %FileT%.7z %FileT%_%time_suffix%.7z
goto :eof
rem end force time format
if exist %FileT%_%suffix%.7z (
   rem file exists
   echo file exists.try add time
   echo rename %FileT%.7z %time_suffix%.7z
   rename %FileT%.7z %FileT%_%time_suffix%.7z
) else (
   rem file doesn't exist.
   echo file doesn't exist.
   echo rename %FileT%.7z %FileT%_%suffix%.7z
   rename %FileT%.7z %FileT%_%suffix%.7z
   goto :eof 
)
goto:eof
   