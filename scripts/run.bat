@echo off
cd /d "%~dp0..\build\Release"
if exist "chatbot.exe" (
    chatbot.exe
) else (
    echo Error: chatbot.exe not found!
    echo Please run setup_windows.bat first.
    pause
)