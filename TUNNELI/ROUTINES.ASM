          .MODEL  large,PASCAL
          .CODE
          .386

oldpos    dw     7000 dup (0)
rows      dw     210 dup(0)
op        dw     0

rowinit   proc   far
          PUBLIC rowinit
          mov    cx,200
          mov    ax,0
          mov    bx,0
@ri:      mov    cs:rows[bx],ax
          add    ax,320
          add    bx,2
          loop   @ri
          ret
rowinit   endp


init      proc   far
          PUBLIC init
          mov    word ptr cs:op,0
          ret
init      endp

Putrouts  PROC FAR
          PUBLIC Putrouts
          push   ds
          mov    ds,ax

          mov    ax,0a000h
          mov    es,ax

;          mov    byte ptr cs:[@c+3],cl
          mov    word ptr cs:[@yad+1],bx

          mov    ax,cs:Op
          mov    cx,64
@dr:
          mov    bx,ax
          mov    di,word ptr cs:oldpos[bx]
          mov    byte ptr es:[di],0

@yad:     mov    bx,1234                 ; absolute change, Y base of circle
          add    bx,word ptr ds:[si+2]   ; add y value
          cmp    bx,199
          ja     @yli
          mov    di,word ptr ds:[si]   ; { get x value }
          add    di,dx
          cmp    di,319
          ja     @yli
          add    bx,bx
          add    di,word ptr cs:rows[bx]
@c:       mov    byte ptr es:[di],15

@yli:     mov    bx,ax
          mov    word ptr cs:oldpos[bx],di
          add    ax,2

          add    si,4
          loop   @dr
          mov    cs:Op,ax
          pop    ds
          ret
Putrouts  ENDP

end