rem not to be run directly, but called from an app's build directory
@echo off
setlocal

if [%1]==[] (set BUILD_TYPE_RAW=Debug) else (set BUILD_TYPE_RAW=%1)
set  BUILD_TYPE_FULL=-DCMAKE_BUILD_TYPE=%BUILD_TYPE_RAW%

if [%2]==[] (set RENDER_TYPE_RAW=OPENGL2) else (set RENDER_TYPE_RAW=%2)
set  RENDER_TYPE_FULL=-DYI_RENDER_TYPE=%RENDER_TYPE_RAW%

if "%3"=="TestApp" (set BUILD_APP_TEST_RAW=1) else (set BUILD_APP_TEST_RAW=0)
set  BUILD_APP_TEST_FULL=-DYI_BUILD_APP_TEST=%BUILD_APP_TEST_RAW%

if "%4"=="Win64" (
	set BUILD_GENERATOR="Visual Studio 12 2013 Win64"
	set BUILD_ARCHITECTURE=-DYI_BUILD_FOR_WIN64=1
) else (
	set BUILD_GENERATOR="Visual Studio 12 2013"
	set BUILD_ARCHITECTURE=""
)

echo "build generator = %BUILD_GENERATOR%"

cmake ^
 -G %BUILD_GENERATOR% ^
 %BUILD_TYPE_FULL% ^
 %RENDER_TYPE_FULL% ^
 %BUILD_APP_TEST_FULL% ^
 %BUILD_ARCHITECTURE% ^
 ..\..\src

if %ERRORLEVEL% equ 0 (
cmake ^
 --build . ^
 --config %BUILD_TYPE_RAW% ^
 --clean-first
 )

endlocal
