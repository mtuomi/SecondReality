;/****************************************************************************
;** MODULE:	adata.asm
;** AUTHOR:	Sami Tammilehto / Fennosoftec OY
;** DOCUMENT:	?
;** VERSION:	1.0
;** REFERENCE:	-
;** REVISED BY:	-
;*****************************************************************************
;**
;** Assembler / Data
;**
;****************************************************************************/

include a.inc

asm_data SEGMENT para public use16 'DATA'

_datanull	dd	12345678h	

;offsets to rows in vram
_rows		LABEL WORD
		dw	MAXROWS dup(0)
_rowlen		dw	0

_cdataseg	dw	0
		
;segment to video memory
_vramseg	dw	0

		ALIGN 4	
;projection clip window
_projclipx	dd	0,319 ;(xmin,xmax)
_projclipy	dd	0,199 ;(ymin,ymax)
_projclipz	dd	256,1000000000 ;(zmin,zmax)
;projection variables
_projmulx	dd	250
_projmuly	dd	220
_projaddx	dd	160
_projaddy	dd	100
_projaspect	dw	256 ;aspect ratio (ratio=256*ypitch/xpitch)
_projoversampleshr dw	0

;video driver routine pointers 
vr	LABEL WORD
	dw	VRSIZE dup(0)
	
_polydrw LABEL WORD
	dw	1024 dup(0)
	
_sintable LABEL WORD
include adatasin.inc

_avistan LABEL WORD
include avistan.inc

_afilldiv LABEL WORD
include afilldiv.inc

asm_data ENDS
	END
