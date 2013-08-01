;Actual DIS server routines
;==========================
;note: lines with TEMP! are temporary to be replaced by smarter 
;techniques in the future...

LOCALS
.386
ALIGN 16

;北北北北北北北北北北 Variables 北北北北北北北北北
exitflag dw     0       ;1=exit key pressed
indemoflag dw   0       ;1=inside the great big ultra cool demo
passmuscode dw  0

ALIGN 4
msgarea0 db     64 dup(0)
msgarea1 db     64 dup(0)
msgarea2 db     64 dup(0)
msgarea3 db     64 dup(0)

;北北北北北北北 List of service routines 北北北北北北
service0 LABEL WORD
        dw      OFFSET version_0
        dw      OFFSET waitb_1
        dw      OFFSET exit_2
        dw      OFFSET indemo_3
        dw      OFFSET loader_4
        dw      OFFSET msgarea_5
        dw      OFFSET muscode_6
        dw      OFFSET setcopper_7
        dw      OFFSET fastvmode_8
        dw      OFFSET musframe_9
        dw      OFFSET sync_10
service9 LABEL WORD

;北北北北北北北北北北 General stuff 北北北北北北北北北

dis_oldint dd   ?

dis_setint PROC NEAR
        push    ds
        xor     ax,ax
        mov     ds,ax
        mov     ax,word ptr ds:[0fch*4+0]
        mov     bx,word ptr ds:[0fch*4+2]
        mov     word ptr cs:dis_oldint[0],ax
        mov     word ptr cs:dis_oldint[2],bx
        mov     word ptr ds:[0fch*4+0],OFFSET dis_int
        mov     word ptr ds:[0fch*4+2],cs
        pop     ds
        ret
dis_setint ENDP

dis_resetint PROC NEAR
        xor     ax,ax
        mov     ds,ax
        mov     ax,word ptr cs:dis_oldint[0]
        mov     bx,word ptr cs:dis_oldint[2]
        mov     word ptr ds:[0fch*4+0],ax
        mov     word ptr ds:[0fch*4+2],bx
        ret
dis_resetint ENDP

        dw      0fc0h ;signature for dis
        dw      0fc0h ;installation check
dis_int PROC FAR ;interrupt 0fch server, BX=function number
        sti
        shl     bx,1
        cmp     bx,(service9-service0)
        jae     @@1
        call    cs:service0[bx]
@@1:    iret
dis_int ENDP

;北北北北北北北北北北 Service Routines 北北北北北北北北北

;entry: -
; exit: AX=version
;descr: -
version_0 PROC NEAR
        ;indemo?
        IFDEF INDEMO
        mov     ax,1
        ELSE
        mov     ax,0
        ENDIF
        mov     cs:indemoflag,ax
        ;initialize
        mov     cs:passmuscode,0
        mov     cs:exitflag,0
        ;return version
        mov     ax,0100h
        ret
version_0 ENDP

ff      PROC FAR
        IFDEF INDEMO
        push    es
        mov     es,cs:stmikseg
        mov     bx,es:_np_ord
        inc     bx
        xor     ax,ax
        push    ax
        push    bx
        call    _zgotosong
        add     sp,4
        pop     es
        ENDIF
        ret
ff      ENDP

checkkeys PROC NEAR
        IFDEF INDEMO
        cmp     cs:forcebreak,0
        jne     @@3
        ENDIF
        call    ctrldown
        jc      @@6
        mov     ah,1                                    ;TEMP!
        int     16h
        jz      @@3
        mov     ah,0
        int     16h
        cmp     al,'0'
        jne     @@9
        pusha
        push    ds
        push    es
        mov     ax,0fcfch
        mov     bx,1
        int     33h
        pop     es
        pop     ds
        popa
        ret
@@9:    cmp     al,'9'
        jne     @@8
        pusha
        push    ds
        push    es
        mov     ax,0fcfch
        mov     bx,2
        int     33h
        pop     es
        pop     ds
        popa
        ret
@@8:    cmp     al,'1'
        jne     @@5
        call    ff
@@5:    cmp     al,27
        jne     @@4
@@6:    mov     cs:exitflag,1
@@4:    mov     cs:passmuscode,1
@@3:    ret
checkkeys ENDP

