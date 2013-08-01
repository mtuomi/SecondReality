;/****************************************************************************
;** MODULE:	acalc.asm
;** AUTHOR:	Sami Tammilehto / Fennosoftec OY
;** DOCUMENT:	?
;** VERSION:	1.0
;** REFERENCE:	-
;** REVISED BY:	-
;*****************************************************************************
;**
;** Assembler / Calculations
;**
;****************************************************************************/

include a.inc

asm_code SEGMENT para public use16 'CODE'
	ASSUME cs:asm_code
	
;entry: bx=angle (0..65535)
; exit: ax=sin(angle) [range -unit..unit]
sin	PROC NEAR
	shr	bx,4-1
	and	bx,not 1
	mov	ax,ds:_sintable[bx]
	ret
sin	ENDP
	
;entry: bx=angle (0..65535)
; exit: ax=cos(angle) [range -unit..unit]
cos	PROC NEAR
	shr	bx,4-1
	add	bx,1024*2
	and	bx,not (8192 or 1)
	mov	ax,ds:_sintable[bx]
	ret
cos	ENDP
	
;entry: bx=angle (0..65535)
; exit: ax=sin(angle) [range -unit..unit]
;       bx=cos(angle) [range -unit..unit]
sincos	PROC NEAR
	shr	bx,4-1
	and	bx,not 1
	mov	ax,ds:_sintable[bx]
	add	bx,1024*2
	and	bx,not (8192 or 1)
	mov	bx,ds:_sintable[bx]
	ret
sincos	ENDP

;used for matrix multiply EXPECTS the matrices to be of integer size
mulmacro MACRO	row,col
	mov	eax,ds:[si+0+row*12]
	imul	dword ptr es:[di+0+col*4]
	mov	ebx,eax
	mov	eax,ds:[si+4+row*12]
	imul	dword ptr es:[di+12+col*4]
	add	ebx,eax
	mov	eax,ds:[si+8+row*12]
	imul	dword ptr es:[di+24+col*4]
	add	ebx,eax
	sar	ebx,unitshr
	ENDM

;entry:	fs:si=matrix1, es:di=matrix2
; exit: fs:si=matrix1*matrix2 (matrix 1 overwritten)
mulmatrices PROC NEAR
	push	ds
	mov	ax,fs
	mov	ds,ax
	
	mulmacro 0,0
	push	ebx
	mulmacro 0,1
	push	ebx
	mulmacro 0,2
	push	ebx
	
	mulmacro 1,0
	push	ebx
	mulmacro 1,1
	push	ebx
	mulmacro 1,2
	push	ebx
	
	mulmacro 2,0
	push	ebx
	mulmacro 2,1
	push	ebx
	mulmacro 2,2

	mov	          ds:[si+8+24],ebx
	pop	dword ptr ds:[si+4+24]
	pop	dword ptr ds:[si+0+24]
	pop	dword ptr ds:[si+8+12]
	pop	dword ptr ds:[si+4+12]
	pop	dword ptr ds:[si+0+12]
	pop	dword ptr ds:[si+8]
	pop	dword ptr ds:[si+4]
	pop	dword ptr ds:[si+0]
	pop	ds
	ret
mulmatrices ENDP

;entry:	fs:si=matrix1, es:di=matrix2
; exit: es:di=matrix1*matrix2 (matrix 2 overwritten)
mulmatrices2 PROC NEAR
	push	ds
	mov	ax,fs
	mov	ds,ax
	
	mulmacro 0,0
	push	ebx
	mulmacro 0,1
	push	ebx
	mulmacro 0,2
	push	ebx
	
	mulmacro 1,0
	push	ebx
	mulmacro 1,1
	push	ebx
	mulmacro 1,2
	push	ebx
	
	mulmacro 2,0
	push	ebx
	mulmacro 2,1
	push	ebx
	mulmacro 2,2

	mov	          es:[di+8+24],ebx
	pop	dword ptr es:[di+4+24]
	pop	dword ptr es:[di+0+24]
	pop	dword ptr es:[di+8+12]
	pop	dword ptr es:[di+4+12]
	pop	dword ptr es:[di+0+12]
	pop	dword ptr es:[di+8]
	pop	dword ptr es:[di+4]
	pop	dword ptr es:[di+0]
	pop	ds
	ret
