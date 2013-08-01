;Register usage:		
;eax	low...	ray heigth 	
;ebx	-       scenary heigth	
;ecx	low...	ray direction 	(always negative)
;edx	-	tmp		
;esi	-	xsin		
;edi	-	ysin		
;ebp	-	p->screen	
;ds	->codesegment
;es	->screenbuffer
;fs	->wavetablex
;gs	->wavetabley

ALIGN 16 ;following should stay in cache
theloop_waveseg dw 0
theloop_xsina1 dw 0
theloop_ysina1 dw 0
theloop_xsina2 dw 0
theloop_ysina2 dw 0
theloop_heigth dw 0

;entry: es=videbuf
;	bp=pointer to bottom of current column in videbuf
;	fs=waveXsegment
;	gs=waveYsegment
;	si=waveXpos
;	di=waveYpos
;	cx=waveXadd
;	dx=waveYadd
;	ax=camera heigth adder
theloop PROC FAR
mov cs:theloop_xsina1,cx
mov cs:theloop_ysina1,dx
add cx,cx
mov cs:theloop_xsina2,cx
add dx,dx
mov cs:theloop_ysina2,dx
mov cs:theloop_heigth,ax
mov ax,cs
mov ds,ax
xor eax,eax
mov ecx,0EC00FFFAh
_@seek0:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-240
cmp ax,bx
jge short _@seeko0
lea dx,[bx+140]
shr dl,1
_@hit0:
add eax,000000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a0
add eax,000000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a0
add eax,000000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko0
jmp _@hit0
_@seek2a0:
sub bp,160
add ecx,00A000000h
_@seek1a0:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko0:
add eax,ecx
adc ax,-1
_@seek1:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-239
cmp ax,bx
jge short _@seeko1
lea dx,[bx+140]
shr dl,1
_@hit1:
add eax,00A000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a1
add eax,00A000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a1
add eax,00A000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko1
jmp _@hit1
_@seek2a1:
sub bp,160
add ecx,00A000000h
_@seek1a1:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko1:
add eax,ecx
adc ax,-1
_@seek2:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-237
cmp ax,bx
jge short _@seeko2
lea dx,[bx+140]
shr dl,1
_@hit2:
add eax,014000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a2
add eax,014000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a2
add eax,014000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko2
jmp _@hit2
_@seek2a2:
sub bp,160
add ecx,00A000000h
_@seek1a2:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko2:
add eax,ecx
adc ax,-1
_@seek3:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-236
cmp ax,bx
jge short _@seeko3
lea dx,[bx+140]
shr dl,1
_@hit3:
add eax,01E000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a3
add eax,01E000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a3
add eax,01E000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko3
jmp _@hit3
_@seek2a3:
sub bp,160
add ecx,00A000000h
_@seek1a3:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko3:
add eax,ecx
adc ax,-1
_@seek4:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-234
cmp ax,bx
jge short _@seeko4
lea dx,[bx+140]
shr dl,1
_@hit4:
add eax,028000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a4
add eax,028000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a4
add eax,028000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko4
jmp _@hit4
_@seek2a4:
sub bp,160
add ecx,00A000000h
_@seek1a4:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko4:
add eax,ecx
adc ax,-1
_@seek5:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-233
cmp ax,bx
jge short _@seeko5
lea dx,[bx+140]
shr dl,1
_@hit5:
add eax,032000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a5
add eax,032000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a5
add eax,032000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko5
jmp _@hit5
_@seek2a5:
sub bp,160
add ecx,00A000000h
_@seek1a5:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko5:
add eax,ecx
adc ax,-1
_@seek6:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-232
cmp ax,bx
jge short _@seeko6
lea dx,[bx+140]
shr dl,1
_@hit6:
add eax,03C000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a6
add eax,03C000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a6
add eax,03C000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko6
jmp _@hit6
_@seek2a6:
sub bp,160
add ecx,00A000000h
_@seek1a6:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko6:
add eax,ecx
adc ax,-1
_@seek7:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-230
cmp ax,bx
jge short _@seeko7
lea dx,[bx+140]
shr dl,1
_@hit7:
add eax,046000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a7
add eax,046000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a7
add eax,046000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko7
jmp _@hit7
_@seek2a7:
sub bp,160
add ecx,00A000000h
_@seek1a7:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko7:
add eax,ecx
adc ax,-1
_@seek8:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-229
cmp ax,bx
jge short _@seeko8
lea dx,[bx+139]
shr dl,1
_@hit8:
add eax,050000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a8
add eax,050000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a8
add eax,050000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko8
jmp _@hit8
_@seek2a8:
sub bp,160
add ecx,00A000000h
_@seek1a8:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko8:
add eax,ecx
adc ax,-1
_@seek9:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-228
cmp ax,bx
jge short _@seeko9
lea dx,[bx+139]
shr dl,1
_@hit9:
add eax,05A000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a9
add eax,05A000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a9
add eax,05A000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko9
jmp _@hit9
_@seek2a9:
sub bp,160
add ecx,00A000000h
_@seek1a9:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko9:
add eax,ecx
adc ax,-1
_@seek10:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-227
cmp ax,bx
jge short _@seeko10
lea dx,[bx+139]
shr dl,1
_@hit10:
add eax,064000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a10
add eax,064000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a10
add eax,064000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko10
jmp _@hit10
_@seek2a10:
sub bp,160
add ecx,00A000000h
_@seek1a10:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko10:
add eax,ecx
adc ax,-1
_@seek11:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-226
cmp ax,bx
jge short _@seeko11
lea dx,[bx+139]
shr dl,1
_@hit11:
add eax,06E000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a11
add eax,06E000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a11
add eax,06E000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko11
jmp _@hit11
_@seek2a11:
sub bp,160
add ecx,00A000000h
_@seek1a11:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko11:
add eax,ecx
adc ax,-1
_@seek12:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-226
cmp ax,bx
jge short _@seeko12
lea dx,[bx+139]
shr dl,1
_@hit12:
add eax,078000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a12
add eax,078000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a12
add eax,078000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko12
jmp _@hit12
_@seek2a12:
sub bp,160
add ecx,00A000000h
_@seek1a12:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko12:
add eax,ecx
adc ax,-1
_@seek13:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-225
cmp ax,bx
jge short _@seeko13
lea dx,[bx+139]
shr dl,1
_@hit13:
add eax,082000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a13
add eax,082000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a13
add eax,082000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko13
jmp _@hit13
_@seek2a13:
sub bp,160
add ecx,00A000000h
_@seek1a13:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko13:
add eax,ecx
adc ax,-1
_@seek14:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-225
cmp ax,bx
jge short _@seeko14
lea dx,[bx+139]
shr dl,1
_@hit14:
add eax,08C000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a14
add eax,08C000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a14
add eax,08C000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko14
jmp _@hit14
_@seek2a14:
sub bp,160
add ecx,00A000000h
_@seek1a14:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko14:
add eax,ecx
adc ax,-1
_@seek15:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-225
cmp ax,bx
jge short _@seeko15
lea dx,[bx+139]
shr dl,1
_@hit15:
add eax,096000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a15
add eax,096000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a15
add eax,096000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko15
jmp _@hit15
_@seek2a15:
sub bp,160
add ecx,00A000000h
_@seek1a15:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko15:
add eax,ecx
adc ax,-1
_@seek16:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-224
cmp ax,bx
jge short _@seeko16
lea dx,[bx+138]
shr dl,1
_@hit16:
add eax,0A0000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a16
add eax,0A0000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a16
add eax,0A0000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko16
jmp _@hit16
_@seek2a16:
sub bp,160
add ecx,00A000000h
_@seek1a16:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko16:
add eax,ecx
adc ax,-1
_@seek17:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-225
cmp ax,bx
jge short _@seeko17
lea dx,[bx+138]
shr dl,1
_@hit17:
add eax,0AA000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a17
add eax,0AA000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a17
add eax,0AA000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko17
jmp _@hit17
_@seek2a17:
sub bp,160
add ecx,00A000000h
_@seek1a17:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko17:
add eax,ecx
adc ax,-1
_@seek18:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-225
cmp ax,bx
jge short _@seeko18
lea dx,[bx+138]
shr dl,1
_@hit18:
add eax,0B4000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a18
add eax,0B4000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a18
add eax,0B4000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko18
jmp _@hit18
_@seek2a18:
sub bp,160
add ecx,00A000000h
_@seek1a18:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko18:
add eax,ecx
adc ax,-1
_@seek19:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-225
cmp ax,bx
jge short _@seeko19
lea dx,[bx+138]
shr dl,1
_@hit19:
add eax,0BE000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a19
add eax,0BE000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a19
add eax,0BE000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko19
jmp _@hit19
_@seek2a19:
sub bp,160
add ecx,00A000000h
_@seek1a19:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko19:
add eax,ecx
adc ax,-1
_@seek20:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-226
cmp ax,bx
jge short _@seeko20
lea dx,[bx+138]
shr dl,1
_@hit20:
add eax,0C8000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a20
add eax,0C8000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a20
add eax,0C8000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko20
jmp _@hit20
_@seek2a20:
sub bp,160
add ecx,00A000000h
_@seek1a20:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko20:
add eax,ecx
adc ax,-1
_@seek21:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-226
cmp ax,bx
jge short _@seeko21
lea dx,[bx+138]
shr dl,1
_@hit21:
add eax,0D2000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a21
add eax,0D2000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a21
add eax,0D2000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko21
jmp _@hit21
_@seek2a21:
sub bp,160
add ecx,00A000000h
_@seek1a21:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko21:
add eax,ecx
adc ax,-1
_@seek22:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-227
cmp ax,bx
jge short _@seeko22
lea dx,[bx+138]
shr dl,1
_@hit22:
add eax,0DC000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a22
add eax,0DC000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a22
add eax,0DC000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko22
jmp _@hit22
_@seek2a22:
sub bp,160
add ecx,00A000000h
_@seek1a22:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko22:
add eax,ecx
adc ax,-1
_@seek23:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-228
cmp ax,bx
jge short _@seeko23
lea dx,[bx+138]
shr dl,1
_@hit23:
add eax,0E6000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a23
add eax,0E6000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a23
add eax,0E6000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko23
jmp _@hit23
_@seek2a23:
sub bp,160
add ecx,00A000000h
_@seek1a23:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko23:
add eax,ecx
adc ax,-1
_@seek24:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-229
cmp ax,bx
jge short _@seeko24
lea dx,[bx+137]
shr dl,1
_@hit24:
add eax,0F0000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a24
add eax,0F0000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a24
add eax,0F0000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko24
jmp _@hit24
_@seek2a24:
sub bp,160
add ecx,00A000000h
_@seek1a24:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko24:
add eax,ecx
adc ax,-1
_@seek25:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-230
cmp ax,bx
jge short _@seeko25
lea dx,[bx+137]
shr dl,1
_@hit25:
add eax,0FA000000h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a25
add eax,0FA000000h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a25
add eax,0FA000000h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko25
jmp _@hit25
_@seek2a25:
sub bp,160
add ecx,00A000000h
_@seek1a25:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko25:
add eax,ecx
adc ax,-1
_@seek26:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-232
cmp ax,bx
jge short _@seeko26
lea dx,[bx+137]
shr dl,1
_@hit26:
add eax,004000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a26
add eax,004000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a26
add eax,004000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko26
jmp _@hit26
_@seek2a26:
sub bp,160
add ecx,00A000000h
_@seek1a26:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko26:
add eax,ecx
adc ax,-1
_@seek27:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-233
cmp ax,bx
jge short _@seeko27
lea dx,[bx+137]
shr dl,1
_@hit27:
add eax,00E000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a27
add eax,00E000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a27
add eax,00E000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko27
jmp _@hit27
_@seek2a27:
sub bp,160
add ecx,00A000000h
_@seek1a27:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko27:
add eax,ecx
adc ax,-1
_@seek28:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-234
cmp ax,bx
jge short _@seeko28
lea dx,[bx+137]
shr dl,1
_@hit28:
add eax,018000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a28
add eax,018000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a28
add eax,018000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko28
jmp _@hit28
_@seek2a28:
sub bp,160
add ecx,00A000000h
_@seek1a28:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko28:
add eax,ecx
adc ax,-1
_@seek29:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-236
cmp ax,bx
jge short _@seeko29
lea dx,[bx+137]
shr dl,1
_@hit29:
add eax,022000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a29
add eax,022000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a29
add eax,022000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko29
jmp _@hit29
_@seek2a29:
sub bp,160
add ecx,00A000000h
_@seek1a29:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko29:
add eax,ecx
adc ax,-1
_@seek30:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-237
cmp ax,bx
jge short _@seeko30
lea dx,[bx+137]
shr dl,1
_@hit30:
add eax,02C000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a30
add eax,02C000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a30
add eax,02C000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko30
jmp _@hit30
_@seek2a30:
sub bp,160
add ecx,00A000000h
_@seek1a30:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko30:
add eax,ecx
adc ax,-1
_@seek31:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-239
cmp ax,bx
jge short _@seeko31
lea dx,[bx+137]
shr dl,1
_@hit31:
add eax,036000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a31
add eax,036000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a31
add eax,036000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko31
jmp _@hit31
_@seek2a31:
sub bp,160
add ecx,00A000000h
_@seek1a31:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko31:
add eax,ecx
adc ax,-1
_@seek32:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-240
cmp ax,bx
jge short _@seeko32
lea dx,[bx+136]
shr dl,1
_@hit32:
add eax,040000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a32
add eax,040000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a32
add eax,040000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko32
jmp _@hit32
_@seek2a32:
sub bp,160
add ecx,00A000000h
_@seek1a32:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko32:
add eax,ecx
adc ax,-1
_@seek33:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-241
cmp ax,bx
jge short _@seeko33
lea dx,[bx+136]
shr dl,1
_@hit33:
add eax,04A000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a33
add eax,04A000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a33
add eax,04A000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko33
jmp _@hit33
_@seek2a33:
sub bp,160
add ecx,00A000000h
_@seek1a33:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko33:
add eax,ecx
adc ax,-1
_@seek34:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-243
cmp ax,bx
jge short _@seeko34
lea dx,[bx+136]
shr dl,1
_@hit34:
add eax,054000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a34
add eax,054000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a34
add eax,054000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko34
jmp _@hit34
_@seek2a34:
sub bp,160
add ecx,00A000000h
_@seek1a34:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko34:
add eax,ecx
adc ax,-1
_@seek35:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-244
cmp ax,bx
jge short _@seeko35
lea dx,[bx+136]
shr dl,1
_@hit35:
add eax,05E000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a35
add eax,05E000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a35
add eax,05E000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko35
jmp _@hit35
_@seek2a35:
sub bp,160
add ecx,00A000000h
_@seek1a35:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko35:
add eax,ecx
adc ax,-1
_@seek36:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-246
cmp ax,bx
jge short _@seeko36
lea dx,[bx+136]
shr dl,1
_@hit36:
add eax,068000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a36
add eax,068000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a36
add eax,068000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko36
jmp _@hit36
_@seek2a36:
sub bp,160
add ecx,00A000000h
_@seek1a36:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko36:
add eax,ecx
adc ax,-1
_@seek37:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-247
cmp ax,bx
jge short _@seeko37
lea dx,[bx+136]
shr dl,1
_@hit37:
add eax,072000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a37
add eax,072000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a37
add eax,072000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko37
jmp _@hit37
_@seek2a37:
sub bp,160
add ecx,00A000000h
_@seek1a37:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko37:
add eax,ecx
adc ax,-1
_@seek38:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-248
cmp ax,bx
jge short _@seeko38
lea dx,[bx+136]
shr dl,1
_@hit38:
add eax,07C000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a38
add eax,07C000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a38
add eax,07C000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko38
jmp _@hit38
_@seek2a38:
sub bp,160
add ecx,00A000000h
_@seek1a38:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko38:
add eax,ecx
adc ax,-1
_@seek39:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-250
cmp ax,bx
jge short _@seeko39
lea dx,[bx+136]
shr dl,1
_@hit39:
add eax,086000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a39
add eax,086000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a39
add eax,086000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko39
jmp _@hit39
_@seek2a39:
sub bp,160
add ecx,00A000000h
_@seek1a39:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko39:
add eax,ecx
adc ax,-1
_@seek40:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-251
cmp ax,bx
jge short _@seeko40
lea dx,[bx+135]
shr dl,1
_@hit40:
add eax,090000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a40
add eax,090000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a40
add eax,090000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko40
jmp _@hit40
_@seek2a40:
sub bp,160
add ecx,00A000000h
_@seek1a40:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko40:
add eax,ecx
adc ax,-1
_@seek41:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-252
cmp ax,bx
jge short _@seeko41
lea dx,[bx+135]
shr dl,1
_@hit41:
add eax,09A000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a41
add eax,09A000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a41
add eax,09A000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko41
jmp _@hit41
_@seek2a41:
sub bp,160
add ecx,00A000000h
_@seek1a41:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko41:
add eax,ecx
adc ax,-1
_@seek42:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-253
cmp ax,bx
jge short _@seeko42
lea dx,[bx+135]
shr dl,1
_@hit42:
add eax,0A4000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a42
add eax,0A4000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a42
add eax,0A4000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko42
jmp _@hit42
_@seek2a42:
sub bp,160
add ecx,00A000000h
_@seek1a42:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko42:
add eax,ecx
adc ax,-1
_@seek43:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-254
cmp ax,bx
jge short _@seeko43
lea dx,[bx+135]
shr dl,1
_@hit43:
add eax,0AE000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a43
add eax,0AE000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a43
add eax,0AE000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko43
jmp _@hit43
_@seek2a43:
sub bp,160
add ecx,00A000000h
_@seek1a43:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko43:
add eax,ecx
adc ax,-1
_@seek44:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-254
cmp ax,bx
jge short _@seeko44
lea dx,[bx+135]
shr dl,1
_@hit44:
add eax,0B8000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a44
add eax,0B8000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a44
add eax,0B8000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko44
jmp _@hit44
_@seek2a44:
sub bp,160
add ecx,00A000000h
_@seek1a44:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko44:
add eax,ecx
adc ax,-1
_@seek45:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-255
cmp ax,bx
jge short _@seeko45
lea dx,[bx+135]
shr dl,1
_@hit45:
add eax,0C2000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a45
add eax,0C2000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a45
add eax,0C2000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko45
jmp _@hit45
_@seek2a45:
sub bp,160
add ecx,00A000000h
_@seek1a45:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko45:
add eax,ecx
adc ax,-1
_@seek46:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-255
cmp ax,bx
jge short _@seeko46
lea dx,[bx+135]
shr dl,1
_@hit46:
add eax,0CC000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a46
add eax,0CC000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a46
add eax,0CC000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko46
jmp _@hit46
_@seek2a46:
sub bp,160
add ecx,00A000000h
_@seek1a46:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko46:
add eax,ecx
adc ax,-1
_@seek47:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-255
cmp ax,bx
jge short _@seeko47
lea dx,[bx+135]
shr dl,1
_@hit47:
add eax,0D6000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a47
add eax,0D6000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a47
add eax,0D6000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko47
jmp _@hit47
_@seek2a47:
sub bp,160
add ecx,00A000000h
_@seek1a47:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko47:
add eax,ecx
adc ax,-1
_@seek48:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-256
cmp ax,bx
jge short _@seeko48
lea dx,[bx+134]
shr dl,1
_@hit48:
add eax,0E0000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a48
add eax,0E0000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a48
add eax,0E0000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko48
jmp _@hit48
_@seek2a48:
sub bp,160
add ecx,00A000000h
_@seek1a48:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko48:
add eax,ecx
adc ax,-1
_@seek49:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-255
cmp ax,bx
jge short _@seeko49
lea dx,[bx+134]
shr dl,1
_@hit49:
add eax,0EA000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a49
add eax,0EA000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a49
add eax,0EA000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko49
jmp _@hit49
_@seek2a49:
sub bp,160
add ecx,00A000000h
_@seek1a49:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko49:
add eax,ecx
adc ax,-1
_@seek50:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-255
cmp ax,bx
jge short _@seeko50
lea dx,[bx+134]
shr dl,1
_@hit50:
add eax,0F4000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a50
add eax,0F4000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a50
add eax,0F4000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko50
jmp _@hit50
_@seek2a50:
sub bp,160
add ecx,00A000000h
_@seek1a50:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko50:
add eax,ecx
adc ax,-1
_@seek51:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-255
cmp ax,bx
jge short _@seeko51
lea dx,[bx+134]
shr dl,1
_@hit51:
add eax,0FE000001h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a51
add eax,0FE000001h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a51
add eax,0FE000001h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko51
jmp _@hit51
_@seek2a51:
sub bp,160
add ecx,00A000000h
_@seek1a51:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko51:
add eax,ecx
adc ax,-1
_@seek52:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-254
cmp ax,bx
jge short _@seeko52
lea dx,[bx+134]
shr dl,1
_@hit52:
add eax,008000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a52
add eax,008000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a52
add eax,008000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko52
jmp _@hit52
_@seek2a52:
sub bp,160
add ecx,00A000000h
_@seek1a52:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko52:
add eax,ecx
adc ax,-1
_@seek53:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-254
cmp ax,bx
jge short _@seeko53
lea dx,[bx+134]
shr dl,1
_@hit53:
add eax,012000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a53
add eax,012000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a53
add eax,012000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko53
jmp _@hit53
_@seek2a53:
sub bp,160
add ecx,00A000000h
_@seek1a53:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko53:
add eax,ecx
adc ax,-1
_@seek54:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-253
cmp ax,bx
jge short _@seeko54
lea dx,[bx+134]
shr dl,1
_@hit54:
add eax,01C000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a54
add eax,01C000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a54
add eax,01C000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko54
jmp _@hit54
_@seek2a54:
sub bp,160
add ecx,00A000000h
_@seek1a54:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko54:
add eax,ecx
adc ax,-1
_@seek55:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-252
cmp ax,bx
jge short _@seeko55
lea dx,[bx+134]
shr dl,1
_@hit55:
add eax,026000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a55
add eax,026000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a55
add eax,026000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko55
jmp _@hit55
_@seek2a55:
sub bp,160
add ecx,00A000000h
_@seek1a55:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko55:
add eax,ecx
adc ax,-1
_@seek56:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-251
cmp ax,bx
jge short _@seeko56
lea dx,[bx+133]
shr dl,1
_@hit56:
add eax,030000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a56
add eax,030000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a56
add eax,030000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko56
jmp _@hit56
_@seek2a56:
sub bp,160
add ecx,00A000000h
_@seek1a56:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko56:
add eax,ecx
adc ax,-1
_@seek57:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-250
cmp ax,bx
jge short _@seeko57
lea dx,[bx+133]
shr dl,1
_@hit57:
add eax,03A000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a57
add eax,03A000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a57
add eax,03A000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko57
jmp _@hit57
_@seek2a57:
sub bp,160
add ecx,00A000000h
_@seek1a57:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko57:
add eax,ecx
adc ax,-1
_@seek58:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-248
cmp ax,bx
jge short _@seeko58
lea dx,[bx+133]
shr dl,1
_@hit58:
add eax,044000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a58
add eax,044000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a58
add eax,044000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko58
jmp _@hit58
_@seek2a58:
sub bp,160
add ecx,00A000000h
_@seek1a58:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko58:
add eax,ecx
adc ax,-1
_@seek59:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-247
cmp ax,bx
jge short _@seeko59
lea dx,[bx+133]
shr dl,1
_@hit59:
add eax,04E000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a59
add eax,04E000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a59
add eax,04E000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko59
jmp _@hit59
_@seek2a59:
sub bp,160
add ecx,00A000000h
_@seek1a59:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko59:
add eax,ecx
adc ax,-1
_@seek60:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-246
cmp ax,bx
jge short _@seeko60
lea dx,[bx+133]
shr dl,1
_@hit60:
add eax,058000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a60
add eax,058000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a60
add eax,058000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko60
jmp _@hit60
_@seek2a60:
sub bp,160
add ecx,00A000000h
_@seek1a60:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko60:
add eax,ecx
adc ax,-1
_@seek61:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-244
cmp ax,bx
jge short _@seeko61
lea dx,[bx+133]
shr dl,1
_@hit61:
add eax,062000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a61
add eax,062000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a61
add eax,062000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko61
jmp _@hit61
_@seek2a61:
sub bp,160
add ecx,00A000000h
_@seek1a61:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko61:
add eax,ecx
adc ax,-1
_@seek62:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-243
cmp ax,bx
jge short _@seeko62
lea dx,[bx+133]
shr dl,1
_@hit62:
add eax,06C000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a62
add eax,06C000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a62
add eax,06C000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko62
jmp _@hit62
_@seek2a62:
sub bp,160
add ecx,00A000000h
_@seek1a62:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko62:
add eax,ecx
adc ax,-1
_@seek63:
add si,ds:theloop_xsina1
mov bx,fs:[si]
add di,ds:theloop_ysina1
add bx,gs:[di]
add bx,-241
cmp ax,bx
jge short _@seeko63
lea dx,[bx+133]
shr dl,1
_@hit63:
add eax,076000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a63
add eax,076000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a63
add eax,076000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko63
jmp _@hit63
_@seek2a63:
sub bp,160
add ecx,00A000000h
_@seek1a63:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko63:
add eax,ecx
adc ax,-1
_@seek64:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-240
cmp ax,bx
jge short _@seeko64
lea dx,[bx+132]
shr dl,1
_@hit64:
add eax,080000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a64
add eax,080000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a64
add eax,080000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko64
jmp _@hit64
_@seek2a64:
sub bp,160
add ecx,00A000000h
_@seek1a64:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko64:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek66:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-237
cmp ax,bx
jge short _@seeko66
lea dx,[bx+132]
shr dl,1
_@hit66:
add eax,094000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a66
add eax,094000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a66
add eax,094000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko66
jmp _@hit66
_@seek2a66:
sub bp,160
add ecx,00A000000h
_@seek1a66:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko66:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek68:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-234
cmp ax,bx
jge short _@seeko68
lea dx,[bx+132]
shr dl,1
_@hit68:
add eax,0A8000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a68
add eax,0A8000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a68
add eax,0A8000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko68
jmp _@hit68
_@seek2a68:
sub bp,160
add ecx,00A000000h
_@seek1a68:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko68:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek70:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-232
cmp ax,bx
jge short _@seeko70
lea dx,[bx+132]
shr dl,1
_@hit70:
add eax,0BC000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a70
add eax,0BC000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a70
add eax,0BC000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko70
jmp _@hit70
_@seek2a70:
sub bp,160
add ecx,00A000000h
_@seek1a70:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko70:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek72:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-229
cmp ax,bx
jge short _@seeko72
lea dx,[bx+131]
shr dl,1
_@hit72:
add eax,0D0000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a72
add eax,0D0000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a72
add eax,0D0000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko72
jmp _@hit72
_@seek2a72:
sub bp,160
add ecx,00A000000h
_@seek1a72:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko72:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek74:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-227
cmp ax,bx
jge short _@seeko74
lea dx,[bx+131]
shr dl,1
_@hit74:
add eax,0E4000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a74
add eax,0E4000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a74
add eax,0E4000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko74
jmp _@hit74
_@seek2a74:
sub bp,160
add ecx,00A000000h
_@seek1a74:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko74:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek76:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-226
cmp ax,bx
jge short _@seeko76
lea dx,[bx+131]
shr dl,1
_@hit76:
add eax,0F8000002h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a76
add eax,0F8000002h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a76
add eax,0F8000002h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko76
jmp _@hit76
_@seek2a76:
sub bp,160
add ecx,00A000000h
_@seek1a76:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko76:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek78:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-225
cmp ax,bx
jge short _@seeko78
lea dx,[bx+131]
shr dl,1
_@hit78:
add eax,00C000003h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a78
add eax,00C000003h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a78
add eax,00C000003h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko78
jmp _@hit78
_@seek2a78:
sub bp,160
add ecx,00A000000h
_@seek1a78:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko78:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek80:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-224
cmp ax,bx
jge short _@seeko80
lea dx,[bx+130]
shr dl,1
_@hit80:
add eax,020000003h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a80
add eax,020000003h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a80
add eax,020000003h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko80
jmp _@hit80
_@seek2a80:
sub bp,160
add ecx,00A000000h
_@seek1a80:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko80:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek82:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-225
cmp ax,bx
jge short _@seeko82
lea dx,[bx+130]
shr dl,1
_@hit82:
add eax,034000003h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a82
add eax,034000003h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a82
add eax,034000003h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko82
jmp _@hit82
_@seek2a82:
sub bp,160
add ecx,00A000000h
_@seek1a82:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko82:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek84:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-226
cmp ax,bx
jge short _@seeko84
lea dx,[bx+130]
shr dl,1
_@hit84:
add eax,048000003h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a84
add eax,048000003h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a84
add eax,048000003h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko84
jmp _@hit84
_@seek2a84:
sub bp,160
add ecx,00A000000h
_@seek1a84:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko84:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek86:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-227
cmp ax,bx
jge short _@seeko86
lea dx,[bx+130]
shr dl,1
_@hit86:
add eax,05C000003h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a86
add eax,05C000003h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a86
add eax,05C000003h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko86
jmp _@hit86
_@seek2a86:
sub bp,160
add ecx,00A000000h
_@seek1a86:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko86:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek88:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-229
cmp ax,bx
jge short _@seeko88
lea dx,[bx+129]
shr dl,1
_@hit88:
add eax,070000003h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a88
add eax,070000003h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a88
add eax,070000003h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko88
jmp _@hit88
_@seek2a88:
sub bp,160
add ecx,00A000000h
_@seek1a88:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko88:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek90:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-232
cmp ax,bx
jge short _@seeko90
lea dx,[bx+129]
shr dl,1
_@hit90:
add eax,084000003h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a90
add eax,084000003h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a90
add eax,084000003h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko90
jmp _@hit90
_@seek2a90:
sub bp,160
add ecx,00A000000h
_@seek1a90:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko90:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek92:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-234
cmp ax,bx
jge short _@seeko92
lea dx,[bx+129]
shr dl,1
_@hit92:
add eax,098000003h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a92
add eax,098000003h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a92
add eax,098000003h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko92
jmp _@hit92
_@seek2a92:
sub bp,160
add ecx,00A000000h
_@seek1a92:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko92:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek94:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-237
cmp ax,bx
jge short _@seeko94
lea dx,[bx+129]
shr dl,1
_@hit94:
add eax,0AC000003h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a94
add eax,0AC000003h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a94
add eax,0AC000003h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko94
jmp _@hit94
_@seek2a94:
sub bp,160
add ecx,00A000000h
_@seek1a94:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko94:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek96:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-240
cmp ax,bx
jge short _@seeko96
lea dx,[bx+128]
shr dl,1
_@hit96:
add eax,0C0000003h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a96
add eax,0C0000003h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a96
add eax,0C0000003h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko96
jmp _@hit96
_@seek2a96:
sub bp,160
add ecx,00A000000h
_@seek1a96:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko96:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek98:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-243
cmp ax,bx
jge short _@seeko98
lea dx,[bx+128]
shr dl,1
_@hit98:
add eax,0D4000003h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a98
add eax,0D4000003h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a98
add eax,0D4000003h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko98
jmp _@hit98
_@seek2a98:
sub bp,160
add ecx,00A000000h
_@seek1a98:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko98:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek100:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-246
cmp ax,bx
jge short _@seeko100
lea dx,[bx+128]
shr dl,1
_@hit100:
add eax,0E8000003h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a100
add eax,0E8000003h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a100
add eax,0E8000003h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko100
jmp _@hit100
_@seek2a100:
sub bp,160
add ecx,00A000000h
_@seek1a100:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko100:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek102:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-248
cmp ax,bx
jge short _@seeko102
lea dx,[bx+128]
shr dl,1
_@hit102:
add eax,0FC000003h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a102
add eax,0FC000003h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a102
add eax,0FC000003h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko102
jmp _@hit102
_@seek2a102:
sub bp,160
add ecx,00A000000h
_@seek1a102:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko102:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek104:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-251
cmp ax,bx
jge short _@seeko104
lea dx,[bx+127]
shr dl,1
_@hit104:
add eax,010000004h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a104
add eax,010000004h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a104
add eax,010000004h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko104
jmp _@hit104
_@seek2a104:
sub bp,160
add ecx,00A000000h
_@seek1a104:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko104:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek106:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-253
cmp ax,bx
jge short _@seeko106
lea dx,[bx+127]
shr dl,1
_@hit106:
add eax,024000004h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a106
add eax,024000004h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a106
add eax,024000004h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko106
jmp _@hit106
_@seek2a106:
sub bp,160
add ecx,00A000000h
_@seek1a106:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko106:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek108:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-254
cmp ax,bx
jge short _@seeko108
lea dx,[bx+127]
shr dl,1
_@hit108:
add eax,038000004h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a108
add eax,038000004h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a108
add eax,038000004h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko108
jmp _@hit108
_@seek2a108:
sub bp,160
add ecx,00A000000h
_@seek1a108:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko108:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek110:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-255
cmp ax,bx
jge short _@seeko110
lea dx,[bx+127]
shr dl,1
_@hit110:
add eax,04C000004h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a110
add eax,04C000004h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a110
add eax,04C000004h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko110
jmp _@hit110
_@seek2a110:
sub bp,160
add ecx,00A000000h
_@seek1a110:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko110:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek112:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-256
cmp ax,bx
jge short _@seeko112
lea dx,[bx+126]
shr dl,1
_@hit112:
add eax,060000004h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a112
add eax,060000004h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a112
add eax,060000004h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko112
jmp _@hit112
_@seek2a112:
sub bp,160
add ecx,00A000000h
_@seek1a112:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko112:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek114:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-255
cmp ax,bx
jge short _@seeko114
lea dx,[bx+126]
shr dl,1
_@hit114:
add eax,074000004h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a114
add eax,074000004h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a114
add eax,074000004h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko114
jmp _@hit114
_@seek2a114:
sub bp,160
add ecx,00A000000h
_@seek1a114:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko114:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek116:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-254
cmp ax,bx
jge short _@seeko116
lea dx,[bx+126]
shr dl,1
_@hit116:
add eax,088000004h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a116
add eax,088000004h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a116
add eax,088000004h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko116
jmp _@hit116
_@seek2a116:
sub bp,160
add ecx,00A000000h
_@seek1a116:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko116:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek118:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-253
cmp ax,bx
jge short _@seeko118
lea dx,[bx+126]
shr dl,1
_@hit118:
add eax,09C000004h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a118
add eax,09C000004h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a118
add eax,09C000004h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko118
jmp _@hit118
_@seek2a118:
sub bp,160
add ecx,00A000000h
_@seek1a118:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko118:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek120:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-251
cmp ax,bx
jge short _@seeko120
lea dx,[bx+125]
shr dl,1
_@hit120:
add eax,0B0000004h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a120
add eax,0B0000004h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a120
add eax,0B0000004h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko120
jmp _@hit120
_@seek2a120:
sub bp,160
add ecx,00A000000h
_@seek1a120:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko120:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek122:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-248
cmp ax,bx
jge short _@seeko122
lea dx,[bx+125]
shr dl,1
_@hit122:
add eax,0C4000004h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a122
add eax,0C4000004h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a122
add eax,0C4000004h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko122
jmp _@hit122
_@seek2a122:
sub bp,160
add ecx,00A000000h
_@seek1a122:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko122:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek124:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-246
cmp ax,bx
jge short _@seeko124
lea dx,[bx+125]
shr dl,1
_@hit124:
add eax,0D8000004h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a124
add eax,0D8000004h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a124
add eax,0D8000004h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko124
jmp _@hit124
_@seek2a124:
sub bp,160
add ecx,00A000000h
_@seek1a124:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko124:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek126:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-243
cmp ax,bx
jge short _@seeko126
lea dx,[bx+125]
shr dl,1
_@hit126:
add eax,0EC000004h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a126
add eax,0EC000004h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a126
add eax,0EC000004h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko126
jmp _@hit126
_@seek2a126:
sub bp,160
add ecx,00A000000h
_@seek1a126:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko126:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek128:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-240
cmp ax,bx
jge short _@seeko128
lea dx,[bx+124]
shr dl,1
_@hit128:
add eax,000000005h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a128
add eax,000000005h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a128
add eax,000000005h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko128
jmp _@hit128
_@seek2a128:
sub bp,160
add ecx,00A000000h
_@seek1a128:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko128:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek130:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-237
cmp ax,bx
jge short _@seeko130
lea dx,[bx+124]
shr dl,1
_@hit130:
add eax,014000005h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a130
add eax,014000005h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a130
add eax,014000005h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko130
jmp _@hit130
_@seek2a130:
sub bp,160
add ecx,00A000000h
_@seek1a130:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko130:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek132:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-234
cmp ax,bx
jge short _@seeko132
lea dx,[bx+124]
shr dl,1
_@hit132:
add eax,028000005h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a132
add eax,028000005h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a132
add eax,028000005h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko132
jmp _@hit132
_@seek2a132:
sub bp,160
add ecx,00A000000h
_@seek1a132:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko132:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek134:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-232
cmp ax,bx
jge short _@seeko134
lea dx,[bx+124]
shr dl,1
_@hit134:
add eax,03C000005h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a134
add eax,03C000005h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a134
add eax,03C000005h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko134
jmp _@hit134
_@seek2a134:
sub bp,160
add ecx,00A000000h
_@seek1a134:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko134:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek136:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-229
cmp ax,bx
jge short _@seeko136
lea dx,[bx+123]
shr dl,1
_@hit136:
add eax,050000005h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a136
add eax,050000005h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a136
add eax,050000005h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko136
jmp _@hit136
_@seek2a136:
sub bp,160
add ecx,00A000000h
_@seek1a136:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko136:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek138:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-227
cmp ax,bx
jge short _@seeko138
lea dx,[bx+123]
shr dl,1
_@hit138:
add eax,064000005h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a138
add eax,064000005h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a138
add eax,064000005h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko138
jmp _@hit138
_@seek2a138:
sub bp,160
add ecx,00A000000h
_@seek1a138:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko138:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek140:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-226
cmp ax,bx
jge short _@seeko140
lea dx,[bx+123]
shr dl,1
_@hit140:
add eax,078000005h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a140
add eax,078000005h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a140
add eax,078000005h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko140
jmp _@hit140
_@seek2a140:
sub bp,160
add ecx,00A000000h
_@seek1a140:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko140:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek142:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-225
cmp ax,bx
jge short _@seeko142
lea dx,[bx+123]
shr dl,1
_@hit142:
add eax,08C000005h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a142
add eax,08C000005h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a142
add eax,08C000005h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko142
jmp _@hit142
_@seek2a142:
sub bp,160
add ecx,00A000000h
_@seek1a142:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko142:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek144:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-224
cmp ax,bx
jge short _@seeko144
lea dx,[bx+122]
shr dl,1
_@hit144:
add eax,0A0000005h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a144
add eax,0A0000005h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a144
add eax,0A0000005h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko144
jmp _@hit144
_@seek2a144:
sub bp,160
add ecx,00A000000h
_@seek1a144:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko144:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek146:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-225
cmp ax,bx
jge short _@seeko146
lea dx,[bx+122]
shr dl,1
_@hit146:
add eax,0B4000005h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a146
add eax,0B4000005h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a146
add eax,0B4000005h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko146
jmp _@hit146
_@seek2a146:
sub bp,160
add ecx,00A000000h
_@seek1a146:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko146:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek148:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-226
cmp ax,bx
jge short _@seeko148
lea dx,[bx+122]
shr dl,1
_@hit148:
add eax,0C8000005h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a148
add eax,0C8000005h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a148
add eax,0C8000005h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko148
jmp _@hit148
_@seek2a148:
sub bp,160
add ecx,00A000000h
_@seek1a148:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko148:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek150:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-227
cmp ax,bx
jge short _@seeko150
lea dx,[bx+122]
shr dl,1
_@hit150:
add eax,0DC000005h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a150
add eax,0DC000005h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a150
add eax,0DC000005h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko150
jmp _@hit150
_@seek2a150:
sub bp,160
add ecx,00A000000h
_@seek1a150:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko150:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek152:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-229
cmp ax,bx
jge short _@seeko152
lea dx,[bx+121]
shr dl,1
_@hit152:
add eax,0F0000005h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a152
add eax,0F0000005h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a152
add eax,0F0000005h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko152
jmp _@hit152
_@seek2a152:
sub bp,160
add ecx,00A000000h
_@seek1a152:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko152:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek154:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-232
cmp ax,bx
jge short _@seeko154
lea dx,[bx+121]
shr dl,1
_@hit154:
add eax,004000006h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a154
add eax,004000006h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a154
add eax,004000006h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko154
jmp _@hit154
_@seek2a154:
sub bp,160
add ecx,00A000000h
_@seek1a154:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko154:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek156:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-234
cmp ax,bx
jge short _@seeko156
lea dx,[bx+121]
shr dl,1
_@hit156:
add eax,018000006h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a156
add eax,018000006h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a156
add eax,018000006h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko156
jmp _@hit156
_@seek2a156:
sub bp,160
add ecx,00A000000h
_@seek1a156:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko156:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek158:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-237
cmp ax,bx
jge short _@seeko158
lea dx,[bx+121]
shr dl,1
_@hit158:
add eax,02C000006h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a158
add eax,02C000006h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a158
add eax,02C000006h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko158
jmp _@hit158
_@seek2a158:
sub bp,160
add ecx,00A000000h
_@seek1a158:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko158:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek160:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-240
cmp ax,bx
jge short _@seeko160
lea dx,[bx+120]
shr dl,1
_@hit160:
add eax,040000006h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a160
add eax,040000006h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a160
add eax,040000006h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko160
jmp _@hit160
_@seek2a160:
sub bp,160
add ecx,00A000000h
_@seek1a160:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko160:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek162:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-243
cmp ax,bx
jge short _@seeko162
lea dx,[bx+120]
shr dl,1
_@hit162:
add eax,054000006h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a162
add eax,054000006h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a162
add eax,054000006h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko162
jmp _@hit162
_@seek2a162:
sub bp,160
add ecx,00A000000h
_@seek1a162:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko162:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek164:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-246
cmp ax,bx
jge short _@seeko164
lea dx,[bx+120]
shr dl,1
_@hit164:
add eax,068000006h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a164
add eax,068000006h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a164
add eax,068000006h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko164
jmp _@hit164
_@seek2a164:
sub bp,160
add ecx,00A000000h
_@seek1a164:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko164:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek166:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-248
cmp ax,bx
jge short _@seeko166
lea dx,[bx+120]
shr dl,1
_@hit166:
add eax,07C000006h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a166
add eax,07C000006h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a166
add eax,07C000006h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko166
jmp _@hit166
_@seek2a166:
sub bp,160
add ecx,00A000000h
_@seek1a166:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko166:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek168:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-251
cmp ax,bx
jge short _@seeko168
lea dx,[bx+119]
shr dl,1
_@hit168:
add eax,090000006h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a168
add eax,090000006h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a168
add eax,090000006h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko168
jmp _@hit168
_@seek2a168:
sub bp,160
add ecx,00A000000h
_@seek1a168:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko168:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek170:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-253
cmp ax,bx
jge short _@seeko170
lea dx,[bx+119]
shr dl,1
_@hit170:
add eax,0A4000006h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a170
add eax,0A4000006h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a170
add eax,0A4000006h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko170
jmp _@hit170
_@seek2a170:
sub bp,160
add ecx,00A000000h
_@seek1a170:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko170:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek172:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-254
cmp ax,bx
jge short _@seeko172
lea dx,[bx+119]
shr dl,1
_@hit172:
add eax,0B8000006h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a172
add eax,0B8000006h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a172
add eax,0B8000006h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko172
jmp _@hit172
_@seek2a172:
sub bp,160
add ecx,00A000000h
_@seek1a172:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko172:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek174:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-255
cmp ax,bx
jge short _@seeko174
lea dx,[bx+119]
shr dl,1
_@hit174:
add eax,0CC000006h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a174
add eax,0CC000006h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a174
add eax,0CC000006h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko174
jmp _@hit174
_@seek2a174:
sub bp,160
add ecx,00A000000h
_@seek1a174:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko174:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek176:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-256
cmp ax,bx
jge short _@seeko176
lea dx,[bx+118]
shr dl,1
_@hit176:
add eax,0E0000006h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a176
add eax,0E0000006h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a176
add eax,0E0000006h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko176
jmp _@hit176
_@seek2a176:
sub bp,160
add ecx,00A000000h
_@seek1a176:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko176:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek178:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-255
cmp ax,bx
jge short _@seeko178
lea dx,[bx+118]
shr dl,1
_@hit178:
add eax,0F4000006h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a178
add eax,0F4000006h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a178
add eax,0F4000006h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko178
jmp _@hit178
_@seek2a178:
sub bp,160
add ecx,00A000000h
_@seek1a178:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko178:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek180:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-254
cmp ax,bx
jge short _@seeko180
lea dx,[bx+118]
shr dl,1
_@hit180:
add eax,008000007h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a180
add eax,008000007h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a180
add eax,008000007h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko180
jmp _@hit180
_@seek2a180:
sub bp,160
add ecx,00A000000h
_@seek1a180:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko180:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek182:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-253
cmp ax,bx
jge short _@seeko182
lea dx,[bx+118]
shr dl,1
_@hit182:
add eax,01C000007h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a182
add eax,01C000007h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a182
add eax,01C000007h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko182
jmp _@hit182
_@seek2a182:
sub bp,160
add ecx,00A000000h
_@seek1a182:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko182:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek184:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-251
cmp ax,bx
jge short _@seeko184
lea dx,[bx+117]
shr dl,1
_@hit184:
add eax,030000007h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a184
add eax,030000007h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a184
add eax,030000007h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko184
jmp _@hit184
_@seek2a184:
sub bp,160
add ecx,00A000000h
_@seek1a184:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko184:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek186:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-248
cmp ax,bx
jge short _@seeko186
lea dx,[bx+117]
shr dl,1
_@hit186:
add eax,044000007h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a186
add eax,044000007h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a186
add eax,044000007h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko186
jmp _@hit186
_@seek2a186:
sub bp,160
add ecx,00A000000h
_@seek1a186:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko186:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek188:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-246
cmp ax,bx
jge short _@seeko188
lea dx,[bx+117]
shr dl,1
_@hit188:
add eax,058000007h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a188
add eax,058000007h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a188
add eax,058000007h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko188
jmp _@hit188
_@seek2a188:
sub bp,160
add ecx,00A000000h
_@seek1a188:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko188:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek190:
add si,ds:theloop_xsina2
mov bx,fs:[si]
add di,ds:theloop_ysina2
add bx,gs:[di]
add bx,-243
cmp ax,bx
jge short _@seeko190
lea dx,[bx+117]
shr dl,1
_@hit190:
add eax,06C000007h
adc ax,0
mov es:[bp],dl
cmp ax,bx
jge short _@seek1a190
add eax,06C000007h
adc ax,0
mov es:[bp-160],dl
cmp ax,bx
jge short _@seek2a190
add eax,06C000007h
adc ax,0
mov es:[bp-320],dl
add ecx,01E000000h
adc cx,0
sub bp,160*3
cmp ax,bx
jge short _@seeko190
jmp _@hit190
_@seek2a190:
sub bp,160
add ecx,00A000000h
_@seek1a190:
adc ecx,00A000000h
adc cx,0
sub bp,160
_@seeko190:
add eax,ecx
adc eax,ecx
adc ax,-1
_@seek192:
_@seekr192:
ret
theloop ENDP