;entry: -
; exit: -
;descr: Waits for border start
waitb_1 PROC NEAR
        call    checkkeys
        IFDEF INDEMO
        sti
        mov     ax,cs:copperframecount
@@v:    cmp     cs:copperframecount,ax
        je      @@v
@@q:    mov     ax,cs:copperframecount
        mov     cs:copperframecount,0
        ELSE
        mov     dx,3dah
@@1:    in      al,dx
        test    al,8
        jnz     @@1
@@2:    in      al,dx
        test    al,8
        jz      @@2
        mov     ax,1 ;number of frames taken            ;TEMP!
        ENDIF
        ret
waitb_1 ENDP

;entry: -
; exit: AX=return
;descr: returns 1 if part should exit.
exit_2 PROC NEAR
        call    checkkeys
        mov     ax,cs:exitflag
        ret
exit_2 ENDP

;entry: -
; exit: AX=return
;descr: returns 1 if inside demo (and not testing from dos)
indemo_3 PROC NEAR
        mov     ax,cs:indemoflag
        ret
indemo_3 ENDP

;entry: AX=area desired (0..3)
; exit: DX:AX=pointer to msgarea
;descr: returns a pointer to interpart communications area.
;       There is ONLY 64 bytes of space in the area, DON'T OVERFLOW IT :-)
msgarea_5 PROC NEAR
        cmp     ax,1
        je      @@1
        cmp     ax,2
        je      @@2
        cmp     ax,3
        je      @@3
@@0:    mov     dx,cs
        mov     ax,OFFSET msgarea0
        ret
@@1:    mov     dx,cs
        mov     ax,OFFSET msgarea1
        ret
@@2:    mov     dx,cs
        mov     ax,OFFSET msgarea2
        ret
@@3:    mov     dx,cs
        mov     ax,OFFSET msgarea3
        ret
msgarea_5 ENDP

;Loader functions (not for parts)
;entry: AX=0/1
; exit: -
;descr: AX=0: restores dos process id
;       AX=1: sets dos process id to the loader
;       AX=100h: call _zloadinstrument(dx)
;       AX=101h: call _zinitmodule(dx:0)
;       This is used to load the music in name of the loader!
loader_4 PROC NEAR
        IFDEF INDEMO
        call    loaderservices
        ENDIF
        ret
loader_4 ENDP

;entry: AX=code you want (are waiting for)
; exit: AX=current code,BX=row
muscode_6 PROC NEAR
        push    ax
        call    checkkeys
        pop     ax
        IFDEF INDEMO
        push    es
        mov     es,cs:stmikseg
        mov     ax,es:_np_zinfo
        mov     bx,es:_np_row
        mov     cx,es:_np_ord
        mov     dx,-32
        cmp     es:_np_zplus,0
        je      @@1a
        cmp     es:_np_zplus,1
        je      @@1b
        cmp     es:_np_zplus,2
        je      @@1e
@@1eb:  cmp     bx,32
        ja      @@1b
@@1e:   mov     dx,bx
        cmp     dx,32
        jb      @@1a
        mov     dx,-32
        jmp     @@1a
@@1b:   ;plus coming
        mov     dx,bx
        sub     dx,64
        cmp     dx,-32
        jge     @@1a
        mov     dx,-32
@@1a:   mov     bx,es:_np_row
	pop     es
        ELSE
        cmp     cs:passmuscode,0
        jne     @@1
        xor     ax,ax
        ENDIF
        ret
@@1:    mov     cs:passmuscode,0
        ret
muscode_6 ENDP

;entry: AX=number of copper interrupt to capture:
;               0=after display start (about scan line 25)
;               1=just before retrace (AVOID USING THIS IF POSSIBLE)
;               2=in the retrace
;       DX:CX=far pointer to routine (0:0=remove routine)
;       The routine pointed to must end in a RETF. It must save any
;       386 registers it uses (including FS/GS)
; exit: -
;descr: sets the specified copper interrupt to call the specified routine.
;       IMPORTANT: The part must reset the copper int before it exits!
setcopper_7 PROC NEAR
        IFDEF INDEMO
        or      dx,dx
        jnz     @@1
        or      cx,cx
        jnz     @@1
        mov     dx,cs
        mov     cx,OFFSET copper_intretf
