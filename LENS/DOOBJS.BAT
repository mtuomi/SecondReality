@echo off
glvp lenspic.lbm -xv!
del lens.exb
ren lenspic.uh lens.exb
doobj lens.ex0 _lensex0 _lensex0.obk
doobj lens.ex1 _lensex1 _lensex1.obk
doobj lens.ex2 _lensex2 _lensex2.obk
doobj lens.ex3 _lensex3 _lensex3.obk
doobj lens.ex4 _lensex4 _lensex4.obk
doobj lens.exb _lensexb _lensexb.obk
doobj lens.exp _lensexp _lensexp.obk
