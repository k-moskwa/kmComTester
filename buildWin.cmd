SET QTVER=5.13.2
SET QTENVIRONMENT=mingw73_32
SET QTCOMPILER=mingw730_32

SET QTDIR=C:\Qt\Qt%QTVER%\%QTVER%\%QTENVIRONMENT%
SET MINGWDIR=C:\Qt\Qt%QTVER%\Tools\%QTCOMPILER%

SET PATH=%PATH%;%QTDIR%\bin;%MINGWDIR%\bin

cd src

qmake
make clean release

REM package content
cd ..
mkdir dist
copy "%MINGWDIR%\bin\libgcc_s_dw2-1.dll" dist\
copy "%MINGWDIR%\bin\libstdc++-6.dll" dist\
copy "%MINGWDIR%\bin\libwinpthread-1.dll" dist\
copy "%QTDIR%\bin\Qt5Core.dll" dist\
copy "%QTDIR%\bin\Qt5Gui.dll" dist\
copy "%QTDIR%\bin\Qt5Widgets.dll" dist\
copy "%QTDIR%\bin\Qt5SerialPort.dll" dist\
copy ".\src\release\kmComTester.exe" dist\
