;/****************************************************************************
;** MODULE:	ad.inc
;** AUTHOR:	Sami Tammilehto / Fennosoftec OY
;** DOCUMENT:	?
;** VERSION:	1.0
;** REFERENCE:	-
;** REVISED BY:	-
;*****************************************************************************
;**
;** Assembler / Include - THIS FILE AND CD.H SHOULD BE IN SYNC
;** - data types and related constants
;**
;****************************************************************************/

pvlist_x equ	0
pvlist_y equ	2
pvlist_vf equ	4
pvlist_normal equ 8
pvlist_size equ 16
pvlist_sizeshl equ 4	;[!!] vlist_sizeshl==pvlist_sizeshl required in many
			;places! seek for [!!] for places expecting this
vlist_x equ	0
vlist_y equ	4
vlist_z equ	8
vlist_normal equ 12
vlist_size equ 16
vlist_sizeshl equ 4 	;[!!] vlist_sizeshl==pvlist_sizeshl required in many
			;places! seek for [!!] for places expecting this
nlist_x equ	0
nlist_y equ	2
nlist_z equ	4
nlist_size equ  8
nlist_sizeshl equ 3
	
rmatrix_m equ	0
rmatrix_x equ	36
rmatrix_y equ	40
rmatrix_z equ	44
rmatrix_size equ 48

unit	equ	16384
unitshr	equ	14

VF_UP	equ	1
VF_DOWN	equ	2
VF_LEFT	equ	4
VF_RIGHT equ	8
VF_NEAR equ	16
VF_FAR equ	32

MAXSIDES equ 16

;offsets to fields inside polylist. First vertex at offset 0, next at 4 
;and so on. For example third vertices X coordinate is at [base+3*4+POLYX]
POLYSIDES equ 0
POLYCOLOR equ 2
POLYFLAGS equ 4
POLYVXSEG equ 6 ;segment
POLYX  equ (16)
POLYY  equ (16+2)
POLYVX equ (16+MAXSIDES*4) 	;offset
POLYGR equ (16+2*MAXSIDES*4)	;color
POLYTX equ (16+2*MAXSIDES*4+2)
POLYSIZE equ (16+3*MAXSIDES*4)

F_DEFAULT	equ 0f001h
F_VISIBLE	equ 00001h
F_FLIP		equ 00100h
F_2SIDE		equ 00200h
F_SHADE8	equ 00400h
F_SHADE16	equ 00800h
F_SHADE32	equ 00C00h
F_GOURAUD	equ 01000h
F_TEXTURE	equ 02000h
