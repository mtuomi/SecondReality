          .MODEL  large,PASCAL
          .CODE
          .386

          ;  dx = Bground seg
          ;  ax = pos seg
          ;  si = pos ofs
          ;  cx = font seg
          ;  bx = font ofs
          ;  Front scroll

Putrouts1 PROC FAR
          PUBLIC Putrouts1
          push  ds
          mov   ds,ax
          mov   ax,0a000h
          mov   es,ax
          mov   fs,cx
          mov   gs,dx

          mov   dx,158*34
@a1:      lodsw
          or    ax,ax
          je    @no1
          mov   cx,ax
@b1:      lodsw
          mov   di,ax
          mov   al,byte ptr fs:[bx]
          or    al,al
          jne   @y
          mov   al,byte ptr gs:[di]
@y:       mov   byte ptr es:[di],al
          loop  @b1
@no1:     inc   bx
          dec   dx
          jnz   @a1

          pop   ds
          ret
Putrouts1 ENDP
          END
