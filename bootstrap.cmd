@ECHO OFF
SETLOCAL

SET srcdir=%~p0

REM Good enough for path names that shouldn't contain spaces.
IF "%1" == "" (
    SET builddir=%CD%\build.Windows
) ELSE (
    SET builddir=%1
)

REM I prefer ninja.
where /q ninja
IF ERRORLEVEL 1 (
    ECHO Using default cmake generator.
) ELSE (
    SET cmakegen=-G Ninja
)

ECHO srcdir: "%srcdir%"
ECHO builddir: "%builddir%"
ECHO cmakegen: "%cmakegen%"

cmake ^
%cmakegen% ^
-B "%builddir%" ^
-S "%srcdir%"
IF ERRORLEVEL 1 (
    GOTO :EOF
)

cmake --build %builddir% --target all
