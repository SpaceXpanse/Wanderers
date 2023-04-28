@echo off
:: ----------------------------------------------------------------------------
:: Batch file to retreive (update) the needed spacexpanse SDK libraries and
:: headers to build the D3D9Client.
::
:: Notes:
:: - This swill always get the HEAD revision of SpaceXpanse trunk
::   (see REV variable at setup)!
::
:: ----------------------------------------------------------------------------
setlocal

:: --- Setup
set BASE_DIR=..\..
set URL=svn://svn.spacexpanse-forum.com/spacexpanse/SpaceXpansesdk
set REV=HEAD
set OUT_DIR=SpaceXpansesdk


:: --- Do it
set OUT_DIR="%BASE_DIR%\%OUT_DIR%"
svn export --force -r %REV% %URL%/include              %OUT_DIR%/include              || goto exit_nok
svn export --force -r %REV% %URL%/lib                  %OUT_DIR%/lib                  || goto exit_nok
svn export --force -r %REV% %URL%/VS2015/PropertyPages %OUT_DIR%/VS2015/PropertyPages || goto exit_nok


:: --- Pass / Fail exit
:exit_ok
exit /B 0

:exit_nok
echo.
echo Failed to retrieve SpaceXpanse SDK libs ^& headers!
exit /B 1
