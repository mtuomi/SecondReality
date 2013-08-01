echo off
nmake
if ERRORLEVEL 1 goto err
cd c
nmake cplay.exe
if ERRORLEVEL 1 goto err
cplay city
type ..\todo
:err
