rem This is used to generate the updates to the program as I go with timestamps. Will be added to the github push things.

@echo off
color 0a
:main
cls
echo.
echo.
type Journal.dat
echo.
echo.
set /p text=#     //
echo.>>journal.dat
echo %date% %time%:>>journal.dat
echo    %text%>>journal.dat
goto main
