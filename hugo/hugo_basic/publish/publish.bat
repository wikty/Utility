@echo off

:: global variables
set blogurl=""
set gitrepo=""
set firefoxpath=""
set hugopath=""
set hugoargs=""
set gitbranch=gh-pages
set publishdir=public
set firefox=firefox
set hugo=hugo
set firefoxargs=-new-tab
set configfile=publish.config

rem Main
call :readconfig
call :configfirefox  %firefox%
call :confighugo  %hugo%
call :runhugo %hugopath% %hugoargs%
set currentdir=%cd%
cd %publishdir%
call :gitinit %gitrepo% %gitbranch%
call :gitpush %gitbranch%
cd %currentdir%
call :runfirefox %firefoxpath% %firefoxargs% %blogurl%
rem End of Main
echo.&pause&exit /B %ERRORLEVEL%

:readconfig
for /f "tokens=1,2 delims==" %%i in (%configfile%) do (
	if "%%i" == "blogurl" (
		set blogurl=%%j
		echo Config [blogurl] =  %%j
	)
	if "%%i" == "gitrepo" (
		set gitrepo=%%j
		echo Config [gitrepo] =  %%j
	)
	if "%%i" == "firefoxpath" (
		set firefoxpath=%%j
		echo Config [firefoxpath] = %%j
	)
	if "%%i" == "hugopath" (
		set hugopath=%%j
		echo Config [hugopath] = %%j
	)
	if "%%i" == "hugoargs" (
		set hugoargs=%%j
		echo Config [hugoargs] = %%j
	)
)
exit /B 0

:configfirefox
if exist %firefoxpath% (exit /B 0)
for /f %%i in ('where %~1 ^2^>nul') do (
	if %ERRORLEVEL% == 0 (
		set firefoxpath=%%i
		echo Reconfig [firefoxpath] = %%i
		exit /B 0
	)
)
echo "Firefox is not in %firefoxpath%"
echo "Please Configure Firefox binary location"
goto :end

:runfirefox
"%~1" %~2 %~3
exit /B 0

:confighugo
if exist %hugopath% (exit /B 0)
for /f %%i in ('where %~1') do (
	if %ERRORLEVEL% == 0 (
		set hugopath=%%i
		echo Reconfig [hugopath] = %%i
		exit /B 0
	)
)
echo "Hugo is not in %hugopath%"
echo "Please Configure Hugo binary location"
goto :end

:runhugo
"%~1" %~2
exit /B 0

:gitinit
setlocal
set flag=""
rem Not a git repository will print a message starts with string "fatal:"
for /f %%i in ('git status ^2^>^&1 ^| findstr ^^fatal:') do (
	set flag="flag"
)
rem init a git repository
if %flag% == "flag" (
	git init
	git remote add origin %~1
	git checkout -b %~2
)
endlocal
exit /B 0

:gitpush
setlocal
set message=
set flag=""
for /f %%i in ('git status ^| findstr /C:"nothing to commit"') do (
	set flag="flag"
)
if %flag% == "flag" (
	echo "NOTHING IS CHANGED!"
	goto :eof
)
git add --all
for /f "tokens=2" %%i in ('git status -s') do (
	set message=%message%(%%i)
)
git commit -m %message%
git push -f origin %~1
endlocal
exit /B 0

:end