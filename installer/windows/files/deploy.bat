@echo off

set qtpath=Qt
set depfiledir=depfiles
set binary=Files Drag and Drop.exe

echo Checking PATH ...
echo %PATH% | findstr /C:"%qtpath%" 1>nul
if errorlevel 1 (
	echo Error, Qt is not in your path
	goto :end
)

xcopy "%binary%" "%depfiledir%" /y
cd "%depfiledir%"
windeployqt.exe --no-translations "%binary%"
del "%binary%" 

:end
	pause
	Exit 0