@@1:    cmp     ax,0
        jne     @@2
        mov     word ptr cs:copper_int0[0],cx
        mov     word ptr cs:copper_int0[2],dx
@@2:    cmp     ax,1
        jne     @@3
        mov     word ptr cs:copper_int1[0],cx
        mov     word ptr cs:copper_int1[2],dx
@@3:    cmp     ax,2
        jne     @@4
        mov     word ptr cs:copper_int2[0],cx
        mov     word ptr cs:copper_int2[2],dx
@@4:    ENDIF
        ret
setcopper_7 ENDP

;entry: AX=area (0..3) containing the saved VGA state.
; exit: -
;descr: Quicksets the desired vga mode (takes one frame)
fastvmode_8 PROC NEAR ;DOESN'T WORK!-(
        IFDEF INDEMO
        push    si
        push    ds
        call    msgarea_5 ;dx:ax=ds:ax
        mov     si,ax
        mov     ax,cs
        mov     ds,ax
        ;data now at ds:si
        push    si
        call    waitb_1
        pop     si
        mov     dx,3dah
        in      al,dx
        ;Syncronous reset
        mov     dx,3c4h
        mov     ax,0200h
        out     dx,ax
        ;Clear CRTC protection flag
        mov     dx,3d4h
        mov     al,011h
        out     dx,al
        inc     dx
        in      al,dx
        and     al,not 128
        out     dx,al
        ;Set misc register
        mov     al,ds:[si]
        inc     si
        mov     dx,3c2h
        out     dx,al
        ;Set Sequencer (3C4)
        mov     dx,3c4h
        xor     al,al
        REPT    04h+1
        mov     ah,ds:[si]
        inc     si
        out     dx,ax
        inc     al
        ENDM
        ;Set CRTC (3D4)
        mov     dx,3d4h
        xor     al,al
        REPT    018h+1
        mov     ah,ds:[si]
        inc     si
        out     dx,ax
        inc     al
        ENDM
        ;Set GFX controller (3CE)
        mov     dx,3ceh
        xor     al,al
        REPT    08h+1
        mov     ah,ds:[si]
        inc     si
        out     dx,ax
        inc     al
        ENDM
        ;Set Attribute controller (3C0)
        mov     dx,3dah
        in      al,dx
        mov     dx,3c0h
        zzz=0
        REPT    014h+1
        mov     al,zzz
        out     dx,al
        mov     al,ds:[si]
        out     dx,al
        inc     si
        zzz=zzz+1
        ENDM
        ;Enable display, enable PEL mask
        mov     dx,3c0h
        mov     al,20h
        out     dx,al
        mov     dx,3c6h
        mov     al,0ffh
        out     dx,al
        pop     ds
        pop     si
        ELSE
        mov     ax,13h
        int     10h
        ENDIF
        ret
fastvmode_8 ENDP

;entry: AX=1 : set frame to DX
;       AX=0 : read frame to AX
; exit: AX=frame)
;descr: controls/returns music frame numbers
musframe_9 PROC NEAR
        IFDEF INDEMO
        push    es
        mov     es,cs:stmikseg
        cmp     ax,0
        je      @@1
        mov     es:_np_zframe,dx
@@1:    mov     ax,es:_np_zframe
        pop     es
        ELSE
        xor     ax,ax
        ENDIF
        ret
musframe_9 ENDP

IFDEF INDEMO
ordersync1 LABEL BYTE ;startpart
dw      0000h,0
dw      0200h,1
dw      0300h,2
dw      032fh,3
dw      042fh,4
dw      052fh,5
dw      062fh,6
dw      072fh,7
dw      082fh,8
dw      0900h,9
dw      0d00h,10
;dw      0800h,8
;dw      0c00h,9
;dw      0f00h,10
dw      3d00h,1
dw      3f00h,2
dw      4100h,3
dw      4200h,4
ENDIF     

sync_10 PROC NEAR
        IFDEF INDEMO
        mov     es,cs:stmikseg
        mov     dh,byte ptr es:_np_ord
        mov     dl,byte ptr es:_np_row
        mov     bx,OFFSET ordersync1
        mov     cx,16
@@2:    cmp     dx,cs:[bx]
        jbe     @@1
        add     bx,4
        loop    @@2
@@1:    mov     ax,cs:[bx-2]
        ENDIF
        ret
sync_10 ENDP
