uses crt;
var   f      : file;
      buf    : array[0..30000] of byte;
      font   : array[0..237*31] of byte;
      x      : word;
begin
    asm
    mov   ax,$13
    int   $10
    end;

    assign(f,'pos1.dat');
    reset(f,1);
    blockread(f,buf,filesize(f));
    close(f);

    fillchar(font,sizeof(font),#0);
    for x := 0 to 30 do font[x*237+20] := 15;
    for x := 0 to 30 do font[x*237+21] := 15;

    asm
    mov   ax,$a000
    mov   es,ax
    mov   si,offset buf

    mov   bx,offset font

    mov   dx,237*31
@a: lodsw
    or    ax,ax
    je    @no
    mov   cx,ax
@b: lodsw
    mov   di,ax
    mov   al,byte ptr ds:[bx]
    mov   byte ptr es:[di],al
    loop  @b
@no:
    inc   bx
    dec   dx
    jnz   @a
    end;


    repeat until keypressed;
end.