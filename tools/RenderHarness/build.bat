@echo off
setlocal
REM Build the headless RenderHarness mesh-preview tool.
REM
REM Assumes the engine is already built (links against the existing Traktor.*.lib
REM in bin/latest/win64/releaseshared + build/win64/releaseshared), so it does NOT
REM rebuild engine dependencies. Build the engine first if those libs are missing.

REM config.bat derives TRAKTOR_HOME from its own location and sets %SOLUTIONBUILDER%.
call "%~dp0..\..\scripts\config.bat"

echo [RenderHarness] Generating msbuild project from .xms ...
%SOLUTIONBUILDER% -f=msvc -i -p=$(TRAKTOR_HOME)\resources\build\configurations\msvc-2022-win64.xml "%TRAKTOR_HOME%\tools\RenderHarness\RenderHarnessWin64.xms"
if errorlevel 1 goto :error

echo [RenderHarness] Locating MSBuild ...
set "MSBUILD="
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" echo Could not find vswhere.exe. & goto :error
REM The escaped outer quotes (^") let "for /f" run a command whose path has spaces.
for /f "usebackq tokens=*" %%i in (`^""%VSWHERE%" -latest -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe"^"`) do set "MSBUILD=%%i"
if not defined MSBUILD echo Could not locate MSBuild via vswhere. & goto :error

echo [RenderHarness] Building (ReleaseShared^|x64) ...
"%MSBUILD%" "%TRAKTOR_HOME%\tools\RenderHarness\build\win64\RenderHarness\RenderHarness.vcxproj" /p:Configuration=ReleaseShared /p:Platform=x64 /p:BuildProjectReferences=false /m /v:minimal /nologo
if errorlevel 1 goto :error

echo [RenderHarness] Deploying exe next to engine DLLs ...
copy /Y "%TRAKTOR_HOME%\tools\RenderHarness\build\win64\ReleaseShared\RenderHarness.exe" "%TRAKTOR_HOME%\bin\latest\win64\releaseshared\" >nul
if errorlevel 1 goto :error

echo [RenderHarness] Done: %TRAKTOR_HOME%\bin\latest\win64\releaseshared\RenderHarness.exe
goto :eof

:error
echo [RenderHarness] BUILD FAILED
exit /b 1
