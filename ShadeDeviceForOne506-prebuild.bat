@echo off
setlocal enableextensions enabledelayedexpansion
SET allParams=%*
SET params=%allParams:' '=","%
SET params=%params:'="%
set /a count=0
for %%i in (%params%) do (
  set /a count+=1
  set param[!count!]=%%i
)

set "studioInstallationDir=%param[4]:"=%"
set "projDir=%~dp0"
set "projDir=%projDir:"=%"

"%studioInstallationDir%/features\com.silabs.external.java.windows.x86_64.feature_1.8.0.92\jre\bin\java.exe" -jar "%studioInstallationDir%/plugins\com.silabs.external.jython_2.7.0.201809121258-107\external_jython\2.7.0\jython-2.7.0.jar"  "%projDir%/ShadeDeviceForOne506-prebuild.py"