uses crt;

type  bc     = record
                 x : integer;
                 y : integer;
               end;
      rengas = record
                 x,y,r : shortint;
               end;


var   pallo  : array[0..100] of bc;
      putki  : array[0..400] of rengas;
      pcalc  : array[0..137,0..63] of bc;
      t      : word;

procedure plot(x,y:integer;c:byte);
begin
    if (x > 0) and (x < 319) and (y > 0) and (y < 199) then
    mem[$a000:x+y*320] := c;
    inc(t);
end;

procedure ball(px,py,r:integer;c:byte);
var  x,y : word;
begin
     for x := 0 to 31 do plot(  px+round(sin(x*pi/16)*round(r*1.1)),  py+round(cos(x*pi/16)*r), x div 8+1 );
end;

var   x,y,z : integer;
      x1,y1 : word;
      a   : word;

procedure clr;
begin
    asm
        mov    ax,$a000
        mov    es,ax
        mov    cx,32000
        mov    di,0
        xor    ax,ax
        rep    stosw
    end;
end;

var  f : file;
     r : word;
begin
    asm
        mov  ax,$13
        int  $10
    end;

    r := 0;
    for Z := 10 to 147 do
     for a := 0 to 63 do
      with pcalc[z-10][a] do
       begin
         x := 160+round(sin(a*pi/32)*round(z*1.7));
         y := 100+round(cos(a*pi/32)*z);
       end;

    for x := 0 to 137 do
     for y := 0 to 63 do plot(pcalc[x][y].x,pcalc[x][y].y,15);

    assign(f,'tunnel.dat');
    rewrite(f,1);
    blockwrite(f,pcalc,sizeof(pcalc));
    close(f);

    repeat until keypressed;

    asm
        mov  ax,$3
        int  $10
    end;
end.