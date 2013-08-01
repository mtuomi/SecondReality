;/****************************************************************************
;** MODULE:	a.inc
;** AUTHOR:	Sami Tammilehto / Fennosoftec OY
;** DOCUMENT:	?
;** VERSION:	1.0
;** REFERENCE:	-
;** REVISED BY:	-
;*****************************************************************************
;**
;** Assembler / Include
;** - variable definitions
;** - constant definitions
;**
;****************************************************************************/

.386
LOCALS

;######## macros ########

;note: C macros expect a LARGE memory model

LOADDS	MACRO	;loads the ds segment to point to asm_data
	mov	ax,SEG _datanull
	mov	ds,ax
	ENDM

LOADES	MACRO	;loads the es segment to point to asm_data
	mov	ax,SEG _datanull
	mov	es,ax
	ENDM

LOADGS	MACRO	;loads the gs segment to point to asm_data
	mov	ax,SEG _datanull
	mov	gs,ax
	ENDM

CBEG	MACRO ;C/Assembler procedure begin
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	LOADDS	;since all routines expect to have DS pointed to the
		;data segment, it's loaded at every public entrypoint
		;(which should all be C procedures).
	ENDM

CEND	MACRO ;C/Assembler procedure end
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
	ENDM

CBEGR	MACRO bytes ;C/Assembler procedure begin - with local variable reserve
	push	bp
	mov	bp,sp
	sub	sp,bytes
	push	si
	push	di
	push	ds
	push	bp ;just in case the asm routine changes bp
	LOADDS	;since all routines expect to have DS pointed to the
		;data segment, it's loaded at every public entrypoint
		;(which should all be C procedures).
	ENDM

CENDR	MACRO ;C/Assembler procedure end
	pop	bp
	pop	ds
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret
	ENDM

movpar	MACRO	reg,par ;loads parameter [par(0..)] to register [reg]
	mov	reg,[bp+(par)*2+6]
	ENDM
	
ldspar	MACRO	reg,par ;loads pointer parameter [par(0..)] to register ds:[reg]
	lds	reg,[bp+(par)*2+6]
	ENDM
	
lespar	MACRO	reg,par ;loads pointer parameter [par(0..)] to register es:[reg]
	les	reg,[bp+(par)*2+6]
	ENDM
	
lfspar	MACRO	reg,par ;loads pointer parameter [par(0..)] to register fs:[reg]
	lfs	reg,[bp+(par)*2+6]
	ENDM
	
lgspar	MACRO	reg,par ;loads pointer parameter [par(0..)] to register gs:[reg]
	lgs	reg,[bp+(par)*2+6]
	ENDM
	
;######## public routines ########

global	_vid_init:far
global	_vid_window:far
global	_vid_cameraangle:far
global	_vid_deinit:far
global	_vid_setpal:far
global	_vid_dotdisplay_pvlist:far
global	_vid_dotdisplay_zcolor:far
global	_vid_clear:far
global	_vid_clear255:far
global	_vid_clearbg:far
global	_vid_switch:far
global	_vid_setswitch:far
global	_vid_waitb:far
global	_vid_poly:far
global	_vid_pset:far
global	_vid_skyclear:far
global	_vid_drawpolylist:far
global	_vid_inittimer:far
global	_vid_deinittimer:far
global	_vid_drawsight:far
global	_vid_pic320200:far
;polygon fillers
global	_vid_drawfill:far
global	_vid_drawfill_nrm:far
global	_vid_drawfill_grd:far
global	_vid_drawdots:far

global	_calc_project:far
global	_calc_project16:far
global	_calc_setrmatrix_rotyxz:far
global	_calc_setrmatrix_rotxyz:far
global	_calc_setrmatrix_rotzyx:far
global	_calc_setrmatrix_ident:far
global	_calc_applyrmatrix:far
global	_calc_rotate:far
global	_calc_rotate16:far
global	_calc_sftranslate:far
global	_calc_singlez:far
global	_calc_nrotate:far
global	_calc_mulrmatrix:far
global	_calc_invrmatrix:far

global	_draw_polylist:far
global	_draw_setfillroutine:far

;######## constants ######## (these should be same as in CDATA.H)

MAXROWS		=	480	;maximum number of rows used in any mode
MAXCOLS		=	640	;maximum number of rows used in any mode (should be divisible with 4)
MAXPOLYSIDES	=	16	;maximum sides in a polygon

;######## variables ########
;for documentation on global variables, see adata.asm

global	_datanull:dword
global	_rows:word
global	_rowlen:word
global	_vramseg:word
global	_cdataseg:word
CLIPMIN	equ 0
CLIPMAX equ 4
global	_projclipx:dword
global	_projclipy:dword
global	_projclipz:dword
global	_projmulx:dword
global	_projmuly:dword
global	_projaddx:dword
global	_projaddy:dword
global	_projaspect:word
global	_projoversampleshr:word
;equs are offsets of routine pointers inside vr for current video driver
PSET	equ	0	
CLEAR	equ	2
SWITCH	equ	4
WAITB	equ	6
VRSIZE	equ	8
global	vr:word

global	_sintable:word
global	_avistan:word
global	_afilldiv:word

global	_polydrw:word

;####### data types ########

include ad.inc