mulmatrices2 ENDP

;entry:	ax=rotx, bx=roty, cx=rotz, es:di=rmatrix.m
; exit: (writes rmatrix.m)
calcmatrixsep PROC NEAR ;calc 3 separate matrices
	push	bp
	mov	bp,sp
	sub	sp,12 ;for local variables
	push	ds
	push	di
	
	push	bx
	push	cx
	mov	bx,ax
	neg	bx
	call	sincos
	mov	ss:[bp-12+0],ax ;rxsin
	mov	ss:[bp-12+2],bx ;rxcos
	pop	bx
	neg	bx
	call	sincos
	mov	ss:[bp-12+4],ax ;rysin
	mov	ss:[bp-12+6],bx ;rycos
	pop	bx
	neg	bx
	call	sincos
	mov	ss:[bp-12+8],ax ;rzsin
	mov	ss:[bp-12+10],bx ;rzcos

	mov	ebx,0
	mov	ecx,unit
	mov	ax,es
	mov	ds,ax
	
	;rX
	mov	ds:[di+2*0],ecx
	mov	ds:[di+2*2],ebx
	mov	ds:[di+2*4],ebx
	mov	ds:[di+2*6],ebx
	movsx	eax,word ptr ss:[bp-12+2]
	mov	ds:[di+2*8],eax
	movsx	eax,word ptr ss:[bp-12+0]
	mov	ds:[di+2*10],eax
	mov	ds:[di+2*12],ebx
	movsx	eax,word ptr ss:[bp-12+0]
	neg	eax
	mov	ds:[di+2*14],eax
	movsx	eax,word ptr ss:[bp-12+2]
	mov	ds:[di+2*16],eax
	add	di,36
	
	;rY
	movsx	eax,word ptr ss:[bp-12+6]
	mov	ds:[di+2*0],eax
	mov	ds:[di+2*2],ebx
	movsx	eax,word ptr ss:[bp-12+4]
	neg	eax
	mov	ds:[di+2*4],eax
	mov	ds:[di+2*6],ebx
	mov	ds:[di+2*8],ecx
	mov	ds:[di+2*10],ebx
	movsx	eax,word ptr ss:[bp-12+4]
	mov	ds:[di+2*12],eax
	mov	ds:[di+2*14],ebx
	movsx	eax,word ptr ss:[bp-12+6]
	mov	ds:[di+2*16],eax
	add	di,36
	
	;rZ
	movsx	eax,word ptr ss:[bp-12+10]
	mov	ds:[di+2*0],eax
	movsx	eax,word ptr ss:[bp-12+8]
	mov	ds:[di+2*2],eax
	mov	ds:[di+2*4],ebx
	movsx	eax,word ptr ss:[bp-12+8]
	neg	eax
	mov	ds:[di+2*6],eax
	movsx	eax,word ptr ss:[bp-12+10]
	mov	ds:[di+2*8],eax
	mov	ds:[di+2*10],ebx
	mov	ds:[di+2*12],ebx
	mov	ds:[di+2*14],ebx
	mov	ds:[di+2*16],ecx

	pop	di
	pop	ds
	mov	sp,bp
	pop	bp
	ret
calcmatrixsep ENDP

;北北北北 _calc_setrmatrix_ident(rmatrix *matrix) 北北北北
;entry: matrix=destination matrix (only rotation fields modified)
; exit:	(data written to matrix)
;descr: Writes an identity rotation matrix to rmatrix
_calc_setrmatrix_ident PROC FAR
	CBEG
	lespar	di,0
	mov	edx,unit
	xor	eax,eax
	mov	es:[di+rmatrix_m+0*4],edx
	mov	es:[di+rmatrix_m+1*4],eax
	mov	es:[di+rmatrix_m+2*4],eax
	mov	es:[di+rmatrix_m+3*4],eax
	mov	es:[di+rmatrix_m+4*4],edx
	mov	es:[di+rmatrix_m+5*4],eax
	mov	es:[di+rmatrix_m+6*4],eax
	mov	es:[di+rmatrix_m+7*4],eax
	mov	es:[di+rmatrix_m+8*4],edx
	CEND
