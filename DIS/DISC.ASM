;DIS - C interface, Large memory model

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

movpar	MACRO	reg,par ;loads parameter [par(0..)] to register [reg]
	mov	reg,[bp+par*2+6]
	ENDM

text_disc SEGMENT para public 'CODE'
	ASSUME cs:text_disc
	LOCALS

public _dis_version ;int _dis_version(void)
_dis_version PROC FAR
	xor	ax,ax
	mov	es,ax
	mov	bx,es:[0fch*4+0]
	mov	es,es:[0fch*4+2]
	cmp	es:[bx-2],0fc0h
	jne	@@1
	cmp	es:[bx-4],0fc0h
	jne	@@1
	xor	bx,bx
	int	0fch
@@1:	ret
_dis_version ENDP
		
public _dis_waitb ;int _dis_waitb(void)
_dis_waitb PROC FAR
	mov	bx,1
	int	0fch
	ret
_dis_waitb ENDP
		
error_nodis db	'ERROR: DIS not loaded.$'
		
public _dis_partstart ;void _dis_partstart(void)
_dis_partstart PROC FAR
	call	_dis_version
	cmp	ax,0
	jne	@@1
	mov	ax,cs
	mov	ds,ax
	mov	dx,OFFSET error_nodis
	mov	ah,9
	int	21h
	mov	ax,4c03h
	int	21h
@@1:	ret
_dis_partstart ENDP
		
public _dis_exit ;int _dis_exit(void)
_dis_exit PROC FAR
	mov	bx,2
	int	0fch
	ret
_dis_exit ENDP
		
public _dis_indemo ;int _dis_indemo(void)
_dis_indemo PROC FAR
	mov	bx,3
	int	0fch
	ret
_dis_indemo ENDP

public _dis_msgarea ;void *_dis_msgarea(void)
_dis_msgarea PROC FAR
	push	bp
	mov	bp,sp
	mov	ax,[bp+6]
	mov	bx,5
	int	0fch
	pop	bp
	ret
_dis_msgarea ENDP

public _dis_muscode ;int _dis_muscode(void)
_dis_muscode PROC FAR
	push	bp
	mov	bp,sp
	mov	ax,[bp+6]
	mov	bx,6
	int	0fch
	pop	bp
	ret
_dis_muscode ENDP

public _dis_musplus ;int _dis_musplus(void)
_dis_musplus PROC FAR
	push	bp
	mov	bp,sp
	mov	ax,[bp+6]
	mov	bx,6
	int	0fch
	mov	ax,dx
	pop	bp
	ret
_dis_musplus ENDP

public _dis_musrow ;int _dis_musrow(void)
_dis_musrow PROC FAR
	push	bp
	mov	bp,sp
	mov	bx,6
	int	0fch
	mov	ax,bx
	pop	bp
	ret
_dis_musrow ENDP

public _dis_setcopper ;void _dis_copper(int routine_number,void *routine)
_dis_setcopper PROC FAR
	push	bp
	mov	bp,sp
	mov	ax,[bp+6]
	mov	cx,[bp+8]
	mov	dx,[bp+10]
	mov	bx,7
	int	0fch
	pop	bp
	ret
_dis_setcopper ENDP
		
public _dis_setmframe ;void _dis_setmframe(int frame)
_dis_setmframe PROC FAR
	push	bp
	mov	bp,sp
	mov	dx,[bp+6]
	mov	ax,1
	mov	bx,9
	int	0fch
	pop	bp
	ret
_dis_setmframe ENDP
		
public _dis_getmframe ;void _dis_getmframe(void)
_dis_getmframe PROC FAR
	push	bp
	mov	bp,sp
	xor	ax,ax
	mov	bx,9
	int	0fch
	pop	bp
	ret
_dis_getmframe ENDP
		
public _dis_sync ;void _dis_sync(void)
_dis_sync PROC FAR
	push	bp
	mov	bp,sp
	mov	bx,10
	int	0fch
	pop	bp
	ret
_dis_sync ENDP
		
text_disc ENDS
	END
	