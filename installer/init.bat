@echo off
setlocal

rem  init QT_PATH PLATFORM ARCH

set qt_path=%1
set platform=%2
set arch=%3

set pwd=%CD%
set out_path=%pwd%\output
set data_path="%pwd%\packages\ir.tooska.confiqt\data\"

rem create_paths
mkdir %out_path%

rem  copy files
xcopy /y /q QtConfig.exe %data_path% 

rem  make setup
binary_creator=%qt_path%\Tools\QtInstallerFramework\3.2\bin\binarycreator.exe
$binary_creator -c %pwd%/config.xml -p %pwd%/packages %out_path%/qtconfig-%platform%-%arch%.exe