_calc_setrmatrix_ident ENDP

;北北北北 _calc_setrmatrix_rotyxz(rmatrix *matrix,angle rotx,angle roty,angle rotz) 北北北北
;entry: matrix=destination matrix (only rotation fields modified)
;	rotx/y/z=rotation angles
; exit:	(data written to matrix)
;descr: Calculates a rotation matrix
_calc_setrmatrix_rotyxz PROC FAR
	CBEGR	36*3
	mov	ax,ss
	mov	es,ax
	mov	fs,ax
	movpar	ax,2
	movpar	bx,3
	movpar	cx,4
	lea	di,[bp-36*3]
	call	calcmatrixsep
	lea	si,[bp-36*3+36*1]
	lea	di,[bp-36*3+36*0]
	call	mulmatrices ;Y*=X
	lea	si,[bp-36*3+36*1]
	lea	di,[bp-36*3+36*2]
	call	mulmatrices ;Y*=Z
	lespar	di,0
	zzz=0
	REPT	9
	mov	eax,fs:[si+zzz]
	mov	es:[di+rmatrix_m+zzz],eax
	zzz=zzz+4
	ENDM
	CENDR
_calc_setrmatrix_rotyxz ENDP

;北北北北 _calc_setrmatrix_rotxyz(rmatrix *matrix,angle rotx,angle roty,angle rotz) 北北北北
;entry: matrix=destination matrix (only rotation fields modified)
;	rotx/y/z=rotation angles
; exit:	(data written to matrix)
;descr: Calculates a rotation matrix
_calc_setrmatrix_rotxyz PROC FAR
	CBEGR	36*3
	mov	ax,ss
	mov	es,ax
	mov	fs,ax
	movpar	ax,2
	movpar	bx,3
	movpar	cx,4
	lea	di,[bp-36*3]
	call	calcmatrixsep
	lea	si,[bp-36*3+36*0]
	lea	di,[bp-36*3+36*1]
	call	mulmatrices ;X*=Y
	lea	si,[bp-36*3+36*0]
	lea	di,[bp-36*3+36*2]
	call	mulmatrices ;X*=Z
	lespar	di,0
	zzz=0
	REPT	9
	mov	eax,fs:[si+zzz]
	mov	es:[di+rmatrix_m+zzz],eax
	zzz=zzz+4
	ENDM
	CENDR
_calc_setrmatrix_rotxyz ENDP

;北北北北 _calc_setrmatrix_rotzyx(rmatrix *matrix,
;                angle rotx,angle roty,angle rotz) 北北北北
;entry: matrix=destination matrix (only rotation fields modified)
;	rotx/y/z=rotation angles
; exit:	(data written to matrix)
;descr: Calculates a rotation matrix
_calc_setrmatrix_rotzyx PROC FAR
	CBEGR	36*3
	mov	ax,ss
	mov	es,ax
	mov	fs,ax
	movpar	ax,2
	movpar	bx,3
	movpar	cx,4
	lea	di,[bp-36*3]
	call	calcmatrixsep
	lea	si,[bp-36*3+36*2]
	lea	di,[bp-36*3+36*1]
	call	mulmatrices ;Z*=Y
	lea	si,[bp-36*3+36*2]
	lea	di,[bp-36*3+36*0]
	call	mulmatrices ;Z*=X
	lespar	di,0
	zzz=0
	REPT	9
	mov	eax,fs:[si+zzz]
	mov	es:[di+rmatrix_m+zzz],eax
	zzz=zzz+4
	ENDM
	CENDR
_calc_setrmatrix_rotzyx ENDP

