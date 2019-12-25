@echo off
set SOURCE_DIR=".\glfw"
set BUILD_DIR=".\glfw\proj"
set FLATFORM="Visual Studio 16 2019"

if exist %BUILD_DIR% (
	rmdir /S /Q %BUILD_DIR%
	)

call cmake -G %FLATFORM% -DGLFW_BUILD_EXAMPLES="" -DGLFW_BUILD_DOCS="" -DGLFW_BUILD_TESTS="" -S %SOURCE_DIR% -B %BUILD_DIR%
