@echo off
rem Opens the MSYS2 UCRT64 shell (gcc, gdb, make) inside the current terminal,
rem starting in the project folder. Usage: from PowerShell type  ./msys  (or  msys  in cmd)
cd /d "%~dp0"
call C:\msys64\msys2_shell.cmd -defterm -here -no-start -ucrt64 -use-full-path