;北北北北 _calc_mulrmatrix(rmatrix *dest, rmatrix *source) 北北北北
;entry: dest=destination matrix (matrix modified)
;	source=source matrix (modifying matrix)
; exit:	(data written to dest matrix)
;descr: dest=source*dest. Transposition first, rotation second.
_calc_mulrmatrix PROC FAR
	CBEG
	;fs:si=dest, es:di=source
	lfspar	si,0
	lespar	di,2
	call	mulmatrices
	;fs:si now has the new rotation matrix, next rotate position
	push	bp
	lespar	di,0
	add	di,rmatrix_x
	ldspar	si,2 ;DS destroyed
	lfspar	bp,0
	add	bp,rmatrix_x
	call	rotatesingle
	pop	bp
	;translate (inverse)
	lespar	di,2
	lfspar	si,0
	mov	eax,es:[di+rmatrix_x]
	add	fs:[si+rmatrix_x],eax
	mov	eax,es:[di+rmatrix_y]
	add	fs:[si+rmatrix_y],eax
	mov	eax,es:[di+rmatrix_z]
	add	fs:[si+rmatrix_z],eax
@@x:	CEND
_calc_mulrmatrix ENDP

;北北北北 _calc_applyrmatrix(rmatrix *dest, rmatrix *apply) 北北北北
;entry: dest=destination matrix (matrix modified)
;	apply=apply matrix (modifying matrix)
; exit:	(data written to dest matrix)
;descr: The apply matrix is the camera matrix, the dest contains
;	the objects own rotation/position, which is modified
;	according to the camera.
_calc_applyrmatrix PROC FAR
	CBEG
	;fs:si=source, es:di=dest
	lfspar	si,2
	lespar	di,0
	call	mulmatrices2
	;es:di now has the new rotation matrix, next rotate position
	push	bp
	lespar	di,0
	add	di,rmatrix_x
	ldspar	si,2 ;rotate according to apply matrix
	lfspar	bp,0
	add	bp,rmatrix_x
	call	rotatesingle
	pop	bp
	;translate
	lespar	di,2
	lfspar	si,0
	mov	eax,es:[di+rmatrix_x]
	add	fs:[si+rmatrix_x],eax
	mov	eax,es:[di+rmatrix_y]
	add	fs:[si+rmatrix_y],eax
	mov	eax,es:[di+rmatrix_z]
	add	fs:[si+rmatrix_z],eax
@@x:	CEND
_calc_applyrmatrix ENDP

rotatesingle PROC NEAR
	;ds:si=rmatrix
	;fs:bp=source[]: x,y,z (long)
	;es:di=destination[]: x,y,z (long)
	;destination and source can be same
	mov	eax,fs:[bp+0]
	imul	dword ptr ds:[si+rmatrix_m+2*0]
	mov	ebx,eax
	mov	ecx,edx
	mov	eax,fs:[bp+4]
	imul	dword ptr ds:[si+rmatrix_m+2*2]
	add	ebx,eax
	adc	ecx,edx
	mov	eax,fs:[bp+8]
	imul	dword ptr ds:[si+rmatrix_m+2*4]
	add	ebx,eax
	adc	ecx,edx
	shrd	ebx,ecx,unitshr
	push	ebx
	
	mov	eax,fs:[bp+0]
	imul	dword ptr ds:[si+rmatrix_m+2*6]
	mov	ebx,eax
	mov	ecx,edx
	mov	eax,fs:[bp+4]
	imul	dword ptr ds:[si+rmatrix_m+2*8]
	add	ebx,eax
	adc	ecx,edx
	mov	eax,fs:[bp+8]
	imul	dword ptr ds:[si+rmatrix_m+2*10]
	add	ebx,eax
	adc	ecx,edx
	shrd	ebx,ecx,unitshr
	push	ebx
	
	mov	eax,fs:[bp+0]
	imul	dword ptr ds:[si+rmatrix_m+2*12]
	mov	ebx,eax
	mov	ecx,edx
	mov	eax,fs:[bp+4]
	imul	dword ptr ds:[si+rmatrix_m+2*14]
	add	ebx,eax
	adc	ecx,edx
	mov	eax,fs:[bp+8]
	imul	dword ptr ds:[si+rmatrix_m+2*16]
	add	ebx,eax
	adc	ecx,edx
	shrd	ebx,ecx,unitshr

	mov	          es:[di+8],ebx
	pop	dword ptr es:[di+4]
	pop	dword ptr es:[di+0]
	ret
