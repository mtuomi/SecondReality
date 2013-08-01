echo off
font1 %1.lbm %1.tmp
font2 %1.tmp %1.fnt
del %1.tmp
cls
echo Fonttitiedosto: %1.fnt
