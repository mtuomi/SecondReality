;/****************************************************************************
;** MODULE:	amain.asm
;** AUTHOR:	Sami Tammilehto / Fennosoftec OY
;** DOCUMENT:	?
;** VERSION:	1.0
;** REFERENCE:	-
;** REVISED BY:	-
;*****************************************************************************
;**
;** Assembler / Main
;** - segment definitions
;** - public routines
;**
;****************************************************************************/

include a.inc

;######## Segment definitions ########

asm_data SEGMENT para public use16 'DATA' 
asm_data ENDS ;(ad.asm)

asm_code SEGMENT para public use16 'CODE' 
asm_code ENDS ;(am.asm,ac.asm,ad.asm)

;######## Public Routines ########

asm_code SEGMENT para public 'CODE' 
	ASSUME cs:asm_code
	
asm_code ENDS
	END
	