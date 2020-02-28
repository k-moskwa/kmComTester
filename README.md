# kmComTester
Desktop application written with Qt, for testing MCU applications with commands sent via UART

### Compilation

If not installed - download and install **[Qt Development Platform](https://download.qt.io/archive/qt/5.13/5.13.2/qt-opensource-windows-x86-5.13.2.exe)** 
(in current version application is compiled against Qt 5.13.2 version)

Clone kmComTester application into directory of your choice with following application

```
git clone git@github.com:k-moskwa/kmComTester.git
```

For Windows operating system and default qt installation place - set following environment variables:

```
SET QTDIR=C:\Qt\Qt5.13.2\5.13.2\mingw73_32
SET MINGW_DIR=C:\Qt\Qt5.13.2\Tools\mingw730_32
SET PATH=%PATH%;%QTDIR%\bin;%MINGW_DIR%\bin
```

Go to the *src* directory of the kmComTester application and issue *qmake*, than *make clean release commands*

```
cd src
qmake
make clean release
```

After compilation the exe file appears in kmComTester\src\release directory

Alternatively, adjust **buildWin.cmd** file according to your Qt version and run it from command line. When script is finished, the complete distribution including all necessary .dll files is located in **kmComTester\dist** directory.

When cloned from git, this directory contains also last released version of kmComTester application packed in the .zip file. It can be used directly without necessity to comile project.

[Qt]: https://download.qt.io/archive/qt/5.13/5.13.2/qt-opensource-windows-x86-5.13.2.exe	"Qt Development Platform"





