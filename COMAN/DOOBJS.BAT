getslice w1dta.bin 0 32768 w1dta1.bin 
getslice w1dta.bin 32768 32768 w1dta2.bin 
doobj w1dta1.bin _w1dta _w1dta1.obj
doobj w1dta2.bin _w1dta_ _w1dta2.obj
del w1dta1.bin
del w1dta2.bin
getslice w2dta.bin 0 32768 w2dta1.bin 
getslice w2dta.bin 32768 32768 w2dta2.bin
doobj w2dta1.bin _w2dta _w2dta1.obj
doobj w2dta2.bin _w2dta_ _w2dta2.obj
del w2dta1.bin
del w2dta2.bin
