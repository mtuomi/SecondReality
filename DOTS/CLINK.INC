FIXWORD	MACRO ;go to word boundary
	if ($-start) mod 1
	db 0
	endif
	ENDM
	
CDS	MACRO
	mov	ax,cs
	mov	ds,ax
	ENDM

CBEG	MACRO ;C/Assembler procedure begin
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	ENDM

CEND	MACRO ;C/Assembler procedure end
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
	ENDM

CBEG0	MACRO ;C/Assembler procedure begin, minimal
	push	bp
	mov	bp,sp
	ENDM

CEND0	MACRO ;C/Assembler procedure end, minimal
	pop	bp
	ret
	ENDM

movpar	MACRO	reg,par ;loads parameter [par(0..)] to register [reg]
	mov	reg,[bp+par*2+6]
	ENDM