rotatesingle ENDP

;北北北北 _calc_sftranslate(int count,vlist *dest,long tx,long ty,long tz) 北北北北
;entry:	count=number of vertices to sftranslate
;	dest=destination 3D list
;	matrix=rmatrix containing rotation / moving
; exit: -
;descr: Translates dest with matrix and and (starfield)
_calc_sftranslate PROC FAR
	CBEG
	movpar	ax,0
	or	ax,ax
	jz	@@0
	ldspar	si,1 ;destination
	movpar	bx,3
	movpar	cx,5
	movpar	dx,7
	mov	bp,ax
@@1:
	mov	ax,ds:[si+vlist_x]
	add	ax,bx
	mov	ds:[si+vlist_x],ax

	mov	ax,ds:[si+vlist_y]
	add	ax,cx
	mov	ds:[si+vlist_y],ax

	mov	ax,ds:[si+vlist_z]
	add	ax,dx
	mov	ds:[si+vlist_z],ax

	add	si,vlist_size
	dec	bp
	jnz	@@1
@@0:	CEND
_calc_sftranslate ENDP

;北北北北 _calc_rotate(int count,vlist *dest,vlist *source,rmatrix *matrix) 北北北北
;entry:	count=number of vertices to rotate/move
;	dest=destination 3D list
;	source=source 3D list
;	matrix=rmatrix containing rotation / moving
; exit: -
;descr: Rotates (and moves) the given list
_calc_rotate PROC FAR
	CBEG
	movpar	cx,0
	jcxz	@@0
	lespar	di,1 ;destination
	ldspar	si,5 ;matrix - dataseg not used in procedure, so DS can be used
	lfspar	bp,3 ;source - NOTE: bp/parameter pointer destroyed!
@@1:	push	cx

	movsx	eax,word ptr ds:[si+rmatrix_m+2*0]
	imul	dword ptr fs:[bp+vlist_x]
	mov	ebx,eax
	mov	ecx,edx
	movsx	eax,word ptr ds:[si+rmatrix_m+2*2]
	imul	dword ptr fs:[bp+vlist_y]
	add	ebx,eax
	adc	ecx,edx
	movsx	eax,word ptr ds:[si+rmatrix_m+2*4]
	imul	dword ptr fs:[bp+vlist_z]
	add	ebx,eax
	adc	ecx,edx
	shrd	ebx,ecx,unitshr
	add	ebx,ds:[si+rmatrix_x]
	mov	dword ptr es:[di+vlist_x],ebx
	
	movsx	eax,word ptr ds:[si+rmatrix_m+2*6]
	imul	dword ptr fs:[bp+vlist_x]
	mov	ebx,eax
	mov	ecx,edx
	movsx	eax,word ptr ds:[si+rmatrix_m+2*8]
	imul	dword ptr fs:[bp+vlist_y]
	add	ebx,eax
	adc	ecx,edx
	movsx	eax,word ptr ds:[si+rmatrix_m+2*10]
	imul	dword ptr fs:[bp+vlist_z]
	add	ebx,eax
	adc	ecx,edx
	shrd	ebx,ecx,unitshr
	add	ebx,ds:[si+rmatrix_y]
	mov	dword ptr es:[di+vlist_y],ebx
	
	movsx	eax,word ptr ds:[si+rmatrix_m+2*12]
	imul	dword ptr fs:[bp+vlist_x]
	mov	ebx,eax
	mov	ecx,edx
	movsx	eax,word ptr ds:[si+rmatrix_m+2*14]
	imul	dword ptr fs:[bp+vlist_y]
	add	ebx,eax
	adc	ecx,edx
	movsx	eax,word ptr ds:[si+rmatrix_m+2*16]
	imul	dword ptr fs:[bp+vlist_z]
	add	ebx,eax
	adc	ecx,edx
	shrd	ebx,ecx,unitshr
	add	ebx,ds:[si+rmatrix_z]
	mov	dword ptr es:[di+vlist_z],ebx

	mov	ax,word ptr fs:[bp+vlist_normal]
	mov	word ptr es:[di+vlist_normal],ax
	
	;next point
	add	bp,vlist_size
	add	di,vlist_size
	pop	cx
	loop	@@1
