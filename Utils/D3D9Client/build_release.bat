@echo off
:: ----------------------------------------------------------------------------
:: Batch file to create a build package (ZIP) of the D3D9Client
::
:: Notes:
:: - you need to have 7-Zip installed (you might however still need to adjust
::   the ZIP_CMD setting below to make it work)
:: - The generated ZIP file name contains the revision number in brackets.
::   If that number ends with an 'M' (like e.g. r35884M), it means that it
::   was build containing local modifications!
::   This should *not* be a officially released version!
::
:: ----------------------------------------------------------------------------
setlocal

:: --- Setup
set BASE_DIR=..\..
set OUT_DIR=_release
set VERSION=Beta30.7


:: Check if SDK and other needed resources are present
for %%a in ("%BASE_DIR%\SpaceXpansesdk\lib\spacexpanse.lib" ^
            "%BASE_DIR%\SpaceXpansesdk\VS2015\PropertyPages\spacexpanse.props") ^
do if not exist %%a (
  echo ======================================================================
  echo   Getting SpaceXpanse SDK libs ^& headers^.^.^.
  echo ======================================================================

  pushd .
  call get_spacexpanse_libs.bat
  if errorlevel 1 goto exit_nok
  popd
)


:: Enhance Version by SpaceXpanse Version
echo ======================================================================
echo   Getting Version information
echo ======================================================================
echo   ^.^.^.

for /F "usebackq tokens=*" %%i in (`over /N ..\..\SpaceXpansesdk\lib\spacexpanse.lib`) do set OVER=%%i
if "%OVER%"=="BETA r62" set OVER=SpaceXpanse2016
set VERSION=%VERSION%-for%OVER%

if "%VS141COMNTOOLS%"=="" if "%VS142COMNTOOLS%"=="" call helper_vswhere.bat

:: Visual Studio 2019
if not "%VS142COMNTOOLS%"=="" (
  set "SETVCVARS=%VS142COMNTOOLS%..\..\VC\Auxiliary\Build\vcvarsall.bat"
  set SOLUTIONFILE=D3D9ClientVS2019.sln
  set CAM_SOLUTIONFILE=GenericCamera.sln
  set EXTMFD_SOLUTIONFILE=DX9ExtMFD.sln
  set GCAPI_PROJECTFILE=gcAPI.vs201x.vcxproj
  set BUILD_FLAG=-m
  goto assign
)
:: Visual Studio 2017
if not "%VS141COMNTOOLS%"=="" (
  set "SETVCVARS=%VS141COMNTOOLS%..\..\VC\Auxiliary\Build\vcvarsall.bat"
  set SOLUTIONFILE=D3D9ClientVS2017.sln
  set CAM_SOLUTIONFILE=GenericCamera.sln
  set EXTMFD_SOLUTIONFILE=DX9ExtMFD.sln
  set GCAPI_PROJECTFILE=gcAPI.vs201x.vcxproj
  set BUILD_FLAG=/p:XPDeprecationWarning=false /m
  goto assign
)
:: Visual Studio 2015
if not "%VS140COMNTOOLS%"=="" (
  set "SETVCVARS=%VS140COMNTOOLS%..\..\VC\vcvarsall.bat"
  set SOLUTIONFILE=D3D9ClientVS2015.sln
  set CAM_SOLUTIONFILE=GenericCamera.sln
  set EXTMFD_SOLUTIONFILE=DX9ExtMFD.sln
  set GCAPI_PROJECTFILE=gcAPI.vs201x.vcxproj
  goto assign
)

:assign
set ZIP_NAME=D3D9Client%VERSION%
set VC=msbuild.exe
set BUILD_FLAG=%BUILD_FLAG% /t:build /v:minimal /nologo
set SOLUTIONFILE="%BASE_DIR%\SpaceXpansesdk\D3D9Client\%SOLUTIONFILE%"
set CONFIG=/p:Configuration=Release /p:Platform=Win32
set CONFIG_DBG=/p:Configuration=Debug /p:Platform=Win32
set ZIP_CMD="C:\Program Files\7-Zip\7z.exe"


:: --- Update working copy & get revision number
svn update --quiet -rHEAD %BASE_DIR%
for /F "tokens=*" %%i IN ('svnversion %BASE_DIR%') DO set REVISION=%%i


:: --- Create folder structure
if exist "%OUT_DIR%" (
  rmdir /S /Q "%OUT_DIR%"
)
mkdir "%OUT_DIR%"


:: --- Start build environment
:: Prevent vcvarsall.bat of Visual Studio 2017 from changing the current working directory
set "VSCMD_START_DIR=%CD%"
call "%SETVCVARS%" x86
if errorlevel 1 goto exit_nok
echo.


:: D3D9Client (RELEASE)
echo ======================================================================
echo   Building D3D9Client
echo ======================================================================

call %VC% %BUILD_FLAG% %SOLUTIONFILE% %CONFIG%
if errorlevel 1 goto exit_nok

:: gcAPI_dbg.lib (DEBUG)
:: if not exist "%BASE_DIR%\SpaceXpansesdk\lib\gcAPI_dbg.lib" (
::   echo ======================================================================
::   echo   Building gcAPI_dbg^.lib ^(DEBUG version of gcAPI.lib^)
::   echo ======================================================================
::   call %VC% "%BASE_DIR%\SpaceXpansesdk\D3D9Client\gcAPI\%GCAPI_PROJECTFILE%" ^
::        %BUILD_FLAG% %CONFIG_DBG%
::   if errorlevel 1 goto exit_nok
:: )

:: gcAPI.lib (RELEASE)
:: call %VC% "%BASE_DIR%\SpaceXpansesdk\D3D9Client\gcAPI\%GCAPI_PROJECTFILE%" ^
::           %BUILD_FLAG% %CONFIG%
:: if errorlevel 1 goto exit_nok

