uses crt;
var   f      : file;
      buf    : array[0..41000] of byte;
      pal    : array[0..768] of byte;
      font   : array[0..320*38] of byte;
      kuva   : pointer;
      x      : word;

procedure setrgb(c,r,g,b:byte);
begin
     port[$3c8] := c;
     port[$3c9] := r;
     port[$3c9] := g;
     port[$3c9] := b;
end;

procedure waitr;
begin
                setrgb(0,0,0,0);
                asm
                mov     dx,$3da
@@13:       in  al,dx
        test    al,8
        jnz @@13
@@14:       in  al,dx
        test    al,8
        jz  @@14
                end;
                setrgb(0,43,0,0);
end;

procedure joku;
begin
    asm
    mov   ax,$a000
    mov   es,ax
    mov   si,offset buf

    mov   bx,offset font

    mov   dx,158*37
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

end;

var  a: byte;

begin
    getmem(kuva,64000);
    asm
    mov   ax,$13
    int   $10
    end;
    fillchar(font,sizeof(font),#15);
    for x := 0 to sizeof(font) do font[x] := x and 15;

     assign(f,'bkg.clx');
     reset(f,1);
     seek(f,10);
     blockread(f,pal,768);
     for x := 0 to 255 do setrgb(x,pal[x*3+0],pal[x*3+1],pal[x*3+2]);
{     blockread(f,mem[$a000:0],64000);}
     close(f);


{
    assign(f,'o:wat4.dat');
    reset(f,1);
    blockread(f,buf,filesize(f));
    close(f);
    joku;
    assign(f,'o:wat3.dat');
    reset(f,1);
    blockread(f,buf,filesize(f));
    close(f);
    joku;
    assign(f,'o:wat2.dat');
    reset(f,1);
    blockread(f,buf,filesize(f));
    close(f);
    joku;
}

    assign(f,'o:\wat1.dat');
    reset(f,1);
    blockread(f,buf,filesize(f));
    close(f);
{    joku;}

    a := 157;
    repeat
    waitr;
    joku;
    for x := 0 to 38 do font[x*158+a] := x+1;
    dec(a);
    until keypressed;
end.