@@0:	CEND
_calc_rotate ENDP

;北北北北 _calc_singlez(int vertex,vlist *vertexlist,rmatrix *matrix) 北北北北
;entry:	vertex=number of vertex to process
;	vertexlist=list from which to pick the vertex
; exit: -
;descr: Rotates the single vertex and returns the resulting Z coordinate.
_calc_singlez PROC FAR
	CBEG
	ldspar	si,3 ;matrix - dataseg not used in procedure, so DS can be used
	movpar	ax,0
	lfspar	bp,1 ;source - NOTE: bp/parameter pointer destroyed!
	shl	ax,vlist_sizeshl
	add	bp,ax

	mov	eax,ds:[si+rmatrix_m+24]
	imul	dword ptr fs:[bp+vlist_x]
	mov	ebx,eax
	mov	ecx,edx
	mov	eax,ds:[si+rmatrix_m+28]
	imul	dword ptr fs:[bp+vlist_y]
	add	ebx,eax
	adc	ecx,edx
	mov	eax,ds:[si+rmatrix_m+32]
	imul	dword ptr fs:[bp+vlist_z]
	add	ebx,eax
	adc	ecx,edx
	shrd	ebx,ecx,unitshr
	add	ebx,ds:[si+rmatrix_z]

	mov	ax,bx
	shr	ebx,16
	mov	dx,bx
	CEND
_calc_singlez ENDP

;北北北北 _calc_nrotate(int count,nlist *dest,nlist *source,rmatrix *matrix) 北北北北
;entry:	count=number of normals to rotate
;	dest=destination 3Dnormal list
;	source=source 3Dnormal list
;	matrix=rmatrix containing rotation (moving part of rmatrix not used)
; exit: -
;descr: Rotates the given normal list
_calc_nrotate PROC FAR
	CBEG
	movpar	cx,0
	jcxz	@@0
	lespar	di,1 ;destination
	ldspar	si,5 ;matrix - dataseg not used in procedure, so DS can be used
	lfspar	bp,3 ;source - NOTE: bp/parameter pointer destroyed!
@@1:	push	cx

	mov	ax,word ptr fs:[bp+nlist_x]
	imul	word ptr ds:[si+rmatrix_m+2*0]
	mov	bx,ax
	mov	cx,dx
	mov	ax,word ptr fs:[bp+nlist_y]
	imul	word ptr ds:[si+rmatrix_m+2*2]
	add	bx,ax
	adc	cx,dx
	mov	ax,word ptr fs:[bp+nlist_z]
	imul	word ptr ds:[si+rmatrix_m+2*4]
	add	bx,ax
	adc	cx,dx
	shrd	bx,cx,unitshr
	mov	word ptr es:[di+nlist_x],bx
	
	mov	ax,word ptr fs:[bp+nlist_x]
	imul	word ptr ds:[si+rmatrix_m+2*6]
	mov	bx,ax
	mov	cx,dx
	mov	ax,word ptr fs:[bp+nlist_y]
	imul	word ptr ds:[si+rmatrix_m+2*8]
	add	bx,ax
	adc	cx,dx
	mov	ax,word ptr fs:[bp+nlist_z]
	imul	word ptr ds:[si+rmatrix_m+2*10]
	add	bx,ax
	adc	cx,dx
	shrd	bx,cx,unitshr
	mov	word ptr es:[di+nlist_y],bx
	
	mov	ax,word ptr fs:[bp+nlist_x]
	imul	word ptr ds:[si+rmatrix_m+2*12]
	mov	bx,ax
	mov	cx,dx
	mov	ax,word ptr fs:[bp+nlist_y]
	imul	word ptr ds:[si+rmatrix_m+2*14]
	add	bx,ax
	adc	cx,dx
	mov	ax,word ptr fs:[bp+nlist_z]
	imul	word ptr ds:[si+rmatrix_m+2*16]
	add	bx,ax
	adc	cx,dx
	shrd	bx,cx,unitshr
	mov	word ptr es:[di+nlist_z],bx

	;next point
	add	bp,nlist_size
	add	di,nlist_size
	pop	cx
	loop	@@1
