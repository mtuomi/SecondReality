uses crt;

const veke = 980;    { frame count to exit }

type
      bc     = record
                 x : integer;
                 y : integer;
               end;

      rengas = record
                 x,y   : integer;
                 r     : byte;
                 c     : byte;
               end;

var
      putki  : array[0..102] of rengas;
      pcalc  : array[0..137,0..63] of bc;

      aa1    : array[0..200] of word;
      rows   : array[0..200] of word;
      aa2    : array[0..200] of word;

      sinit  : array[0..4098] of word;
      cosit  : array[0..2050] of word;
      frame  : word;
      bor    : byte;
      frames : word;

{$L tunneli.obj}
procedure tun;external;
{$L sinit.obj}
procedure sini;far;external;

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
	mov	bx,1
	int	0fch
        mov     frames,ax
        end;
        if mem[$40:$17] and 16 = 16 then setrgb(0,0,0,15);
end;

function dis_exit:boolean;
var   a : byte;
begin
        asm
	mov	bx,2
	int	0fch
        mov     a,al
        end;
        if a = 0 then dis_exit := false;
end;


var  oldpos : array[0..7500] of word;
     op     : word;
     ry     : word;

var  x,y,z : integer;
     x1,y1 : word;
     a   : word;
     f   : file;

     sx,sy : word;
     _bx,by : word;
     br    : byte;
     bbc   : byte;
     pcp   : word;
     pc    : byte;
     mx,my : integer;
     addi,yvalue : word;
     ch          : char;
     oo          : word;
     flip        : byte;
     quit        : boolean;
     sync        : word;

label poies;
begin

    x := 0;y := 0;z := 0;x1 := 0;y1 := 0;a := 0;sx := 0;sy := 0;

    quit := false;

{    fillchar(oldpos,sizeof(oldpos),#0);}

    for x := 0 to 200 do aa1[x] := 64000;
    move(aa1,aa2,sizeof(aa1));
    for x := 0 to 200 do rows[x] := x*320;
    move(mem[seg(sini):ofs(sini)],sinit,4097*2);
    move(mem[seg(sini):ofs(sini)+4097*2],cosit,2048*2);
    move(mem[seg(tun):ofs(tun)],pcalc,sizeof(pcalc));

{
    for x := 0 to 4096 do Sinit[x] := round(sin((x)/128*pi)*((x*3) div 16));
    for x := 0 to 2048 do Cosit[x] := round(cos((x)/256*pi)*((x*4) div 32));
}
    asm
        mov  ax,$13
        int  $10

	xor	bx,bx
	int	0fch
    end;

    for x := 0 to 64 do setrgb(64+x,(64-x) div 2,(64-x) div 2,64-x);
    for x := 0 to 64 do setrgb(128+x,(64-x) div 4,(64-x) div 3,(64-x) div 2);

    setrgb(68,0,0,0);
    setrgb(132,0,0,0);


    setrgb(255,0,63,0);

    for x := 0 to 100 do
     begin
      putki[x].x := 0;
      putki[x].y := 0;
      putki[x].r := 0;
      putki[x].c := 0;
     end;

  sx := 0;
  sy := 0;

  pc := 60;
  addi := 40;
  flip := 0;
  bor := 0;
  frame := 0;
  quit := false;

  waitr;
  for Z := 0 to 100 do putki[z].r := round(16384 div ((Z*7)+95));

  repeat
    waitr;


    ry := 0;
    for x := 80 downto 4 do
     begin
     _bx := putki[x].x-putki[5].x;
      by := putki[x].y-putki[5].y;
      br := putki[x].r;
      bbc := putki[x].c+round(x / 1.3);
      pcp := ofs(pcalc[br][0]);

      if bbc >= 64 then
       asm
        mov    ax,$a000
        mov    es,ax
        mov    si,PCP
        mov    dx,_BX
        mov    al,bbc
        mov    byte ptr cs:[@c+3],al

        mov    cx,64
        mov    ax,RY

        push   bp
        mov    bp,BY
@a:     mov    bx,ax
        mov    di,word ptr ds:oldpos[bx]
        mov    byte ptr es:[di],0

        mov    di,word ptr ds:[si]
        add    di,dx
        cmp    di,319
        ja     @yli
        mov    bx,bp
        add    bx,word ptr ds:[si+2]
        add    bx,bx
        add    di,word ptr ds:rows[bx]
@c:     mov    byte ptr es:[di],15
@yli:
        mov    bx,ax
        mov    word ptr ds:oldpos[bx],di
        add    si,4
        add    ax,2
        dec    cx
        jnz    @a
        pop    bp
        mov    RY,ax
       end;
     end;

     for sync := 1 to frames do
     begin
     putki[100].x := cosit[sy and 2047]-sinit[sy*3 and 4095]-cosit[sx and 2047];
     putki[100].y := sinit[sx*2 and 4095]-cosit[sx and 2047]+sinit[y and 4095];
{     move(putki[1],putki[0],ofs(putki[100])-ofs(putki[0]));}
      asm

{
                 x,y   : integer;
                 r     : byte;
                 c     : byte;
}

      mov  si,offset putki[1]
      mov  di,offset putki[0]
      mov  cx,600
@a:   mov  ax,ds:[si]
      mov  ds:[di],ax
      mov  ax,ds:[si+2]
      mov  ds:[di+2],ax
      mov  al,ds:[si+4]
      mov  ds:[si+4],al
      add  si,6
      add  di,6
      dec  cx
      jnz  @a
      end;
     inc(sy);
     inc(sx);
     asm
     mov   ax,0
     mov   bx,6
     int   0fch
     cmp   ax,-4
     jnz   @a
     mov   quit,1
@a:  end;
     if (sy and 15) > 7 then putki[99].c := 128 else putki[99].c := 64;
     if frame >= veke-102 then putki[99].c := 0;
     if frame = veke then quit := true else inc(frame);
     if dis_exit then quit := true;
     if quit then goto poies;
     end;
poies:

  until quit;
end.