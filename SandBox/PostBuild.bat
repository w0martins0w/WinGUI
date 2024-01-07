@echo off

rem %~1: "$(SolutionDir)"
rem %~2: "$(Platform)\$(Configuration)\"
rem %~3: "$(ProjectName)\"
rem %~4: "$(SolutionName)"

setlocal

echo Copying %~4.dll . . .
copy /Y "%~1Library\%~2Output\%~4.dll" /B "%~1%~3%~2Output\%~4.dll"

endlocal

@echo on
@exit /B 0