@@0:	CEND
_calc_nrotate ENDP

;北北北北 _calc_rotate16(int count,nlist *dest,nlist *source,rmatrix *matrix) 北北北北
;entry:	count=number of normals to rotate
;	dest=destination 3Dnormal list
;	source=source 3Dnormal list
;	matrix=rmatrix containing rotation (moving part of rmatrix not used)
; exit: -
;descr: Rotates the given normal list
_calc_rotate16 PROC FAR
	CBEG
	movpar	cx,0
	jcxz	@@0
	lespar	di,1 ;destination
	ldspar	si,5 ;matrix - dataseg not used in procedure, so DS can be used
	lfspar	bp,3 ;source - NOTE: bp/parameter pointer destroyed!
@@1:	push	cx

	mov	ax,word ptr fs:[bp+vlist_x]
	imul	word ptr ds:[si+rmatrix_m+2*0]
	mov	bx,ax
	mov	cx,dx
	mov	ax,word ptr fs:[bp+vlist_y]
	imul	word ptr ds:[si+rmatrix_m+2*2]
	add	bx,ax
	adc	cx,dx
	mov	ax,word ptr fs:[bp+vlist_z]
	imul	word ptr ds:[si+rmatrix_m+2*4]
	add	bx,ax
	adc	cx,dx
	shrd	bx,cx,unitshr
	mov	word ptr es:[di+vlist_x],bx
	
	mov	ax,word ptr fs:[bp+vlist_x]
	imul	word ptr ds:[si+rmatrix_m+2*6]
	mov	bx,ax
	mov	cx,dx
	mov	ax,word ptr fs:[bp+vlist_y]
	imul	word ptr ds:[si+rmatrix_m+2*8]
	add	bx,ax
	adc	cx,dx
	mov	ax,word ptr fs:[bp+vlist_z]
	imul	word ptr ds:[si+rmatrix_m+2*10]
	add	bx,ax
	adc	cx,dx
	shrd	bx,cx,unitshr
	mov	word ptr es:[di+vlist_y],bx
	
	mov	ax,word ptr fs:[bp+vlist_x]
	imul	word ptr ds:[si+rmatrix_m+2*12]
	mov	bx,ax
	mov	cx,dx
	mov	ax,word ptr fs:[bp+vlist_y]
	imul	word ptr ds:[si+rmatrix_m+2*14]
	add	bx,ax
	adc	cx,dx
	mov	ax,word ptr fs:[bp+vlist_z]
	imul	word ptr ds:[si+rmatrix_m+2*16]
	add	bx,ax
	adc	cx,dx
	shrd	bx,cx,unitshr
	mov	word ptr es:[di+vlist_z],bx

	;next point
	add	bp,vlist_size
	add	di,vlist_size
	pop	cx
	loop	@@1
@@0:	CEND
_calc_rotate16 ENDP

;北北北北 _calc_project(int count,pvlist *dest,vlist *source) 北北北北
;entry:	count=number of vertices to project
;	dest=destination projected list
;	source=source 3D list
;	(_proj* variables in data segment define the projection)
; exit: logical and of visibility flags for all vertices (!=0 == object invis.)
;descr: Projects the given list = does perspective transformation
_calc_project PROC FAR
	CBEG
	lfspar	si,3
	lespar	di,1
	mov	ax,0ffffh
	movpar	cx,0
	jcxz	@@0
