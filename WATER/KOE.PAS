uses crt;
type    pbuf = ^buf;
        buf = array[0..64000] of byte;

var    r,g,b : pbuf;
       x,y   : word;
       f     : file;
       pal   : array[0..768] of byte;
       row   : array[0..320*3-1] of byte;

procedure setrgb(c,r,g,b:byte);
begin
    port[$3c8] := c;
    port[$3c9] := r;
    port[$3c9] := g;
    port[$3c9] := b;
end;

procedure waitk;
var  ch : char;
begin
     ch := readkey;
     if ch = #0 then ch := readkey;
end;

begin
     asm
     mov   ax,$13
     int   $10
     end;
     getmem(r,64000);
     getmem(g,64000);
     getmem(b,64000);

     assign(f,'bkg.clx');
     reset(f,1);
     seek(f,10);
     blockread(f,pal,768);
     for x := 0 to 255 do setrgb(x,pal[x*3+0],pal[x*3+1],pal[x*3+2]);
     blockread(f,mem[$a000:0],64000);
     close(f);


     assign(f,'o:k1.mtv');
     reset(f,1);
     seek(f,8);
     for y := 0 to 199 do
     begin
      blockread(f,row,320*3);
      for x := 0 to 319 do r^[x+y*320] := row[x*3+0];
      for x := 0 to 319 do g^[x+y*320] := row[x*3+1];
      for x := 0 to 319 do b^[x+y*320] := row[x*3+2];
     end;
     close(f);

{     move(g^,mem[$a000:0],64000);}

     for y := 250 downto 1 do
     for x := 0 to 64000 do if (r^[x]  = y) and (g^[x] and 1 = 1) then mem[$a000:x] := 255;

     waitk;

     freemem(r,64000);
     freemem(g,64000);
     freemem(b,64000);
end.