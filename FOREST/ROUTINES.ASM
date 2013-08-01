          .MODEL  large,PASCAL
          .CODE
          .386

          ; dx = Bground seg
          ;  ax = pos seg
          ;  si = pos ofs
          ;  cx = font seg
          ;  bx = font ofs

Putrouts  PROC FAR
          PUBLIC Putrouts

          push   ds
          mov    ds,ax
          mov    ax,0a000h
          mov    es,ax
          mov    fs,cx
          mov    gs,dx

          mov    dx,237*31
@a:       lodsw                        ; get byte count
          or     ax,ax                 ; if 0 then hidden pixel
          je     @ei                   ; next pixel
          mov    cx,ax                 ; set loop value
@c:       lodsw                        ; get destination address
          mov    di,ax
          mov    al,byte ptr gs:[di]   ; get bground pixel
          add    al,byte ptr fs:[bx]   ; add font value to pixel
          mov    byte ptr es:[di],al   ; store pixel to screen
          loop   @c
@ei:
          inc    bx
          dec    dx
          jnz    @a
          pop    ds

          ret
Putrouts  ENDP
          END
