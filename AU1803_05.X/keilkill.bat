:: kill temp files.

set lst1=
set lst1=%lstl% *.bak
set lst1=%lstl% *.ddk
set lst1=%lstl% *.edk
set lst1=%lstl% *.lst
set lst1=%lstl% *.lnp
set lst1=%lstl% *.mpf
set lst1=%lstl% *.mpj
set lst1=%lstl% *.obj
set lst1=%lstl% *.omf

set lst2=
set lst2=%lst2% *.plg
set lst2=%lst2% *.rpt
set lst2=%lst2% *.tmp
set lst2=%lst2% *.__i
set lst2=%lst2% *.crf
set lst2=%lst2% *.o
set lst2=%lst2% *.d
:: set lst2=%lst2% *.axf
set lst2=%lst2% *.tra
set lst2=%lst2% *.dep         
set lst2=%lst2% JLinkLog.txt

::ctags -R && gtags temp files.
set lst3=
set lst3=%lst3% GPATH
set lst3=%lst3% GRTAGS
set lst3=%lst3% GTAGS
set lst3=%lst3% tags
set lst3=%lst3% NppGTags.cfg

@echo on
del %lst1% %lst2% %lst3% /s
@echo off

goto :eof
rem 
rem del *.iex /s
rem del *.htm /s
rem ::del *.sct /s
rem del *.map /s
rem 
rem ::keil
rem del .\MDK-ARM\*.uvguix.jammar /s
rem del .\MDK-ARM.\RTE /q
rem rmdir .\MDK-ARM.\RTE 
