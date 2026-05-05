@echo off
echo ========================================
echo  AI Chatbot CLI - Windows Setup
echo ========================================
echo.

:: Check for admin rights
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo WARNING: Some installations may require administrator privileges!
    echo Please run as administrator if you encounter issues.
    echo.
)

:: Check if vcpkg is installed
set VCPKG_ROOT=C:\vcpkg
if exist "%VCPKG_ROOT%\vcpkg.exe" (
    echo vcpkg found at %VCPKG_ROOT%
) else (
    echo vcpkg not found. Installing vcpkg...
    git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
    cd C:\vcpkg
    call bootstrap-vcpkg.bat
    cd %~dp0..\
)

:: Install dependencies via vcpkg
echo Installing dependencies...
vcpkg install curl:x64-windows nlohmann-json:x64-windows libmysql:x64-windows

:: Create build directory
echo Creating build directory...
if not exist "build" mkdir build
cd build

:: Run CMake
echo Running CMake...
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake

:: Build
echo Building project...
cmake --build . --config Release

echo.
echo ========================================
echo  Setup Complete!
echo ========================================
echo.
echo To run the chatbot:
echo   cd build\Release
echo   chatbot.exe
echo.
echo Or use Docker:
echo   docker-compose up -d mysql
echo   docker-compose run --rm chatbot
echo ========================================
pause