:: GenericCamera (RELEASE)
if not "%CAM_SOLUTIONFILE%"=="" (
  echo.
  echo.
  echo ======================================================================
  echo   Building GenericCamera MFD
  echo ======================================================================
  call %VC% "%BASE_DIR%\SpaceXpansesdk\samples\GenericCamera\%CAM_SOLUTIONFILE%" ^
            %BUILD_FLAG% %CONFIG%
  if errorlevel 1 goto exit_nok
)

:: DX9ExtMFD (RELEASE)
if not "%EXTMFD_SOLUTIONFILE%"=="" (
  echo.
  echo.
  echo.======================================================================
  echo   Building DX9ExtMFD
  echo ======================================================================
  call %VC% "%BASE_DIR%\SpaceXpansesdk\samples\DX9ExtMFD\%EXTMFD_SOLUTIONFILE%" ^
            %BUILD_FLAG% %CONFIG%
  if errorlevel 1 goto exit_nok
)


echo.
echo.
echo ======================================================================
echo   Creating ZIP files
echo ======================================================================

:: --- Export
set ABS_PATH=%cd%
svn export --force --quiet "%BASE_DIR%\Config" "%ABS_PATH%\%OUT_DIR%\Config"
svn export --force --quiet "%BASE_DIR%\Doc" "%ABS_PATH%\%OUT_DIR%\Doc"
svn export --force --quiet "%BASE_DIR%\Meshes" "%ABS_PATH%\%OUT_DIR%\Meshes"
svn export --force --quiet "%BASE_DIR%\Modules" "%ABS_PATH%\%OUT_DIR%\Modules"
svn export --force --quiet "%BASE_DIR%\SpaceXpansesdk" "%ABS_PATH%\%OUT_DIR%\SpaceXpansesdk"
svn export --force --quiet "%BASE_DIR%\Textures" "%ABS_PATH%\%OUT_DIR%\Textures"
svn export --force --quiet "%BASE_DIR%\Utils" "%ABS_PATH%\%OUT_DIR%\Utils"
svn export --force --quiet "%BASE_DIR%\D3D9Client.cfg" "%ABS_PATH%\%OUT_DIR%"


:: --- Remove some files we don't need to have in the source-package
del "%OUT_DIR%\SpaceXpansesdk\D3D9Client\doc\images\*.psd"


:: --- Copy the DLLs
if not exist "%OUT_DIR%\Modules\Plugin" mkdir "%OUT_DIR%\Modules\Plugin"
if not exist "%OUT_DIR%\SpaceXpansesdk\lib" mkdir "%OUT_DIR%\SpaceXpansesdk\lib"
copy /y %BASE_DIR%\Modules\Plugin\D3D9Client.dll ^
         %OUT_DIR%\Modules\Plugin\D3D9Client.dll > nul
copy /y %BASE_DIR%\SpaceXpansesdk\lib\gcAPI.lib ^
         %OUT_DIR%\SpaceXpansesdk\lib\gcAPI.lib > nul
:: copy /y %BASE_DIR%\SpaceXpansesdk\lib\gcAPI_dbg.lib ^
::          %OUT_DIR%\SpaceXpansesdk\lib\gcAPI_dbg.lib > nul
copy /y %BASE_DIR%\Modules\Plugin\GenericCamera.dll ^
         %OUT_DIR%\Modules\Plugin\GenericCamera.dll > nul
copy /y %BASE_DIR%\Modules\Plugin\DX9ExtMFD.dll ^
         %OUT_DIR%\Modules\Plugin\DX9ExtMFD.dll > nul


:: --- Packing
pushd %OUT_DIR%
%ZIP_CMD% a -tzip -mx9 "..\%ZIP_NAME%(r%REVISION%)+src.zip" *

rmdir /S /Q "SpaceXpansesdk\D3D9Client"
rmdir /S /Q "Utils"
%ZIP_CMD% a -tzip -mx9 "..\%ZIP_NAME%(r%REVISION%).zip" *
popd

:: --- Publishing
::ftp_helper.bat is supposed to contain folowing imformation if used
::set SERVER=
::set USER=
::set TGTPATH=
if not exist ftp_helper.bat goto exit_ok
echo --------------------------
set /p GOBUILD=Publish a Build [Y/N] ? 
if %GOBUILD%==N goto exit_ok
if %GOBUILD%==n goto exit_ok
call ftp_helper.bat
set /p STA=Stable or Beta [S/B] ? 
set TYPE=beta
if %STA%==S set TYPE=stable
if %STA%==s set TYPE=stable
echo --------------------------
set /p PASS=Password: 
echo user %USER% %PASS%>ftp.tmp
echo cd %TGTPATH%>>ftp.tmp
echo get index.html html.tmp>>ftp.tmp
echo bye>>ftp.tmp
ftp -n -i -s:ftp.tmp %SERVER%
call ModHTML.exe html.tmp "%ZIP_NAME%(r%REVISION%).zip" BETA %TYPE%
echo --------------------------
echo user %USER% %PASS%>ftp.tmp
echo cd %TGTPATH%>>ftp.tmp
echo send html.tmp index.html>>ftp.tmp
echo binary>>ftp.tmp
echo send "%ZIP_NAME%(r%REVISION%).zip">>ftp.tmp
echo bye>>ftp.tmp
ftp -n -i -s:ftp.tmp %SERVER%
del ftp.tmp
del html.tmp
pause

:: --- Pass / Fail exit
:exit_ok
cd %ABS_PATH%
call :cleanup
exit /B 0

:exit_nok
echo.
echo Build failed!
call :cleanup
exit /B 1


:: --- Cleanup
:cleanup
rmdir /S /Q "%OUT_DIR%"
endlocal
