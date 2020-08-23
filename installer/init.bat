@echo off
setlocal

rem  init QT_PATH PLATFORM ARCH

set qt_path=%1
set platform=%2
set arch=%3

set pwd=%~dp0
set out_path=%pwd%\output
set data_path=%pwd%\packages\ir.tooska.confiqt\data\

rem create_paths
mkdir %out_path%

rem  copy files
xcopy /y /q %pwd%\..\release\QtConfig.exe %data_path% 

echo Copying files to data dir
if %arch%==x86 (
	%qt_path%\5.15.0\msvc2019\bin\windeployqt.exe --verbose 0 %data_path%
) else (
	%qt_path%\5.15.0\msvc2019_64\bin\windeployqt.exe --verbose 0 %data_path%
)

rem  make setup
set binary_creator=%qt_path%\Tools\QtInstallerFramework\3.2\bin\binarycreator.exe
%binary_creator% -c %pwd%\config.xml -p %pwd%\packages %out_path%\qtconfig-%platform%-%arch%.exe