@@1:	push	cx
	push	ax
	
	mov	ecx,fs:[si+vlist_x]
	mov	eax,fs:[si+vlist_y]
	mov	ebx,fs:[si+vlist_z]
	
	xor	bp,bp
	cmp	ebx,ds:_projclipz[CLIPMIN]
	jge	@@21
	or	bp,VF_NEAR
	mov	ebx,ds:_projclipz[CLIPMIN]
	jmp	@@22
@@21:	cmp	ebx,ds:_projclipz[CLIPMAX]
	jle	@@22
	or	bp,VF_FAR
@@22:	;
	imul	ds:_projmuly
	idiv	ebx
	add	eax,ds:_projaddy
	cmp	eax,ds:_projclipy[CLIPMAX]
	jng	@@41
	or	bp,VF_DOWN
@@41:	cmp	eax,ds:_projclipy[CLIPMIN]
	jnl	@@42
	or	bp,VF_UP
@@42:	mov	es:[di+pvlist_y],ax ;store Y
	;
	mov	eax,ds:_projmulx
	imul	ecx
	idiv	ebx
	add	eax,ds:_projaddx
	cmp	eax,ds:_projclipx[CLIPMAX]
	jng	@@43
	or	bp,VF_RIGHT
@@43:	cmp	eax,ds:_projclipx[CLIPMIN]
	jnl	@@44
	or	bp,VF_LEFT
@@44:	mov	es:[di+pvlist_x],ax ;store X

@@5:	mov	es:[di+pvlist_vf],bp ;store visiblity flags
	
	;next point
	add	si,vlist_size
	add	di,pvlist_size

	pop	ax	
	pop	cx
	and	ax,bp
	loop	@@1
@@0:	CEND
_calc_project ENDP
	
;北北北北 _calc_project16(int count,pvlist *dest,vlist *source) 北北北北
;entry:	count=number of vertices to project
;	dest=destination projected list
;	source=source 3D list
;	(_proj* variables in data segment define the projection)
; exit: logical and of visibility flags for all vertices (!=0 == object invis.)
;descr: Projects the given list = does perspective transformation
_calc_project16 PROC FAR
	CBEG
	lfspar	si,3
	lespar	di,1
	mov	ax,0ffffh
	movpar	cx,0
	jcxz	@@0
@@1:	push	cx
	push	ax
	
	movsx	ecx,word ptr fs:[si+vlist_x]
	movsx	eax,word ptr fs:[si+vlist_y]
	movsx	ebx,word ptr fs:[si+vlist_z]
	
	xor	bp,bp
	cmp	ebx,ds:_projclipz[CLIPMIN]
	jge	@@21
	or	bp,VF_NEAR
	mov	ebx,ds:_projclipz[CLIPMIN]
	jmp	@@22
@@21:	cmp	ebx,ds:_projclipz[CLIPMAX]
	jle	@@22
	or	bp,VF_FAR
@@22:	;
	imul	ds:_projmuly
	idiv	ebx
	add	eax,ds:_projaddy
	cmp	eax,ds:_projclipy[CLIPMAX]
	jng	@@41
	or	bp,VF_DOWN
@@41:	cmp	eax,ds:_projclipy[CLIPMIN]
	jnl	@@42
	or	bp,VF_UP
@@42:	mov	es:[di+pvlist_y],ax ;store Y
	;
	mov	eax,ds:_projmulx
	imul	ecx
	idiv	ebx
	add	eax,ds:_projaddx
	cmp	eax,ds:_projclipx[CLIPMAX]
	jng	@@43
	or	bp,VF_RIGHT
@@43:	cmp	eax,ds:_projclipx[CLIPMIN]
	jnl	@@44
	or	bp,VF_LEFT
@@44:	mov	es:[di+pvlist_x],ax ;store X

@@5:	mov	es:[di+pvlist_vf],bp ;store visiblity flags
	
	;next point
	add	si,vlist_size
	add	di,pvlist_size

	pop	ax	
	pop	cx
	and	ax,bp
	loop	@@1
@@0:	CEND
_calc_project16 ENDP
	
asm_code ENDS
	END
